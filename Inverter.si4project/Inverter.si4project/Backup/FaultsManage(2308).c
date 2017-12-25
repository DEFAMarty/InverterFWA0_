//###########################################################################
// File: FaultManage.c
// Description:  Read fault data, clear fault flags and send fault data to host computer.
// Created on: 2015.12.07
//###########################################################################


#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File


// Declaration of external variables.
extern ADCVALStruct adc1;

extern Uint16 gu16_TZFaultFlag;

SYSTEMFAULTStruct SystemFalutFlag = {0};



Uint16 CntWarnHeatSinkTemp = 0;
Uint16 CntShutHeatSinkTemp = 0;

Uint16 CntShutPhaseCurrentOverrun = 0;
Uint16 CntShutPhaseCurrentOffset = 0;

Uint16 CntBatShutOverVoltage = 0;
Uint16 CntBatUndervoltage = 0;

Uint16 CntBatShutOverCurrent = 0;


Uint16 CntVHVDCShutOverVoltage = 0;
Uint16 CntVHVDCUndervoltage = 0;

Uint16 CntVHVDCShutOverCurrent = 0;

Uint16 CntFanFault = 0;

Uint16 Cnt10Hz = 0;

Uint16 CntVoltage10Hz = 0;






//---------------------------------------------------------------------------
// FaultManage:
//---------------------------------------------------------------------------
// Set fault flags and send to host computer.

void FaultManage(void)
{
	SYSTEMFAULTStruct FaultFlag1;
	_iq Temp;

	FaultFlag1.FatalUnion.All = SystemFalutFlag.FatalUnion.All;

	// Temperature Execute frequency is 10Hz.
	Cnt10Hz++;
	if(Cnt10Hz >= 100)
	{
		Cnt10Hz = 0;
		// Heat sink temperature warning fault
		if((adc1.HS1TempOut > _IQ16(80)) || (adc1.HS2TempOut > _IQ16(80)) || (adc1.TRANS1TempOut > _IQ16(80)) || (adc1.TRANS2TempOut > _IQ16(80)))
		{
			if(CntWarnHeatSinkTemp < 10)
			{
				CntWarnHeatSinkTemp++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.inv_temp_warn = 1;
			}
		}
		else
		{
			CntWarnHeatSinkTemp = 0;
		}

		// Heat sink temperature shutdown fault
		if((adc1.HS1TempOut > _IQ16(100)) || (adc1.HS2TempOut > _IQ16(100)) || (adc1.TRANS1TempOut > _IQ16(100)) || (adc1.TRANS2TempOut > _IQ16(100)))
		{
			if(CntShutHeatSinkTemp< 10)
			{
				CntShutHeatSinkTemp++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.inv_temp_shutdown = 1;
			}
		}
		else
		{
			CntShutHeatSinkTemp = 0;
		}
	}


	if(CntVoltage10Hz > 100)
	{
		// Battery high voltage
		if(adc1.VBATOut >= _IQ16(16.0))
		{
			if(CntBatShutOverVoltage < 3)
			{
				CntBatShutOverVoltage++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.ov_vbat_sw = 1;
			}
		}
		else
		{
			CntBatShutOverVoltage = 0;
		}

		// Battery Low voltage
		if(adc1.VBATOut <= _IQ16(10.5))
		{
			if(CntBatUndervoltage < 50)
			{
				CntBatUndervoltage++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.lv_vbat_sw = 1;
			}
		}
		else
		{
			CntBatUndervoltage = 0;
			FaultFlag1.FatalUnion.bit.lv_vbat_sw = 0;
		}

		// VHVDC over voltage
		if(adc1.VHVDCOut > _IQ16(400.0))
		{
			if(CntVHVDCShutOverVoltage < 3)
			{
				CntVHVDCShutOverVoltage++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.ov_vhdc_sw = 1;
			}
		}
		else
		{
			CntVHVDCShutOverVoltage = 0;
		}

		// VHVDC Low voltage
		if(adc1.VHVDCOut < _IQ16(300.0))
		{
			if(CntVHVDCUndervoltage < 50)
			{
				CntVHVDCUndervoltage++;
			}
			else
			{
				FaultFlag1.FatalUnion.bit.lv_vhdc_sw = 1;
			}
		}
		else
		{
			CntVHVDCUndervoltage = 0;
			FaultFlag1.FatalUnion.bit.lv_vhdc_sw = 0;
		}
	}
	else
	{
		CntVoltage10Hz++;
	}


	// Battery over Current
	if(adc1.IBATOut > _IQ16(170))
	{
		if(CntBatShutOverCurrent < 3)
		{
			CntBatShutOverCurrent++;
		}
		else
		{
			FaultFlag1.FatalUnion.bit.oc_ibat_sw = 1;
		}
	}
	else if(adc1.IBATOut < _IQ16(-170.0))
	{
		if(CntBatShutOverCurrent < 3)
		{
			CntBatShutOverCurrent++;
		}
		else
		{
			FaultFlag1.FatalUnion.bit.oc_ibat_sw = 1;
		}
	}
	else
	{
		CntBatShutOverCurrent = 0;
	}

	// IHVDC over Current
	if(adc1.IHVDCOut > _IQ16(16.0))
	{
		if(CntVHVDCShutOverCurrent < 3)
		{
			CntVHVDCShutOverCurrent++;
		}
		else
		{
			FaultFlag1.FatalUnion.bit.oc_ihdc_sw = 1;
		}
	}
	else
	{
		CntVHVDCShutOverCurrent = 0;
	}

	SystemFalutFlag.FatalUnion.All = FaultFlag1.FatalUnion.All;
}

