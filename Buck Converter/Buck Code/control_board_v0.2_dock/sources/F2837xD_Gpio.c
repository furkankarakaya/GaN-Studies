//###########################################################################
// FILE:	F2837xD_Gpio.c
// TITLE:	GPIO module support functions
// AUTHOR:	Adam Haun
// NOTES:
//###########################################################################
// $TI Release: F2837xD Support Library v100 $
// $Release Date: Mon Dec  9 12:58:09 CST 2013 $
//###########################################################################

#include "F2837xD_Device.h"
#include "F2837xD_Examples.h"

//Low-level functions for GPIO configuration (CPU1 only)

#ifdef CPU1
	//Sets all pins to be muxed to GPIO in input mode with pull-ups enabled.
	//Also resets CPU control to CPU1 and disables open drain and polarity
	//inversion and sets the qualification to synchronous. Also unlocks all GPIOs
	//Only one CPU should call this function.
	void InitGpio()
	{
		volatile Uint32 *gpioBaseAddr;
		Uint16 regOffset;

		//Disable pin locks
		EALLOW;
		GpioCtrlRegs.GPALOCK.all = 0x00000000;
		GpioCtrlRegs.GPBLOCK.all = 0x00000000;
		GpioCtrlRegs.GPCLOCK.all = 0x00000000;
		GpioCtrlRegs.GPDLOCK.all = 0x00000000;
		GpioCtrlRegs.GPELOCK.all = 0x00000000;
		GpioCtrlRegs.GPFLOCK.all = 0x00000000;

		//Fill all registers with zeros. Writing to each register separately
		//for six GPIO modules would make this function *very* long. Fortunately,
		//we'd be writing them all with zeros anyway, so this saves a lot of space.
		gpioBaseAddr = (Uint32 *)&GpioCtrlRegs;
		for (regOffset = 0; regOffset < sizeof(GpioCtrlRegs)/2; regOffset++)
		{
			gpioBaseAddr[regOffset] = 0x00000000;
		}

		gpioBaseAddr = (Uint32 *)&GpioDataRegs;
		for (regOffset = 0; regOffset < sizeof(GpioDataRegs)/2; regOffset++)
		{
			gpioBaseAddr[regOffset] = 0x00000000;
		}

		EDIS;
	}


	//Set the peripheral muxing for the specified pin. The appropriate
	//parameters can be found in the pinout spreadsheet.
	void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral)
	{
		volatile Uint32 *gpioBaseAddr;
		volatile Uint32 *mux, *gmux, *csel;
		Uint16 pin32, pin16, pin8;

		pin32 = pin % 32;
		pin16 = pin % 16;
		pin8 = pin % 8;
		gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

		//Sanity check for valid cpu and peripheral values
		if (cpu > GPIO_MUX_CPU2CLA || peripheral > 0xF)
			return;

		//Create pointers to the appropriate registers. This is a workaround
		//for the way GPIO registers are defined. The standard definition
		//in the header file makes it very easy to do named accesses of one
		//register or bit, but hard to do arbitrary numerical accesses. It's
		//easier to have an array of GPIO modules with identical registers,
		//including arrays for multi-register groups like GPyCSEL1-4. But
		//the header file doesn't define anything we can turn into an array,
		//so manual pointer arithmetic is used instead.
		mux = gpioBaseAddr + GPYMUX + pin32/16;
		gmux = gpioBaseAddr + GPYGMUX + pin32/16;
		csel = gpioBaseAddr + GPYCSEL + pin32/8;

		//Now for the actual function
		EALLOW;

		//To change the muxing, set the peripheral mux to 0/GPIO first to avoid
		//glitches, then change the group mux, then set the peripheral mux to
		//its target value. Finally, set the CPU select. This procedure is
		//described in the TRM. Unfortunately, since we don't know the pin in
		//advance we can't hardcode a bitfield reference, so there's some tricky
		//bit twiddling here.
		*mux &= ~(0x3UL << (2*pin16));
		*gmux &= ~(0x3UL << (2*pin16));
		*gmux |= (Uint32)((peripheral >> 2) & 0x3UL) << (2*pin16);
		*mux |= (Uint32)(peripheral & 0x3UL) << (2*pin16);

		*csel &= ~(0x3L << (4*pin8));
		*csel |= (Uint32)(cpu & 0x3L) << (4*pin8);

		//WARNING: This code does not touch the analog mode select registers,
		//which are needed to give the USB module control of its IOs.

		EDIS;
	}


	//Setup up the GPIO input/output options for the specified pin.
	//The flags are a 16-bit mask produced by ORing together options.
	//For input pins, the valid flags are:
	//GPIO_PULLUP	Enable pull-up
	//GPIO_INVERT	Enable input polarity inversion
	//GPIO_SYNC		Synchronize the input latch to PLLSYSCLK (default -- you don't need to specify this)
	//GPIO_QUAL3	Use 3-sample qualification
	//GPIO_QUAL6	Use 6-sample qualification
	//GPIO_ASYNC	Do not use synchronization or qualification
	//(Note: only one of SYNC, QUAL3, QUAL6, or ASYNC is allowed)
	//
	//For output pins, the valid flags are:
	//GPIO_OPENDRAIN	Output in open drain mode
	//GPIO_PULLUP		If open drain enabled, also enable the pull-up
	//and the input qualification flags (SYNC/QUAL3/QUAL6/SYNC) listed above.
	//
	//With no flags, the default input state is synchronous with no
	//pull-up or polarity inversion. The default output state is
	//the standard digital output.
	void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags)
	{
		volatile Uint32 *gpioBaseAddr;
		volatile Uint32 *dir, *pud, *inv, *odr, *qsel;
		Uint32 pin32, pin16, pinMask, qual;

		pin32 = pin % 32;
		pin16 = pin % 16;
		pinMask = 1UL << pin32;
		gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

		//Create pointers to the appropriate registers. This is a workaround
		//for the way GPIO registers are defined. The standard definition
		//in the header file makes it very easy to do named accesses of one
		//register or bit, but hard to do arbitrary numerical accesses. It's
		//easier to have an array of GPIO modules with identical registers,
		//including arrays for multi-register groups like GPyQSEL1-2. But
		//the header file doesn't define anything we can turn into an array,
		//so manual pointer arithmetic is used instead.
		dir = gpioBaseAddr + GPYDIR;
		pud = gpioBaseAddr + GPYPUD;
		inv = gpioBaseAddr + GPYINV;
		odr = gpioBaseAddr + GPYODR;
		qsel = gpioBaseAddr + GPYQSEL + pin32/16;

		EALLOW;

		//Set the data direction
		*dir &= ~pinMask;
		if (output == 1)
		{
			//Output, with optional open drain mode and pull-up
			*dir |= pinMask;

			//Enable open drain if necessary
			if (flags & GPIO_OPENDRAIN)
				*odr |= pinMask;
			else
				*odr &= ~pinMask;

			//Enable pull-up if necessary. Open drain mode must be active.
			if (flags & (GPIO_OPENDRAIN | GPIO_PULLUP))
				*pud &= ~pinMask;
			else
				*pud |= pinMask;
		} else
		{
			//Input, with optional pull-up, qualification, and polarity inversion
			*dir &= ~pinMask;

			//Enable pull-up if necessary
			if (flags & GPIO_PULLUP)
				*pud &= ~pinMask;
			else
				*pud |= pinMask;

			//Invert polarity if necessary
			if (flags & GPIO_INVERT)
				*inv |= pinMask;
			else
				*inv &= ~pinMask;
		}

		//Extract the qualification parameter and load it into the register. This is
		//also needed for open drain outputs, so we might as well do it all the time.
		qual = (flags & GPIO_ASYNC) / GPIO_QUAL3;
		*qsel &= ~(0x3L << (2 * pin16));
		if (qual != 0x0)
			*qsel |= qual << (2 * pin16);

		EDIS;
	}

	//Enable or disable the GPIO register bit lock for the specified pin.
	//The valid flags are:
	//GPIO_UNLOCK	Unlock the pin setup register bits for the specified pin
	//GPIO_LOCK 	Lock the pin setup register bits for the specified pin
	void GPIO_SetupLock(Uint16 pin, Uint16 flags)
	{
		volatile Uint32 *gpioBaseAddr;
		volatile Uint32 *lock;
		Uint32 pin32, pinMask;

		pin32 = pin % 32;
		pinMask = 1UL << pin32;
		gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;

		//Create pointers to the appropriate registers. This is a workaround
		//for the way GPIO registers are defined. The standard definition
		//in the header file makes it very easy to do named accesses of one
		//register or bit, but hard to do arbitrary numerical accesses. It's
		//easier to have an array of GPIO modules with identical registers,
		//including arrays for multi-register groups like GPyQSEL1-2. But
		//the header file doesn't define anything we can turn into an array,
		//so manual pointer arithmetic is used instead.
		lock = gpioBaseAddr + GPYLOCK;


		EALLOW;
		if(flags){
			//Lock the pin
			*lock |= pinMask;
		}else{
			//Unlock the pin
			*lock &= ~pinMask;
		}
		EDIS;
	}

