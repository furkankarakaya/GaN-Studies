/*
 * Furkan Karakaya - Middle East Technical University
 * furkan.karakaya@metu.edu.tr
 * Two phase interleaved bi-directional step-down converter
 * Thesis Study
 */

#include <F2837xD_Device.h>
#include <math.h>
#include <F2837xD_Pie_defines.h>
#include <F2837xD_Examples.h>
//#include <F2837xD_Adc_defines.h>
#include <F2837xD_EPwm_defines.h>

__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);
__interrupt void xint1_isr(void);
__interrupt void xint2_isr(void);
__interrupt void xint3_isr(void);
__interrupt void xint4_isr(void);
__interrupt void epwm3_tzint_isr(void);
__interrupt void adca2_isr(void);
__interrupt void adcb2_isr(void);


void InitEpwm1(); // PWM1
void InitEpwm2(); // PWM2
void InitEpwm3(); // PWM3
void InitEpwm4(); // PWM4 for ADC
void HardTurnOFF(void);
void SoftTurnOFF(void);
void ConfigureADC(void);
void SetupADCEpwm(void);

extern void GPIO_SetupXINT4Gpio(Uint16);

#define LED1 GPIO97 // Class D
#define LED2 GPIO56 // Class B
#define SW1 GPIO104 // Class D
#define SW2 GPIO29 // Class A
#define TRIG GPIO32 // Class B
#define DUMMYIO GPIO6 // Class A
#define EN1 GPIO58 // Class B
#define EN2 GPIO59 // Class B
#define SOFT1 GPIO122 // Class D
#define SOFT2 GPIO123 // Class D
#define OCP1 GPIO16 // Class A
#define OCP2 GPIO24 // Class A
#define FREEWHEEL GPIO0 // Class A
#define CHARGER GPIO1 // Class A

#define RESULTS_BUFFER_SIZE 256
#define sysclk_frequency    200000000   // Hz
#define sysclk_period       5               // ns
#define pwmclk_frequency    200000000   // Hz
#define pwmclk_period       5               // ns
#define PI                  3.141592654
#define switching_frequency 500000           // Hz
#define dead_time           100             // ns
#define sample_window       65          // clock
#define adc_frequency       50000000    //ns
#define adc_period          200         //ns

#define DPT 1; // BUCK or DPT or SCTEST
#define PB_DEACTIVE 1; // PB_ACTIVE or PB_DEACTIVE

// Global Variables
Uint16 AdcaResults[RESULTS_BUFFER_SIZE];
Uint16 resultsIndex;
volatile Uint16 bufferFull;
volatile Uint16 AdcCurr1;
volatile Uint16 AdcCurr2;
volatile Uint16 AdcVin;
volatile Uint16 AdcVout;
volatile float Vout;
volatile float Vin;
volatile float Curr1;
volatile float Curr2;
volatile Uint16 CodeTrig = 0;

float VoltsPerBit = 0.00004577637;
float CurrSensorSensitivity = 30.3030303;
float VinpGain = 195.9868743;
float VoutGain = 147.6634885;


void GpioSelect(void);


/**
 * main.c
 */
