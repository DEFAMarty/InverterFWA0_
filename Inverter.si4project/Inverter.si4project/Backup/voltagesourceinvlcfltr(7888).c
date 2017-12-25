/* ==============================================================================
System Name:	Voltage source inverter

File Name:	  	voltagesourceinvlcfltr.c

Target:			F28035

Author:			Ruson.Su, 5/23/2017

Description:	Project implements output voltage control i.e. a voltage source inverter.

 	 	 	 	Current and voltages on the design are sensed using the SDFM
 	 	 	 	or ADC selectable through the powerSUITE page.

				The Inverter ISR rate is 20Khz and is triggered by EPWM1 when ADC sensing is used,
				Of by EPWM 11 when SD reading is used , this is because EPWM11 provides periodic
				sync resets to the SDFM module and after each reset the SDFM data is ready after
				3 OSRs and needs to be read immediately.

				following is the decription of other files that are used by this system
				<solution name>.c -> This file, which has the main.c of the project
				<solution name>.h -> This is the main header file for the project
				<solution name>_settings.h -> powerSUITE generated settings
				<boad name>_board.c -> This is the file that is used to keep common functions
									   related to the board hardware like setting up the PWM, ADC, SDFM,
									   reading data and common variables liked current, voltage etc.

Copyright (C) {2017} Defa Incorporated - http://www.defa.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */

//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped using
// the linker cmd file.
#ifdef FLASH
#pragma CODE_SECTION(InverterISR, "ramfuncs");
#endif




float32 Ts = 0.001 / ISR_CONTROL_FREQUENCY;    // Samping period (sec)

Uint16 IsrTicker = 0;
Uint16 BackTicker = 0;
Uint16 TZ1Count = 0;
Uint16 gu16_TZFaultFlag = 0;

// SFRA lib Object
volatile SFRA_IQ sfra1;

//extern to access tables in ROM
extern long FPUsinTable[];

//--------------------------------- SFRA Related Variables ----------------------------
int32 Plant_MagVect[SFRA_FREQ_LENGTH];
int32 Plant_PhaseVect[SFRA_FREQ_LENGTH];
int32 OL_MagVect[SFRA_FREQ_LENGTH];
int32 OL_PhaseVect[SFRA_FREQ_LENGTH];
float32 FreqVect[SFRA_FREQ_LENGTH];

int16	SerialCommsTimer;
int16	CommsOKflg;
//Flag for reinitializing SFRA variables
int16 initializationFlag;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DECLARATIONS - CCS WatchWindow / GUI support
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// -------------------------------- FRAMEWORK --------------------------------------
//GUI support variables
// sets a limit on the amount of external GUI controls - increase as necessary
int16 *varSetTxtList[16]; //16 textbox controlled variables
int16 *varSetBtnList[16]; //16 button controlled variables
int16 *varSetSldrList[16]; //16 slider controlled variables
int16 *varGetList[16]; //16 variables sendable to GUI
int32 *arrayGetList[16]; //16 arrays sendable to GUI
Uint32 *dataSetList[16]; //16 32-bit textbox or label controlled variables


// for tuning PR and R controllers
float32 fs;
float32 kpV_1H;
float32 kiV_1H, kiV_3H, kiV_5H, kiV_7H;
float32 woF_1H, woF_3H, woF_5H, woF_7H;
float32 wrcV_1H, wrcV_3H, wrcV_5H, wrcV_7H;
float32 gain, z0;

int16 wSumCnt;

// Reference variables
// Inverter modulation index, used for open loop check

volatile Uint16 EnableFlag = FALSE;

// Instance a enable PWM drive driver
DRIVEStruct drv1 = DRIVE_DEFAULTS;

// Create an instance of bipolar ADC driver Module
ADCVALStruct adc1 = ADCVAL_DEFAULTS;

// RAMP to generate forced angle when grid is not present
RAMPGEN_IQ rgen1;

// RAMP to generate forced angle when grid is not present
SOFTSTARTGENStruct ssgen1;

// Sine analyzer block for RMS Volt, Curr and Power measurements
SINEANALYZER_DIFF_wPWR_IQ sine_mains;

// Instance PID regulators to regulate the voltage/currents.
PIDREG3 pid1 = PIDREG3_DEFAULTS;

// CNTL 3p3z for inverter current control
#pragma DATA_SECTION(cntl3p3z_InvI_vars,"cntl_var_RAM")
#pragma DATA_SECTION(cntl3p3z_InvI_coeff,"cntl_coeff_RAM")
CNTL_3P3Z_IQ_VARS cntl3p3z_InvI_vars;
CNTL_3P3Z_IQ_COEFFS cntl3p3z_InvI_coeff;

//Proportional Resonant controller for the fundamental voltage
#pragma DATA_SECTION(cntl2p2z_PRV_vars,"cntl_var_RAM")
#pragma DATA_SECTION(cntl2p2z_PRV_coeff,"cntl_coeff_RAM")
CNTL_2P2Z_IQ_VARS cntl2p2z_PRV_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_PRV_coeff;

//Resonant controller for the 3rd harmonics voltage
#pragma DATA_SECTION(cntl2p2z_RV3_vars,"cntl_var_RAM2")
#pragma DATA_SECTION(cntl2p2z_RV3_coeff,"cntl_coeff_RAM2")
CNTL_2P2Z_IQ_VARS cntl2p2z_RV3_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV3_coeff;

//Resonant controller for the 5th harmonic voltage
#pragma DATA_SECTION(cntl2p2z_RV5_vars,"cntl_var_RAM")
#pragma DATA_SECTION(cntl2p2z_RV5_coeff,"cntl_coeff_RAM")
CNTL_2P2Z_IQ_VARS cntl2p2z_RV5_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV5_coeff;

//Resonant controller for the 7th harmonics voltage
#pragma DATA_SECTION(cntl2p2z_RV7_vars,"cntl_var_RAM")
#pragma DATA_SECTION(cntl2p2z_RV7_coeff,"cntl_coeff_RAM")
CNTL_2P2Z_IQ_VARS cntl2p2z_RV7_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV7_coeff;

