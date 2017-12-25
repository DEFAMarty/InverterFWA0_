//###########################################################################
// File: AdcProcess.h
// Description: Header file containing constants, data type definitions, and
//                    function prototypes for the AdcProcess.c
// Created on: 2015.12.07
//###########################################################################

#ifndef F2803XADCPROCESS_H_
#define F2803XADCPROCESS_H_

// Included headerfiles.
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define PICONTROL 1
#define PRCONTROL 0


#if (PICONTROL == 1)
#define VAC_TYPICAL 200
#elif (PRCONTROL == 1)
#define VAC_TYPICAL 10
#endif

#define SQRT2 1.4142135623731

#define ADC_PU_IQ12_SCALE_FACTOR 0x0FFF // 1/4096 = 0.000244140625 Q12
#define ADC_PU_IQ16_SCALE_FACTOR 0x0010 // 1/4096 = 0.000244140625 Q16

#define IAC_SCALE_MAX_SENSE 46.27636364  // 2400/220*1.5*1.414*2
#define VAC_SCALE_MAX_SENSE 353.6067892503   // 500/1.414
//#define VAC_SCALE_MAX_SENSE 500   // 500
#define IHVDC_SCALE_MAX_SENSE 23.1381818182   // 2400/220*1.5*1.414
#define VHVDC_SCALE_MAX_SENSE 500   // 500
//#define VHVDC_SCALE_MAX_SENSE 20   // 20 for test with current loop	20170803
#define IBAT_SCALE_MAX_SENSE 313.043478261  // 2400/12*3
#define VBAT_SCALE_MAX_SENSE 30   // 30V



// Default Initializer, Below is for filter part.
#define FREQ_C_PHASE_CURRENT 1 // Cutoff frequency for phase current is 1 KHz.
#define FREQ_S_PHASE_CURRENT 10 //Sample frequency for phase current is 10 KHz.
#define PHASE_CURRENT_B0 ((FREQ_S_PHASE_CURRENT-(PI*FREQ_C_PHASE_CURRENT))/(FREQ_S_PHASE_CURRENT+(PI*FREQ_C_PHASE_CURRENT))) //Coefficient for Y(k-1).
#define PHASE_CURRENT_A1 (1-PHASE_CURRENT_B0)/2.0

#define FREQ_C_TEMP    0.015     // Cutoff frequency for temperature is 0.015 KHz.
#define FREQ_S_TEMP    0.2     //Sample frequency for phase current is 0.2 KHz.
#define TEMPERATURE_B0    ((FREQ_S_TEMP - (PI * FREQ_C_TEMP)) / (FREQ_S_TEMP + (PI * FREQ_C_TEMP)))      //Coefficient for Y(k-1).
#define TEMPERATURE_A1    (1 - TEMPERATURE_B0) / 2.0      //Coefficient for X(k).
#define TEMPERATURE_A0    TEMPERATURE_A1      //Coefficient for X(k-1).


#define IAC_D_VALUE		0
#define VAC_D_VALUE		1
#define IHVDC_D_VALUE		2
#define VHVDC_D_VALUE		3
#define IBAT_D_VALUE		4
#define VBAT_D_VALUE		5
#define NTC_TEMP_TRANS1_D_VALUE		6
#define NTC_TEMP_HS1_D_VALUE		7
#define NTC_TEMP_TRANS2_D_VALUE		8
#define NTC_TEMP_HS2_D_VALUE		9
#define REF2V5_D_VALUE		10
#define REF0V_D_VALUE		11

#define NUM_ADC_CHANS		12


// Define the enum of the ADC Channel Object.
typedef enum
{
	REF0V_D_CHAN = 0x00,
	NTC_TEMP_HS2_D_CHAN = 0x01,
	ADrsvd2 = 0x02,
	NTC_TEMP_HS1_D_CHAN = 0x03,
	IBAT_D_CHAN,
	ADrsvd3 = 0x05,
	IHVDC_D_CHAN,
	IAC_D_CHAN,
	REF2V5_D_CHAN = 0x08,
	NTC_TEMP_TRANS2_D_CHAN = 0x09,
	ADrsvd5 = 0x0A,
	NTC_TEMP_TRANS1_D_CHAN = 0x0B,
	VBAT_D_CHAN,
	ADrsvd6 = 0x0D,
	VHVDC_D_CHAN,
	VAC_D_CHAN
}ADCCHANNELEnum;


