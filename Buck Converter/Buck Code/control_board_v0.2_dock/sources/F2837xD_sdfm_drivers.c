//###########################################################################
// FILE:   F2837xD_sdfm_drivers.c
// TITLE:  SDFM Driver functions
//###########################################################################
// $TI Release:
// $Release Date: 
//###########################################################################

#include "F28x_Project.h"
#include "F2837xD_struct.h"
#include "F2837xD_sdfm_drivers.h"



//---------------------------------------------------------------------------
// Function name: Sdfm_configureInputCtrl
//---------------------------------------------------------------------------
// This function configures SDFM Input control unit
//
// Input control unit can be configured in four different modes:
// MODE_0 : Modulator clock rate = Modulator data rate
// MODE_1 : Modulator clock rate = (Modulator data rate / 2)
// MODE_2 : Manchester encoded data (Modulator clock is encoded into data)
// MODE_3 : Modulator clock rate = (2 x Modulator data rate)
//
// sdfmNumber    :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber  :This parameter is used to select which filter
//                 (FILTER1,FILTER2,FILTER3,FILTER4) needs to be configured.
// mode          :This parameter is used to select one of the modes mentioned above
//

void Sdfm_configureInputCtrl(Uint16 sdfmNumber, Uint16 filterNumber, Uint16 mode)
{

	EALLOW;

	switch (filterNumber)
	{
		case FILTER1:
			(*SDFM[sdfmNumber]).SDFCTLPARM1.bit.MOD = mode;
			break;

		case FILTER2:
			(*SDFM[sdfmNumber]).SDFCTLPARM2.bit.MOD = mode;
			break;

		case FILTER3:
			(*SDFM[sdfmNumber]).SDFCTLPARM3.bit.MOD = mode;
			break;

		case FILTER4:
			(*SDFM[sdfmNumber]).SDFCTLPARM4.bit.MOD = mode;
			break;
	}

	EDIS;
}


//---------------------------------------------------------------------------
// Function name: Sdfm_configureComparator
//---------------------------------------------------------------------------
// This function configures SDFM Comparator unit
//
// Comparator unit can be configured to monitor input conditions with a fast settling time.
// This module can be programmed to detect over and under value conditions.
// Filter type: SINC1 / SINC2 / SINC3 / SINCFAST
//
// sdfmNumber    :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber  :This parameter is used to select which filter (FILTER1,FILTER2,FILTER3,FILTER3)
//                needs to be configured.
// filterType    :This parameter is used to select one of the filter type mentioned above
// OSR			 :This parameter is used to configure oversampling ratio for comparator
// HLT			 :This parameter is used to configure to detect over value condition
// LLT			 :This parameter is used to configure to detect under value condition
//

void Sdfm_configureComparator(Uint16 sdfmNumber, Uint16 filterNumber, Uint16 filterType, Uint16 OSR, Uint16 HLT, Uint16 LLT)
{

	EALLOW;

	switch (filterNumber)
	{

		case FILTER1:		//Filter 1
			//Configure filter type : Sincfast / Sinc1 / Sinc2 / Sinc3 //
			(*SDFM[sdfmNumber]).SDFCPARM1.bit.CS1_CS0  = filterType;

			//Configure OSR value //
			if(OSR<=COMPARATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFCPARM1.bit.COSR = OSR;}
			else{(*SDFM[sdfmNumber]).SDFCPARM1.bit.COSR = COMPARATOR_MAX_OSR;}

			(*SDFM[sdfmNumber]).SDFCMPH1.bit.HLT = HLT;
			(*SDFM[sdfmNumber]).SDFCMPL1.bit.LLT = LLT;
			break;

		case FILTER2:		//Filter 2
			//Configure filter type : Sincfast / Sinc1 / Sinc2 / Sinc3 //
			(*SDFM[sdfmNumber]).SDFCPARM2.bit.CS1_CS0  = filterType;

			//Configure OSR value //
			if(OSR<=COMPARATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFCPARM2.bit.COSR = OSR;}
			else{(*SDFM[sdfmNumber]).SDFCPARM2.bit.COSR = COMPARATOR_MAX_OSR;}

			(*SDFM[sdfmNumber]).SDFCMPH2.bit.HLT = HLT;
			(*SDFM[sdfmNumber]).SDFCMPL2.bit.LLT = LLT;
			break;

		case FILTER3:		//Filter 3
			//Configure filter type : Sincfast / Sinc1 / Sinc2 / Sinc3 //
			(*SDFM[sdfmNumber]).SDFCPARM3.bit.CS1_CS0  = filterType;

			//Configure OSR value //
			if(OSR<=COMPARATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFCPARM3.bit.COSR = OSR;}
			else{(*SDFM[sdfmNumber]).SDFCPARM3.bit.COSR = COMPARATOR_MAX_OSR;}

			(*SDFM[sdfmNumber]).SDFCMPH3.bit.HLT = HLT;
			(*SDFM[sdfmNumber]).SDFCMPL3.bit.LLT = LLT;
			break;

		case FILTER4:		//Filter 4
			//Configure filter type : Sincfast / Sinc1 / Sinc2 / Sinc3 //
			(*SDFM[sdfmNumber]).SDFCPARM4.bit.CS1_CS0  = filterType;

			//Configure Comparator OSR value //
			if(OSR<=COMPARATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFCPARM4.bit.COSR = OSR;}
			else{(*SDFM[sdfmNumber]).SDFCPARM4.bit.COSR = COMPARATOR_MAX_OSR;}

			(*SDFM[sdfmNumber]).SDFCMPH4.bit.HLT = HLT;
			(*SDFM[sdfmNumber]).SDFCMPL4.bit.LLT = LLT;
			break;
	}

	EDIS;

}