int main(void)
{

    InitSysCtrl();
    InitPeripheralClocks();
    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;   /*disable epwm clock to initialize epwm modules*/
    CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC = 0;
    EDIS;

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0; // EPWM Clock Divide Select: /1 of PLLSYSCLK
    EDIS;
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    ConfigCpuTimer(&CpuTimer0, 200, 1000000); //1 seconds
    ConfigCpuTimer(&CpuTimer1, 200, 1000000); //1 seconds
    ConfigCpuTimer(&CpuTimer2, 200, 1000000); //1 seconds

    CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0


    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.XINT3_INT = &xint3_isr;
    PieVectTable.XINT4_INT = &xint4_isr;
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    PieVectTable.EPWM3_TZ_INT = &epwm3_tzint_isr;
#ifdef BUCK
    PieVectTable.ADCA2_INT = &adca2_isr;
    PieVectTable.ADCB2_INT = &adcb2_isr;
#endif
    EDIS;


    PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; //for XINT3
    PieCtrlRegs.PIEIER12.bit.INTx2 = 1; //for XINT4
#ifdef BUCK
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1; // for ADCA2
    PieCtrlRegs.PIEIER10.bit.INTx6 = 1; // for ADCB2
#endif
    PieCtrlRegs.PIEIER2.bit.INTx3 = 1; // for TZ of EPWM3

    IER |= M_INT1;
    IER |= M_INT2; // For TZ Int. of EPWM3
    IER |= M_INT10; // For ADC Int.
    IER |= M_INT12;
    IER |= M_INT13;
    IER |= M_INT14;


    GpioSelect();
    GPIO_SetupXINT3Gpio(104);
    XintRegs.XINT3CR.bit.POLARITY = 0; // falling edge interrupt
    XintRegs.XINT3CR.bit.ENABLE = 1; // Enable XINT3
    GPIO_SetupXINT4Gpio(29);
    XintRegs.XINT4CR.bit.POLARITY = 0;
    XintRegs.XINT4CR.bit.ENABLE = 1;


    // Enable Gate Drivers
    GpioDataRegs.GPBSET.bit.EN2 = 1;
    GpioDataRegs.GPBSET.bit.EN1 = 1;

    // Initialize PWMs
    InitEpwm1();
    InitEpwm2();
    InitEpwm3();

    ConfigureADC(); // Configure ADC and power it up
    InitEpwm4(); // Initialize ePWM4
    SetupADCEpwm();

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Initialize results buffer
    //
    for(resultsIndex = 0; resultsIndex < RESULTS_BUFFER_SIZE; resultsIndex++)
    {
        AdcaResults[resultsIndex] = 0;
    }
    resultsIndex = 0;
    bufferFull = 0;
    AdcCurr1 = 0;

    EALLOW;
    #ifdef BUCK
        CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    #endif
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;   /*enable epwm clock to initialize epwm modules*/
    CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC =1;
    EDIS;

        //
    EPwm4Regs.ETSEL.bit.SOCAEN = 1;  //enable SOCA
    EPwm4Regs.TBCTL.bit.CTRMODE = 0; //unfreeze, and enter up count mode

        //
        //wait while ePWM causes ADC conversions, which then cause interrupts,
        //which fill the results buffer, eventually setting the bufferFull

    while(1)
    {
#ifdef SCTEST
#ifdef PB_DEACTIVE
        if (CodeTrig == 1)
        {
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPASET.bit.FREEWHEEL = 1;
        int j = 0;
        for (j = 0; j<100; j++)
            asm(" RPT #100 || NOP");
        GpioDataRegs.GPBSET.bit.TRIG = 1;
        asm(" RPT #25 || NOP");
        GpioDataRegs.GPASET.bit.CHARGER = 1;
        asm(" RPT #40 || NOP");
        EALLOW;
        EPwm3Regs.TZFRC.bit.OST = 1;
        EDIS;
        //GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        asm(" RPT #2 || NOP");
        GpioDataRegs.GPBCLEAR.bit.TRIG = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;

        CodeTrig = 0;
        }
#endif
#endif



    }
}