// Define the structure of the ADCVALSB Object.
typedef struct
{
	_iq IACGain;         // Parameter: Gain (Q12)					Range Scale +/-1.00
    _iq IACOffset;       // Parameter: Offset (Q24)
    _iq IACOut;          // Output: output (Q24)
    _iq IACNoFilter;     // Output: output (Q15)

    _iq VACGain;         // Parameter: Gain (Q12)					Range Scale +/-1.00
    _iq VACOffset;       // Parameter: Offset (Q24)
    _iq VACOut;          // Output: output (Q24)
    _iq VACNoFilter;     // Output: output (Q15)

    Uint16 IHVDCGain;         // Parameter: Gain (Q16)				Range Scale 0~23.13A
    Uint16 IHVDCOffset;       // Parameter: Offset (Q16)
    _iq IHVDCOut;          // Output: output (Q24)

    Uint16 VHVDCGainIQ12;         // Parameter: Gain (Q12)				Range Scale 0~500V
    Uint16 VHVDCGainIQ16;         // Parameter: Gain (Q16)				Range Scale 0~500V
    _iq VHVDCOffset;       // Parameter: Offset (Q24)
    _iq VHVDCOut;          // Output: output (Q24)


    Uint16 IBATGain;         // Parameter: Gain (Q16)
    Uint16 IBATOffset;       // Parameter: Offset (Q16)
    _iq IBATOut;          // Output: output (Q24)

    Uint16 VBATGain;         // Parameter: Gain (Q16)
    Uint16 VBATOffset;       // Parameter: Offset (Q16)
    _iq VBATOut;          // Output: output (Q24)

    Uint16 TRANS1TempGain;       // Parameter: Gain (Q16)
    Uint16 TRANS1TempOffset;     // Parameter: Offset (Q16)
    _iq TRANS1TempOut;        // Output: output (Q24)

    Uint16 HS1TempGain;       // Parameter: Gain (Q16)
    Uint16 HS1TempOffset;     // Parameter: Offset (Q16)
    _iq HS1TempOut;        // Output: output (Q24)

    Uint16 TRANS2TempGain;       // Parameter: Gain (Q16)
    Uint16 TRANS2TempOffset;     // Parameter: Offset (Q16)
    _iq TRANS2TempOut;        // Output: output (Q24)

    Uint16 HS2TempGain;       // Parameter: Gain (Q16)
    Uint16 HS2TempOffset;     // Parameter: Offset (Q16)
    _iq HS2TempOut;        // Output: output (Q24)

	void (*init)();			// Pointer to the init function
	void (*read)();        // Pointer to the read function

} ADCVALStruct;

typedef ADCVALStruct *ADCVALptr;

/*--------------------------- [PUBLIC TYPES END] -----------------------------*/

/*-------------------- [PUBLIC DEFINES FOR STRUCT BEGIN] ---------------------*/


/*	Initial ADC structure values including gain, offset and initial value for
	each ADC channel */
#define ADCVAL_DEFAULTS {							\
		ADC_PU_IQ12_SCALE_FACTOR,0,0x0000,0,            \
		ADC_PU_IQ12_SCALE_FACTOR,0,0x0000,0,            \
		ADC_PU_IQ16_SCALE_FACTOR,0x0000,0x0000,             \
		ADC_PU_IQ12_SCALE_FACTOR,ADC_PU_IQ16_SCALE_FACTOR,0x0000,0x0000,             \
		ADC_PU_IQ16_SCALE_FACTOR,0x0000,0x0000,             \
		ADC_PU_IQ16_SCALE_FACTOR,0x0000,0x0000,             \
		0x1000,0x0000,0x0000,             \
		0x1000,0x0000,0x0000,             \
		0x1000,0x0000,0x0000,             \
		0x1000,0x0000,0x0000,             \
		F2803XADCInit,				\
		F2803XADCRead				\
}


// Prototypes for the functions
void F2803XADCInit(ADCVALptr p);
void F2803XADCRead(ADCVALptr p);
void CalACValue(ADCVALptr p);
void CalACNoFilterValue(ADCVALptr p);
void CalHVDCValue(ADCVALptr p);
void CalBATValue(ADCVALptr p);
int32 CalHeatSinkTempValue(Uint16 n);
void SetOffset(ADCVALptr p);

extern void F2803XADCRead(ADCVALptr p);
extern void CalACValue(ADCVALptr p);
extern void CalACNoFilterValue(ADCVALptr p);
extern void CalHVDCValue(ADCVALptr p);
extern void CalBATValue(ADCVALptr p);
extern int32 CalHeatSinkTempValue(Uint16 n);
extern void SetOffset(ADCVALptr p);

#endif     // end of  ADCPROCESS_H_ definition.

//===========================================================================
// End of file.
//===========================================================================