//Resonant controller for the 9th harmonics voltage
#pragma DATA_SECTION(cntl2p2z_RV9_vars,"cntl_var_RAM2")
#pragma DATA_SECTION(cntl2p2z_RV9_coeff,"cntl_coeff_RAM2")
CNTL_2P2Z_IQ_VARS cntl2p2z_RV9_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV9_coeff;

//Resonant controller for the 11th harmonics voltage
#pragma DATA_SECTION(cntl2p2z_RV11_vars,"cntl_var_RAM2")
#pragma DATA_SECTION(cntl2p2z_RV11_coeff,"cntl_coeff_RAM2")
CNTL_2P2Z_IQ_VARS cntl2p2z_RV11_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_RV11_coeff;

// Lead lag compensator to improve phase margin
#pragma DATA_SECTION(cntl2p2z_LeadLag_vars,"cntl_var_RAM")
#pragma DATA_SECTION(cntl2p2z_LeadLag_coeff,"cntl_coeff_RAM")
CNTL_2P2Z_IQ_VARS cntl2p2z_LeadLag_vars;
CNTL_2P2Z_IQ_COEFFS cntl2p2z_LeadLag_coeff;



//TODO Main
void main(void)
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																					{
//=================================================================================
//	INITIALISATION - General
//=================================================================================

//
// Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2803x_SysCtrl.c file.
//
    InitSysCtrl();

#ifdef FLASH

// Copy time critical code and Flash setup code to RAM
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the linker files.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
	InitFlash();	// Call the flash wrapper init function

#endif //(FLASH)

	MemCopy(&IQmathLoadStart, &IQmathLoadEnd, &IQmathRunStart);

// Initialize GPIO:
// This example function is found in the DSP2803x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
//
	drv1.init(&drv1);

//
// Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
	DINT;

//
// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2803x_PieCtrl.c file.
//
	InitPieCtrl();

//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
	IER = 0x0000;
	IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2803x_DefaultIsr.c.
// This function is found in DSP2803x_PieVect.c.
//
	InitPieVectTable();

//
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
//
	EALLOW;	// This is needed to write to EALLOW protected registers
	PieVectTable.EPWM1_INT = &InverterISR;
	PieVectTable.TINT0 = &CPUTimer0ISR;
	PieVectTable.EPWM1_TZINT = &TZ1ISR;
	PieVectTable.I2CINT1A = &I2CInt1AISR;
	EDIS;

//-------------------------------- FRAMEWORK --------------------------------------
// Only used if running from FLASH
// Note that the variable FLASH is defined by the compiler with -d FLASH
	PRControlInitialize();

// Configure PWM3 for 25Khz switching Frequency
	PWM_1ch_Fan_CNF(3,1200,5,5);	// ePWM3A ePWM3B

// Initialize PWM module for SPWM
	gi16_CarrierPeriod = SYSTEM_FREQUENCY * 1000000 * Ts / 2;  // Prescaler X1 (T1), ISR period = T x 1
	PWM_Inv_SPWM_unipolar_CNF(1,gi16_CarrierPeriod,14,14);

// Configure CPU-Timer 0 to interrupt tasks.
// 60MHz CPU Freq, 0.1 msecond Period (in uSeconds)
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, SYSTEM_FREQUENCY, T0_PERIOD_SET);
	StartCpuTimer0();

// Initialize ADC module
	adc1.init(&adc1);

// Initialize GPIO:
	drv1.init(&drv1);

//RAMPGEN
	RAMPGEN_IQ_init(&rgen1);

//SOFTSTARTGEN
	SoftstartGenInit(&ssgen1);

// Initialize SPI module
	SpiInit();


// Initialize I2C module
	I2CAInit();

// 60000000 is the LSPCLK or the Clock used for the SCI Module
// 57600 is the Baudrate desired of the SCI module
	SCIA_Init(60000000, 57600);

	CommsOKflg = 0;
	SerialCommsTimer = 0;

//================================
	//"Set" variables
	//assign GUI Buttons to desired flag addresses
	varSetBtnList[0] = (int16*)&initializationFlag;

	//"Get" variables
	//---------------------------------------
	// assign a GUI "getable" parameter address
	varGetList[0] = (int16*)&(sfra1.Vec_Length);			//int16
	varGetList[1] = (int16*)&(sfra1.status);			    //int16
	varGetList[2] = (int16*)&(sfra1.FreqIndex);				//int16

	//"Setable" variables
	//----------------------------------------
	// assign GUI "setable" by Text parameter address
	dataSetList[0] = (Uint32*)&(sfra1.Freq_Start);      //Float32
	dataSetList[1] = (Uint32*)&(sfra1.amplitude);	   //Int32
	dataSetList[2] = (Uint32*)&(sfra1.Freq_Step);	   //Float32

	// assign a GUI "getable" parameter array address
	arrayGetList[0] = (int32*)FreqVect;			        //Float32
	arrayGetList[1] = (int32*)OL_MagVect;			    //
	arrayGetList[2] = (int32*)OL_PhaseVect;		        //
	arrayGetList[3] = (int32*)Plant_MagVect;			//
	arrayGetList[4] = (int32*)Plant_PhaseVect;			//
	arrayGetList[5] = (int32*)&(sfra1.Freq_Start);      //Float 32
	arrayGetList[6] = (int32*)&(sfra1.amplitude);	   //Int32
	arrayGetList[7] = (int32*)&(sfra1.Freq_Step);	   //Float32
//==================================================

// Enable CPU INT3 for EPWM1_INT,Enable CPU INT1 for CPU-Timer:0, CPU INT2 for TZ1, CPU INT8 for I2C;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;     // Enable PIE group 3 interrupt 1 for EPWM1_INT.
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;     // Enable PIE Group 1 interrupt 7 for TINT0.
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;     // Enable PIE group 2 interrupt 1 for TZ1_INT.
	PieCtrlRegs.PIEIER8.bit.INTx1 = 1;     // Enable I2C interrupt 1 in the PIE: Group 8 interrupt 1
	IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT8) ;

	SetOffset(&adc1);     // Calculate offset values.



// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM

	//sine analyzer initialization
	SINEANALYZER_DIFF_wPWR_IQ_init(&sine_mains);
	sine_mains.SampleFreq = _IQ15(ISR_FREQUENCY);
	sine_mains.Threshold = _IQ15(0.001);
//	sine_mains.nsamplesMax = _IQ15div(ISR_FREQUENCY, UNIVERSAL_GRID_MAX_FREQ);
//	sine_mains.nsamplesMin = _IQ15div(ISR_FREQUENCY, UNIVERSAL_GRID_MIN_FREQ);
	sine_mains.nsamplesMax = _IQmpy(_IQdiv((1.0),(UNIVERSAL_GRID_MAX_FREQ)) ,(ISR_FREQUENCY));//181@60Hz,20kHz;
	sine_mains.nsamplesMin = _IQmpy(_IQdiv((1.0),(UNIVERSAL_GRID_MIN_FREQ)) ,(ISR_FREQUENCY));//153@60Hz,20kHz;

    initGlobalVariables();

	invVoRefCal = _IQmpy(_IQdiv(VAC_TYPICAL, VAC_SCALE_MAX_SENSE), _IQ(SQRT2));

	setupSFRA();

// IDLE loop. Just sit and loop forever, periodically will branch into A0-A3, B0-B3, C0-C3 tasks
// Frequency of this brnaching is set in setupDevice routine:
	while(1)
	{

		BackTicker++;

//		if(0 == GetCANTaskCount())     // CAN Send and Receive.
//		{
//			ResetCANTaskCount();
//		}

		if(0 == GetVBusTaskCount())     // Calculate bus voltage.
		{
			ResetVBusTaskCount();
			CalHVDCValue(&adc1);
			CalBATValue(&adc1);


		}

		if(0 == GetHeatSinkTempTaskCount())     // Calculate HeatSinkTemp voltage.
		{
			ResetHeatSinkTempTaskCount();
			adc1.TRANS1TempOut = CalHeatSinkTempValue(NTC_TEMP_TRANS1_D_VALUE);
			adc1.HS1TempOut = CalHeatSinkTempValue(NTC_TEMP_HS1_D_VALUE);
			adc1.TRANS2TempOut = CalHeatSinkTempValue(NTC_TEMP_TRANS2_D_VALUE);
			adc1.HS2TempOut = CalHeatSinkTempValue(NTC_TEMP_HS2_D_VALUE);
			Fan_CTRL_Update();

		}

		if(0 == GetSystemManageTaskCount())     // Manage system's fault, command and states.
		{
			ResetSystemManageTaskCount();
			SystemManage();
		}

		if(0 == GetFaultManageTaskCount())     // Manage system's fault, command and states.
		{
			ResetFaultManageTaskCount();
			FaultManage();
		}

		if(0 == GetIOManageTaskCount())     // Manage the IO port of hardware.
		{
			ResetIOManageTaskCount();
			drv1.update(&drv1);
		}

		if(0 == GetSpiManageTaskCount())     //transfer data to DAC.
		{
			ResetSpiManageTaskCount();
			SpiManage();
		}

		if(0 == GetSciManageTaskCount())     //transfer data to DAC.
		{
			ResetSciManageTaskCount();
			SerialHostComms();

			//============================================
			if(1 == initializationFlag )
			{
				SFRA_IQ_INIT(&sfra1);
				initializationFlag = 0;
				sfra1.start = 1;
			}

			SFRA_IQ_BACKGROUND(&sfra1);
		}

		if(0 == GetReadI2CTaskCount())     // Manage EEPROM data.
		{
			ResetReadI2CTaskCount();
			// judge hardware fault
			if((0 != gu16_TZFaultFlag) || (0 != SystemFalutFlag.FatalUnion.All))
			{
				I2CWritedatatoEEPROMsection();
			}
			else
			{
				I2CReaddatafromEEPROMsection();
			}
		}
	}
}



int32 Test = 0;

