//###########################################################################
// File: DigitalIOProcess.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the DigitalIOProcess.c
// Created on: 2015.12.07
//###########################################################################

#ifndef F2803XGPIOPROCESS_H_
#define F2803XGPIOPROCESS_H_


//Macro definition

#define DPPFCPowerDisable() 		GpioDataRegs.GPACLEAR.bit.GPIO6 = 1 		// Load output latch Low
#define DPPFCPowerEnable()			GpioDataRegs.GPASET.bit.GPIO6 = 1			//Load output latch High

#define DBeeperControlDisable() 	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1 		// Load output latch Low
#define DBeeperControlEnable()		GpioDataRegs.GPASET.bit.GPIO7 = 1			//Load output latch High

#define DStateLedEnable() 			GpioDataRegs.GPACLEAR.bit.GPIO8 = 1 		// Load output latch Low
#define DStateLedDisable()			GpioDataRegs.GPASET.bit.GPIO8 = 1			//Load output latch High
#define DStateLedToggle()			GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1			//Load output latch High

#define FanSpeedDRead() 			GpioDataRegs.GPADAT.bit.GPIO9				// Fan Speed Signal

#define DPPFCShutDownControlEnable() 			GpioDataRegs.GPASET.bit.GPIO10 = 1 		// Load output latch High
#define DPPFCShutDownControlDisable()			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1	//Load output latch Low

#define DRelayACINPEEnable() 			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1 		// Load output latch Low
#define DRelayACINPEDisable()			GpioDataRegs.GPASET.bit.GPIO11 = 1			//Load output latch High

#define SwitchODRead() 			GpioDataRegs.GPADAT.bit.GPIO20				// Switch Outside Signal

#define SwitchIDRead() 			GpioDataRegs.GPADAT.bit.GPIO21				// Switch Inside Signal

#define DRelayNEnable() 			GpioDataRegs.GPACLEAR.bit.GPIO22 = 1 		// Load output latch Low
#define DRelayNDisable()			GpioDataRegs.GPASET.bit.GPIO22 = 1			//Load output latch High

#define DRelayLEnable() 			GpioDataRegs.GPACLEAR.bit.GPIO23 = 1 		// Load output latch Low
#define DRelayLDisable()			GpioDataRegs.GPASET.bit.GPIO23 = 1			//Load output latch High

#define ACINDRead() 			GpioDataRegs.GPADAT.bit.GPIO24				// AC in Signal



// Define the structure of the DRIVE Object.
typedef struct {
        Uint16 EnableFlag;     // Input: Enable/Disable (1/0) flag (Q0)
        void (*init)();        // Pointer to the init function
        void (*update)();      // Pointer to the update function
        } DRIVEStruct;

typedef DRIVEStruct *DRIVEptr;

// Default Initializer for the DRIVE Object
#define DRIVE_DEFAULTS { 		0,   \
                       	   	    (void (*)(Uint32))F2803XDRIVEInit,  \
							    (void (*)(Uint32))F2803XDRIVEUpdate \
                    	}


// Prototypes for the functions
void F2803XDRIVEInit(DRIVEptr p);
void F2803XDRIVEUpdate(DRIVEptr p);

void ACinRelayControl(void);


#endif     // end of  DIGITALIOPROCESS_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
