//###########################################################################
//	FILE:			GlobalVariable.c
//
//	Description:	Set the Global Variable
//
//###########################################################################

#ifndef GLOBALVARIABLE_H_
#define GLOBALVARIABLE_H_

//TODO Inverter Control Variables
// Inverter Variables
//NOTE: Io & Ig are used interchangeably,
//NOTE: Vo and Vg are used Interchangeably
// Measurement Variables
// Inverter Side Inductor Current Measurement
extern volatile _iq24 invIiInst,invIiInstADC,invIiInstSD;
// Output Voltage measurement
extern volatile _iq24 invVoInst,invVoInstADC,invVoInstSD;
// Inverter Side Inductor Current Measurement Offset
extern volatile _iq24 invIiOffset;
// Cap Current SD measurement
extern volatile _iq24 invIfInstSD;
// Cap Current SD measurement offset
extern volatile _iq24 invIfSDOffset;
// Output current measurement
extern volatile _iq24 invIoInst, invIoInstSD;
// Output current measurement
extern volatile _iq24 invIoSDOffset;
// Output Voltage measurement Offset
// Vo Offset May not be applicable in real application as Vo may be present always
extern volatile _iq24 invVoOffset,invVoSDOffset;
// Inv DC Bus Voltage Measurement
extern volatile _iq24 invVbusInst,invVbusInstADC,invVbusInstSD;
// Inv DC Bus Voltage Measurement Offset
extern volatile _iq24 invVbusOffset,invVbusSDOffset;
// Inv Filtered DC bus measurement
extern volatile _iq24 invVbusAvg;
// Inv DC Bus measurement by SD Offset
// Reference variables
// Inverter modulation index, used for open loop check
//extern volatile _iq24 invModIndex;
// Peak value of the Ii/ Io current set point
extern volatile _iq24 invIiRef, invIoRef;
// Peak value of the Vo  set point
extern volatile _iq24 invVoRef,invVoRefCal;
// Instantaneous Ii, Io reference = invIiRef* Angle
extern volatile _iq24 invIiRefInst, invIoRefInst;
// Instantaneous Vo reference = invVoRef* Angle
extern volatile _iq24 invVoRefInst;
// DC bus reference
extern volatile _iq24 invVbusRef;

// Display Values
extern int32 guiVbus;
extern int32 guiACFreq;
extern int32 guiPrms;
extern int32 guiIrms;
extern volatile int32 guiVrms;
extern int32 guiVavg;
extern int32 guiVo;
extern int32 guiIi;
extern int32 guiIo;
extern int32 guiPowerFactor;
extern int32 guiVA;
extern int32 guiFreqAvg;
extern int32 guiVema;


// Variables used to calibrate measurement offsets
//Offset filter coefficient K1: 0.05/(T+0.05);
extern _iq24 k1;
//Offset filter coefficient K2: T/(T+0.05)
extern _iq24 k2;

// Duty variables for inverter (per unit and compare register values)
// per unit duty value
extern _iq24 invDutyPU,invDutyPUPrev;
// duty value written into the PWM register
extern int32 invDuty,invDutyCompl;
// PWM TBPRD Half
extern volatile _iq24 halfPWMPeriod;

// Flags for clearing trips and closing the loops and the Relay
extern int16 closeILoopInv,closeVLoopInv, clearInvTrip, rlyConnect;

// Flags for detecting ZCD
extern _iq24 invSine, invSinePrev;
extern int16 zeroCrossDetectFlag;

extern int16 ClearInvTrip;

extern volatile int16 tempChange;
extern volatile int16 period;

// variable that used to calculate ePWM frequency.
extern int16 gi16_CarrierPeriod;

void initGlobalVariables(void);




extern void initGlobalVariables(void);





//Macro define of DSP parameter.
#define DSPPARASIZE	30
#define DSP_FUN_KPV_1H		0
#define	DSP_FUN_KIV_1H		1
#define DSP_FUN_WRCV_1H		2
#define DSP_FUN_WOV_1H		3
#define DSP_FUN_KIV_3H		4
#define DSP_FUN_WRCV_3H		5
#define DSP_FUN_WOV_3H		6
#define DSP_FUN_KIV_5H		7
#define DSP_FUN_WRCV_5H		8
#define DSP_FUN_WOV_5H		9
#define DSP_FUN_KIV_7H		10
#define DSP_FUN_WRCV_7H		11
#define DSP_FUN_WOV_7H		12
#define DSP_FUN_KIV_11H		13
#define DSP_FUN_WRCV_11H	14
#define DSP_FUN_WOV_11H		15
#define DSP_FUN_KPI_1H		16
#define	DSP_FUN_KII_1H		17
#define DSP_FUN_WRCI_1H		18
#define DSP_FUN_WOI_1H		19
#define DSP_FUN_AC_OUTPUT_FREQ		20
#define DSP_FUN_AC_OUTPUT_VOLT		21
#define DSP_FUN_BAT_INPUT_VOLT		22
#define DSP_FUN_DACCSELCH1		23
#define DSP_FUN_DACCGAINCH1		24
#define DSP_FUN_DACCSELCH2		25
#define DSP_FUN_DACCGAINCH2		26
#define DSP_FUN_DACCSELCH3		27
#define DSP_FUN_DACCGAINCH3		28
#define DSP_FUN_DACCSELCH4		29
#define DSP_FUN_DACCGAINCH4		30





extern Uint16 gu16_DspParameter[];
extern const Uint16 gu16_DspParaMaxVal[];
extern const Uint16 gu16_DspParaMinVal[];



#endif

//===========================================================================
// End of file.
//===========================================================================