//External interrupt setup
void GPIO_SetupXINT1Gpio(Uint16 pin)
{
	EALLOW;
	TrigRegs.INPUT4SELECT = pin; 		//Set XINT1 source to GPIO-pin
	EDIS;
}
void GPIO_SetupXINT2Gpio(Uint16 pin)
{
	EALLOW;
	TrigRegs.INPUT5SELECT = pin; 		//Set XINT2 source to GPIO-pin
	EDIS;
}
void GPIO_SetupXINT3Gpio(Uint16 pin)
{
	EALLOW;
	TrigRegs.INPUT6SELECT = pin; 		//Set XINT3 source to GPIO-pin
	EDIS;
}
void GPIO_SetupXINT4Gpio(Uint16 pin)
{
	EALLOW;
	TrigRegs.INPUT13SELECT = pin; 	//Set XINT4 source to GPIO-pin
	EDIS;
}
void GPIO_SetupXINT5Gpio(Uint16 pin)
{
	EALLOW;
	TrigRegs.INPUT14SELECT = pin; 	//Set XINT5 source to GPIO-pin
	EDIS;
}
#endif //CPU1


//Read the GPyDAT register bit for the specified pin. Note that this returns
//the actual state of the pin, not the state of the output latch.
Uint16 GPIO_ReadPin(Uint16 pin)
{
	volatile Uint32 *gpioDataReg;
	Uint16 pinVal;

	gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
	pinVal = (gpioDataReg[GPYDAT] >> (pin % 32)) & 0x1;

	return pinVal;
}

