//###########################################################################
// File: SpiProcess.c
// Description: Initialization of SPI module, transfer and receive data.
// Created on: 2015.12.07
//###########################################################################

// Included headerfiles.
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Definition of some variables, which will be used in this file.
unsigned int SpiA_Tx_data[4]={0x3A00,0x76FF,0xB3FF,0xE100};
unsigned int SpiA_Rx_data[4]={0};

extern Uint16 IsrTicker;
extern Uint16 BackTicker;


extern CNTL_3P3Z_IQ_COEFFS cntl3p3z_InvI_coeff;
extern CNTL_2P2Z_IQ_VARS cntl2p2z_LeadLag_vars;
extern CNTL_2P2Z_IQ_VARS cntl2p2z_PRV_vars;


//---------------------------------------------------------------------------
// InitSpi:
//---------------------------------------------------------------------------
// This function initializes the SPI to a known state.

void SpiInit(void)
{

    SpiaRegs.SPICCR.bit.SPISWRESET=0;    // Reset SPI
    SpiaRegs.SPICCR.all = 0x000F;       //16-bit character,  mode
    SpiaRegs.SPICTL.all = 0x0017;       //Interrupt enabled, Master/Slave XMIT enabled
    SpiaRegs.SPISTS.all = 0x0000;
    SpiaRegs.SPIBRR = 63;           // Baud rate

    // Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all = 0xE044;      // Enable FIFO's, set TX FIFO level to 4
    SpiaRegs.SPIFFRX.all = 0x6044;      // Set RX FIFO level to 4
    SpiaRegs.SPIFFCT.all = 0xA;
    //SpiaRegs.SPIPRI.all = 0x0010;		// Set so breakpoints don't disturb xmission

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;  // Enable SPI

    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

}


//---------------------------------------------------------------------------
// SpiManage:
//---------------------------------------------------------------------------
// This function is used to transfer and receive data.

void SpiManage(void)
{
	int16 i = 0;
	int16 OutPutData;

	// Transfer data.


	OutPutData = DataCalcForSpi(gu16_DspParameter[DSP_FUN_DACCSELCH1], gu16_DspParameter[DSP_FUN_DACCGAINCH1]);
    DAC_OutPutA(OutPutData);

    OutPutData = DataCalcForSpi(gu16_DspParameter[DSP_FUN_DACCSELCH2], gu16_DspParameter[DSP_FUN_DACCGAINCH2]);
	DAC_OutPutB(OutPutData);

	OutPutData = DataCalcForSpi(gu16_DspParameter[DSP_FUN_DACCSELCH3], gu16_DspParameter[DSP_FUN_DACCGAINCH3]);
	DAC_OutPutC(OutPutData);

	OutPutData = DataCalcForSpi(gu16_DspParameter[DSP_FUN_DACCSELCH4], gu16_DspParameter[DSP_FUN_DACCGAINCH4]);
	DAC_OutPutD(OutPutData);


	if(1 == SpiaRegs.SPIFFTX.bit.TXFFINT)
	{
 	  for(i=0;i<4;i++)
 	   {
 		   SpiaRegs.SPITXBUF=SpiA_Tx_data[i];      // Send data
  	   }
		SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	}

	// Receive data.
	if(1 == SpiaRegs.SPIFFRX.bit.RXFFINT)
	{
		for(i=0;i<4;i++)
		{
		   SpiA_Rx_data[i]=SpiaRegs.SPIRXBUF;		// Read data
		}
		SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
		SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	}
}



//---------------------------------------------------------------------------
// DateCalcForSpi:
//---------------------------------------------------------------------------
//Calculate Dates which would sent to DAC.

int16 DataCalcForSpi(Uint16 DataIndex, Uint16 Gain)
{
   Uint16 Data;
   _iq Temp;

   switch(DataIndex)
   {
      case 0:		//CHD
				Temp = (int16)_IQtoIQ11(invVoRefInst);	//invDutyPU
				Data = Temp + 2048;
				break;
      case 1:		//CHC
//				Temp = (int16)_IQtoIQ11(invVoInst);	//invSine
				Temp = (int16)_IQtoIQ11(invVoInst);
				Data = Temp + 2048;
                  break;
      case 2:		//CHB
				Temp = (int16)_IQtoIQ11(invDutyPU);
				Data = Temp + 2048;
				// Data = (int16)_IQtoIQ12(cntl2p2z_PRV_vars.Ref));
				break;
      case 3:
				  Temp = (int16)_IQtoIQ11(cntl2p2z_PRV_vars.Fdbk);
				  Data = Temp + 2048;
    	  	  	  // Data = (int16)_IQtoIQ12(cntl2p2z_PRV_vars.Fdbk);
                  break;
      case 4:
				  Temp = (int16)_IQtoIQ11(cntl2p2z_LeadLag_vars.Ref);
				  Data = Temp + 2048;
    	  	  	 // Data = (int16)_IQtoIQ12(_IQabs(cntl2p2z_LeadLag_vars.Ref));
                  break;
      case 5:
				  Temp = (int16)_IQtoIQ11(cntl2p2z_LeadLag_vars.Fdbk);
				  Data = Temp + 2048;
    	  	  	  // Data = (int16)_IQtoIQ12(_IQabs(cntl2p2z_LeadLag_vars.Fdbk));
                  break;

      case 6:		
  	  	  	  	  Data = (BackTicker)>> 4;	
                  break;
      default :		//CHA
    	  	  	  Data = (IsrTicker)>> 4;	
   }

   Temp = (int32)(Gain) * 0x00001000;
//   Temp = _IQ12mpy(Temp, _IQ12(0.01));
   Data = (Uint16)_IQ12mpy(Temp, Data);


   return(Data);
}



//===========================================================================
// End of file.
//===========================================================================
