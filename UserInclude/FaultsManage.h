//###########################################################################
// File: FaultsManage.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the FaultsManage.c
// Created on: 2015.12.07
//###########################################################################

#ifndef FAULTSMANAGE_H_
#define FAULTSMANAGE_H_

//-------- Fault Response Message (RX)
// Define the structure of the SYSTEMFAULT Object.
typedef struct
{
  union
  {
    struct
    {
    	Uint16  oc_iac_sw		: 1;
    	Uint16  ov_vac_sw		: 1;
    	Uint16  lv_vac_sw		: 1;
    	Uint16  oc_ihdc_sw		: 1;
    	Uint16  ov_vhdc_sw		: 1;
    	Uint16  lv_vhdc_sw		: 1;
    	Uint16  oc_ibat_sw		: 1;
    	Uint16  ov_vbat_sw		: 1;
    	Uint16  lv_vbat_sw		: 1;
    	Uint16  inv_temp_warn		: 1;
    	Uint16  inv_temp_shutdown		: 1;
    	Uint16  oc_iac_hw		: 1;
    	Uint16  fan_speed_err		: 1;
    	Uint16  drive_enable		: 1;
    	Uint32  rsvd1			: 18;
    }bit;
    Uint32 All;
  }FatalUnion;
}SYSTEMFAULTStruct;




// Declaration of external variables.
extern SYSTEMFAULTStruct SystemFalutFlag;

// Prototypes for the functions in FaultsManage.c
void FaultManage(void);
void FaultFlagClear(void);


#endif     // end of  FAULTSMANAGE_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
