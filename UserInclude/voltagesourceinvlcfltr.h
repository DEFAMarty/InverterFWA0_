/* ==============================================================================
System Name:

File Name:	  	voltagesourceinvlcfltr.h

Target:

Author:

Description:

Copyright (C) {2017} Defa Incorporated - http://www.defa.com/
 * ALL RIGHTS RESERVED*
=================================================================================  */
#ifndef VOLTAGE_SOURCE_INV_LC_FLTR_H
#define VOLTAGE_SOURCE_INV_LC_FLTR_H

#ifdef __cplusplus

extern "C" {
#endif


#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

// Define the system frequency (MHz)
#define CPU_SYS_CLOCK (60*1000000)
#define PWMSYSCLOCK_FREQ (60*1000000)
#define SYSTEM_FREQUENCY 60


/* Control Loop Design */
// Define the ISR frequency (kHz)
#define ISR_CONTROL_FREQUENCY 20
#define ISR_FREQUENCY 20 * 1000

#define AC_FREQ	50

#define PI 3.141592653589

#define INV_DEADBAND_US 0.2
#define INV_PWM_PERIOD (PWMSYSCLOCK_FREQ)/(INV_PWM_SWITCHING_FREQUENCY)
#define INV_DEADBAND_PWM_COUNT (int)((float)INV_DEADBAND_US*(float)PWMSYSCLOCK_FREQ*(float)0.000001)

#define THD_1H				1
#define THD_3H				3
#define THD_5H				5
#define THD_7H				7
#define THD_9H				9
#define THD_11H				11

#define KPV_1H        		1
#define KIV_1H        		2000
#define WRCV_1H        		0.00628
#define KIV_3H        		1000
#define WRCV_3H        		0.031415
#define KIV_5H        		1000
#define WRCV_5H        		0.031415
#define KIV_7H        		500
#define WRCV_7H        		0.031415


#define KPI_3P3Z        		0.486
#define KII_3P3Z        		2430

#define CNTL_INV_I_3P3Z_A1 1.0000000000
#define CNTL_INV_I_3P3Z_A2 0.0000000000
#define CNTL_INV_I_3P3Z_A3 0.0000000000
#define CNTL_INV_I_3P3Z_B0 0.5467500000
#define CNTL_INV_I_3P3Z_B1 -0.4252500000
#define CNTL_INV_I_3P3Z_B2 0.0000000000
#define CNTL_INV_I_3P3Z_B3 0.0000000000
#define CNTL_INV_I_3P3Z_IMIN -1.0
#define CNTL_INV_I_3P3Z_MAX 1.0
#define CNTL_INV_I_3P3Z_MIN -1.0

#define CNTL_LEADLAG_V_2P2Z_A1 0.5170939860
#define CNTL_LEADLAG_V_2P2Z_A2 0.0000000000
#define CNTL_LEADLAG_V_2P2Z_B0 0.8618532335
#define CNTL_LEADLAG_V_2P2Z_B1 -0.8353010320
#define CNTL_LEADLAG_V_2P2Z_B2 0.0000000000
#define CNTL_LEADLAG_V_2P2Z_IMIN -1.0
#define CNTL_LEADLAG_V_2P2Z_MAX 1.0
#define CNTL_LEADLAG_V_2P2Z_MIN -1.0


//SFRA Options
//1 FRA for the Voltage Loop
//2 FRA for the Current Loop
#define SFRA_TYPE			2
#define SFRA_ISR_FREQ 		ISR_CONTROL_FREQUENCY * 1000
//==================================================================================
// System Settings
//----------------------------------------------------------------------------------
//Add any system specific setting below
#if SFRA_TYPE == 1 // voltage loop
#define SFRA_FREQ_START 10
#define SFRA_FREQ_LENGTH 100
//SFRA step Multiply = 10^(1/No of steps per decade(40))
#define FREQ_STEP_MULTIPLY (float)1.045
#define SFRA_AMPLITUDE _IQ26(0.025)
#elif SFRA_TYPE == 2 // current loop
#define SFRA_FREQ_START 100
#define SFRA_FREQ_LENGTH 100
//SFRA step Multiply = 10^(1/No of steps per decade(40))
#define FREQ_STEP_MULTIPLY (float)1.045
#define SFRA_AMPLITUDE _IQ26(0.01)
#endif


#define UNIVERSAL_GRID_MAX_VRMS 240
#define UNIVERSAL_GRID_MIN_VRMS 80
#define UNIVERSAL_GRID_MAX_FREQ 65
#define UNIVERSAL_GRID_MIN_FREQ 45

// Prototypes for the functions
interrupt void InverterISR(void);
interrupt void TZ1ISR(void);


void Fan_CTRL_Update(void);


void detectZeroCrossingClearPWMTrip(void);
void setupSFRA(void);
void PRControlInitialize(void);

void computePRcontrollerCoeff(CNTL_2P2Z_IQ_COEFFS *coef1, float32 kp, float32 ki, float32 wofreq, float32 fs, float32 wrc);
void computePIcontrollerCoeff2P2Z(CNTL_2P2Z_IQ_COEFFS *coef1, float32 gain, float32 z0, float32 fsw);
void computePIcontrollerCoeff3P3Z(CNTL_3P3Z_IQ_COEFFS *coef1, float32 gain, float32 z0, float32 fsw);

//*****************************************************************************
// the includes
//*****************************************************************************

#endif
