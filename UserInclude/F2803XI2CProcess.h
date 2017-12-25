/*
 * I2CProcess.h
 *
 */

#ifndef F2803XI2CPROCESS_H_
#define F2803XI2CPROCESS_H_

//--------------------------------------------
// Defines
//--------------------------------------------


#define EEPROMWRITEEnable()			GpioDataRegs.GPACLEAR.bit.GPIO23 = 1 		// Load output latch Low
#define EEPROMREADEnable()			GpioDataRegs.GPASET.bit.GPIO23 = 1			//Load output latch High




// Prototype statements for functions found within this file.
void I2CAInit(void);
Uint16 I2CAWriteData(struct I2CMSG *msg);
Uint16 I2CAReadData(struct I2CMSG *msg);
void I2CWritedatatoEEPROMsection(void);
void I2CReaddatafromEEPROMsection(void);
interrupt void I2CInt1AISR(void);


void pass();
void fail();


extern void I2CAInit(void);
extern void I2CWritedatatoEEPROMsection(void);
extern void I2CReaddatafromEEPROMsection(void);

#endif /* I2CPROCESS_H_ */