//TODO Inverter ISR Code
interrupt void InverterISR(void)
{
	// Verifying the ISR
	IsrTicker++;

	if (1 == ClearInvTrip)
	{
		EALLOW;
		EPwm1Regs.TZCLR.bit.OST = 0x1;
		EPwm2Regs.TZCLR.bit.OST = 0x1;
		EDIS;
		ClearInvTrip = 0;
	}

	DStateLedToggle();

	// Read ADC module
	F2803XADCRead(&adc1);

	CalACValue(&adc1);

	CalACNoFilterValue(&adc1);

	if (1 == drv1.EnableFlag)
	{
		// Generate the RAMP Angle
		rgen1.Freq = _IQ24(AC_FREQ);
		rgen1.StepAngleMax = _IQ24(Ts);
		RAMPGEN_IQ_FUNC(&rgen1);
	}
	else
	{
		// Generate the RAMP Angle
		rgen1.Freq = _IQ24(0);
		rgen1.StepAngleMax = _IQ24(Ts);
		RAMPGEN_IQ_init(&rgen1);
	}

	invSine = _IQsinPU(rgen1.Out);

	detectZeroCrossingClearPWMTrip();

//	if ((1 == SystemFalutFlag.FatalUnion.bit.lv_vhdc_sw) || (1 == SystemFalutFlag.FatalUnion.bit.ov_vhdc_sw))
//	{
//		invIiRefInst = invIiRef;
//	}
//	else
//	{
//		invIiRefInst = _IQmpy(invIiRef, invSine);
//	}

	if (0 == drv1.EnableFlag)
	{
		gu16_TZFaultFlag = 0;
	}

	// Normal Input with VoRef
	invVoRefInst = _IQmpy(invSine, invVoRef);

	if(1 == closeILoopInv)
	{

//		// SFRA Input with VoRef
//		invVoRefInst = SFRA_IQ_INJECT(invVoRefInst);

		#if(PRCONTROL == 1)

		cntl2p2z_PRV_vars.Ref = invVoRefInst;
		cntl2p2z_PRV_vars.Fdbk = invVoInst;
		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_PRV_coeff,&cntl2p2z_PRV_vars);

//		cntl2p2z_RV3_vars.Ref = invVoRefInst;
//		cntl2p2z_RV3_vars.Fdbk = invVoInst;
//		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_RV3_coeff,&cntl2p2z_RV3_vars);

//		cntl2p2z_RV5_vars.Ref = invVoRefInst;
//		cntl2p2z_RV5_vars.Fdbk = invVoInst;
//		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_RV5_coeff,&cntl2p2z_RV5_vars);

//		cntl2p2z_RV7_vars.Ref = invVoRefInst;
//		cntl2p2z_RV7_vars.Fdbk = invVoInst;
//		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_RV7_coeff,&cntl2p2z_RV7_vars);

//		cntl2p2z_RV9_vars.Ref = invVoRefInst;
//		cntl2p2z_RV9_vars.Fdbk = invVoInst;
//		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_RV9_coeff,&cntl2p2z_RV9_vars);

//		cntl2p2z_LeadLag_vars.Ref = cntl2p2z_PRV_vars.Out + cntl2p2z_RV3_vars.Out + cntl2p2z_RV5_vars.Out + cntl2p2z_RV7_vars.Out;
//		cntl2p2z_LeadLag_vars.Ref = cntl2p2z_PRV_vars.Out ;

//		cntl2p2z_LeadLag_vars.Fdbk = 0;
//		CNTL_2P2Z_IQ_FUNC(&cntl2p2z_LeadLag_coeff,&cntl2p2z_LeadLag_vars);
//		invIiRefInst = cntl2p2z_LeadLag_vars.Out;

		invIiRefInst = cntl2p2z_PRV_vars.Out ;

		cntl3p3z_InvI_vars.Ref = invIiRefInst;
		cntl3p3z_InvI_vars.Fdbk = invIiInst;
		CNTL_3P3Z_IQ_FUNC(&cntl3p3z_InvI_coeff, &cntl3p3z_InvI_vars);

		invDutyPU = _IQdiv((cntl3p3z_InvI_vars.Out + invVoInst), invVbusInst);

		#endif
		
		#if(PICONTROL == 1)

		// PID Control for voltage loop
		pid1.Ref = invVoRefInst;
		pid1.Fdb = invVoInst;
		pid1.calc(&pid1);

		invDutyPU = pid1.Out;

//		// voltage Open loop
//		invDutyPU = invVoRefInst;
		
		#endif

		invDutyPU = (invDutyPU > _IQ(0.98)) ? _IQ(0.98) : invDutyPU;
		invDutyPU = (invDutyPU < _IQ(-0.98)) ? _IQ(-0.98) : invDutyPU;

	}
	else
	{
		invDutyPU = 0;
	}


	PWM_Inv_SPWM_unipolar_Update(1,gi16_CarrierPeriod,invDutyPU);

	invDutyPUPrev = invDutyPU;

//	guiVbus = _IQ15mpy(invVbusInst, VHVDC_SCALE_MAX_SENSE);
//	guiVo = _IQ15mpy(invVoInst, VAC_SCALE_MAX_SENSE);
//	guiIi = _IQ15mpy(invIiInst, IAC_SCALE_MAX_SENSE);

	sine_mains.Iin = _IQtoIQ15(invIiInst);
	sine_mains.Vin = _IQtoIQ15(invVoInst);
	SINEANALYZER_DIFF_wPWR_IQ_FUNC(&sine_mains);

	guiIrms = _IQ15mpy(sine_mains.Irms, _IQ15(IAC_SCALE_MAX_SENSE));
	guiVrms = _IQ15mpy(sine_mains.Vrms, _IQ15(VAC_SCALE_MAX_SENSE));
	guiPrms = _IQ15mpy(sine_mains.Prms, _IQ15(IAC_SCALE_MAX_SENSE * VAC_SCALE_MAX_SENSE));

//	//Running FRA on Open loop
//	SFRA_IQ_COLLECT(&invDutyPU,&invIiInst);

//	//Running FRA on Current
//	SFRA_IQ_COLLECT(&invIiRefInst,&invIiInst);

//	//Running FRA on Voltage
//	SFRA_IQ_COLLECT(&cntl2p2z_PRV_vars.Out,&cntl2p2z_PRV_vars.Fdbk);



	// Enable more interrupts from this timer
	EPwm1Regs.ETCLR.bit.INT = 1;

	// Acknowledge interrupt to recieve more interrupts from PIE group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}// MainISR Ends Here



interrupt void TZ1ISR(void)
{
	TZ1Count++;
	gu16_TZFaultFlag = 1;
	RunReadI2CTaskImmediately();			//Launch read hardware fault process, the I2C interrupt will finish the rest read code



	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}


void Fan_CTRL_Update(void)
{
	// Fan control by temperature
	if((adc1.TRANS1TempOut < _IQ16(40)) && (adc1.HS1TempOut < _IQ16(40)) && (adc1.TRANS2TempOut < _IQ16(40)) && (adc1.HS2TempOut < _IQ16(40)))
	{
		PWM_1ch_Fan_Update(3,1200,0);
	}
	else if((adc1.TRANS1TempOut < _IQ16(50)) && (adc1.HS1TempOut < _IQ16(50)) && (adc1.TRANS2TempOut < _IQ16(50)) && (adc1.HS2TempOut < _IQ16(50)))
	{
		PWM_1ch_Fan_Update(3,1200,4194304);		// _IQ24(0.25) = 4194304		0x00400000

	}
	else if((adc1.TRANS1TempOut < _IQ16(65)) && (adc1.HS1TempOut < _IQ16(65)) && (adc1.TRANS2TempOut < _IQ16(65)) && (adc1.HS2TempOut < _IQ16(65)))
	{
		PWM_1ch_Fan_Update(3,1200,8388608);		// _IQ24(0.5) = 8388608			0x00800000
	}
	else if((adc1.TRANS1TempOut < _IQ16(85)) && (adc1.HS1TempOut < _IQ16(85)) && (adc1.TRANS2TempOut < _IQ16(85)) && (adc1.HS2TempOut < _IQ16(85)))
	{
		PWM_1ch_Fan_Update(3,1200,12582912);		// _IQ24(0.75) = 12582912			0x00C00000

	}
	else
	{
		PWM_1ch_Fan_Update(3,1200,16777216);		// _IQ24(1.00) = 16777216			0x01000000
	}
}