//---------------------------------------------------------------------------
// Function name: SDFM_configureSinc_filter
//---------------------------------------------------------------------------
// This function configures SDFM Sinc filter unit
//
// SDFM Sinc filter unit can be configured in any of four different Sinc filter types:
// Filter type: SINC1 / SINC2 / SINC3 / SINCFAST
//
// sdfmNumber     :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber   :This parameter is used to select which filter
//                  (FILTER1,FILTER2,FILTER3,FILTER3) needs to be configured
// Filter_switch  :This parameter is used to enable / disable a filter
// filterType     :This parameter is used to select one of the filter type mentioned above
// OSR			   :This parameter is used to configure oversampling ratio
//                  for Sinc filter (Upto OSR_256)
//

void Sdfm_configureSinc_filter(Uint16 sdfmNumber, Uint16 filterNumber, Uint16 Filter_switch, Uint16 filterType, Uint16 OSR)
{

	EALLOW;

	switch(filterNumber)
	{

		case FILTER1:		//Filter 1
			(*SDFM[sdfmNumber]).SDFSFPARM1.bit.FEN  = Filter_switch;
			(*SDFM[sdfmNumber]).SDFSFPARM1.bit.SST  = filterType;

			//Configure Sinc filter OSR value //
			if(OSR<=SINC_FILTER_MAX_OSR){(*SDFM[sdfmNumber]).SDFSFPARM1.bit.SOSR  = OSR;}
			else{(*SDFM[sdfmNumber]).SDFSFPARM1.bit.SOSR  = SINC_FILTER_MAX_OSR;}
			break;

		case FILTER2:		//Filter 2
			(*SDFM[sdfmNumber]).SDFSFPARM2.bit.FEN  = Filter_switch;
			(*SDFM[sdfmNumber]).SDFSFPARM2.bit.SST  = filterType;

			//Configure Sinc filter OSR value //
			if(OSR<=SINC_FILTER_MAX_OSR){(*SDFM[sdfmNumber]).SDFSFPARM2.bit.SOSR  = OSR;}
			else{(*SDFM[sdfmNumber]).SDFSFPARM2.bit.SOSR  = SINC_FILTER_MAX_OSR;}
			break;

		case FILTER3:		//Filter 3
			(*SDFM[sdfmNumber]).SDFSFPARM3.bit.FEN  = Filter_switch;
			(*SDFM[sdfmNumber]).SDFSFPARM3.bit.SST  = filterType;

			//Configure Sinc filter OSR value //
			if(OSR<=SINC_FILTER_MAX_OSR){(*SDFM[sdfmNumber]).SDFSFPARM3.bit.SOSR  = OSR;}
			else{(*SDFM[sdfmNumber]).SDFSFPARM3.bit.SOSR  = SINC_FILTER_MAX_OSR;}
			break;

		case FILTER4:		//Filter 4
			(*SDFM[sdfmNumber]).SDFSFPARM4.bit.FEN = Filter_switch;
			(*SDFM[sdfmNumber]).SDFSFPARM4.bit.SST  = filterType;

			//Configure Sinc filter OSR value //
			if(OSR<=SINC_FILTER_MAX_OSR){(*SDFM[sdfmNumber]).SDFSFPARM4.bit.SOSR  = OSR;}
			else{(*SDFM[sdfmNumber]).SDFSFPARM4.bit.SOSR  = SINC_FILTER_MAX_OSR;}
			break;
	}

	EDIS;

}