//Set the GPyDAT register bit for the specified pin.
void GPIO_WritePin(Uint16 pin, Uint16 outVal)
{
	volatile Uint32 *gpioDataReg;
	Uint32 pinMask;

	gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
	pinMask = 1UL << (pin % 32);

	if (outVal == 0)
		gpioDataReg[GPYCLEAR] = pinMask;
	else
		gpioDataReg[GPYSET] = pinMask;
}


//Optional higher-level functions to let CPU2 also configure the GPIO pins and muxing
static Uint16 gpioIpcIntFlag;
#ifdef CPU1
	static interrupt void GPIO_ConfigInterruptHandler();
#endif


//Select the IPC flag to be used for the GPIO configuration interrupt. In CPU1,
//this function will also enable the interrupt in the PIE. The user must enable
//CPU interrupt line 1. If the function is called with an invalid flag (not 0-3),
//it will not change the GPIO interrupt setup.
//
//WARNING: BOTH CPUS MUST CALL THIS FUNCTION WITH THE SAME FLAG BEFORE CPU2 CAN
//USE ANY OTHER GPIO FUNCTIONS.
void GPIO_SelectIpcInt(Uint16 newFlag)
{
	#ifdef CPU1
		switch (newFlag)
		{
			case 0:
				EALLOW;
				PieVectTable.IPC1_INT = &GPIO_ConfigInterruptHandler;
				PieCtrlRegs.PIEIER1.bit.INTx13 = 1;
				EDIS;
				break;
			case 1:
				EALLOW;
				PieVectTable.IPC2_INT = &GPIO_ConfigInterruptHandler;
				PieCtrlRegs.PIEIER1.bit.INTx14 = 1;
				EDIS;
				break;
			case 2:
				EALLOW;
				PieVectTable.IPC3_INT = &GPIO_ConfigInterruptHandler;
				PieCtrlRegs.PIEIER1.bit.INTx15 = 1;
				EDIS;
				break;
			case 3:
				EALLOW;
				PieVectTable.IPC4_INT = &GPIO_ConfigInterruptHandler;
				PieCtrlRegs.PIEIER1.bit.INTx16 = 1;
				EDIS;
				break;
			default:
				return;
		}
	#else
		if (newFlag > 3)
			return;
	#endif

	gpioIpcIntFlag = newFlag;
}


