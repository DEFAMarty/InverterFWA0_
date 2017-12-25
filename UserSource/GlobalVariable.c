//###########################################################################
//	FILE:			GlobalVariable.c
//
//	Description:	Set the Global Variable
//
//###########################################################################

// Included headerfiles.
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

//*********************** USER Variables *************************************//
// Global variables used that are applicable to this board
//****************************************************************************//

//TODO Inverter Control Variables
// Inverter Variables
//NOTE: Io & Ig are used interchangeably,
//NOTE: Vo and Vg are used Interchangeably
// Measurement Variables
// Inverter Side Inductor Current Measurement
volatile _iq24 invIiInst,invIiInstADC,invIiInstSD;
// Output Voltage measurement
volatile _iq24 invVoInst,invVoInstADC,invVoInstSD;
// Inverter Side Inductor Current Measurement Offset
volatile _iq24 invIiOffset;
// Cap Current SD measurement
volatile _iq24 invIfInstSD;
// Cap Current SD measurement offset
volatile _iq24 invIfSDOffset;
// Output current measurement
volatile _iq24 invIoInst,invIoInstSD;
// Output current measurement
volatile _iq24 invIoSDOffset;
// Output Voltage measurement Offset
// Vo Offset May not be applicable in real application as Vo may be present always
volatile _iq24 invVoOffset,invVoSDOffset;
// Inv DC Bus Voltage Measurement
volatile _iq24 invVbusInst,invVbusInstADC,invVbusInstSD;
// Inv DC Bus Voltage Measurement Offset
volatile _iq24 invVbusOffset,invVbusSDOffset;
// Inv Filtered DC bus measurement
volatile _iq24 invVbusAvg;
// Inv DC Bus measurement by SD Offset
// Reference variables
// Inverter modulation index, used for open loop check
//volatile _iq24 invModIndex;
// Peak value of the Ii, Io current set point
volatile _iq24 invIiRef, invIoRef;
// Peak value of the Vo  set point
volatile _iq24 invVoRef,invVoRefCal;
// Instantaneous Ii or Io reference = invIiRef* Angle
volatile _iq24 invIiRefInst,invIoRefInst;
// Instantaneous Vo reference = invVoRef* Angle
volatile _iq24 invVoRefInst;
// DC bus reference
volatile _iq24 invVbusRef;

// Display Values
int32 guiVbus;
int32 guiACFreq;
int32 guiPrms;
int32 guiIrms;
volatile int32 guiVrms;
int32 guiVavg;
int32 guiVo;
int32 guiIi;
int32 guiIo;
int32 guiPowerFactor;
int32 guiVA;
int32 guiFreqAvg;
int32 guiVema;

// Variables used to calibrate measurement offsets
//Offset filter coefficient K1: 0.05/(T+0.05);
_iq24 k1 = 0.998;
//Offset filter coefficient K2: T/(T+0.05)
_iq24 k2 = 0.001999;
int16 offsetCalCounter;
_iq24 offset165;

// Duty variables for inverter (per unit and compare register values)
// per unit duty value
_iq24 invDutyPU,invDutyPUPrev;
// duty value written into the PWM register
int32 invDuty,invDutyCompl;
// PWM TBPRD Half
volatile _iq24 halfPWMPeriod;

// Flags for clearing trips and closing the loops and the Relay
int16 closeILoopInv,closeVLoopInv, clearInvTrip, rlyConnect;

// Flags for detecting ZCD
_iq invSine, invSinePrev;
int16 zeroCrossDetectFlag;

int16 ClearInvTrip;


// variable that used to calculate ePWM frequency.
int16 gi16_CarrierPeriod = 1500;



Uint16 gu16_DspParameter[DSPPARASIZE + 6]={0,0,0,0,0,	//0
										   0,0,0,0,0,	//5
										   0,0,0,0,0,	//10
										   0,0,0,0,0,	//15
										   0,0,0,8,1,	//20
										   2,1,1,1,0,	//25
										   1,0,0,0,0,	//30
										   0			//35
											};

const Uint16 gu16_DspParaMaxVal[DSPPARASIZE + 1]={0x0000,0xFFFF,0xFFFF,6000,0xFFFF,		//0
												  10000,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	//5
												  0xFFFF,0xFFFF,0xFFFF,50,0xFFFF,		//10
												  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	//15
												  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	//20
												  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,	//25
												  0xFFFF								//30
												  };

const Uint16 gu16_DspParaMinVal[DSPPARASIZE + 1]={0,0,0,0,0,	//0
												  0,0,0,0,0,	//5
												  0,0,0,0,0,	//10
												  0,0,0,0,0,	//15
												  0,0,0,0,0,	//20
												  0,0,0,0,0,	//25
												  0				//30
												  };





//TODO initGlobalVariables()
void initGlobalVariables(void)
{

	//Variable initialization
	invIiInst = 0;
	invIiOffset = 0;
	invIiInstSD = 0;

	invIfInstSD = 0;
	invIfSDOffset = 0;
	invIoInstSD = 0;
	invIoSDOffset = 0;
	invVoInst = 0;
	invVoOffset = 0.5;
	invVoInstSD = 0;
	invVoSDOffset = 0;
	invVbusInst = 0;
	invVbusOffset = 0;
	invVbusAvg = 0;
	invVbusInstSD = 0;
	invVbusSDOffset = 0;
	invIiRef = 0.5;
	invIoRef = 0;
	invIoRefInst = 0;
	invVoRef = 0;
	invVoRefCal = 0;
	invIiRefInst = 0;
	invVoRefInst = 0;
	invVbusRef = 0;
	offsetCalCounter = 0;
	invDutyPU = 0;
	invDuty = 0;
	halfPWMPeriod = 0;

//	invModIndex = 0;

	closeILoopInv = 0;
	closeVLoopInv = 0;
	clearInvTrip = 0;
	rlyConnect = 0;
	invSine = 0;
	invSinePrev = 0;
	zeroCrossDetectFlag = 0;

}



//===========================================================================
// End of file.
//===========================================================================
