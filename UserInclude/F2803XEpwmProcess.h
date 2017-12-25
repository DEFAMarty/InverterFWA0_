//###########################################################################
// File: EpwmProcess.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the EpwmProcess.c
// Created on: 2015.12.07
//###########################################################################

#ifndef F2803XEPWMPROCESS_H_
#define F2803XEPWMPROCESS_H_


/*----------------------------------------------------------------------------
Initialization constant for the F280X Time-Base Control Registers for PWM Generation.
Sets up the timer to run free upon emulation suspend, count up-down mode
prescaler 1.

/*----------------------------------------------------------------------------
Initialization constant for the F280X Action Qualifier Software Force Register.
----------------------------------------------------------------------------*/
#define AQSFRC_INIT_STATE ( RLDCSF_PRD )

/*----------------------------------------------------------------------------
Initialization constant for the F280X Trip Zone Select Register
----------------------------------------------------------------------------*/
#define  TZSEL_INIT_STATE  ENABLE_TZ1_OST

/*----------------------------------------------------------------------------
Initialization constant for the F280X Trip Zone Control Register
----------------------------------------------------------------------------*/
#define  TZCTL_INIT_STATE ( TZA_FORCE_HI + TZB_FORCE_HI + \
                            DCAEVT1_HI_Z + DCAEVT2_HI_Z + \
                            DCBEVT1_HI_Z + DCBEVT2_HI_Z )



void PWM_Inv_SPWM_unipolar_CNF(Uint16 n,Uint16 period,Uint16 deadband_rising,Uint16 deadband_falling);
void PWM_Inv_SPWM_unipolar_Update(Uint16 n, Uint16 p, int32 v);
void F280X3_PWM_Open(void);
void F280X3_PWM_Close(void);
void PWM_1ch_Fan_CNF(Uint16 n,Uint16 period, Uint16 deadband_rising, Uint16 deadband_falling);
void PWM_1ch_Fan_Update(Uint16 n, Uint16 frequency , Uint32 duty);


extern void PWM_Inv_SPWM_unipolar_CNF(Uint16 n,Uint16 period,Uint16 deadband_rising,Uint16 deadband_falling);
extern void PWM_Inv_SPWM_unipolar_Update(Uint16 n, Uint16 p, int32 v);
extern void F280X3_PWM_Open(void);
extern void F280X3_PWM_Close(void);
extern void PWM_1ch_Fan_CNF(Uint16 n,Uint16 period, Uint16 deadband_rising, Uint16 deadband_falling);
extern void PWM_1ch_Fan_Update(Uint16 n, Uint16 frequency , Uint32 duty);


#endif     // end of  EPWMPROCESS_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
