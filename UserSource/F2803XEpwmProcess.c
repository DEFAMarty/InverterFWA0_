//###########################################################################
//	FILE:			F2803XEpwmProess.h
//
//	Description:	Driver Config to support a 1 phase inverter with unipolar modulation
//	Version: 		1.0
//  Target:  		TMS320F2802x, TMS320F2803x
//
// Function arguments defined as:
//-------------------------------
// n = 		Target ePWM module, 1,2,...16.  e.g. if n=2, then target is ePWM2
// period = PWM period in Sysclks
//
//###########################################################################

// Included headerfiles.
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#pragma CODE_SECTION(PWM_Inv_SPWM_unipolar_CNF, "ramfuncs");


//---------------------------------------------------------------------------
// Used to indirectly access all EPWM modules
volatile struct EPWM_REGS *ePWM[] =
 				  { &EPwm1Regs,			//intentional: (ePWM[0] not used)
				  	&EPwm1Regs,
					&EPwm2Regs,
					&EPwm3Regs,
					&EPwm4Regs,
					&EPwm5Regs,
					&EPwm6Regs
				  };


void PWM_Inv_SPWM_unipolar_CNF(Uint16 n,Uint16 period,Uint16 deadband_rising,Uint16 deadband_falling)
{
    EALLOW;                       /* Enable EALLOW */

	/* 	---------------------- Init EPWM n ------------------------------------------------ */
	(*ePWM[n]).TBPRD 		   			= period;     		/* Set timer period */
	(*ePWM[n]).CMPA.half.CMPA 			= 1000;
	(*ePWM[n]).TBCTL.bit.FREE_SOFT 		= FREE_RUN_FLAG;
	(*ePWM[n]).TBCTL.bit.CLKDIV 		= CLKDIV_PRESCALE_X_1;	/* SYSCLKOUT / 1*/
	(*ePWM[n]).TBCTL.bit.HSPCLKDIV 		= HSPCLKDIV_PRESCALE_X_1;	/* Clock ratio to SYSCLKOUT*/
	(*ePWM[n]).TBCTL.bit.SYNCOSEL  		= TB_CTR_ZERO;		/* Sync with Sync in signal*/
	(*ePWM[n]).TBCTL.bit.PRDLD 			= TB_IMMEDIATE;		/* Shadow load for PRD*/
	(*ePWM[n]).TBCTL.bit.PHSEN     		= TB_DISABLE;      	/* Disable phase loading*/
	(*ePWM[n]).TBCTL.bit.CTRMODE   		= TB_COUNT_UPDOWN;	/* Symmetric wave*/
	(*ePWM[n]).TBPHS.half.TBPHS 		= 0;
	(*ePWM[n]).TBCTR 					= 0x0000;                      // Clear counter
	/* Setup shadowing*/
	(*ePWM[n]).CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;		/* Shadow Mode*/
	(*ePWM[n]).CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;		/* Shadow Mode*/
	(*ePWM[n]).CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO;  	/* Load A on Zero*/
	(*ePWM[n]).CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		/* Load B on Zero*/
	/* Set Actions */
	(*ePWM[n]).AQCTLA.bit.CAD 			= AQ_SET;
	(*ePWM[n]).AQCTLA.bit.CAU			= AQ_CLEAR;
	(*ePWM[n]).AQCTLA.bit.ZRO 			= AQ_NO_ACTION;
	/* Action-Qualifier Continuous Software Force Register */
	(*ePWM[n]).AQCSFRC.bit.CSFA			= CSFA_LOW;
	(*ePWM[n]).AQCSFRC.bit.CSFB			= CSFB_HIGH;
    /* Init Action Qualifier S/W Force Register for EPWM1-EPWM2*/
	(*ePWM[n]).AQSFRC.all 				= AQSFRC_INIT_STATE;
	/* Set Dead Band */
	(*ePWM[n]).DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE;	/* Rising Delay on 1A & Falling Delay on 1B*/
//	(*ePWM[n]).DBCTL.bit.POLSEL 		= DB_ACTV_HIC;		/* Active Hi complementary mode (EPWMxB is inverted)*/
	(*ePWM[n]).DBCTL.bit.POLSEL 		= DB_ACTV_LOC;		/* Active Hi complementary mode (EPWMxA is inverted)*/	//20171026
	(*ePWM[n]).DBCTL.bit.IN_MODE 		= DBA_ALL; 			/* 2A for Rising Falling */
	(*ePWM[n]).DBRED 					= deadband_rising;	/* Delay at Rising edge*/
	(*ePWM[n]).DBFED 					= deadband_falling;	/* Delay at Falling edge*/
	/* PWM-Chopper Control Register */
	(*ePWM[n]).PCCTL.all				= CHPEN_DISABLE;

	/* 	---------------------- End Init EPWM n --------------------------------------------*/
	/* 	---------------------- Init EPWM n+1 ----------------------------------------------*/
	(*ePWM[n+1]).TBPRD 		   			= period;     		/* Set timer period */
	(*ePWM[n+1]).CMPA.half.CMPA 		= 1000;
	(*ePWM[n+1]).TBCTL.bit.FREE_SOFT 	= FREE_RUN_FLAG;
	(*ePWM[n+1]).TBCTL.bit.PHSDIR 		= TB_UP;
	(*ePWM[n+1]).TBCTL.bit.CLKDIV 		= CLKDIV_PRESCALE_X_1;	/* SYSCLKOUT / 1*/
	(*ePWM[n+1]).TBCTL.bit.HSPCLKDIV 	= HSPCLKDIV_PRESCALE_X_1;	/* Clock ratio to SYSCLKOUT*/
	(*ePWM[n+1]).TBCTL.bit.SYNCOSEL  	= TB_SYNC_IN;		/* Sync with Sync in signal*/
	(*ePWM[n+1]).TBCTL.bit.PRDLD 		= TB_IMMEDIATE;		/* Shadow load for PRD*/
	(*ePWM[n+1]).TBCTL.bit.PHSEN    	= TB_ENABLE;      	/* Disable phase loading*/
	(*ePWM[n+1]).TBCTL.bit.CTRMODE  	= TB_COUNT_UPDOWN;	/* Symmetric wave*/
	(*ePWM[n+1]).TBPHS.half.TBPHS 		= 0;
	(*ePWM[n+1]).TBCTR 					= 0x0000;                      // Clear counter
	/* Setup shadowing*/
	(*ePWM[n+1]).CMPCTL.bit.SHDWAMODE 	= CC_SHADOW;		/* Shadow Mode*/
	(*ePWM[n+1]).CMPCTL.bit.SHDWBMODE 	= CC_SHADOW;		/* Shadow Mode*/
	(*ePWM[n+1]).CMPCTL.bit.LOADAMODE 	= CC_CTR_ZERO;      /* Load A on PRD*/
	(*ePWM[n+1]).CMPCTL.bit.LOADBMODE 	= CC_CTR_ZERO; 		/* Load B on Zero*/
	/* Set Actions */
	(*ePWM[n+1]).AQCTLA.bit.CAD 		= AQ_SET;
	(*ePWM[n+1]).AQCTLA.bit.CAU			= AQ_CLEAR;
	(*ePWM[n+1]).AQCTLA.bit.ZRO 		= AQ_NO_ACTION;
	/* Action-Qualifier Continuous Software Force Register */
	(*ePWM[n+1]).AQCSFRC.bit.CSFA		= CSFA_LOW;
	(*ePWM[n+1]).AQCSFRC.bit.CSFB		= CSFB_HIGH;
    /* Init Action Qualifier S/W Force Register for EPWM1-EPWM2*/
    (*ePWM[n+1]).AQSFRC.all 			= AQSFRC_INIT_STATE;
	/* Set Dead Band for PWM1*/
	(*ePWM[n+1]).DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;	/* Rising Delay on 1A & Falling Delay on 1B*/
//	(*ePWM[n+1]).DBCTL.bit.POLSEL 		= DB_ACTV_HIC;		/* Active high complementary mode (EPWMxB is inverted)*/
	(*ePWM[n+1]).DBCTL.bit.POLSEL 		= DB_ACTV_LOC;		/* Active high complementary mode (EPWMxA is inverted)*/	//20171026
	(*ePWM[n+1]).DBCTL.bit.IN_MODE 		= DBA_ALL; 			/* 1A for Rising& Falling*/
	(*ePWM[n+1]).DBRED 					= deadband_rising;	/* Delay at Rising edge*/
	(*ePWM[n+1]).DBFED 					= deadband_falling;	/* Delay at Falling edge*/
	/* PWM-Chopper Control Register */
	(*ePWM[n+1]).PCCTL.all				= CHPEN_DISABLE;

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0x1;

	//
	// Enable PWM1-2 on GPIO0-GPIO3
	//
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   // Enable pullup on GPIO0
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   // Enable pullup on GPIO1
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   // Enable pullup on GPIO2
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   // Enable pullup on GPIO3

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  // GPIO0 = PWM1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  // GPIO1 = PWM1B
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  // GPIO2 = PWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  // GPIO3 = PWM2B

    //
    // Enable Trip Zone inputs on GPIO12
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pullup on GPIO12
    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3; // asynch input
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // GPIO12 = TZ1

    // CPU Halt Trip
    /* Init Trip Zone Select Register*/
    (*ePWM[n]).TZSEL.all = TZSEL_INIT_STATE;
    (*ePWM[n+1]).TZSEL.all = TZSEL_INIT_STATE;

    // What do we want the OST/CBC events to do?
    // TZA events can force EPWMxA
    // TZB events can force EPWMxB
    /* Init Trip Zone Control Register*/
    (*ePWM[n]).TZCTL.all = TZCTL_INIT_STATE;
    (*ePWM[n+1]).TZCTL.all = TZCTL_INIT_STATE;
	(*ePWM[n]).TZFRC.bit.OST = 1;			// Turn off the PWM
	(*ePWM[n+1]).TZFRC.bit.OST = 1;			// Turn off the PWM

    // Clear any spurious OV trip
	// Enable TZ interrupt
    (*ePWM[n]).TZCLR.all = 0x0005;
    (*ePWM[n+1]).TZCLR.all = 0x0005;

    /* Enable TZ interrupt */
    (*ePWM[n]).TZEINT.bit.OST = 1;

// Enable CNT_zero interrupt using EPWM1 Time-base
	(*ePWM[n]).ETSEL.bit.INTEN = 1;   // Enable EPWM1INT generation
	(*ePWM[n]).ETSEL.bit.INTSEL = 1;  // Enable interrupt CNT_zero event
	(*ePWM[n]).ETPS.bit.INTPRD = 1;   // Generate interrupt on the 1st event
	(*ePWM[n]).ETCLR.bit.INT = 1;     // Enable more interrupts

    EDIS;                         /* Disable EALLOW*/

}




