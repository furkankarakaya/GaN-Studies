//###########################################################################
// FILE:   pbist.c
// TITLE:  Driver for the PBIST controller
//###########################################################################
// $TI Release: F2837xD Support Library v100 $
// $Release Date: Mon Dec  9 12:58:09 CST 2013 $
//###########################################################################

#include "F2837xD_Device.h"
#include "F2837xD_Examples.h"   // Examples Include File

//*****************************************************************************
//!
//! Enables the clocking to the PBIST controller,
//! enabling access to PBIST Control registers.
//!
//*****************************************************************************
void PBISTInit(void)
{
	// Enable clock to PBIST, Turning it on
    PbistRegs.PACT.all = 0x1;

	// Even if you plan to override ALGO and RINFO
	// PBIST_OVER must be set to 0x9 first before setting to 1
    PbistRegs.OVERRIDE.all = 0x9;
}


//*****************************************************************************
//!
//! Enables the clocking to the PBIST controller,
//! enabling access to PBIST Control registers.
//!
//*****************************************************************************
void PBISTDisable(void)
{
	// Disable clock to PBIST, Turning it off
	PbistRegs.PACT.all = 0x1;
}

//*****************************************************************************
//! Selects the Algorthim(s) to be run on the device.
//!
//! \param ulALGO specifies the the Algorithms to be run
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTAlgoSelect(unsigned long ulALGO)
{
	PbistRegs.ALGO = ulALGO;
}

//*****************************************************************************
//! Selects the Memory Groups which the Algorithm will run on.
//!
//! \param ulRINFOL specifies the the desired memory groups.
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTRinfoLSelect(unsigned long ulRINFOL)
{
	PbistRegs.RINFOL = ulRINFOL;
}

//*****************************************************************************
//! Selects the Memory Groups which the Algorithm will run on.
//!
//! \param ulRINFOU specifies the the desired memory groups.
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTRinfoUSelect(unsigned long ulRINFOU)
{
	PbistRegs.RINFOU = ulRINFOU;
}

//*****************************************************************************
//! Overrides the current ALGO and RINFOL/RINFOU registers
//!
//! \param bOVER specifies if the ALGO and RINFOL?RINFU must be overwritten
//! 	1 = Overwrite both ALGO and RINFOL/RINFOU
//! 	0 = Overwrite ONLY ALGO
//!
//! \return None.
//*****************************************************************************
void PBISTOverride(_Bool bOVER)
{
	if(bOVER)
		PbistRegs.OVERRIDE.all = 0x0;
	else
		PbistRegs.OVERRIDE.all = 0x1;
}

//*****************************************************************************
//! Starts the PBIST tests
//!
//! \return None.
//*****************************************************************************
void PBISTStart(void)
{
	// Write to the DLR register to kick off the PBIST execution
	PbistRegs.DLRT.all = 0x21c;
}


//*****************************************************************************
//! Check if the PBIST is stopped due to a failure
//!
//! \return
//! 	true: There was a failure
//!		false: There are no failures
//*****************************************************************************
_Bool PBISTFailureStatus(void)
{
	// check the Fail Statuse bits for both Port 0 and Port 1
	// if one or the other is not 0, there was a failure.
	if((PbistRegs.FSRF0 > 0x0) || (PbistRegs.FSRF1 >0x0))
		return true;
	else
		return false;
}

//*****************************************************************************
//! Stop the PBIST where it is. Since the PBIST pauses wherever it finds
//! a falilure, you may choose to stop testing for other failures.
//!
//! \return: None
//*****************************************************************************
void PBISTStop(void)
{
	// Stop the PBIST if where it is stopped.
	PbistRegs.DLRT.all = 0x218;
}
//*****************************************************************************
//!	Continue the PBIST testing. Since the PBIST pauses wherever it finds
//! a falilure, you may choose to continue testing for other failures
//!
//! \return: None
//*****************************************************************************
void PBISTResume(void)
{
	// Check the Fail Statuse bits for both Port 0 and Port 1
	// if one or the other is not 0, there was a failure.
	PbistRegs.STR.all = PbistRegs.STR.all | 0x2;
}
