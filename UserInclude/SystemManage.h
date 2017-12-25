//###########################################################################
//
// FILE:    SystemManage.h
//
// TITLE:   System Manage
//
//
//
//###########################################################################

// $Copyright:
// Copyright (C) 2017 Defa Incorporated - http://www.defa.com/

//###########################################################################

#ifndef SYSTEMMANAGE_H_
#define SYSTEMMANAGE_H_







//------Board State define start------

typedef enum  {

	boardState_Initialize = 0,
	boardState_DriveReady = 1,
	boardState_DriveEnable = 2,
	boardState_InverterOFF = 3,
	boardState_InverterON=4,
	boardState_TripCondition = 5,
	boardState_SleepMode = 6,
	boardState_Invalid = 7,

}BOARDSTATEEnmu;



//------Command define end---------

// Prototypes for the functions in SystemManage.c
void SystemManage(void);
void BoardStateInitialize(void);
void BoardStateDriveReady(void);
void BoardStateDriveEnable(void);
void BoardStateTripCondition(void);
void BoardStateONOFFAction(void);
void BoardStateSleepMode(void);

extern void SystemManage(void);
#endif    // end of  SYSTEMMANAGE_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
