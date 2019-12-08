//###########################################################################
//
// FILE:    F2837xD_struct.c
//
// TITLE:   F2837xD SDFM structure 
//
//###########################################################################
// $TI Release: 
// $Release Date: 
//###########################################################################


#include "F2837xD_Device.h"
#include "F2837xD_struct.h"


#if defined(CPU1)
volatile struct ADC_REGS *ADC[MAX_ADC] =
{	0, &AdcaRegs, &AdcbRegs,
       &AdccRegs, &AdcdRegs };
#endif

volatile struct ECAP_REGS *ECAP[MAX_ECAP] =
{	0, &ECap1Regs, &ECap2Regs, &ECap3Regs, 
       &ECap4Regs, &ECap5Regs, &ECap6Regs };

volatile struct EPWM_REGS *EPWM[MAX_EPWM] =
{	0, &EPwm1Regs, &EPwm2Regs, &EPwm3Regs, 
       &EPwm4Regs, &EPwm5Regs, &EPwm6Regs, 
	   &EPwm7Regs, &EPwm8Regs, &EPwm9Regs, 
	   &EPwm10Regs, &EPwm11Regs, &EPwm12Regs };

volatile struct EQEP_REGS *EQEP[MAX_EQEP] =
{	0, &EQep1Regs, &EQep2Regs, &EQep3Regs };

volatile struct I2C_REGS *I2C[MAX_I2C] =
{	0, &I2caRegs };

volatile struct McBSP_REGS *MCBSP[MAX_MCBSP] =
{	0, &McbspaRegs };

volatile struct SCI_REGS *SCI[MAX_SCI] =
{	0, &SciaRegs };

volatile struct SPI_REGS *SPI[MAX_SPI] =
{	0, &SpiaRegs, &SpibRegs, &SpicRegs };

volatile struct SDFM_REGS *SDFM[MAX_SDFM] =
{	0, &Sdfm1Regs, &Sdfm2Regs};

#if defined(CPU1)
volatile Uint16 *TRIP_SEL[MAX_TRIPSEL] =
{	0, &TrigRegs.INPUT1SELECT, &TrigRegs.INPUT2SELECT,
       &TrigRegs.INPUT3SELECT, &TrigRegs.INPUT4SELECT,
	   &TrigRegs.INPUT5SELECT, &TrigRegs.INPUT6SELECT,
	   &TrigRegs.INPUT7SELECT, &TrigRegs.INPUT8SELECT,
	   &TrigRegs.INPUT9SELECT, &TrigRegs.INPUT10SELECT,
	   &TrigRegs.INPUT11SELECT, &TrigRegs.INPUT12SELECT,
	   &TrigRegs.INPUT13SELECT, &TrigRegs.INPUT14SELECT,
	   &TrigRegs.INPUT15SELECT, &TrigRegs.INPUT16SELECT };
#endif
