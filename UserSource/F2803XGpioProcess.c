//###########################################################################
//
// FILE:    F28035GpioSetup.c
//
// TITLE:   GPIO Setup
//

//! In general:
//!  - All pullup resistors are enabled.  For ePWMs this may not be desired.
//!  - Input qual for communication ports (eCAN, SPI, SCI, I2C) is asynchronous
//!  - Input qual for Trip pins (TZ) is asynchronous
//!  - Input qual for eCAP and eQEP signals is synch to SYSCLKOUT
//!  - Input qual for some I/O's and interrupts may have a sampling window
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

//
// Prototype statements
//
extern SOFTSTARTGENStruct ssgen1;

extern BOARDSTATEEnmu boardState;

void F2803XDRIVEInit(DRIVEptr p)
{

    EALLOW;
    //
    // Basic Pinout.
    // This basic pinout includes:
    // PWM1-3, TZ1, SPI-A, SCI-A, I2C, CAN
    // and a number of I/O pins
    //

    //
    // These can be combined into single statements for improved
    // code efficiency.
    //

    //
    // Enable an GPIO output on GPIO6 D-PPFC-Power-EN, set it Low
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   // Enable pullup on GPIO6
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;   // Load output latch Low
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;  // GPIO6 = GPIO6 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;   // GPIO6 = output


    //
    // Enable an GPIO output on GPIO7 D-BEEPER-CONTROL, set it Low
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;   // Enable pullup on GPIO7
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // Load output latch Low
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;  // GPIO7 = GPIO7 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;   // GPIO7 = output

    //
    // Enable GPIO outputs on GPIO8 D-STATE-LED, set it high
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   // Enable pullup on GPIO8
    GpioDataRegs.GPASET.bit.GPIO8 = 1;   // Load output latch High
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8 = GPIO8 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // GPIO8 = output

    //
    // Enable GPIO inputs on GPIO9 Fan-Speed-D
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;   // Enable pullup on GPIO9
    GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 2;   // Synch to SYSCLOUT 6 times samping
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  // GPIO9 = GPIO9 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 0;   // GPIO9 = input


    //
    // Enable GPIO outputs on GPIO10 D-PPFC-ShutDown-CTL, set it high
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;  // Enable pullup on GPIO10
    GpioDataRegs.GPASET.bit.GPIO10 = 1;  // Load output latch High
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0; // GPIO10 = GPIO10 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;  // GPIO10 = output

    //
    // Enable GPIO outputs on GPIO11 D-Relay-ACIN-PE-EN, set it Low
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   // Enable pullup on GPIO11
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1; // Load output latch Low
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  // GPIO11 = GPIO11 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;   // GPIO11 = output



//    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pullup on GPIO13
//    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;   // Enable pullup on GPIO14
//    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;   // Enable pullup on GPIO15
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // asynch input
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3; // asynch input
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3; // asynch input
//    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // GPIO13 = TZ2
//    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // GPIO14 = TZ3
//    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // GPIO15 = TZ1

    //
    // Enable SPI-A on GPIO16 - GPIO19
    //
	//Enable internal pull-up for the selected pins
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pullup on GPIO16	D-SPI-MOSI
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pullup on GPIO17	SPI-MISO-D
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pullup on GPIO18	D-SPI-CLK
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pullup on GPIO19	D\-\S\P\I\-\C\S\

	//Set qualification for selected pins to asynch only
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // asynch input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // asynch input

	//Configure SPI-A pins using GPIO regs
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;  // GPIO16 = SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;  // GPIO17 = SPIS0MIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;  // GPIO18 = SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;  // GPIO19 = SPISTEA

    //
    // Enable GPIO inputs on GPIO20 Switch-O-D
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;   // Enable pullup on GPIO20
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 2;   // Synch to SYSCLOUT 6 times samping
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;  // GPIO20 = GPIO20 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;   // GPIO20 = input

    //
    // Enable GPIO inputs on GPIO21 Switch-I-D
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;   // Enable pullup on GPIO21
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 2;   // Synch to SYSCLOUT 6 times samping
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;  // GPIO21 = GPIO21 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;   // GPIO21 = input

    //
    // Enable GPIO outputs on GPIO22 D-Relay-N-EN, set it high
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   // Enable pullup on GPIO22
    GpioDataRegs.GPASET.bit.GPIO22 = 1;   // Load output latch high
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;  // GPIO22 = GPIO22 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;   // GPIO22 = output

    //
    // Enable GPIO outputs on GPIO23 D-Relay-L-EN, set it high
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   // Enable pullup on GPIO23
    GpioDataRegs.GPASET.bit.GPIO23 = 1;   // Load output latch high
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;  // GPIO23 = GPIO23 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;   // GPIO23 = output

    //
    // Enable GPIO inputs on GPIO24 AC-IN-D
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   // Enable pullup on GPIO24
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 2;   // Synch to SYSCLOUT 6 times samping
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;  // GPIO24 = GPIO24 as I/O
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;   // GPIO24 = input

//    //
//    // Set input qualification period for GPIO25 & GPIO26
//    //
//    GpioCtrlRegs.GPACTRL.bit.QUALPRD3=1;  // Qual period = SYSCLKOUT/2
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO25=2;   // 6 samples
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO26=2;   // 6 samples
//
//    //
//    // Make GPIO25 the input source for XINT1
//    //
//    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;  // GPIO25 = GPIO25
//    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;   // GPIO25 = input
//    GpioIntRegs.GPIOXINT1SEL.all = 25;    // XINT1 connected to GPIO25
//
//    //
//    // Make GPIO26 the input source for XINT2
//    //
//    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  // GPIO26 = GPIO26
//    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;   // GPIO26 = input
//    GpioIntRegs.GPIOXINT2SEL.all = 26;    // XINT2 connected to GPIO26
//
//    //
//    // Make GPIO27 wakeup from HALT/STANDBY Low Power Modes
//    //
//    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;  // GPIO27 = GPIO27
//    GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;   // GPIO27 = input
//    GpioIntRegs.GPIOLPMSEL.bit.GPIO27=1;  // GPIO27 will wake the device
//    SysCtrlRegs.LPMCR0.bit.QUALSTDBY=2;   // Qualify GPIO27 by 2 OSCCLK
//                                         // cycles before waking the device
//                                         // from STANDBY

    //
    // Enable SCI-A on GPIO28 - GPIO29
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;   // Enable pullup on GPIO28	RS232-RX-D
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3; // Asynch input
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;  // GPIO28 = SCIRXDA
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;   // Enable pullup on GPIO29	D-RS232-TX
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;  // GPIO29 = SCITXDA


    //
    // Enable CAN-A on GPIO30 - GPIO31
    //
    // Enable internal pull-up for the selected CAN pins
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;   // Enable pullup on GPIO30	CAN-RX-D
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;   // Enable pullup on GPIO31	D-CAN-TX
    // Set qualification for selected CAN pins to asynch only
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3; // Asynch input
    // Configure eCAN-A pins using GPIO regs
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;  // GPIO30 = CANRXA
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;  // GPIO31 = CANTXA

	//
	// Enable I2C-A on GPIO32 - GPIO33
	//
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pullup on GPIO32	D-SDAA-DATA
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3; // Asynch input
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;  // GPIO32 = SDAA
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pullup on GPIO33	D-SCLA-CLK
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3; // Asynch input
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;  // GPIO33 = SCLA

    //
    // Enable GPIO outputs on GPIO34 D-SCI-WC, set it Low
    //
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;   // Enable pullup on GPIO34
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1; // Load output latch Low
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // GPIO34 = GPIO34 as I/O
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   // GPIO34 = output


    EDIS;
}




