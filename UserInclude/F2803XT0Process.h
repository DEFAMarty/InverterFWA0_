//###########################################################################
// File: T0Process.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the T0Process.c
// Created on: 2015.12.07
//###########################################################################

#ifndef F2803XT0PROCESS_H_
#define F2803XT0PROCESS_H_

// Macro definition
#define T0_PERIOD				10			                     //10KHZ
#define T0_PERIOD_SET      (1000/T0_PERIOD)     //Unit is us.


// Prototypes for the functions in T0Process.c
void ResetCANTaskCount(void);
void ResetVBusTaskCount(void);
void ResetHeatSinkTempTaskCount(void);
void ResetStateTaskCount(void);
void ResetFaultTaskCount(void);
void ResetTestTaskCount(void);
void ResetSystemManageTaskCount(void);
void ResetFaultManageTaskCount(void);
void ResetIOManageTaskCount(void);
void ResetSpiManageTaskCount(void);
void ResetSciManageTaskCount(void);
void ResetReadI2CTaskCount(void);

int16 GetCANTaskCount(void);
int16 GetVBusTaskCount(void);
int16 GetHeatSinkTempTaskCount(void);
int16 GetFaultTaskCount(void);
int16 GetTestTaskCount(void);
int16 GetSystemManageTaskCount(void);
int16 GetFaultManageTaskCount(void);
int16 GetIOManageTaskCount(void);
int16 GetSpiManageTaskCount(void);
int16 GetSciManageTaskCount(void);
int16 GetReadI2CTaskCount(void);

void RunReadI2CTaskImmediately(void);

interrupt void CPUTimer0ISR(void);

#endif     // end of  T0PROCESS_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
