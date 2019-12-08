#include <F2837xD_Device.h>
#include <math.h>
#include <F2837xD_Pie_defines.h>
#include <F2837xD_Examples.h>
//#include <F2837xD_Adc_defines.h>
#include <F2837xD_EPwm_defines.h>

__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);

#define benimdefinem 3
#define DEBUGMODE 0
#define TOGGLE_BENIM_SARILED GpioDataRegs.GPCTOGGLE.bit.GPIO94 = 1;
#define BENIMUSTTRANSISTORUM EPwm1Regs

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
    CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC =0;
    EDIS;
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    ConfigCpuTimer(&CpuTimer0, 200, 1000000); //1 seconds
    ConfigCpuTimer(&CpuTimer1, 200, 1000000); //1 seconds
    ConfigCpuTimer(&CpuTimer2, 200, 1000000); //1 seconds

    CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0


    InitPieCtrl();// first link F2837xD_PieCtrl.c
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    EDIS;


    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC =1;

    EDIS;
    IER |= M_INT1;
    IER |= M_INT13;
    IER |= M_INT14;

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
#if DEBUGMODE
printfler
gpiolar

#endif

    GpioSelect();
    while(1)
    {

    }

}
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    GpioDataRegs.GPDTOGGLE.bit.GPIO97 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;
    //GpioDataRegs.GPCTOGGLE.bit.GPIO94 = 1;
    TOGGLE_BENIM_SARILED

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
    GpioCtrlRegs.GPFMUX2.all = 0;
    GpioCtrlRegs.GPFGMUX1.all = 0;
    GpioCtrlRegs.GPFGMUX2.all = 0;


    GpioCtrlRegs.GPDPUD.bit.GPIO97 = 0; // enable pull up
    GpioDataRegs.GPDSET.bit.GPIO97 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPDDIR.bit.GPIO97 = 1; // set it as output

    GpioCtrlRegs.GPCPUD.bit.GPIO94 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO94 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO94 = 1; // set it as output

    GpioDataRegs.GPDCLEAR.bit.GPIO97 = 1;
    GpioDataRegs.GPCSET.bit.GPIO94 = 1;
    EDIS;
}