//TODO detectZeroCrossingClearPWMTrip()
void detectZeroCrossingClearPWMTrip(void)
{
	if((invSinePrev <= _IQ24(0.00)) && (invSine > _IQ24(0.00)))

		zeroCrossDetectFlag = 1;
	else

		zeroCrossDetectFlag = 0;

	if (1 == clearInvTrip && 1 == zeroCrossDetectFlag)
	{
		// clear all the configured trip sources for the PWM module
		EALLOW;
		EPwm1Regs.TZCLR.bit.OST = 0x1;
		EPwm2Regs.TZCLR.bit.OST = 0x1;
		EDIS;

		clearInvTrip = 0;
		closeILoopInv = 1;
	}

	if((invSinePrev < _IQ2(-0.95)) && (invSine > _IQ2(-0.95)))
	{
		if(1 == rlyConnect)
		{
//			closeRelay();
		}
	}

	if (0 == rlyConnect)
	{
//		openRelay();
	}

	invSinePrev = invSine;

}





/*------------------------------------------------------------------------
 * Function Name: sRMSCal
 * Description: Calculate the RMS value of the AC sample
 * Parameter: dwSum --- The squre sum of the AC sample
 * 			  dwRatio -- Q24 * dwRatio = Real value
 * 			  wFormat -- The float point
 * -----------------------------------------------------------------------*/
int16    sRMSCal(long dwSum,long dwRatio,int wFormat)
{
	_iq24 sqrt_in,sqrt_out;
	long  dwTemp;
	int16 wTemp;

	sqrt_in = (long)(dwSum/wSumCnt);
	sqrt_out = _IQ24sqrt(sqrt_in);
	dwTemp = _IQ7mpyIQX(sqrt_out,24,dwRatio,18);
	wTemp = (int16)((long)dwTemp*wFormat>>7);
	return wTemp;
}

/*------------------------------------------------------------------------
 * Function Name:sAverageVoltCal
 * Description: Calculate the DC bus voltage average value.
 *
 * -----------------------------------------------------------------------*/
 int16   sAverageVoltCal(long dwSum,long dwRatio,int wFormat)
 {
 	_iq21 ave_in;
	long  dwTemp;
	int16 wTemp;

	ave_in = (long)(dwSum/wSumCnt);
	dwTemp = _IQ7mpyIQX(ave_in,21,dwRatio,18);
	wTemp = (int16)((long)dwTemp*wFormat>>7);
	return wTemp;
 }

 /*------------------------------------------------------------------------
 * Function Name:sActivePowerCal
 * Description: Calculate the active power
 *
 * -----------------------------------------------------------------------*/
 int16   sActivePowerCal(long dwSum,long dwRatio,int wFormat)
 {
 	_iq24 ave_in;
	long  dwTemp;
	int16 wTemp;

	ave_in = (long)(dwSum/wSumCnt);
	dwTemp = _IQ7mpyIQX(ave_in,24,dwRatio,10);
	wTemp = (int16)((long)dwTemp*wFormat>>7);
	return wTemp;
 }