__interrupt void epwm3_tzint_isr(void)
{
    asm(" RPT #10 || NOP");
    HardTurnOFF();
    int i = 0;
    GpioDataRegs.GPBSET.bit.LED2 = 1;
    GpioDataRegs.GPDSET.bit.LED1 = 1;
    for(i = 0; i<1000 ; i++)
    {
        GpioDataRegs.GPBTOGGLE.bit.LED2 = 1;
        GpioDataRegs.GPDTOGGLE.bit.LED1 = 1;
        DELAY_US(1000000);
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}
__interrupt void xint4_isr(void)
{
    GpioDataRegs.GPBTOGGLE.bit.LED2 = 1;
    EALLOW;
    WdRegs.WDCR.all = 0x0000;
    EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
__interrupt void xint3_isr(void)
{
    #ifdef DPT
    int i = 0;
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPBSET.bit.TRIG = 1;

        for (i = 0; i < 1; ++i)
        {
            GpioDataRegs.GPASET.bit.CHARGER = 1;
            asm(" RPT #75 || NOP");
        }
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        asm(" RPT #10|| NOP");
        asm(" RPT #10 || NOP");
        GpioDataRegs.GPASET.bit.FREEWHEEL = 1;
        asm(" RPT #10 || NOP");
        asm(" RPT #10 || NOP");
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        asm(" RPT #10 || NOP");
        asm(" RPT #10 || NOP");
        GpioDataRegs.GPASET.bit.CHARGER = 1;
        asm(" RPT #10 || NOP");
        asm(" RPT #10 || NOP");
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPBCLEAR.bit.TRIG = 1;
        asm(" RPT #2000 || NOP");
    #endif

    #ifdef SCTEST
#ifdef PB_ACTIVE
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPASET.bit.FREEWHEEL = 1;
        int j = 0;
        for (j = 0; j<100; j++)
            asm(" RPT #100 || NOP");
        GpioDataRegs.GPBSET.bit.TRIG = 1;
        asm(" RPT #25 || NOP");
        GpioDataRegs.GPASET.bit.CHARGER = 1;
        asm(" RPT #40 || NOP");
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
        /*EALLOW;
        EPwm3Regs.TZFRC.bit.OST = 1;
        EDIS;*/
        asm(" RPT #2 || NOP");
        GpioDataRegs.GPBCLEAR.bit.TRIG = 1;
        GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
        GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
    #endif

    #ifdef PB_DEACTIVE
       CodeTrig = 1;
    #endif
#endif
    GpioDataRegs.GPDTOGGLE.bit.LED1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
/*__interrupt void xint2_isr(void)
{

}
__interrupt void xint1_isr(void)
{

//    asm(" RPT #40 || NOP");

}*/

__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer2_isr(void)
{
    CpuTimer2.InterruptCount++;

}
void GpioSelect(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.all = 0;
    GpioCtrlRegs.GPAMUX2.all = 0;
    GpioCtrlRegs.GPAGMUX1.all = 0;
    GpioCtrlRegs.GPAGMUX2.all = 0;

    GpioCtrlRegs.GPBMUX1.all = 0;
    GpioCtrlRegs.GPBMUX2.all = 0;
    GpioCtrlRegs.GPBGMUX1.all = 0;
    GpioCtrlRegs.GPBGMUX2.all = 0;

    GpioCtrlRegs.GPCMUX1.all = 0;
    GpioCtrlRegs.GPCMUX2.all = 0;
    GpioCtrlRegs.GPCGMUX1.all = 0;
    GpioCtrlRegs.GPCGMUX2.all = 0;

    GpioCtrlRegs.GPDMUX1.all = 0;
    GpioCtrlRegs.GPDMUX2.all = 0;
    GpioCtrlRegs.GPDGMUX1.all = 0;
    GpioCtrlRegs.GPDGMUX2.all = 0;

    GpioCtrlRegs.GPEMUX1.all = 0;
    GpioCtrlRegs.GPEMUX2.all = 0;
    GpioCtrlRegs.GPEGMUX1.all = 0;
    GpioCtrlRegs.GPEGMUX2.all = 0;

    GpioCtrlRegs.GPFMUX1.all = 0;
//    GpioCtrlRegs.GPFMUX2.all = 0;
    GpioCtrlRegs.GPFGMUX1.all = 0;
//    GpioCtrlRegs.GPFGMUX2.all = 0;


// LED GPIOs
    GpioCtrlRegs.GPDPUD.bit.LED1 = 0; // enable pull up
    GpioDataRegs.GPDSET.bit.LED1 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPDDIR.bit.LED1 = 1; // set it as output

    GpioCtrlRegs.GPBPUD.bit.LED2 = 0; // enable pull up
    GpioDataRegs.GPBSET.bit.LED2 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPBDIR.bit.LED2 = 1; // set it as output

    GpioDataRegs.GPDCLEAR.bit.LED1 = 1;
    GpioDataRegs.GPBCLEAR.bit.LED2 = 1;

// TRIG GPIO
    GpioCtrlRegs.GPBPUD.bit.TRIG = 1; // enable pull up
    GpioCtrlRegs.GPBDIR.bit.TRIG = 1; // set it as output
    GpioDataRegs.GPBCLEAR.bit.TRIG = 1; // Clear data
    GpioCtrlRegs.GPAPUD.bit.DUMMYIO = 1; // enable pull up
    GpioCtrlRegs.GPADIR.bit.DUMMYIO = 1; // set it as output
    GpioDataRegs.GPACLEAR.bit.DUMMYIO = 1; // Clear data


// Push Button GPIOs
    GpioCtrlRegs.GPDPUD.bit.SW1 = 1;
    GpioCtrlRegs.GPDDIR.bit.SW1 = 0; // PB1
    GpioCtrlRegs.GPDQSEL1.bit.SW1 = 0; // XINT1 Synch to SYSCLKOUT only
    GpioCtrlRegs.GPDCTRL.bit.QUALPRD1 = 0xFF;  //510*SYSCLOCK Low-pass Filter

    GpioCtrlRegs.GPAPUD.bit.SW2 = 1;
    GpioCtrlRegs.GPADIR.bit.SW2 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.SW2 = 0;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0xFF;

// Gate Driver Enable GPIOs
    GpioCtrlRegs.GPBPUD.bit.EN1 = 1; // disable pull up
    GpioCtrlRegs.GPBDIR.bit.EN1 = 1; // set it as output
    GpioDataRegs.GPBCLEAR.bit.EN1 = 1; // set 0

    GpioCtrlRegs.GPBPUD.bit.EN2 = 1; // enable pull up
    GpioCtrlRegs.GPBDIR.bit.EN2 = 1; // set it as output
    GpioDataRegs.GPBCLEAR.bit.EN2 = 1; // set 0


// PWM GPIOs
GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
#ifdef BUCK
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;  // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;  // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
#endif
#ifdef DPT
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
#endif
#ifdef SCTEST
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
#endif
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;  // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;  // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B


#ifdef SCTEST
#ifdef PB_ACTIVE
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
#endif
#ifdef PB_DEACTIVE
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;  // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;  // Configure GPIO3 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO3 as EPWM3B
#endif
#endif

#ifdef BUCK
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;  // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;  // Configure GPIO3 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO3 as EPWM3B
#endif

#ifdef DPT
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;  // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;  // Configure GPIO3 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO2 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO3 as EPWM3B
#endif
// Over-current Protection GPIOs
    GpioCtrlRegs.GPAPUD.bit.OCP1 = 0; // Pull up is not disabled
    GpioCtrlRegs.GPADIR.bit.OCP1 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.OCP1 = 3; // Asynch input
    InputXbarRegs.INPUT1SELECT = 16;

    GpioCtrlRegs.GPAPUD.bit.OCP2 = 0; // Pull up is not disabled
    GpioCtrlRegs.GPADIR.bit.OCP2 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.OCP2 = 3; // Asynch input
    InputXbarRegs.INPUT2SELECT = 24;


    EDIS;
}

void InitEpwm1(void)
{

#ifdef BUCK
    EPwm1Regs.TBCTL.all = 0x00;
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm1Regs.TBPRD = (pwmclk_frequency) / (switching_frequency) /2; // PWM Block has half clock freq of system & up-down mode requires division by 2
    EPwm1Regs.TBCTR = 0x0000;          // Clear counter

    EPwm1Regs.CMPCTL.all = 0x00;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm1Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm1Regs.AQCTLA.all = 0x00;
    EPwm1Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm1Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm1Regs.AQCTLB.all = 0x00;
    //EPwm1Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm1Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm1Regs.CMPA.bit.CMPA = EPwm1Regs.TBPRD / 2;    // Set compare A value
    //EPwm1Regs.CMPB.half.CMPB = EPwm1Regs.TBPRD/2;    // Set Compare B value

    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0

    EPwm1Regs.TBCTL2.all = 0x00;
    EPwm1Regs.CMPCTL2.all = 0x00;
    EPwm1Regs.DBCTL.all = 0x00;
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBCTL.bit.POLSEL = 2;
    EPwm1Regs.DBFED.bit.DBFED = dead_time / pwmclk_period;
    EPwm1Regs.DBRED.bit.DBRED = dead_time / pwmclk_period;
    EPwm1Regs.DBCTL2.all = 0x00;
    //EPwm1Regs.TBCTL.bit.SYNCOSEL = 3;
    EPwm1Regs.ETSEL.all = 0x00;

    EPwm1Regs.TBCTL.bit.PHSEN = 0;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 3; //SYNCOUT

#endif
//    EPwm1Regs.ETSEL.bit.INTSEL = 1; // When TBCTR == 0
//    EPwm1Regs.ETSEL.bit.INTEN = 1;  // Enable INT
    //EPwm1Regs.ETPS.all = 0x00;
//    EPwm1Regs.ETPS.bit.INTPRD = 1;  // Generate INT on first event

}

void InitEpwm2(void)
{

    EPwm2Regs.TBCTL.all = 0x00;
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm2Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;


    EPwm2Regs.TBPRD = (pwmclk_frequency) / (switching_frequency) /2; // PWM Block has half clock freq of system & up-down mode requires division by 2
    EPwm2Regs.TBCTR = 0x0000;          // Clear counter

    EPwm2Regs.CMPCTL.all = 0x00;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm2Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm2Regs.AQCTLA.all = 0x00;
    EPwm2Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm2Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm2Regs.AQCTLB.all = 0x00;
    //EPwm2Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm2Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm2Regs.CMPA.bit.CMPA = EPwm2Regs.TBPRD / 2;    // Set compare A value
    //EPwm2Regs.CMPB.half.CMPB = EPwm2Regs.TBPRD/2;    // Set Compare B value



    EPwm2Regs.TBCTL2.all = 0x00;
    EPwm2Regs.CMPCTL2.all = 0x00;
    EPwm2Regs.DBCTL.all = 0x00;
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBFED.bit.DBFED = dead_time / pwmclk_period;
    EPwm2Regs.DBRED.bit.DBRED = dead_time / pwmclk_period;
    EPwm2Regs.DBCTL2.all = 0x00;

    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0; //SYNCIN
    EPwm2Regs.TBCTL.bit.PHSDIR = 0;
    EPwm2Regs.TBPHS.bit.TBPHS = (pwmclk_frequency) / (switching_frequency) /2;          // Phase is 180
    EPwm2Regs.TBCTL.bit.SWFSYNC = 1;


}

void InitEpwm3(void)
{

    EALLOW;
    EPwm3Regs.TZSEL.bit.OSHT1 = 1;
    EPwm3Regs.TZSEL.bit.OSHT2 = 1;

    EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_HI; // set EPWM3A to HIGH
    EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_HI; // set EPWM3B to HIGH

    EPwm3Regs.TZEINT.bit.OST = 1;
    EDIS;

    EPwm3Regs.TBPRD = 12000;                        // Set timer period
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                       // Clear counter

    //
    // Setup TBCLK
    //
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV4;

    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

    //
    // Setup compare
    //
    EPwm3Regs.CMPA.bit.CMPA = 12001;

    //
    // Set actions
    //
    EPwm3Regs.AQCTLA.all = 0x00;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on CAU
    EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;           // Clear PWM1A on CAD
    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;

}



void InitEpwm4(void)
{
    // after enabling the ePWM clock
    EALLOW;
    EPwm4Regs.ETSEL.bit.SOCAEN = 0;
    EPwm4Regs.ETSEL.bit.SOCASEL = 4;
    EPwm4Regs.ETPS.bit.SOCAPRD = 1;
    EPwm4Regs.CMPA.bit.CMPA = (3*sample_window - 1)/2;
    EPwm4Regs.TBPRD = 3*sample_window;
    EPwm4Regs.TBCTL.bit.CTRMODE = 3; // freeze counter
    EDIS;
}

void ConfigureADC(void)
{
    EALLOW;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_DIFFERENTIAL);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1; // Set pulse pos. to late
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1; // Power up ADC block
    DELAY_US(1000);

    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_DIFFERENTIAL);
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1; // Set pulse pos. to late
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1; // Power up ADC block
    DELAY_US(1000);
    EDIS;
}

void SetupADCEpwm()
{

    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert (ADCINA0 - ADCINA1)
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = sample_window; // 1.1MSPS -> 909 ns sampling window -> 910ns/5ns = 182 -> acqps = 182 - 1
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 11; // ePWM4 for Trig Signal
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 2;  //SOC1 will convert (ADCINA2 - ADCINA3)
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = sample_window; // 1.1MSPS -> 909 ns sampling window -> 910ns/5ns = 182 -> acqps = 182 - 1
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 11; // ePWM4 for Trig Signal
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 4;  //SOC2 will convert (ADCINA4 - ADCINA5)
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = sample_window; // 1.1MSPS -> 909 ns sampling window -> 910ns/5ns = 182 -> acqps = 182 - 1
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 11; // ePWM4 for Trig Signal

    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 0; // end of SOC2 will set INT2 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 1; // enable INT2 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // re-clear the flag
    // It is assumed all channels will be sampled in 910 nsec in order. Therefore, the results registers can be read after EOC2 signal./
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 4; //SOC2 will convert (ADCINB4 - ADCINB5)
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = sample_window; // 1.1MSPS -> 909 ns sampling window -> 910ns/5ns = 182 -> acqps = 182 - 1
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 11; // ePWM4 for Trig Signal

    AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = 0; // end of SOC0 will set INT2 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT2E = 1; // enable INT2 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // re-clear the flag

    // For removing the offset or for flagging high/low threshold PPB block can be configured accordingly
    EDIS;

}
void HardTurnOFF(void)
{
    // Disable Gate Drivers
      GpioDataRegs.GPBCLEAR.bit.EN2 = 1;
      GpioDataRegs.GPBCLEAR.bit.EN1 = 1;

      EPwm2Regs.CMPA.bit.CMPA = EPwm2Regs.TBPRD + 1;
#ifdef BUCK
      EPwm1Regs.CMPA.bit.CMPA = EPwm1Regs.TBPRD + 1;
#endif
#ifdef DPT
      GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
      GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
#endif
#ifdef SCTEST
      GpioDataRegs.GPACLEAR.bit.FREEWHEEL = 1;
      GpioDataRegs.GPACLEAR.bit.CHARGER = 1;
#endif
}

void SoftTurnOFF(void)
{

}

interrupt void adca2_isr(void)
{
    Curr1 = (AdcaResultRegs.ADCRESULT0*VoltsPerBit - 1.5) * CurrSensorSensitivity;
    Curr2 = (AdcaResultRegs.ADCRESULT1*VoltsPerBit - 1.5) * CurrSensorSensitivity;
    Vin = (AdcaResultRegs.ADCRESULT2*VoltsPerBit - 1.5) * VinpGain;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT2 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}
interrupt void adcb2_isr(void)
{
    //AdcVout = AdcbResultRegs.ADCRESULT0;
    Vout = (AdcbResultRegs.ADCRESULT0*VoltsPerBit - 1.5) * VoutGain;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT2 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}