//---------------------------------------------------------------------------
// Function name: SDFM_configureIntegrator
//---------------------------------------------------------------------------
// This function configures SDFM Integrator unit
//
// SDFM Integrator filter unit can be configured in any of four different
// Sinc filter types:
// Filter type: SINC1 / SINC2 / SINC3 / SINCFAST
//
// sdfmNumber      :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber    :This parameter is used to select which filter
//                  (FILTER1,FILTER2,FILTER3,FILTER3) needs to be configured
// INTG_switch     :This parameter is used to enable / disable integrator unit
// filterType      :This parameter is used to select one of the filter type mentioned above
// OSR			   :This parameter is used to configure oversampling ratio for
//                  Sinc filter (Upto OSR_128)
// DR_switch	   :This parameter selects whether data is represented in 16 (or) 32 bits
// shift_bits	   :When user chooses 16 bit representation, this variable allows
//                  to right shift by specific number of bits
// DEN_switch	   :This parameter is used to enable / disable demodulator unit
//


void Sdfm_configureIntegrator(Uint16 sdfmNumber, Uint16 filterNumber, Uint16 INTG_switch, Uint16 filterType, Uint16 OSR, Uint16 DR_switch,Uint16 shift_bits ,Uint16 DEN_switch)
{

	EALLOW;

	switch(filterNumber)
	{

		case FILTER1:		//Filter 1
			(*SDFM[sdfmNumber]).SDFIPARM1.bit.IEN   = INTG_switch;					//Enable / Disable Integrator
			(*SDFM[sdfmNumber]).SDFIPARM1.bit.DR    = DR_switch;			//Data Representation (0/1 = 16/32b 2's complement)
			if(DR_switch == 0){(*SDFM[sdfmNumber]).SDFIPARM1.bit.SH    = shift_bits;}			//Shift control
			(*SDFM[sdfmNumber]).SDFIPARM1.bit.DEN 	 = DEN_switch;			//Enable / Disable Demodulator
			if(OSR<=INTEGRATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFIPARM1.bit.IOSR  = OSR;}	//Configure OSR value //
			else{(*SDFM[sdfmNumber]).SDFIPARM1.bit.IOSR  = INTEGRATOR_MAX_OSR;}
			break;

		case FILTER2:		//Filter 2
			(*SDFM[sdfmNumber]).SDFIPARM2.bit.IEN   = INTG_switch;					//Enable / Disable Integrator
			(*SDFM[sdfmNumber]).SDFIPARM2.bit.DR    = DR_switch;			//Data Representation (0/1 = 16/32b 2's complement)
			if(DR_switch == 0){(*SDFM[sdfmNumber]).SDFIPARM2.bit.SH    = shift_bits;}			//Shift control
			(*SDFM[sdfmNumber]).SDFIPARM2.bit.DEN 	 = DEN_switch;			//Enable / Disable Demodulator
			if(OSR<=INTEGRATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFIPARM2.bit.IOSR  = OSR;}	//Configure OSR value //
			else{(*SDFM[sdfmNumber]).SDFIPARM2.bit.IOSR  = INTEGRATOR_MAX_OSR;}
			break;

		case FILTER3:		//Filter 3
			(*SDFM[sdfmNumber]).SDFIPARM3.bit.IEN   = INTG_switch;					//Enable / Disable Integrator
			(*SDFM[sdfmNumber]).SDFIPARM3.bit.DR    = DR_switch;			//Data Representation (0/1 = 16/32b 2's complement)
			if(DR_switch == 0){(*SDFM[sdfmNumber]).SDFIPARM3.bit.SH    = shift_bits;}			//Shift control
			(*SDFM[sdfmNumber]).SDFIPARM3.bit.DEN 	 = DEN_switch;			//Enable / Disable Demodulator
			if(OSR<=INTEGRATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFIPARM3.bit.IOSR  = OSR;}	//Configure OSR value //
			else{(*SDFM[sdfmNumber]).SDFIPARM3.bit.IOSR  = INTEGRATOR_MAX_OSR;}
			break;

		case FILTER4:		//Filter 4
			(*SDFM[sdfmNumber]).SDFIPARM4.bit.IEN   = INTG_switch;					//Enable / Disable Integrator
			(*SDFM[sdfmNumber]).SDFIPARM4.bit.DR    = DR_switch;			//Data Representation (0/1 = 16/32b 2's complement)
			if(DR_switch == 0){(*SDFM[sdfmNumber]).SDFIPARM4.bit.SH    = shift_bits;}			//Shift control
			(*SDFM[sdfmNumber]).SDFIPARM4.bit.DEN 	 = DEN_switch;			//Enable / Disable Demodulator
			if(OSR<=INTEGRATOR_MAX_OSR){(*SDFM[sdfmNumber]).SDFIPARM4.bit.IOSR  = OSR;}	//Configure OSR value //
			else{(*SDFM[sdfmNumber]).SDFIPARM4.bit.IOSR  = INTEGRATOR_MAX_OSR;}
			break;
	}

	EDIS;

}