//TODO setupSFRA
void setupSFRA(void)
{
	int16 i;
//===========================================================================
//	SFRA Initialization
//===========================================================================

	//SFRA Related
	//SFRA Object Initialization
	//Specify the injection amplitude
	sfra1.amplitude = SFRA_AMPLITUDE;
	//Specify the length of SFRA
	sfra1.Vec_Length = SFRA_FREQ_LENGTH;
	//Specify the SFRA ISR Frequency
	sfra1.ISR_Freq = SFRA_ISR_FREQ;
	//Specify the Start Frequency of the SFRA analysis
	sfra1.Freq_Start = SFRA_FREQ_START;
	//Specify the Frequency Step
	sfra1.Freq_Step = FREQ_STEP_MULTIPLY;
	//Assign array location to Pointers in the SFRA object
	sfra1.FreqVect = FreqVect;
	sfra1.GH_MagVect = OL_MagVect;
	sfra1.GH_PhaseVect = OL_PhaseVect;
	sfra1.H_MagVect = Plant_MagVect;
	sfra1.H_PhaseVect = Plant_PhaseVect;

	initializationFlag = 0;

	SFRA_IQ_INIT(&sfra1);

	// Re-initialize the frequency array to make SFRA sweep go fast
	i = 0;
	#if SFRA_TYPE == 1
	sfra1.FreqVect[i] = SFRA_FREQ_START;
	for(i++;i<sfra1.Vec_Length;i++)
	{
		/*if(sfra1.FreqVect[i-1]<AC_FREQ-11)
			sfra1.FreqVect[i]=sfra1.FreqVect[i-1]+10;
		else if(sfra1.FreqVect[i-1]<AC_FREQ-2)
			sfra1.FreqVect[i]=sfra1.FreqVect[i-1]+2;
		else if(sfra1.FreqVect[i-1]<AC_FREQ-0.2)
					sfra1.FreqVect[i]=sfra1.FreqVect[i-1]+2;
		else
			sfra1.FreqVect[i]=sfra1.FreqVect[i-1]*sfra1.Freq_Step;*/
	}
	i = 0;
	sfra1.FreqVect[i++]=5;
	sfra1.FreqVect[i++]=10;
	sfra1.FreqVect[i++]=20;
	sfra1.FreqVect[i++]=30;
	sfra1.FreqVect[i++]=40;
	sfra1.FreqVect[i++]=50;
	sfra1.FreqVect[i++]=55;
	sfra1.FreqVect[i++]=56;
	sfra1.FreqVect[i++]=57;
	sfra1.FreqVect[i++]=58;
	sfra1.FreqVect[i++]=58.5;
	sfra1.FreqVect[i++]=58.75;
	sfra1.FreqVect[i++]=59;
	sfra1.FreqVect[i++]=59.2;
	sfra1.FreqVect[i++]=59.4;
	sfra1.FreqVect[i++]=59.6;
	sfra1.FreqVect[i++]=59.8;
	sfra1.FreqVect[i++]=60;
	sfra1.FreqVect[i++]=60.2;
	sfra1.FreqVect[i++]=60.4;
	sfra1.FreqVect[i++]=60.6;
	sfra1.FreqVect[i++]=60.8;
	sfra1.FreqVect[i++]=61;
	sfra1.FreqVect[i++]=61.25;
	sfra1.FreqVect[i++]=61.5;
	sfra1.FreqVect[i++]=62;
	sfra1.FreqVect[i++]=65;
	sfra1.FreqVect[i++]=70;
	sfra1.FreqVect[i++]=90;
	sfra1.FreqVect[i++]=100;
	sfra1.FreqVect[i++]=110;
	sfra1.FreqVect[i++]=120;
	sfra1.FreqVect[i++]=140;
	sfra1.FreqVect[i++]=160;
	sfra1.FreqVect[i++]=170;
	sfra1.FreqVect[i++]=175;
	sfra1.FreqVect[i++]=178;
	sfra1.FreqVect[i++]=178.5;
	sfra1.FreqVect[i++]=179;
	sfra1.FreqVect[i++]=179.2;
	sfra1.FreqVect[i++]=179.4;
	sfra1.FreqVect[i++]=179.6;
	sfra1.FreqVect[i++]=179.8;
	sfra1.FreqVect[i++]=180;
	sfra1.FreqVect[i++]=180.2;
	sfra1.FreqVect[i++]=180.4;
	sfra1.FreqVect[i++]=180.6;
	sfra1.FreqVect[i++]=180.8;
	sfra1.FreqVect[i++]=181;
	sfra1.FreqVect[i++]=182;
	sfra1.FreqVect[i++]=185;
	sfra1.FreqVect[i++]=190;
	sfra1.FreqVect[i++]=200;
	sfra1.FreqVect[i++]=220;
	sfra1.FreqVect[i++]=250;
	sfra1.FreqVect[i++]=280;
	sfra1.FreqVect[i++]=290;
	sfra1.FreqVect[i++]=295;
	sfra1.FreqVect[i++]=298;
	sfra1.FreqVect[i++]=298.5;
	sfra1.FreqVect[i++]=299;
	sfra1.FreqVect[i++]=299.4;
	sfra1.FreqVect[i++]=299.6;
	sfra1.FreqVect[i++]=299.8;
	sfra1.FreqVect[i++]=300;
	sfra1.FreqVect[i++]=300.2;
	sfra1.FreqVect[i++]=300.4;
	sfra1.FreqVect[i++]=300.6;
	sfra1.FreqVect[i++]=301;
	sfra1.FreqVect[i++]=305;
	sfra1.FreqVect[i++]=310;
	sfra1.FreqVect[i++]=330;
	sfra1.FreqVect[i++]=360;
	sfra1.FreqVect[i++]=380;
	sfra1.FreqVect[i++]=390;
	sfra1.FreqVect[i++]=410;
	sfra1.FreqVect[i++]=415;
	sfra1.FreqVect[i++]=418;
	sfra1.FreqVect[i++]=418.5;
	sfra1.FreqVect[i++]=419;
	sfra1.FreqVect[i++]=419.4;
	sfra1.FreqVect[i++]=419.6;
	sfra1.FreqVect[i++]=419.8;
	sfra1.FreqVect[i++]=420;
	sfra1.FreqVect[i++]=420.2;
	sfra1.FreqVect[i++]=420.4;
	sfra1.FreqVect[i++]=420.8;
	sfra1.FreqVect[i++]=421.2;
	sfra1.FreqVect[i++]=422;
	sfra1.FreqVect[i++]=425;
	sfra1.FreqVect[i++]=430;
	sfra1.FreqVect[i++]=450;
	sfra1.FreqVect[i++]=500;
	sfra1.FreqVect[i++]=600;
	sfra1.FreqVect[i++]=700;
	sfra1.FreqVect[i++]=800;
	sfra1.FreqVect[i++]=900;
	sfra1.FreqVect[i++]=1000;
	sfra1.FreqVect[i++]=1100;
	sfra1.FreqVect[i++]=1200;

	#else
	sfra1.FreqVect[i]=SFRA_FREQ_START;
		for(i++;i<sfra1.Vec_Length;i++)
		{
			if(sfra1.FreqVect[i-1]<10)
				sfra1.FreqVect[i]=sfra1.FreqVect[i-1]+2;
			else if(sfra1.FreqVect[i-1]<50)
				sfra1.FreqVect[i]=sfra1.FreqVect[i-1]+4;
			else
				sfra1.FreqVect[i]=sfra1.FreqVect[i-1]*sfra1.Freq_Step;
		}
		/*
	sfra1.FreqVect[i++]=5;
	sfra1.FreqVect[i++]=20;
	sfra1.FreqVect[i++]=40;
	sfra1.FreqVect[i++]=55;
	sfra1.FreqVect[i++]=57;
	sfra1.FreqVect[i++]=58;
	sfra1.FreqVect[i++]=59;
	sfra1.FreqVect[i++]=59.2; //10
	sfra1.FreqVect[i++]=59.4;
	sfra1.FreqVect[i++]=59.6;
	sfra1.FreqVect[i++]=59.8;
	sfra1.FreqVect[i++]=60;
	sfra1.FreqVect[i++]=60.2;
	sfra1.FreqVect[i++]=60.4;
	sfra1.FreqVect[i++]=60.6;
	sfra1.FreqVect[i++]=60.8;
	sfra1.FreqVect[i++]=61;
	sfra1.FreqVect[i++]=64;
	sfra1.FreqVect[i++]=70;
	sfra1.FreqVect[i++]=100;
	sfra1.FreqVect[i++]=130;
	sfra1.FreqVect[i++]=170;
	sfra1.FreqVect[i++]=175;
	sfra1.FreqVect[i++]=177;
	sfra1.FreqVect[i++]=178;
	sfra1.FreqVect[i++]=179;
	sfra1.FreqVect[i++]=179.2;
	sfra1.FreqVect[i++]=179.4;
	sfra1.FreqVect[i++]=179.6;
	sfra1.FreqVect[i++]=179.8;
	sfra1.FreqVect[i++]=180;
	sfra1.FreqVect[i++]=180.2;
	sfra1.FreqVect[i++]=180.4;
	sfra1.FreqVect[i++]=180.6;
	sfra1.FreqVect[i++]=180.8;
	sfra1.FreqVect[i++]=181;
	sfra1.FreqVect[i++]=182;
	sfra1.FreqVect[i++]=185;
	sfra1.FreqVect[i++]=200;
	sfra1.FreqVect[i++]=240;
	sfra1.FreqVect[i++]=280;
	sfra1.FreqVect[i++]=290;
	sfra1.FreqVect[i++]=295;
	sfra1.FreqVect[i++]=297;
	sfra1.FreqVect[i++]=298;
	sfra1.FreqVect[i++]=299;
	sfra1.FreqVect[i++]=299.2;
	sfra1.FreqVect[i++]=299.4;
	sfra1.FreqVect[i++]=299.6;
	sfra1.FreqVect[i++]=299.8;
	sfra1.FreqVect[i++]=300;
	sfra1.FreqVect[i++]=300.2;
	sfra1.FreqVect[i++]=300.4;
	sfra1.FreqVect[i++]=300.6;
	sfra1.FreqVect[i++]=300.8;
	sfra1.FreqVect[i++]=301;
	sfra1.FreqVect[i++]=305;
	sfra1.FreqVect[i++]=310;
	sfra1.FreqVect[i++]=330;
	sfra1.FreqVect[i++]=360;
	sfra1.FreqVect[i++]=400;
	sfra1.FreqVect[i++]=410;
	sfra1.FreqVect[i++]=415;
	sfra1.FreqVect[i++]=417;
	sfra1.FreqVect[i++]=418;
	sfra1.FreqVect[i++]=419;
	sfra1.FreqVect[i++]=419.2;
	sfra1.FreqVect[i++]=419.4;
	sfra1.FreqVect[i++]=419.6;
	sfra1.FreqVect[i++]=419.8;
	sfra1.FreqVect[i++]=420;
	sfra1.FreqVect[i++]=420.2;
	sfra1.FreqVect[i++]=420.4;
	sfra1.FreqVect[i++]=420.6;
	sfra1.FreqVect[i++]=420.8;
	sfra1.FreqVect[i++]=421;
	sfra1.FreqVect[i++]=425;
	sfra1.FreqVect[i++]=440;
	sfra1.FreqVect[i++]=490;
	sfra1.FreqVect[i++]=530;
	sfra1.FreqVect[i++]=535;
	sfra1.FreqVect[i++]=537;
	sfra1.FreqVect[i++]=538;
	sfra1.FreqVect[i++]=539;
	sfra1.FreqVect[i++]=539.2;
	sfra1.FreqVect[i++]=539.4;
	sfra1.FreqVect[i++]=539.6;
	sfra1.FreqVect[i++]=539.8;
	sfra1.FreqVect[i++]=540;
	sfra1.FreqVect[i++]=540.2;
	sfra1.FreqVect[i++]=540.4;
	sfra1.FreqVect[i++]=540.6;
	sfra1.FreqVect[i++]=540.8;
	sfra1.FreqVect[i++]=545;
	sfra1.FreqVect[i++]=580;
	sfra1.FreqVect[i++]=630;
	sfra1.FreqVect[i++]=700;
	sfra1.FreqVect[i++]=800;
	sfra1.FreqVect[i++]=900;
	sfra1.FreqVect[i++]=1000;
	*/
	#endif




}