void PWM_Inv_SPWM_unipolar_Update(Uint16 n, Uint16 p, int32 v)
{

	if ( _IQ24(v) >= _IQ24(0.0))
	{
		(*ePWM[n]).CMPA.half.CMPA	= 0 ;
		(*ePWM[n+1]).CMPA.half.CMPA	= _IQ15mpy(_IQ15(p),_IQtoIQ15(_IQ24abs(v)))>>15;
	}
	else
	{
		(*ePWM[n]).CMPA.half.CMPA	= _IQ15mpy(_IQ15(p),_IQtoIQ15(_IQ24abs(v)))>>15;
		(*ePWM[n+1]).CMPA.half.CMPA	= 0 ;
	}
}

void F280X3_PWM_Open(void)
{
	EALLOW;
	//Enable Interrupt clear fault flag
	EPwm1Regs.TZCLR.bit.OST				= 0x1;
	EPwm1Regs.TZCLR.bit.INT 			= 0x1;
	EPwm2Regs.TZCLR.bit.OST				= 0x1;
	EPwm2Regs.TZCLR.bit.INT 			= 0x1;
	EDIS;
	// Forcing disabled, i.e., has no effect
	EPwm1Regs.AQCSFRC.all = CSFA_DISABLE + CSFB_DISABLE;
	EPwm2Regs.AQCSFRC.all = CSFA_DISABLE + CSFB_DISABLE;

}

