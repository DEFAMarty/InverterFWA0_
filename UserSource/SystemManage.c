//###########################################################################
//
// FILE:    SystemManage.c
//
// TITLE:   System Manage
//
//
//
//###########################################################################

// $Copyright:
// Copyright (C) 2017 Defa Incorporated - http://www.defa.com/

//###########################################################################

//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

BOARDSTATEEnmu boardState = boardState_Invalid;

// CNTL 3p3z for inverter current control
extern CNTL_3P3Z_IQ_VARS cntl3p3z_InvI_vars;
extern CNTL_3P3Z_IQ_COEFFS cntl3p3z_InvI_coeff;

//Proportional Resonant controller for the fundamental voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_PRV_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_PRV_coeff;

//Resonant controller for the 3rd harmonics voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_RV3_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV3_coeff;

//Resonant controller for the 5th harmonic voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_RV5_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV5_coeff;

//Resonant controller for the 7th harmonics voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_RV7_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV7_coeff;

//Resonant controller for the 9th harmonics voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_RV9_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV9_coeff;

//Resonant controller for the 11th harmonics voltage
extern CNTL_2P2Z_IQ_VARS cntl2p2z_RV11_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV11_coeff;

// Lead lag compensator to improve phase margin
extern CNTL_2P2Z_IQ_VARS cntl2p2z_LeadLag_vars;
extern CNTL_2P2Z_IQ_COEFFS cntl2p2z_LeadLag_coeff;


void SystemManage(void)
{
//	if(0 != (SystemFalutFlag.FatalUnion.All & 0x3FFF))
//	if(0 != (SystemFalutFlag.FatalUnion.All & 0x3C7F)) 	// 0x3C7F only for Test
	if(0 != (SystemFalutFlag.FatalUnion.All & 0x0000)) 	// 0x0000 only for Cancel the Fault Detection
	{

		if(boardState_TripCondition != boardState)
		{
			boardState = boardState_TripCondition;
		}
	}


	switch(boardState)
	{
		case boardState_Invalid:
			boardState = boardState_Initialize;
			break;

		case boardState_Initialize:
			BoardStateInitialize();
			boardState = boardState_DriveReady;
			break;

		case boardState_DriveReady:
			BoardStateDriveReady();
			boardState = boardState_DriveEnable;
			break;

		case boardState_DriveEnable:
			BoardStateDriveEnable();
			break;

		case boardState_InverterOFF:
			BoardStateONOFFAction();
			break;

		case boardState_InverterON:
			BoardStateONOFFAction();
			break;

		case boardState_TripCondition:
			BoardStateTripCondition();
//			if ((1 == SwitchIDRead()) || (1 == SwitchODRead()))
//			{
				boardState = boardState_TripCondition;
//			}
//			else
//			{
//				boardState = boardState_DriveReady;
//			}

			break;

		case boardState_SleepMode:
			BoardStateSleepMode();
			boardState = boardState_SleepMode;
			break;


		default:

			break;
	}

}



void BoardStateInitialize(void)
{
	DPPFCShutDownControlEnable();

	DPPFCPowerDisable();
}




void BoardStateDriveReady(void)
{
	I2CReaddatafromEEPROMsection();

//	DPPFCPowerEnable();

//	DPPFCShutDownControlDisable();

}


void BoardStateDriveEnable(void)
{
	if ((1 == ACINDRead()) && ((1 == SwitchIDRead()) || (1 == SwitchODRead())))
	{
		boardState = boardState_InverterON;
	}
	else
	{
		boardState = boardState_InverterOFF;
	}
	invVoRef = 0;
}




void BoardStateTripCondition(void)
{
	F280X3_PWM_Close();

//	DPPFCShutDownControlEnable();
//
//	DPPFCPowerDisable();

}




void BoardStateONOFFAction(void)
{
	if ((1 == ACINDRead()) && ((1 == SwitchIDRead()) || (1 == SwitchODRead())))
	{
		boardState = boardState_InverterON;
		F280X3_PWM_Open();
	}
	else
	{

//		F280X3_PWM_Close();				// Normally we don't need to close the PWM, because of there's no fault happen. 20171108
		boardState = boardState_InverterOFF;



		//CNTL3P3Z Inverter Current Loop
		CNTL_3P3Z_IQ_VARS_init(&cntl3p3z_InvI_vars);

		//CNTL2P2Z PR 1H Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_PRV_vars);

		//CNTL2P2Z R 3HController
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV3_vars);

		//CNTL2P2Z R 5H Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV5_vars);

		//CNTL2P2Z R 7H Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV7_vars);

		//CNTL2P2Z R 9H Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV9_vars);

		//CNTL2P2Z R 11H Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV11_vars);

		//CNTL2P2Z Lead Lag Controller
		CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_LeadLag_vars);
	}
}


void BoardStateSleepMode(void)
{
	F280X3_PWM_Close();

	DPPFCPowerDisable();
}




//===========================================================================
// End of file.
//===========================================================================
