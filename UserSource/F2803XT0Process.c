//###########################################################################
//
// FILE:    2803xCpuTimer.c
//
// TITLE:   Cpu Timer
//
//! This example configures CPU Timer0, 1, and 2 and increments
//! a counter each time the timer asserts an interrupt.
//!
//! - CpuTimer0.InterruptCount
//! - CpuTimer1.InterruptCount
//! - CpuTimer2.InterruptCount
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

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped using
// the linker cmd file.
#pragma CODE_SECTION(CPUTimer0ISR, "ramfuncs");


Uint16 T0Ticker = 0;

//------Task macro definition start------------
#define  OFFCOUNT	-1
#define  CAN_TASK_COUNT_VALUE   2
#define  VBus_TASK_COUNT_VALUE   1
#define  HEATSINKTEMP_TASK_COUNT_VALUE     (T0_PERIOD * 10)
#define  SYSTEM_MANAGE_TASK_COUNT_VALUE   1
#define  FAULT_MANAGE_TASK_COUNT_VALUE     (T0_PERIOD * 1)
#define  IO_MANAGE_TASK_COUNT_VALUE   (T0_PERIOD * 1)
#define  SPI_MANAGE_TASK_COUNT_VALUE   (T0_PERIOD * 1)
#define  SCI_MANAGE_TASK_COUNT_VALUE   (T0_PERIOD * 1)
#define  I2C_READ_TASK_COUNT_VALUE  (T0_PERIOD * 500)
#define  I2C_READ_TASK_COUNT_VALUE0  (T0_PERIOD * 1000)
#define  I2C_READ_TASK_COUNT_VALUE1  (T0_PERIOD * 100)
//-------Task macro definition end--------------

int16 CANTaskCount = 0;
int16 VBusTaskCount = 0;
int16 HeatSinkTempTaskCount = 0;
int16 StateTaskCount = 0;
int16 SystemManageTaskCount = 0;
int16 FaultManageTaskCount = 0;
int16 IOManageTaskCount = 0;
int16 SpiManageTaskCount = 10;
int16 SciManageTaskCount = 10;
int16 ReadI2CTaskCount = I2C_READ_TASK_COUNT_VALUE0;


//
// cpu_timer0_isr - Timer0 counter
//

interrupt void CPUTimer0ISR(void)
{

	T0Ticker++;

	if(CANTaskCount > 0)
	{
		CANTaskCount --;
	}
	if(VBusTaskCount > 0)
	{
		VBusTaskCount --;
	}
	if(HeatSinkTempTaskCount > 0)
	{
		HeatSinkTempTaskCount --;
	}
	if(SystemManageTaskCount >0)
	{
		SystemManageTaskCount--;
	}
	if(FaultManageTaskCount >0)
	{
		FaultManageTaskCount--;
	}
	if(IOManageTaskCount >0)
	{
		IOManageTaskCount--;
	}
	if(SpiManageTaskCount >0)
	{
		SpiManageTaskCount--;
	}
	if(SciManageTaskCount >0)
	{
		SciManageTaskCount--;
	}
	if(ReadI2CTaskCount >0)
	{
		ReadI2CTaskCount--;
	}

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//---------------------------------------------------------------------------------------------
// ResetCANTaskCount & ResetVBusTaskCount & ResetStateTaskCount &
// ResetTestTaskCount & ResetSystemManageTaskCount & ResetIOManageTaskCount &
// ResetSpiManageTaskCount
//---------------------------------------------------------------------------------------------
// Reset to initial value when decrease to zero.

void ResetCANTaskCount(void)
{
   CANTaskCount = CAN_TASK_COUNT_VALUE;
}

void ResetVBusTaskCount(void)
{
	VBusTaskCount = VBus_TASK_COUNT_VALUE;
}
void ResetHeatSinkTempTaskCount(void)
{
	HeatSinkTempTaskCount = HEATSINKTEMP_TASK_COUNT_VALUE;
}

void ResetSystemManageTaskCount(void)
{
	SystemManageTaskCount = SYSTEM_MANAGE_TASK_COUNT_VALUE;
}

void ResetFaultManageTaskCount(void)
{
	FaultManageTaskCount = FAULT_MANAGE_TASK_COUNT_VALUE;
}

void ResetIOManageTaskCount(void)
{
	IOManageTaskCount = IO_MANAGE_TASK_COUNT_VALUE;
}

void ResetSpiManageTaskCount(void)
{
	SpiManageTaskCount = SPI_MANAGE_TASK_COUNT_VALUE;
}

void ResetSciManageTaskCount(void)
{
	SciManageTaskCount = SCI_MANAGE_TASK_COUNT_VALUE;
}

void ResetReadI2CTaskCount(void)
{
	ReadI2CTaskCount = I2C_READ_TASK_COUNT_VALUE;
}

//-----------------------------------------------------------------------------------------
// GetCANTaskCount & GetVBusTaskCount & GetStateTaskCount &
// GetTestTaskCount & GetSystemManageTaskCount & GetIOManageTaskCount &
// GetSpiManageTaskCount
//-----------------------------------------------------------------------------------------
// Get the current value of variables.

int16 GetCANTaskCount(void)
{
	return(CANTaskCount);
}

int16 GetVBusTaskCount(void)
{
	return(VBusTaskCount);
}

int16 GetHeatSinkTempTaskCount(void)
{
	return(HeatSinkTempTaskCount);
}

int16 GetSystemManageTaskCount(void)
{
	return(SystemManageTaskCount);
}

int16 GetFaultManageTaskCount(void)
{
	return(FaultManageTaskCount);
}

int16 GetIOManageTaskCount(void)
{
	return(IOManageTaskCount);
}

int16 GetSpiManageTaskCount(void)
{
	return(SpiManageTaskCount);
}

int16 GetSciManageTaskCount(void)
{
	return(SciManageTaskCount);
}

int16 GetReadI2CTaskCount(void)
{
	return(ReadI2CTaskCount);
}


void RunReadI2CTaskImmediately(void)
{
	ReadI2CTaskCount = I2C_READ_TASK_COUNT_VALUE1;
}

void CancleReadI2CTask(void)
{
	ReadI2CTaskCount = OFFCOUNT;
}

//===========================================================================
// End of file.
//===========================================================================

