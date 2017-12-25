//###########################################################################
//
// FILE:    F2803XDevInit.c
//
// TITLE:   Device initialization
//
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


// This function will copy the specified memory contents from
// one location to another.
//
//	Uint16 *SourceAddr        Pointer to the first word to be moved
//                          SourceAddr < SourceEndAddr
//	Uint16* SourceEndAddr     Pointer to the last word to be moved
//	Uint16* DestAddr          Pointer to the first destination word
//
// No checks are made for invalid memory locations or that the
// end address is > then the first start address.

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


//===========================================================================
// End of file.
//===========================================================================