//---------------------------------------------------------------------------
// FaultFlagClear:
//---------------------------------------------------------------------------
// Clear fault flags.

void FaultFlagClear(void)
{
//	F280X3_PWM_Close();
	SystemFalutFlag.FatalUnion.All = 0;
	SystemFalutFlag.FatalUnion.All = 0;
}

//---------------------------------------------------------------------------
// unFatalFaultRead:
//---------------------------------------------------------------------------
// Read fatal fault flags.

Uint32 FatalUnionRead(void)
{
	return(SystemFalutFlag.FatalUnion.All);
}



//---------------------------------------------------------------------------
// SetParameterError1:
//---------------------------------------------------------------------------
//


void LEDManage(Uint32 LedDispValue1,Uint32 LedDispValue2)
{
	static Uint32 MaskBit = 0x80000000;
	static Uint32 CurrentFaultValue = 0;
	static Uint16 HeadCount = 0;
	static Uint16 BitPerCount = 0;
	static Uint16 CurrentFaultType = 0;
	 //updata MaskBit for Display Bit after display a bit and updata CurrentFaultValue after displayed 16 bit
	if(0 ==BitPerCount)
	{
		if(0x80000000 == MaskBit)
		{
			DStateLedDisable();
			MaskBit = 0x00000001;
			if(0 == CurrentFaultType)
			{
				CurrentFaultValue = LedDispValue1;
				CurrentFaultType = 1;
				HeadCount = 30;
			}
			else
			{
				CurrentFaultValue = LedDispValue2;
				CurrentFaultType = 0;
				HeadCount = 30;
			}
		}
		else
		{
			MaskBit <<=1;
		}
		BitPerCount = 1;
	}
//----------Display value to LED----------------------
	//Display header
	if(0 != HeadCount)
	{
		HeadCount --;
		//Display Head to show this value Type
		if(1 == CurrentFaultType)
		{	//LED ON 3s  show LedValue1 header
			DStateLedEnable();
		}
		else
		{//LED OFF 3s show LedValue2 header
			DStateLedDisable();
		}
	}
	//Display bit
	else
	{
		BitPerCount ++;
		if( 5>BitPerCount)
		{	//LED OFF
			DStateLedDisable();
		}
		else if(7> BitPerCount)
		{ //LED ON
			DStateLedEnable();
		}
		else if(15 > BitPerCount)
		{ 	// control LED according 1 bit
			if(MaskBit & CurrentFaultValue)
			{//LED OFF
				DStateLedDisable();
			}
		}
		else
		{ //Keep LED ON
			BitPerCount = 0;
		}
	}
}
//===========================================================================
// End of file.
//===========================================================================