#ifdef CPU2
	//CPU2 IPC wrapper for GPIO initialization. Only one CPU should call this.
	void InitGpio()
	{
		SendIpcCommand(GPIO_CMD_INIT, 0, 0, gpioIpcIntFlag);
		WaitForIpcAck(gpioIpcIntFlag);
	}

	//CPU2 IPC wrapper for pin mux configuration
	void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral)
	{
		SendIpcCommand(((Uint32)pin << 16) | GPIO_CMD_PINMUX, cpu, peripheral, gpioIpcIntFlag);
		WaitForIpcAck(gpioIpcIntFlag);
	}

	//CPU2 IPC wrapper for pin IO configuration
	void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags)
	{
		SendIpcCommand(((Uint32)pin << 16) | GPIO_CMD_PINOPTS, output, flags, gpioIpcIntFlag);
		WaitForIpcAck(gpioIpcIntFlag);
	}
#endif

//CPU1 interrupt handler for processing CPU2 GPIO commands
#ifdef CPU1
	static interrupt void GPIO_ConfigInterruptHandler()
	{
		Uint32 command, pin, opt1, opt2;

		//Extract the command, pin, and options from the IPC registers
		command = IpcRegs.IPCRECVCOM & 0xFFFF;
		pin = IpcRegs.IPCRECVCOM >> 16;
		opt1 = IpcRegs.IPCRECVADDR;
		opt2 = IpcRegs.IPCRECVDATA;

		//Execute the command
		EALLOW;
		switch (command)
		{
			case GPIO_CMD_INIT:		InitGpio();											break;
			case GPIO_CMD_PINMUX:	GPIO_SetupPinMux(pin, opt1, opt2);					break;
			case GPIO_CMD_PINOPTS:	GPIO_SetupPinOptions(pin, opt1, opt2);				break;
			case GPIO_CMD_WRITE32:	*(volatile Uint32 *)opt1 = opt2;					break;
			case GPIO_CMD_WRITE16:	*(volatile Uint16 *)opt1 = (Uint16)opt2;			break;
			case GPIO_CMD_READ32:	IpcRegs.IPCLOCALREPLY = *(volatile Uint32 *)opt1;	break;
			case GPIO_CMD_READ16:	IpcRegs.IPCLOCALREPLY = *(volatile Uint16 *)opt1;	break;
			default:				break;
		}
		EDIS;

		//Acknowledge the interrupt
		AckIpcFlag(gpioIpcIntFlag);
		PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	}
#endif

