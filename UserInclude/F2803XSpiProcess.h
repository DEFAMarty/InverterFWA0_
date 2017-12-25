//###########################################################################
// File: SpiProcess.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the SpiProcess.c
// Created on: 2015.12.07
//###########################################################################

#ifndef F2803XSPIPROCESS_H_
#define F2803XSPIPROCESS_H_


// Macro definition
#define DAC_OutPutA(dac_value)	SpiA_Tx_data[0] = (0x3000 | ((dac_value) & 0x0FFF))
#define DAC_OutPutB(dac_value)	SpiA_Tx_data[1] = (0x7000 | ((dac_value) & 0x0FFF))
#define DAC_OutPutC(dac_value)	SpiA_Tx_data[2] = (0xB000 | ((dac_value) & 0x0FFF))
#define DAC_OutPutD(dac_value)	SpiA_Tx_data[3] = (0xE000 | ((dac_value) & 0x0FFF))

// Declaration of external variables.
extern unsigned int SpiA_Tx_data[4];
extern unsigned int SpiA_Rx_data[4];

// Prototypes for the functions in SpiProcess.c
void SpiInit(void);
void SpiManage(void);
int16 DataCalcForSpi(Uint16 DataIndex, Uint16 Gain);



extern void SpiInit(void);
extern void SpiManage(void);



#endif     // end of  SPIPROCESS_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