void F280X3_PWM_Close(void)
{
	// Froce not active level
	EPwm1Regs.AQCSFRC.all = CSFA_LOW + CSFB_HIGH;
	EPwm2Regs.AQCSFRC.all = CSFA_LOW + CSFB_HIGH;
}



void PWM_1ch_Fan_CNF(Uint16 n,Uint16 period,Uint16 deadband_rising,Uint16 deadband_falling)
{
	EALLOW;

	//Time Base SubModule Register
	(*ePWM[n]).TBCTL.bit.PRDLD = TB_IMMEDIATE;	// set Immediate load
	(*ePWM[n]).TBPRD= period - 1;
	(*ePWM[n]).TBPHS.half.TBPHS = 0;
	(*ePWM[n]).TBCTR = 0;
	(*ePWM[n]).TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	(*ePWM[n]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[n]).TBCTL.bit.CLKDIV = TB_DIV1;
	(*ePWM[n]).TBCTL.bit.PHSEN = TB_DISABLE;
	(*ePWM[n]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // sync "down-stream"

	// Counter Compare Submodule Registers
	(*ePWM[n]).CMPA.half.CMPA = 0; // set duty 0% initially
	(*ePWM[n]).CMPB = 0; // set duty 0% initially

	// Counter compare submodule registers
	(*ePWM[n]).CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	(*ePWM[n]).CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
	(*ePWM[n]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	(*ePWM[n]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	// Action Qualifier SubModule Registers
	(*ePWM[n]).AQCTLA.bit.ZRO = AQ_SET;
	(*ePWM[n]).AQCTLA.bit.CAU = AQ_CLEAR;

	(*ePWM[n]).AQCTLB.bit.ZRO = AQ_NO_ACTION;
	(*ePWM[n]).AQCTLB.bit.CAU = AQ_NO_ACTION;
	(*ePWM[n]).AQCTLB.bit.PRD = AQ_NO_ACTION;

	// Deadband SubModule Registers
	(*ePWM[n]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //Both rising and falling edge
	(*ePWM[n]).DBCTL.bit.POLSEL = DB_ACTV_HIC;	// Active high complementary
	(*ePWM[n]).DBFED = deadband_rising;						// Deadtime is 2us
	(*ePWM[n]).DBRED = deadband_falling;


	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   // Enable pullup on GPIO4
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   // Enable pullup on GPIO5
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;  // GPIO4 = PWM3A
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;  // GPIO5 = PWM3B

	EDIS;
}

void PWM_1ch_Fan_Update(Uint16 n, Uint16 frequency , Uint32 duty)
{

	(*ePWM[n]).CMPA.half.CMPA = _IQ15mpy(_IQ15(frequency), _IQtoIQ15(duty))>>15;

}
//===========================================================================
// End of file.
//===========================================================================