//---------------------------------------------------------------------------
// F280X_DRIVE_Update:
//---------------------------------------------------------------------------
// This function is used to process the digital I/O ports.

void F2803XDRIVEUpdate(DRIVEptr p)
{

	if (boardState_InverterON == boardState)
	{

		if(invVoRef < invVoRefCal)
		{
//			invVoRef = invVoRef + _IQ(0.005);
			invVoRef = invVoRef + _IQ(0.0005);
		}
		
		p->EnableFlag = 1;
	}
	else
	{
		if(invVoRef > _IQ(0.000))
		{
			invVoRef = invVoRef - _IQ(0.001);
//			invVoRef = 0;
		}
		else
		{
			closeILoopInv = 0;
			clearInvTrip = 1;;
			p->EnableFlag = 0;
		}
	}
	ACinRelayControl();
}

//---------------------------------------------------------------------------
// AC input Relay control bypass:
//---------------------------------------------------------------------------
// This function is used to process the digital I/O ports.

void ACinRelayControl(void)
{

	if(1 == ACINDRead())
	{
		DRelayNDisable();
		DRelayLDisable();
	}
	else
	{
		DRelayNEnable();
		DRelayLEnable();
		invVoRef=0;
		invIiRef=0;
	}
}






void LEDManage(Uint32 LedDispValue1,Uint32 LedDispValue2);

// Definition of some variables, which will be used in this file.
#define LEDDISPLAYTASKCOUNT	100
#define SYSTEMVERSION	0x0033		//point is to bit as 0, 0x0033 is 2.2 version
#define SYSTEMCODEDATA 0x2017  //it's mean 2017 year


Uint16 LedTaskCount = LEDDISPLAYTASKCOUNT;



void InitECap1Gpio(void);

void InitECapture()
{
   InitECap1Gpio();
   ECap1Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
   ECap1Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
   ECap1Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

   // Configure peripheral registers
   ECap1Regs.ECCTL1.bit.PRESCALE = 0;				//BYPASS THE prescaler
   ECap1Regs.ECCTL2.bit.CAP_APWM = 0;		//capture mode
   ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1; // Oneshot mode

   ECap1Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge
   ECap1Regs.ECCTL1.bit.CAP2POL = 1;          // Rising edge
   ECap1Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
   ECap1Regs.ECCTL1.bit.CAP4POL = 1;          // Rising edge

   ECap1Regs.ECCTL1.bit.CTRRST1 = 0;          // Don't reset the counter
   ECap1Regs.ECCTL1.bit.CTRRST2 = 0;          // Don't reset the counter
   ECap1Regs.ECCTL1.bit.CTRRST3 = 0;          // Don't reset the counter
   ECap1Regs.ECCTL1.bit.CTRRST4 = 0;          // Don't reset the counter

   ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;         // Disable sync in
   ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through

   ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units
   ECap1Regs.ECCTL2.bit.STOP_WRAP = 1;
   ECap1Regs.ECCTL2.bit.REARM = 1;

   ECap1Regs.ECEINT.bit.CEVT1 = 1;            // 4 events = interrupt


   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter

}




//
// End of File
//