//---------------------------------------------------------------------------
// Function name: Sdfm_configureInterrupt
//---------------------------------------------------------------------------
// This function configures SDFM Interrupt unit
//
// SDFM Interrupt unit can be configured to enable / disable different sources
// of SDFM interrupts which should trigger CPU interrupt
//
// sdfmNumber      :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber    :This parameter is used to select which filter
//                  (FILTER1,FILTER2,FILTER3,FILTER3) needs to be configured
// IEH_Switch      :This parameter allows over value condition to trigger CPU interrupt
// IEL_Switch      :This parameter allows under value condition to trigger CPU interrupt
// MFIE_Switch	   :This parameter allows modulator failure to trigger CPU interrupt
// AE_Switch	   :This parameter allows new filter data acknowledge interrupt
//                  signal to trigger CPU interrupt
//


void Sdfm_configureInterrupt(Uint16 sdfmNumber, Uint16 filterNumber,Uint16 IEH_Switch, Uint16 IEL_Switch, Uint16 MFIE_Switch, Uint16 AE_Switch)
{

	EALLOW;

	switch(filterNumber)
	{

		case FILTER1:		//Filter 1
			(*SDFM[sdfmNumber]).SDFCPARM1.bit.IEH  =  IEH_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM1.bit.IEL  =  IEL_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM1.bit.MFIE =  MFIE_Switch;
			(*SDFM[sdfmNumber]).SDFSFPARM1.bit.AE  =  AE_Switch;
			break;

		case FILTER2:		//Filter 2
			(*SDFM[sdfmNumber]).SDFCPARM2.bit.IEH  =  IEH_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM2.bit.IEL  =  IEL_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM2.bit.MFIE =  MFIE_Switch;
			(*SDFM[sdfmNumber]).SDFSFPARM2.bit.AE  =  AE_Switch;
			break;

		case FILTER3:		//Filter 3
			(*SDFM[sdfmNumber]).SDFCPARM3.bit.IEH  =  IEH_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM3.bit.IEL  =  IEL_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM3.bit.MFIE =  MFIE_Switch;
			(*SDFM[sdfmNumber]).SDFSFPARM3.bit.AE  =  AE_Switch;
			break;

		case FILTER4:		//Filter 4
			(*SDFM[sdfmNumber]).SDFCPARM4.bit.IEH  =  IEH_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM4.bit.IEL  =  IEL_Switch;
			(*SDFM[sdfmNumber]).SDFCPARM4.bit.MFIE =  MFIE_Switch;
			(*SDFM[sdfmNumber]).SDFSFPARM4.bit.AE  =  AE_Switch;
			break;
	}

	EDIS;

}


//---------------------------------------------------------------------------
// Function name: SDFM_configExternalreset
//---------------------------------------------------------------------------
// This function configures SDFM module to enable / disable external filter reset from PWM
//
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
// filter1_Config_ext_reset  :This parameter is used to enable / disable external PWM reset for filter1
// filter2_Config_ext_reset  :This parameter is used to enable / disable external PWM reset for filter2
// filter3_Config_ext_reset  :This parameter is used to enable / disable external PWM reset for filter3
// filter4_Config_ext_reset  :This parameter is used to enable / disable external PWM reset for filter4
//