void PRControlInitialize(void)
{

	//CNTL3P3Z Inverter Current Loop
	CNTL_3P3Z_IQ_VARS_init(&cntl3p3z_InvI_vars);
	CNTL_3P3Z_IQ_COEFFS_init(&cntl3p3z_InvI_coeff);

	//CNTL2P2Z PR 1H Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_PRV_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_PRV_coeff);

	//CNTL2P2Z R 3HController
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV3_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_RV3_coeff);

	//CNTL2P2Z R 5H Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV5_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_RV5_coeff);

	//CNTL2P2Z R 7H Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV7_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_RV7_coeff);

	//CNTL2P2Z R 9H Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV9_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_RV9_coeff);

	//CNTL2P2Z R 11H Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV11_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_RV11_coeff);

	//CNTL2P2Z Lead Lag Controller
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_LeadLag_vars);
	CNTL_2P2Z_IQ_COEFFS_init(&cntl2p2z_LeadLag_coeff);

	gain = KPI_3P3Z;
	z0 = KII_3P3Z / KPI_3P3Z;
	fs = ISR_CONTROL_FREQUENCY * 1000;
	CNTL_3P3Z_IQ_VARS_init(&cntl3p3z_InvI_vars);
	computePIcontrollerCoeff3P3Z(&cntl3p3z_InvI_coeff, gain, z0, fs);

	kpV_1H = KPV_1H;
	kiV_1H = KIV_1H;
	wrcV_1H = WRCV_1H;
	woF_1H = AC_FREQ * THD_1H;
	fs = ISR_CONTROL_FREQUENCY * 1000;
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_PRV_vars);
	computePRcontrollerCoeff(&cntl2p2z_PRV_coeff, kpV_1H,kiV_1H,woF_1H,fs,wrcV_1H);

	kiV_3H=KIV_3H;
	wrcV_3H=WRCV_3H;
	woF_3H=AC_FREQ*THD_3H;
	fs = ISR_CONTROL_FREQUENCY * 1000;
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV3_vars);
	computePRcontrollerCoeff(&cntl2p2z_RV3_coeff, 0,kiV_3H,woF_3H,fs,wrcV_3H);

	kiV_5H=KIV_5H;
	wrcV_5H=WRCV_5H;
	woF_5H=AC_FREQ*THD_5H;
	fs = ISR_CONTROL_FREQUENCY * 1000;
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV5_vars);
	computePRcontrollerCoeff(&cntl2p2z_RV5_coeff, 0,kiV_5H,woF_5H,fs,wrcV_5H);

	kiV_7H=KIV_7H;
	wrcV_7H=WRCV_7H;
	woF_7H=AC_FREQ*THD_7H;
	fs = ISR_CONTROL_FREQUENCY * 1000;
	CNTL_2P2Z_IQ_VARS_init(&cntl2p2z_RV7_vars);
	computePRcontrollerCoeff(&cntl2p2z_RV7_coeff, 0,kiV_7H,woF_7H,fs,wrcV_7H);

