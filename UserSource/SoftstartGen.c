
//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

//*********** Structure Init Function ****//
void SoftstartGenInit(SOFTSTARTGENptr p)
{
	p->In=0;
	p->StepValueMax=0;
	p->Value=0;
	p->Out=0;
}

//*********** Function Definition ********//
void SoftstartGenUPUpdate(SOFTSTARTGENptr p)
{
/* Compute the Value rate */
	p->Value += _IQmpy(p->StepValueMax, p->In);
/* Saturate the Value rate within (0,1) */
	if (p->Value > p->In)
		p->Value = p->In;
	p->Out = p->Value;
}

void SoftstartGenDOWNUpdate(SOFTSTARTGENptr p)
{
/* Compute the Value rate */
	p->Value -= _IQmpy(p->StepValueMax, p->In);
/* Saturate the Value rate within (0,1) */
	if (p->Value < 0)
		p->Value = 0;
	p->Out = p->Value;
}
