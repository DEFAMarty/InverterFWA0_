#ifndef SOFTSTARTGEN_H
#define SOFTSTARTGEN_H

//*********** Structure Definition ********//
typedef struct {
	int32  In; 		// Input: Softstart In (pu)
	int32  StepValueMax;// Parameter: Maximum step Value (pu)
	int32  Value;		// Variable: Step Value (pu)
	int32  Out;  	 	// Output: Softstart signal (pu)
} SOFTSTARTGENStruct;

typedef SOFTSTARTGENStruct *SOFTSTARTGENptr;

void SoftstartGenInit(SOFTSTARTGENptr p);
void SoftstartGenUPUpdate(SOFTSTARTGENptr p);
void SoftstartGenDOWNUpdate(SOFTSTARTGENptr p);

extern void SoftstartGenInit(SOFTSTARTGENptr p);
extern void SoftstartGenUPUpdate(SOFTSTARTGENptr p);
extern void SoftstartGenDOWNUpdate(SOFTSTARTGENptr p);

#endif /* SOFTSTARTGEN_H */