//	 Lead Lag Compensator
	cntl2p2z_LeadLag_coeff.Coeff_A1 = _IQ(CNTL_LEADLAG_V_2P2Z_A1);
	cntl2p2z_LeadLag_coeff.Coeff_A2 = _IQ(CNTL_LEADLAG_V_2P2Z_A2);
	cntl2p2z_LeadLag_coeff.Coeff_B0 = _IQ(CNTL_LEADLAG_V_2P2Z_B0);
	cntl2p2z_LeadLag_coeff.Coeff_B1 = _IQ(CNTL_LEADLAG_V_2P2Z_B1);
	cntl2p2z_LeadLag_coeff.Coeff_B2 = _IQ(CNTL_LEADLAG_V_2P2Z_B2);
	cntl2p2z_LeadLag_coeff.IMin	 	= _IQ(CNTL_LEADLAG_V_2P2Z_IMIN);
	cntl2p2z_LeadLag_coeff.Min		= _IQ(CNTL_LEADLAG_V_2P2Z_MIN);
	cntl2p2z_LeadLag_coeff.Max		= _IQ(CNTL_LEADLAG_V_2P2Z_MAX);

}



//TODO computePRcontrollerCoeff()
// PI =Kp+ (2*s*wrc)/(s^2+2*s*wrc+wo^2)
void computePRcontrollerCoeff(CNTL_2P2Z_IQ_COEFFS *coef1, float32 kp, float32 ki, float32 wofreq, float32 fs, float32 wrc)
{
	float32 temp1, temp2, wo_adjusted;

	wo_adjusted = 2 * fs * tan((2 * wofreq * PI) / (2 * fs));
	temp1 = 4 * fs * fs + wo_adjusted * wo_adjusted + 4 * fs * wrc;
	temp2 = 4 * ki * wrc * fs / temp1;

	coef1->Coeff_B0 = _IQ(temp2);
	coef1->Coeff_B1 = _IQ(0);
	coef1->Coeff_B2 = _IQ(-temp2);
	coef1->Coeff_A1 = _IQ((-8*fs*fs+2*wo_adjusted*wo_adjusted)/temp1);
	coef1->Coeff_A2 = _IQ((temp1-8*fs*wrc)/temp1);
	coef1->IMin = _IQ(-10.0);
	coef1->Max = _IQ(10.0);
	coef1->Min = _IQ(-10.0);

	if(kp != 0)
	{
		coef1->Coeff_B0 += _IQ(kp);
		coef1->Coeff_B1 += _IQmpy(_IQ(kp), coef1->Coeff_A1);
		coef1->Coeff_B2 += _IQmpy(_IQ(kp), coef1->Coeff_A2);
	}

	coef1->Coeff_A1 = -coef1->Coeff_A1;
	coef1->Coeff_A2 = -coef1->Coeff_A2;
}



//TODO computePIcontrollerCoeff2P2Z()
// PI = (gain)*(s+z0)/s
void computePIcontrollerCoeff2P2Z(CNTL_2P2Z_IQ_COEFFS *coef1, float32 gain, float32 z0, float32 fsw)
{
	float32 temp1;
	temp1 = gain / ( 2 * fsw);

	coef1->Coeff_B0 = _IQ((2 * fsw + z0) * temp1);
	coef1->Coeff_B1 = _IQ((-2 * fsw + z0) * temp1);
	coef1->Coeff_B2 = _IQ(0);
	coef1->Coeff_A1 = _IQ(1);
	coef1->Coeff_A2 = _IQ(0);
	coef1->IMin = _IQ(-1.0);
	coef1->Max = _IQ(1.0);
	coef1->Min = _IQ(-1.0);
}

//TODO computePIcontrollerCoeff3P3Z()
// PI = (gain)*(s+z0)/s
void computePIcontrollerCoeff3P3Z(CNTL_3P3Z_IQ_COEFFS *coef1, float32 gain, float32 z0, float32 fsw)
{
	float32 temp1;
	temp1 = gain / ( 2 * fsw);

	coef1->Coeff_B0 = _IQ((2 * fsw + z0) * temp1);
	coef1->Coeff_B1 = _IQ((-2 * fsw + z0) * temp1);
	coef1->Coeff_B2 = _IQ(0);
	coef1->Coeff_B3 = _IQ(0);
	coef1->Coeff_A1 = _IQ(1);
	coef1->Coeff_A2 = _IQ(0);
	coef1->Coeff_A3 = _IQ(0);
	coef1->IMin = _IQ(-1.0);
	coef1->Max = _IQ(1.0);
	coef1->Min = _IQ(-1.0);
}