void Sdfm_configureExternalreset(Uint16 sdfmNumber, Uint16 filter1_Config_ext_reset, Uint16 filter2_Config_ext_reset, Uint16 filter3_Config_ext_reset, Uint16 filter4_Config_ext_reset)
{

	EALLOW;
	(*SDFM[sdfmNumber]).SDFSFPARM1.bit.FILRESEN = filter1_Config_ext_reset;
	(*SDFM[sdfmNumber]).SDFSFPARM2.bit.FILRESEN = filter2_Config_ext_reset;
	(*SDFM[sdfmNumber]).SDFSFPARM3.bit.FILRESEN = filter3_Config_ext_reset;
	(*SDFM[sdfmNumber]).SDFSFPARM4.bit.FILRESEN = filter4_Config_ext_reset;
	EDIS;

}

//---------------------------------------------------------------------------
// Function name: SDFM_enableMFE
//---------------------------------------------------------------------------
// This function enables Master filter bit of SDFM module
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
//

void Sdfm_enableMFE(Uint16 sdfmNumber)
{

	EALLOW;
	(*SDFM[sdfmNumber]).SDMFILEN.bit.MFE = 1;    //Master Filter bit is enabled
	EDIS;

}

//---------------------------------------------------------------------------
// Function name: SDFM_disableMFE
//---------------------------------------------------------------------------
// This function disable Master filter bit of SDFM module
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
//

void SDFM_disableMFE(Uint16 sdfmNumber)
{

	EALLOW;
	(*SDFM[sdfmNumber]).SDMFILEN.bit.MFE = 0;    //Master Filter bit is disabled
	EDIS;

}

//---------------------------------------------------------------------------
// Function name: SDFM_enableMIE
//---------------------------------------------------------------------------
// This function enable Master Interrupt bit of SDFM module
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
//

void Sdfm_enableMIE(Uint16 sdfmNumber)
{

	EALLOW;
	(*SDFM[sdfmNumber]).SDCTL.bit.MIE = 1;    //Enable MIE (Master Interrupt Enable) bit //
	EDIS;
}

//---------------------------------------------------------------------------
// Function name: Sdfm_disableMIE
//---------------------------------------------------------------------------
// This function disable Master Interrupt bit of SDFM module
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
//

void Sdfm_disableMIE(Uint16 sdfmNumber)
{

	EALLOW;
	(*SDFM[sdfmNumber]).SDCTL.bit.MIE = 0;    //Disable MIE (Master Interrupt Enable) bit //
	EDIS;
}


//---------------------------------------------------------------------------
// Function name: Sdfm_readManchesterStatus
//---------------------------------------------------------------------------
// This function helps user read the manchester status of a give filter
//
// sdfmNumber    			  :This parameter should be used to select SDFM1 (or) SDFM2
// filterNumber 			  :This parameter should be used to select any of the 4 filters
//

Uint16 Sdfm_readManchesterStatus(Uint16 sdfmNumber, Uint16 filterNumber)
{

	Uint16 modulatorStatus = 0;

	EALLOW;

	switch(filterNumber){

		case FILTER1:
			modulatorStatus = (*SDFM[sdfmNumber]).SDFCTLPARM1.bit.MS;
			break;
		case FILTER2:
			modulatorStatus = (*SDFM[sdfmNumber]).SDFCTLPARM1.bit.MS;
			break;
		case FILTER3:
			modulatorStatus = (*SDFM[sdfmNumber]).SDFCTLPARM1.bit.MS;
			break;
		case FILTER4:
			modulatorStatus = (*SDFM[sdfmNumber]).SDFCTLPARM1.bit.MS;
			break;
	}

	EDIS;
	return(modulatorStatus);
}

//---------------------------------------------------------------------------
// Function name: Sdfm_readFlagRegister
//---------------------------------------------------------------------------
// This function helps user read SDFM flag register (SDIFLG)
//
//

Uint32 Sdfm_readFlagRegister(Uint16 sdfmNumber)
{
	return ((*SDFM[sdfmNumber]).SDIFLG.all);
}

//---------------------------------------------------------------------------
// Function name: Sdfm_clearFlagRegister
//---------------------------------------------------------------------------
// This function helps is used to clear SDIFLG register
//
//

void Sdfm_clearFlagRegister(Uint16 sdfmNumber,Uint32 sdfmReadFlagRegister)
{
	(*SDFM[sdfmNumber]).SDIFLGCLR.all = sdfmReadFlagRegister;
}
