#include <F2837xD_Device.h>
#include <math.h>
#include <F2837xD_Pie_defines.h>
#include <picontroller.h>
#include <clarketransform.h>
#include <inverseclarketransform.h>
#include <parktransform.h>
#include <inverseparktransform.h>
#include <F2837xD_Examples.h>
#include <F2837xD_sysctrl.h>

//#include <stdio.h>

#define PI 					3.141592654
#define sqrt3 				1,7321
#define sysclk_frequency 	200000000	// Hz
#define sysclk_period 		5				// ns
#define motor_frequency 50                  // Hz			/*need to define this as constant value rather than macro, for control loop*/
#define switching_frequency 40000           // Hz
#define max_adc_digital 	4095
#define max_adc_analog 		3   			// volt
#define dead_time 			100	 			// ns
#define HIGHSPEED           1
#define LOWSPEED            0
#define CLOCKWISE           1
#define COUNTERCLOCKWISE    0
#define CLOCKHZ             200000000 //200MHz
#define ENCODERRESOLUTION	3600 //3600
#define NUMBEROFTICKSPERQAB 4
#define ENCODERMAXTICKCOUNT (ENCODERRESOLUTION*NUMBEROFTICKSPERQAB-1)
#define NUMBEROFPOLEPAIRS	10
#define VBYFCONTROLENABLE 	1		/*set this zero to disable vbyfcontrol part*/

extern void InitSysCtrl(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitCpuTimers(void);
extern void ConfigCpuTimer(struct CPUTIMER_VARS *, float, float);
extern void InitAdc(void);
extern void InitPeripheralClocks(void);
extern int16 GetTemperatureC(int16);
extern InitTempSensor(float32);
void PositionSpeedCalculate(void);

void Gpio_Select();
void InitSystem();
void Setup_ADC();
void Setup_EQEP();
void InitEpwm1(); // Module-4 Phase-A
void InitEpwm2(); // Module-2 Phase-C
void InitEpwm3(); // Module-2 Phase-B
void InitEpwm4(); // Module-2 Phase-A
void InitEpwm5(); // Module-4 Phase-B
void InitEpwm6(); // Module-1 Phase-C
void InitEpwm7(); // Module-1 Phase-B
void InitEpwm8(); // Module-1 Phase-A
void InitEpwm9(); // Module-3 Phase-C
void InitEpwm10(); // Module-3 Phase-B
void InitEpwm11(); // Module-3 Phase-A
void InitEpwm12(); // Module-4 Phase-C

//interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);
__interrupt void epwm1_isr(void);
__interrupt void adc1_isr(void);
__interrupt void adc2_isr(void);
#if VBYFCONTROLENABLE
__interrupt void isr_Increment_freq(void);
__interrupt void isr_Decrement_freq(void);
void VBYFSetup(void);
#endif
//float out_buffer1[400]; // Buffer for graph-1
//float out_buffer2[400]; // Buffer for graph-2
//const short BUFFERLENGTH = 400; // Size of buffer
//short i = 0; // buffer counter

//float32 fRPM = 0;
float32 fAngularMechanicalPosition = 0;  // mechanical, radians
float32 fAngularMechanicalPositionPrevious = 0;
float32 fDeltaAngularMechanicalPosition = 0;
unsigned long ulPOSCNTPrevious = 0;
float32 fAngularMechanicalSpeed = 0; // mechanical, radians/second
float32 fAngularMechanicalSpeedRPM = 0; //mechanical, RPM
float32 fAngularMechanicalSpeedPrevious = 0;
unsigned int uiHighSpeedFlag = 0; //0:low speed, 1 high speed => simdilik lowda dursun
Uint32 uiPositionLatched = 0;
Uint32 uiPositionLatchedPrevious = 0;
Uint16 uiDirectonOfRotation = 0; //CW=1,CCW=0
Uint32 uiPositionTotalCounted;
unsigned int uiUpEventTickValue = 16;
float fAngularElectricalPosition; // electrical, radians
float fAngularElectricalSpeed; //electrical, radians/second

float Vdc_M1;
float Vdc_M2;
float Vdc_M3;
float Vdc_M4;
float Is_M1_PhA;
float Is_M1_PhB;
float Is_M1_PhC;
float Is_M2_PhA;
float Is_M2_PhB;
float Is_M2_PhC;
float Is_M3_PhA;
float Is_M3_PhB;
float Is_M3_PhC;
float Is_M4_PhA;
float Is_M4_PhB;
float Is_M4_PhC;
float Vdc_M1_adc;
float Vdc_M2_adc;
float Vdc_M3_adc;
float Vdc_M4_adc;
float Is_M1_PhA_adc;
float Is_M1_PhB_adc;
float Is_M1_PhC_adc;
float Is_M2_PhA_adc;
float Is_M2_PhB_adc;
float Is_M2_PhC_adc;
float Is_M3_PhA_adc;
float Is_M3_PhB_adc;
float Is_M3_PhC_adc;
float Is_M4_PhA_adc;
float Is_M4_PhB_adc;
float Is_M4_PhC_adc;
int counter_measurement = 0;
float Is_M1_PhC_max = -1000;
float Is_M1_PhC_min = 1000;
int counter_minmax = 0;
int minmax_enable = 0;

int16 DSP_Temp_Sensor_adc;
int16 DSP_Temp_Sensor;
float32 vrefhi_voltage = 3;

float VdcM1_avg;
float VdcM1_avg_tmp = 0;
float VdcM2_avg;
float VdcM2_avg_tmp = 0;
float VdcM3_avg;
float VdcM3_avg_tmp = 0;
float VdcM4_avg;
float VdcM4_avg_tmp = 0;
int16 Vdc_avg_counter = 0;
int16 Vdc_avg_ctr_max = 200;

float Voltage_TransferFunction = 114.406;
float Current_TransferFunction = 13.333;
float Current_Offset = 1.5;

float Vout_M1_set = 85;	// Volts per phase
float Vout_M2_set = 85;	// Volts per phase
float Vout_M3_set = 85;	// Volts per phase
float Vout_M4_set = 85;	// Volts per phase

float ModulationIndex_M1;
float ModulationIndex_M2;
float ModulationIndex_M3;
float ModulationIndex_M4;
float ModulationIndex_Default = 0.9;

double spwm_counter = 0;

float epwm1_dutycycle = 0.5;
float epwm2_dutycycle = 0.5;
float epwm3_dutycycle = 0.5;
float epwm4_dutycycle = 0.5;
float epwm5_dutycycle = 0.5;
float epwm6_dutycycle = 0.5;
float epwm7_dutycycle = 0.5;
float epwm8_dutycycle = 0.5;
float epwm9_dutycycle = 0.5;
float epwm10_dutycycle = 0.5;
float epwm11_dutycycle = 0.5;
float epwm12_dutycycle = 0.5;

float Vdc_set = 270;
float PI_Vdc_M1_Out[2];
float PI_Vdc_M1_Error;

float Is_M1_Ialfa;
float Is_M1_Ibeta;
float Is_M1_Id;
float Is_M1_Iq;

float Vout_M1_Vd;
float Vout_M1_Vq;
float Vout_M1_Valfa;
float Vout_M1_Vbeta;
float Vout_M1_PhA;
float Vout_M1_PhB;
float Vout_M1_PhC;

#if VBYFCONTROLENABLE /*hakansrc added*/
float32 fVbyfFreqSteps = 5;	   //hz, each push on the button will increase the fundamental frequency by that value
float32 fFrequencySaturation = 50; //hz, max value for fundamental frequency
float32 fVrated = 230.1;		   //Vrms, ma value is calculated depending on this value when there is a change in frequency
float32 fFrequencyInitial = 5;	//hz, initial frequency value
float32 fFundamentalFrequencyToBeApplied = 0;	 //hz, this is the applied frequency to the motor when the motor voltage waveform crosses zero
float32 VbyF_ma_ToBeApplied = 0;
float32 fVdc = 750;

float32 fMotorFundamentalFrequency	= 0;	/*this will be updated when zero cross occurs*/
float32 fAmplitudeModulationIndex 	= 0;	/*this will be updated when zero cross occurs*/
Uint16 uiAzaltFlag=0;
Uint16 uiArtirFlag=0;

#endif

PICONTROLLER picontroller_Vdc_M1 = PICONTROLLER_DEFAULTS;
CLARKETRANSFORM clarke_Is_M1 = CLARKETRANSFORM_DEFAULTS;
INVERSECLARKETRANSFORM inverseclarke_Vout_M1 = INVERSECLARKETRANSFORM_DEFAULTS;
PARKTRANSFORM park_Is_M1 = PARKTRANSFORM_DEFAULTS;
INVERSEPARKTRANSFORM inversepark_Vout_M1 = INVERSEPARKTRANSFORM_DEFAULTS;

int main(void)
{
    //printf("selamlar");
    //memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    //EDIS;
    //InitFlash();

    InitSysCtrl();    // first link F2837xD_SysCtrl.c
    // if you are using in flash
    // add 2837x_FLASH_lnk_cpu1_bist.cmd
    // and add "_FLASH" to your predefined options

    InitPeripheralClocks();

    EALLOW;
    // initperipheral clock diyince bunlara gerek kalmiyor sanirim.
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;/*enable clock for epwm1*/
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0; // EPWM Clock Divide Select: /1 of PLLSYSCLK
    EDIS;

    Gpio_Select();
    //TrigRegs.INPUT10SELECT = 0;

    DINT;
    //disable the interrupts

    InitPieCtrl(); // first link F2837xD_PieCtrl.c
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;
    // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    PieVectTable.ADCC1_INT = &adc1_isr;
    PieVectTable.ADCA2_INT = &adc2_isr;

    //PieVectTable.ECAP4_INT = &ecap4_isr;
    EDIS;

    //InitECapModules();
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    ConfigCpuTimer(&CpuTimer0, 200, 250); //0.5 miliseconds (1 kHz square wave)
    ConfigCpuTimer(&CpuTimer1, 200, 2000000); //2 seconds
    ConfigCpuTimer(&CpuTimer2, 200, 1000000); //2 seconds

    InitEpwm1();
    InitEpwm2();
    InitEpwm3();
    InitEpwm4();
    InitEpwm5();
    InitEpwm6();
    InitEpwm7();
    InitEpwm8();
    InitEpwm9();
    InitEpwm10();
    InitEpwm11();
    InitEpwm12();

    //InitAdc();
    Setup_ADC();
    Setup_EQEP();
	#if VBYFCONTROLENABLE
	VBYFSetup();
	#endif
    InitTempSensor(vrefhi_voltage);

    //CpuTimer0Regs.PRD.all = 0xFFFFFFFF;
    CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit = 0
    IER |= M_INT1;   // ADC-A1
    IER |= M_INT10;   // ADC-A2
    IER |= M_INT3;   // EPWM
    //IER |= M_INT4; // ECAP
    IER |= M_INT13;  // CPU1.TIMER1
    IER |= M_INT14;  // CPU1.TIMER2
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // TIMER0
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // EPWM1
    PieCtrlRegs.PIEIER1.bit.INTx3 = 1; // ADCA1
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1; // ADCA2
    PieCtrlRegs.PIEIER1.bit.INTx4 = VBYFCONTROLENABLE;   /*VBYFCONTROLENABLE: used for XINT1_ISR enabling*/
    PieCtrlRegs.PIEIER1.bit.INTx5 = VBYFCONTROLENABLE;   /*VBYFCONTROLENABLE: used for XINT2_ISR enabling*/
    //PieCtrlRegs.PIEIER4.bit.INTx4 = 1;//enable interrupt for ecap4

    EINT;
    // Enable Global interrupt INTM
    ERTM;
    // Enable Global real time interrupt DBGM

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1; // buna da gerek olmayabilir
    WdRegs.WDCR.all = 0x0028; //set the watch dog
    EDIS;

    // Module-1 Enable lar?n setleri
    GpioDataRegs.GPCSET.bit.GPIO94 = 1;
    GpioDataRegs.GPCSET.bit.GPIO93 = 1;
    GpioDataRegs.GPCSET.bit.GPIO92 = 1;
    while (1)
    {
        while (!CpuTimer0.InterruptCount)
        {
        }
        WdRegs.WDKEY.all = 0x55; // serve to watchdog
        CpuTimer0.InterruptCount = 0;

    }

}

void Gpio_Select()
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

    // Module-1 Phase-A PWM (ePWM8)
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;  // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = 0;  // Configure GPIO15 as EPWM8B
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B

    // Module-1 Phase-B PWM (ePWM7)
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 0;  // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 0;  // Configure GPIO13 as EPWM7B
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B

    // Module-1 Phase-C PWM (ePWM6)
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;  // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;  // Configure GPIO11 as EPWM6B
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B

    // Module-2 Phase-A PWM (ePWM4)
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;  // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO7 = 0;  // Configure GPIO7 as EPWM4B
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B

    // Module-2 Phase-B PWM (ePWM3)
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;  // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;  // Configure GPIO5 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

    // Module-2 Phase-C PWM (ePWM2)
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;  // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;  // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B

    // Module-3 Phase-A PWM (ePWM11)
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;   // Disable pull-up on GPIO20 (EPWM11A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;   // Disable pull-up on GPIO21 (EPWM11B)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 1;  // Configure GPIO20 as EPWM11A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO21 = 1;  // Configure GPIO21 as EPWM11B
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;   // Configure GPIO20 as EPWM11A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;   // Configure GPIO21 as EPWM11B

    // Module-3 Phase-B PWM (ePWM10)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;   // Disable pull-up on GPIO18 (EPWM10A)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;   // Disable pull-up on GPIO19 (EPWM10B)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 1;  // Configure GPIO18 as EPWM10A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 1;  // Configure GPIO19 as EPWM10B
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;   // Configure GPIO18 as EPWM10A
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;   // Configure GPIO19 as EPWM10B

    // Module-3 Phase-C PWM (ePWM9)
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1;    // Disable pull-up on GPIO16 (EPWM9A)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;    // Disable pull-up on GPIO17 (EPWM9B)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 1;  // Configure GPIO16 as EPWM9A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 1;  // Configure GPIO17 as EPWM9B
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;   // Configure GPIO16 as EPWM9A
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;   // Configure GPIO17 as EPWM9B

    // Module-4 Phase-A PWM (ePWM1)
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;  // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;  // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

    // Module-4 Phase-B PWM (ePWM5)
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO8 = 0;  // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAGMUX1.bit.GPIO9 = 0;  // Configure GPIO9 as EPWM5B
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B

    // Module-4 Phase-C PWM (ePWM12)
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;   // Disable pull-up on GPIO22 (EPWM12A)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;   // Disable pull-up on GPIO23 (EPWM12B)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 1;  // Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 1;  // Configure GPIO23 as EPWM12B
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;   // Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO23 as EPWM12B

    // Module-1 Phase-A Enable (GPIO94)
    GpioCtrlRegs.GPCPUD.bit.GPIO94 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO94 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO94 = 1; // set it as output

    // Module-1 Phase-B Enable (GPIO93)
    GpioCtrlRegs.GPCPUD.bit.GPIO93 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO93 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO93 = 1; // set it as output

    // Module-1 Phase-C Enable (GPIO92)
    GpioCtrlRegs.GPCPUD.bit.GPIO92 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO92 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO92 = 1; // set it as output

    // Module-2 Phase-A Enable (GPIO91)
    GpioCtrlRegs.GPCPUD.bit.GPIO91 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO91 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO91 = 1; // set it as output

    // Module-2 Phase-B Enable (GPIO90)
    GpioCtrlRegs.GPCPUD.bit.GPIO90 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO90 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO90 = 1; // set it as output

    // Module-2 Phase-C Enable (GPIO89)
    GpioCtrlRegs.GPCPUD.bit.GPIO89 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO89 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO89 = 1; // set it as output

    // Module-3 Phase-A Enable (GPIO77)
    GpioCtrlRegs.GPCPUD.bit.GPIO77 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO77 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO77 = 1; // set it as output

    // Module-3 Phase-B Enable (GPIO78)
    GpioCtrlRegs.GPCPUD.bit.GPIO78 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO78 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO78 = 1; // set it as output

    // Module-3 Phase-C Enable (GPIO79)
    GpioCtrlRegs.GPCPUD.bit.GPIO79 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO79 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO79 = 1; // set it as output

    // Module-4 Phase-A Enable (GPIO74)
    GpioCtrlRegs.GPCPUD.bit.GPIO74 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO74 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO74 = 1; // set it as output

    // Module-4 Phase-B Enable (GPIO75)
    GpioCtrlRegs.GPCPUD.bit.GPIO75 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO75 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO75 = 1; // set it as output

    // Module-4 Phase-C Enable (GPIO76)
    GpioCtrlRegs.GPCPUD.bit.GPIO76 = 0; // enable pull up
    GpioDataRegs.GPCSET.bit.GPIO76 = 1; // Load output latch. Recommended in rm
    GpioCtrlRegs.GPCDIR.bit.GPIO76 = 1; // set it as output

    // Encoder - EQEP1-A
    GpioCtrlRegs.GPBGMUX2.bit.GPIO50 = 0; // Configure GPIO50 as EQEP1A
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 1;  // Configure GPIO50 as EQEP1A
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO50 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 0; // Input qualification type: Sync

    // Encoder - EQEP1-B
    GpioCtrlRegs.GPBGMUX2.bit.GPIO51 = 0; // Configure GPIO51 as EQEP1B
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 1;  // Configure GPIO51 as EQEP1B
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO51 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 0; // Input qualification type: Sync

    // Encoder - EQEP1-I
    GpioCtrlRegs.GPBGMUX2.bit.GPIO53 = 0; // Configure GPIO53 as EQEP1I
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 1;  // Configure GPIO53 as EQEP1I
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO53 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 0; // Input qualification type: Sync

    // Encoder - EQEP2-A
    GpioCtrlRegs.GPBGMUX2.bit.GPIO54 = 0; // Configure GPIO54 as EQEP2A
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;  // Configure GPIO54 as EQEP2A
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 0; // Input qualification type: Sync

    // Encoder - EQEP2-B
    GpioCtrlRegs.GPBGMUX2.bit.GPIO55 = 0; // Configure GPIO55 as EQEP2B
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;  // Configure GPIO55 as EQEP2B
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 0; // Input qualification type: Sync

    // Encoder - EQEP2-I
    GpioCtrlRegs.GPBGMUX2.bit.GPIO57 = 0; // Configure GPIO57 as EQEP2I
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;  // Configure GPIO57 as EQEP2I
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 1;   // Disable pull up
    //GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;   // set it as input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 0; // Input qualification type: Sync

    EDIS;

}
void InitSystem(void) // burayi yukari alalim
{
    EALLOW;
    WdRegs.WDCR.all = 0x0028;         // Watchdog enabled, 4.3 milliseconds
    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1; //enable cputimer0
    EDIS;
}

__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    WdRegs.WDKEY.all = 0xAA;

    /*
     // Module-4 Enables
     GpioDataRegs.GPCTOGGLE.bit.GPIO74 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO75 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO76 = 1;

     // Module-3 Enables
     GpioDataRegs.GPCTOGGLE.bit.GPIO77 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO78 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO79 = 1;

     // Module-2 Enables
     GpioDataRegs.GPCTOGGLE.bit.GPIO91 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO90 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO89 = 1;

     // Module-1 Enables
     GpioDataRegs.GPCTOGGLE.bit.GPIO94 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO93 = 1;
     GpioDataRegs.GPCTOGGLE.bit.GPIO92 = 1;
     */

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer2_isr(void)
{
    CpuTimer2.InterruptCount++;
    // The CPU acknowledges the interrupt.
    //GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

}
__interrupt void epwm1_isr(void)
{
    //GpioDataRegs.GPCSET.bit.GPIO94 = 1;
    GpioDataRegs.GPCSET.bit.GPIO89 = 1;
    //out_buffer1[i] = Vdc_M1;
    //out_buffer2[i] = Vdc_M2;
    //i++;
    //if (i == BUFFERLENGTH) i=0;

    // Update PWM duty cycles
    EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD * epwm1_dutycycle; // Module-4 Phase-A PWM (ePWM1)
    EPwm2Regs.CMPA.half.CMPA = EPwm2Regs.TBPRD * epwm2_dutycycle; // Module-2 Phase-C PWM (ePWM2)
    EPwm3Regs.CMPA.half.CMPA = EPwm3Regs.TBPRD * epwm3_dutycycle; // Module-2 Phase-B PWM (ePWM3)
    EPwm4Regs.CMPA.half.CMPA = EPwm4Regs.TBPRD * epwm4_dutycycle; // Module-2 Phase-A PWM (ePWM4)
    EPwm5Regs.CMPA.half.CMPA = EPwm5Regs.TBPRD * epwm5_dutycycle; // Module-4 Phase-B PWM (ePWM5)
    EPwm6Regs.CMPA.half.CMPA = EPwm6Regs.TBPRD * epwm6_dutycycle; // Module-1 Phase-C PWM (ePWM6)
    EPwm7Regs.CMPA.half.CMPA = EPwm7Regs.TBPRD * epwm7_dutycycle; // Module-1 Phase-B PWM (ePWM7)
    EPwm8Regs.CMPA.half.CMPA = EPwm8Regs.TBPRD * epwm8_dutycycle; // Module-1 Phase-A PWM (ePWM8)
    //EPwm9Regs.CMPA.half.CMPA = EPwm9Regs.TBPRD * epwm9_dutycycle;// Module-3 Phase-C PWM (ePWM9)
    EPwm10Regs.CMPA.half.CMPA = EPwm10Regs.TBPRD * epwm10_dutycycle; // Module-3 Phase-B PWM (ePWM10)
    EPwm11Regs.CMPA.half.CMPA = EPwm11Regs.TBPRD * epwm11_dutycycle; // Module-3 Phase-A PWM (ePWM11)
    EPwm12Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD * epwm12_dutycycle; // Module-4 Phase-C PWM (ePWM12)

    // temporary
    /*hakansrc: TODO, modify this*/
    EPwm9Regs.CMPA.half.CMPA = EPwm9Regs.TBPRD * epwm7_dutycycle; // Module-3 Phase-C PWM (ePWM9)

    //
    GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1;

    EPwm1Regs.ETCLR.bit.INT = 1;
    // Acknowledge this interrupt to receive more interrupts from group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    //GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1;
}

__interrupt void cpu_timer1_isr(void)
{
	CpuTimer1.InterruptCount++;
	// The CPU acknowledges the interrupt.
	// GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
    //Xint1Count++;
    #if VBYFCONTROLENABLE
	if(uiArtirFlag)
	{
	    if((fFundamentalFrequencyToBeApplied+fVbyfFreqSteps)>fFrequencySaturation)
	    {
	        fFundamentalFrequencyToBeApplied = fFrequencySaturation;
	    }
	    else
	    {
	        fFundamentalFrequencyToBeApplied = fFundamentalFrequencyToBeApplied + fVbyfFreqSteps;
	    }
	    VbyF_ma_ToBeApplied = fFundamentalFrequencyToBeApplied*(fVrated/fFrequencySaturation)*(sqrt(3)/(fVdc*0.612));
	    GpioDataRegs.GPASET.bit.GPIO18 = 1;

	}
	if(uiAzaltFlag)
	{

	    if((fFundamentalFrequencyToBeApplied-fVbyfFreqSteps)<fFrequencyInitial)
	    {
	        fFundamentalFrequencyToBeApplied = fFrequencyInitial;
	    }
	    else
	    {
	        fFundamentalFrequencyToBeApplied = fFundamentalFrequencyToBeApplied - fVbyfFreqSteps;

	    }
	    VbyF_ma_ToBeApplied = fFundamentalFrequencyToBeApplied*(fVrated/fFrequencySaturation)*(sqrt(3)/(fVdc*0.612));
	    GpioDataRegs.GPASET.bit.GPIO18 = 1;

	}
    #endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

__interrupt void adc1_isr(void)
{
    //GpioDataRegs.GPCSET.bit.GPIO93 = 1;
    /*hakansrc: TODO, modify this*/
    GpioDataRegs.GPCSET.bit.GPIO75 = 1;
    fAngularMechanicalPosition = ((float32) EQep1Regs.QPOSCNT
            / (ENCODERMAXTICKCOUNT)) * 360; // mech-degrees
    uiDirectonOfRotation = EQep1Regs.QEPSTS.bit.QDF; // Quadrature direction flag: 1=Clockwise

    if (EQep1Regs.QEPSTS.bit.UPEVNT == 1)
    {
        fAngularMechanicalSpeed = 1.0
                * (((float32) CLOCKHZ / (float32) 1.0)
                        / (float32) EQep1Regs.QCPRD) * (float32) 2
                * (float32) PI;/*TODO; parameterize the values,uiUpEventTickValue comes from UPPS value = 1 for now,1 comes from ccps value*/
        fAngularMechanicalSpeed = fAngularMechanicalSpeed
                / (float32) (ENCODERMAXTICKCOUNT);
        fAngularMechanicalSpeedRPM = fAngularMechanicalSpeed * (float32) 60
                / (2 * PI);
        fAngularElectricalSpeed = fAngularMechanicalSpeed * NUMBEROFPOLEPAIRS; //electrical, radians/second

        EQep1Regs.QEPSTS.bit.UPEVNT = 1;              // Clear status flag
    }
    counter_minmax++;
       if (counter_minmax > 100)
           minmax_enable = 1;
    if (minmax_enable == 1)
       {
           if (fAngularMechanicalSpeedRPM > Is_M1_PhC_max)
           {
               Is_M1_PhC_max = fAngularMechanicalSpeedRPM;
           }
           if (fAngularMechanicalSpeedRPM < Is_M1_PhC_min)
           {
               Is_M1_PhC_min = fAngularMechanicalSpeedRPM;
           }
    }
    counter_measurement++;
    Is_M1_PhC_adc += AdccResultRegs.ADCRESULT3;
    //DELAY_US(1);
    /*hakansrc: TODO, modify this*/
    GpioDataRegs.GPCCLEAR.bit.GPIO75 = 1;

    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clears respective flag bit in the ADCINTFLG register
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE

}

__interrupt void adc2_isr(void)
{
    /*hakansrc: TODO, modify this*/
    GpioDataRegs.GPCSET.bit.GPIO76 = 1;

    // Get position and speed
    //PositionSpeedCalculate();
    fAngularMechanicalPosition = ((float32) EQep1Regs.QPOSCNT
            / (ENCODERMAXTICKCOUNT)) * 360; // mech-degrees
    uiDirectonOfRotation = EQep1Regs.QEPSTS.bit.QDF; // Quadrature direction flag: 1=Clockwise
#if 0
            if(EQep1Regs.QEPSTS.bit.QDLF == 1)
            {
                if(EQep1Regs.QPOSCNT != ulPOSCNTPrevious)
                {
                    fAngularMechanicalPositionPrevious = fAngularMechanicalPosition;
                    fAngularMechanicalPosition = ((float32) EQep1Regs.QPOSCNT/(ENCODERMAXTICKCOUNT))*360; // mech-degrees
                    fDeltaAngularMechanicalPosition = fAngularMechanicalPosition - fAngularMechanicalPositionPrevious;
                    if(fDeltaAngularMechanicalPosition<0)
                    {
                        fDeltaAngularMechanicalPosition = 360 -fAngularMechanicalPositionPrevious+fAngularMechanicalPosition;
                    }
                    fAngularMechanicalSpeed = fDeltaAngularMechanicalPosition*switching_frequency;
                    ulPOSCNTPrevious = EQep1Regs.QPOSCNT;

                }
                else
                {
                    fAngularMechanicalSpeed = fAngularMechanicalSpeedPrevious;
                    fAngularMechanicalPosition = fAngularMechanicalPositionPrevious;
                }
            }
            else /*then it is turning in other direction, the position is decreasing*/
            {
                if(EQep1Regs.QPOSCNT != ulPOSCNTPrevious)
                {
                    fAngularMechanicalPositionPrevious = fAngularMechanicalPosition;
                    fAngularMechanicalPosition = ((float32) EQep1Regs.QPOSCNT/(ENCODERMAXTICKCOUNT))*360; // mech-degrees

                    fDeltaAngularMechanicalPosition = fAngularMechanicalPositionPrevious - fAngularMechanicalPosition;/*this part different*/
                    if(fDeltaAngularMechanicalPosition<0)
                    {
                        fDeltaAngularMechanicalPosition = 360 -fAngularMechanicalPosition+fAngularMechanicalPositionPrevious;/*this part is different*/
                    }
                    fAngularMechanicalSpeed = fDeltaAngularMechanicalPosition*switching_frequency;
                    ulPOSCNTPrevious = EQep1Regs.QPOSCNT;

                }
                else
                {
                    fAngularMechanicalSpeed = fAngularMechanicalSpeedPrevious;
                    fAngularMechanicalPosition = fAngularMechanicalPositionPrevious;
                }
            }
#endif

    /*counter_minmax++;
    if (counter_minmax > 10)
        minmax_enable = 1;
*/
    Is_M1_PhC = (((Is_M1_PhC_adc / (float) counter_measurement) * max_adc_analog
            / max_adc_digital) - Current_Offset) * Current_TransferFunction;
/*
    if (minmax_enable == 1)
    {
        if (Is_M1_PhC > Is_M1_PhC_max)
        {
            Is_M1_PhC_max = Is_M1_PhC;
        }
        if (Is_M1_PhC < Is_M1_PhC_min)
        {
            Is_M1_PhC_min = Is_M1_PhC;
        }
    }
  */
    Is_M1_PhC_adc = 0;
    counter_measurement = 0;

    // First digital readings from ADCs
    Vdc_M3_adc = AdcaResultRegs.ADCRESULT0;
    Vdc_M1_adc = AdcaResultRegs.ADCRESULT1;
    Is_M3_PhC_adc = AdcaResultRegs.ADCRESULT3;
    Is_M1_PhA_adc = AdcaResultRegs.ADCRESULT4;
    Is_M2_PhB_adc = AdcaResultRegs.ADCRESULT5;
    Is_M3_PhB_adc = AdcaResultRegs.ADCRESULT14;
    Is_M3_PhA_adc = AdcaResultRegs.ADCRESULT15;
    Is_M4_PhB_adc = AdcbResultRegs.ADCRESULT0;
    Is_M4_PhC_adc = AdcbResultRegs.ADCRESULT1;
    Is_M2_PhC_adc = AdcbResultRegs.ADCRESULT2;
    Is_M4_PhA_adc = AdcbResultRegs.ADCRESULT3;
    Is_M2_PhA_adc = AdccResultRegs.ADCRESULT2;
    //Is_M1_PhC_adc = AdccResultRegs.ADCRESULT3;
    Is_M1_PhB_adc = AdccResultRegs.ADCRESULT4;
    Vdc_M4_adc = AdcdResultRegs.ADCRESULT0;
    Vdc_M2_adc = AdcdResultRegs.ADCRESULT1;
    DSP_Temp_Sensor_adc = AdcaResultRegs.ADCRESULT13;
    DSP_Temp_Sensor = GetTemperatureC(DSP_Temp_Sensor_adc);

    // Calculate actual measurements
    Vdc_M1 = (Vdc_M1_adc * max_adc_analog / max_adc_digital)
            * Voltage_TransferFunction;
    Vdc_M2 = (Vdc_M2_adc * max_adc_analog / max_adc_digital)
            * Voltage_TransferFunction;
    Vdc_M3 = (Vdc_M3_adc * max_adc_analog / max_adc_digital)
            * Voltage_TransferFunction;
    Vdc_M4 = (Vdc_M4_adc * max_adc_analog / max_adc_digital)
            * Voltage_TransferFunction;
    Is_M1_PhA = ((Is_M1_PhA_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M1_PhB = ((Is_M1_PhB_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    //Is_M1_PhC = ((Is_M1_PhC_adc * max_adc_analog / max_adc_digital)
    //		- Current_Offset) * Current_TransferFunction;
    Is_M2_PhA = ((Is_M2_PhA_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M2_PhB = ((Is_M2_PhB_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M2_PhC = ((Is_M2_PhC_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M3_PhA = ((Is_M3_PhA_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M3_PhB = ((Is_M3_PhB_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M3_PhC = ((Is_M3_PhC_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M4_PhA = ((Is_M4_PhA_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M4_PhB = ((Is_M4_PhB_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;
    Is_M4_PhC = ((Is_M4_PhC_adc * max_adc_analog / max_adc_digital)
            - Current_Offset) * Current_TransferFunction;

    // Averaging of Vdc's

    if (Vdc_avg_counter >= Vdc_avg_ctr_max)
    {
        VdcM1_avg = VdcM1_avg_tmp / Vdc_avg_ctr_max;
        VdcM1_avg_tmp = 0;
        VdcM2_avg = VdcM2_avg_tmp / Vdc_avg_ctr_max;
        VdcM2_avg_tmp = 0;
        VdcM3_avg = VdcM3_avg_tmp / Vdc_avg_ctr_max;
        VdcM3_avg_tmp = 0;
        VdcM4_avg = VdcM4_avg_tmp / Vdc_avg_ctr_max;
        VdcM4_avg_tmp = 0;
        Vdc_avg_counter = 0;
    }
    else
    {
        VdcM1_avg_tmp = VdcM1_avg_tmp + Vdc_M1;
        VdcM2_avg_tmp = VdcM2_avg_tmp + Vdc_M2;
        VdcM3_avg_tmp = VdcM3_avg_tmp + Vdc_M3;
        VdcM4_avg_tmp = VdcM4_avg_tmp + Vdc_M4;
        Vdc_avg_counter++;
    }

    // PI Controller Vdc_M1
    picontroller_Vdc_M1.Kp = 1;
    picontroller_Vdc_M1.Ki = 1;
    picontroller_Vdc_M1.upperlim = 1;
    picontroller_Vdc_M1.lowerlim = 0;
    picontroller_Vdc_M1.Uset = Vdc_set;
    picontroller_Vdc_M1.Uin = Vdc_M1;
    picontroller_Vdc_M1.error[1] = PI_Vdc_M1_Error;
    picontroller_Vdc_M1.Yout[1] = PI_Vdc_M1_Out[1];
    PICONTROLLER (picontroller_Vdc_M1);
    PI_Vdc_M1_Error = picontroller_Vdc_M1.error[1];	// error[n-1]
    PI_Vdc_M1_Out[1] = picontroller_Vdc_M1.Yout[1];	// output[n-1]
    PI_Vdc_M1_Out[0] = picontroller_Vdc_M1.Yout[0];	// output[n]

    // Clarke Transform for Module-1 Currents
    clarke_Is_M1.va = Is_M1_PhA;
    clarke_Is_M1.vb = Is_M1_PhA;
    clarke_Is_M1.vc = Is_M1_PhA;
    CLARKETRANSFORM (clarke_Is_M1);
    Is_M1_Ialfa = clarke_Is_M1.valfa;
    Is_M1_Ibeta = clarke_Is_M1.vbeta;

    // Park Transform for Module-1 Currents
    park_Is_M1.valfa = Is_M1_Ialfa;
    park_Is_M1.vbeta = Is_M1_Ibeta;
    park_Is_M1.theta = fAngularElectricalPosition;
    PARKTRANSFORM (park_Is_M1);
    Is_M1_Id = park_Is_M1.vd;
    Is_M1_Iq = park_Is_M1.vq;

    // Inverse Park Transform for Module-1 Output Voltages
    inversepark_Vout_M1.vd = Vout_M1_Vd;
    inversepark_Vout_M1.vq = Vout_M1_Vq;
    inversepark_Vout_M1.theta = fAngularElectricalPosition;
    INVERSEPARKTRANSFORM (inversepark_Vout_M1);
    Vout_M1_Valfa = inversepark_Vout_M1.valfa;
    Vout_M1_Vbeta = inversepark_Vout_M1.vbeta;

    // Inverse Clarke Transform for Module-1 Output Voltages
    inverseclarke_Vout_M1.valfa = Vout_M1_Valfa;
    inverseclarke_Vout_M1.vbeta = Vout_M1_Vbeta;
    INVERSECLARKETRANSFORM (inverseclarke_Vout_M1);
    Vout_M1_PhA = inverseclarke_Vout_M1.va;
    Vout_M1_PhB = inverseclarke_Vout_M1.vb;
    Vout_M1_PhC = inverseclarke_Vout_M1.vc;

#if !VBYFCONTROLENABLE /*hakansrc: following is the default condition*/

	// sinusoidal PWM
	//ModulationIndex_M1 = Vout_M1_set*sqrt3/(0.612*Vdc_set);
	ModulationIndex_M1 = ModulationIndex_Default;
	epwm8_dutycycle = (ModulationIndex_M1
			* sin(2 * PI * motor_frequency * spwm_counter / switching_frequency)
			+ 1) / 2;
	epwm7_dutycycle = (ModulationIndex_M1
			* sin(
					2 * PI * motor_frequency * spwm_counter
							/ switching_frequency - 2 * PI / 3) + 1) / 2;
	epwm6_dutycycle = (ModulationIndex_M1
			* sin(
					2 * PI * motor_frequency * spwm_counter
							/ switching_frequency + 2 * PI / 3) + 1) / 2;

	spwm_counter += 1;
	if (spwm_counter > ((switching_frequency / motor_frequency) - 1))
		spwm_counter = 0;
#else
	spwm_counter += 1;
	if (spwm_counter > ((switching_frequency / fMotorFundamentalFrequency) - 1))
	{
		fMotorFundamentalFrequency = fFundamentalFrequencyToBeApplied;
		fAmplitudeModulationIndex = VbyF_ma_ToBeApplied;
		spwm_counter = 0;
	    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;

	}

	epwm8_dutycycle = (fAmplitudeModulationIndex*sin(2*PI*fMotorFundamentalFrequency*spwm_counter/switching_frequency)+1)/2;
	epwm7_dutycycle = (fAmplitudeModulationIndex*sin(2*PI*fMotorFundamentalFrequency*spwm_counter/switching_frequency-2*PI/3)+1)/2;
	epwm6_dutycycle = (fAmplitudeModulationIndex*sin(2*PI*fMotorFundamentalFrequency*spwm_counter/switching_frequency+2*PI/3)+1)/2;
#endif

    /*
     epwm1_dutycycle;// Module-4 Phase-A PWM (ePWM1)
     epwm2_dutycycle;// Module-2 Phase-C PWM (ePWM2)
     epwm3_dutycycle;// Module-2 Phase-B PWM (ePWM3)
     epwm4_dutycycle;// Module-2 Phase-A PWM (ePWM4)
     epwm5_dutycycle;// Module-4 Phase-B PWM (ePWM5)
     epwm6_dutycycle;// Module-1 Phase-C PWM (ePWM6)
     epwm7_dutycycle;// Module-1 Phase-B PWM (ePWM7)
     epwm8_dutycycle;// Module-1 Phase-A PWM (ePWM8)
     epwm9_dutycycle;// Module-3 Phase-C PWM (ePWM9)
     epwm10_dutycycle;// Module-3 Phase-B PWM (ePWM10)
     epwm11_dutycycle;// Module-3 Phase-A PWM (ePWM11)
     epwm12_dutycycle;// Module-4 Phase-C PWM (ePWM12)

     AdcRegs.ADCTRL2.bit.RST_SEQ1=1; // Clear INT SEQ1 bit
     AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;     // Clear INT SEQ1 bit
     */
    //DELAY_US(1);


    /*hakansrc: TODO, modify this*/
    GpioDataRegs.GPCCLEAR.bit.GPIO76 = 1;

    //AdcaRegs.ADCINTFLG.bit.ADCINT1 // ADC Interrupt 1 Flag. Reading these flags indicates if the associated ADCINT pulse was generated since the last clear
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // Clears respective flag bit in the ADCINTFLG register
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10; // Acknowledge interrupt to PIE

    //GpioDataRegs.GPCCLEAR.bit.GPIO93 = 1;

}

// designed by hakansrc
void PositionSpeedCalculate(void)
{
    /*TODO this part is unused for now, the speed is calculated at adc1_isr*/
    /*TODO cover up the reverse direction case*/
    fAngularMechanicalPosition = ((float32) EQep1Regs.QPOSCNT
            / (ENCODERMAXTICKCOUNT)) * 360; // mech-radians
    uiDirectonOfRotation = EQep1Regs.QEPSTS.bit.QDF; // Quadrature direction flag: 1=Clockwise
    fAngularElectricalPosition = fAngularMechanicalPosition * NUMBEROFPOLEPAIRS; // electrical, radians

    /*Not used currently*/
    /*
     if (uiHighSpeedFlag == HIGHSPEED)
     {
     if (EQep1Regs.QFLG.bit.UTO == 1)   // If unit timeout (depends on QUPRD)
     {
     uiPositionLatchedPrevious = uiPositionLatched;
     uiPositionLatched = EQep1Regs.QPOSLAT;
     if (uiPositionLatched > uiPositionLatchedPrevious)
     uiPositionTotalCounted = (uiPositionLatched
     - uiPositionLatchedPrevious);
     else
     uiPositionTotalCounted = ENCODERTICKCOUNT + uiPositionLatched
     - uiPositionLatchedPrevious;

     //fRPM = uiPositionTotalCounted*(CLOCKHZ/EQep1Regs.QUPRD)*60;
     //fRPM = fRPM/(float32)(ENCODERTICKCOUNT*4);
     fAngularMechanicalSpeed = uiPositionTotalCounted * (CLOCKHZ / EQep1Regs.QUPRD)
     * 2 * PI;
     fAngularMechanicalSpeed = fAngularMechanicalSpeed / (float32) (ENCODERTICKCOUNT * 4);
     EQep1Regs.QCLR.bit.UTO = 1;
     }
     } else
     */
    /*hakansrc: TODO, modify this, unnecessary part*/
    if (uiHighSpeedFlag == LOWSPEED)
    {
        if (EQep1Regs.QEPSTS.bit.UPEVNT == 1)
        {
            //fRPM = 16*((CLOCKHZ/128)/EQep1Regs.QCPRDLAT)*60;
            //fRPM = fRPM/(float32)(ENCODERTICKCOUNT*4);
            fAngularMechanicalSpeed = uiUpEventTickValue
                    * ((CLOCKHZ / 128) / EQep1Regs.QCPRDLAT) * 2 * PI;/*TODO; parameterize the values,uiUpEventTickValue comes from UPPS value = 16 for now,128 comes from ccps value*/
            fAngularMechanicalSpeed = fAngularMechanicalSpeed
                    / (float32) (ENCODERMAXTICKCOUNT);
            fAngularElectricalSpeed = fAngularMechanicalSpeed
                    * NUMBEROFPOLEPAIRS; //electrical, radians/second
            EQep1Regs.QEPSTS.bit.UPEVNT = 1;              // Clear status flag
        }
    }
}

void Setup_ADC(void)
{

    // AdcaRegs.ADCSOCFRC1.all = 0x000F; // Bi ara bakalim

    EALLOW;
    AdcaRegs.ADCCTL1.all = 0x00;            // ADC Control 1 Register
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1; // All analog circuitry inside the core is powered up
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ? Interrupt pulse generation occurs at the end of the conversion, 1 cycle prior to the ADC result latching into its result register

    AdcbRegs.ADCCTL1.all = 0x00;            // ADC Control 1 Register
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1; // All analog circuitry inside the core is powered up
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ? Interrupt pulse generation occurs at the end of the conversion, 1 cycle prior to the ADC result latching into its result register

    AdccRegs.ADCCTL1.all = 0x00;            // ADC Control 1 Register
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1; // All analog circuitry inside the core is powered up
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ? Interrupt pulse generation occurs at the end of the conversion, 1 cycle prior to the ADC result latching into its result register

    AdcdRegs.ADCCTL1.all = 0x00;            // ADC Control 1 Register
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1; // All analog circuitry inside the core is powered up
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ? Interrupt pulse generation occurs at the end of the conversion, 1 cycle prior to the ADC result latching into its result register

    AdcaRegs.ADCCTL2.all = 0x00;            // ADC Control 2 Register
    AdcaRegs.ADCCTL2.bit.SIGNALMODE = 0;    // Single-ended
    AdcaRegs.ADCCTL2.bit.RESOLUTION = 0;    // 12-bit resolution
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;      // ADCCLK = Input Clock / 4.0

    AdcbRegs.ADCCTL2.all = 0x00;            // ADC Control 2 Register
    AdcbRegs.ADCCTL2.bit.SIGNALMODE = 0;    // Single-ended
    AdcbRegs.ADCCTL2.bit.RESOLUTION = 0;    // 12-bit resolution
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;      // ADCCLK = Input Clock / 4.0

    AdccRegs.ADCCTL2.all = 0x00;            // ADC Control 2 Register
    AdccRegs.ADCCTL2.bit.SIGNALMODE = 0;    // Single-ended
    AdccRegs.ADCCTL2.bit.RESOLUTION = 0;    // 12-bit resolution
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;      // ADCCLK = Input Clock / 4.0

    AdcdRegs.ADCCTL2.all = 0x00;            // ADC Control 2 Register
    AdcdRegs.ADCCTL2.bit.SIGNALMODE = 0;    // Single-ended
    AdcdRegs.ADCCTL2.bit.RESOLUTION = 0;    // 12-bit resolution
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6;      // ADCCLK = Input Clock / 4.0

    AdcaRegs.ADCBURSTCTL.all = 0x00;        // ADC Burst Control Register
    AdcaRegs.ADCBURSTCTL.bit.BURSTEN = 0;   // Burst mode is disabled

    AdcbRegs.ADCBURSTCTL.all = 0x00;        // ADC Burst Control Register
    AdcbRegs.ADCBURSTCTL.bit.BURSTEN = 0;   // Burst mode is disabled

    AdccRegs.ADCBURSTCTL.all = 0x00;        // ADC Burst Control Register
    AdccRegs.ADCBURSTCTL.bit.BURSTEN = 0;   // Burst mode is disabled

    AdcdRegs.ADCBURSTCTL.all = 0x00;        // ADC Burst Control Register
    AdcdRegs.ADCBURSTCTL.bit.BURSTEN = 0;   // Burst mode is disabled

    AdcaRegs.ADCINTSEL1N2.all = 0x00; // ADC Interrupt 1 and 2 Selection Register
    AdcaRegs.ADCINTSEL1N2.bit.INT2CONT = 0; // No further ADCINT1 pulses are generated until ADCINT1 flag (in ADCINTFLG register) is cleared by user
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 1;    // ADCINT1 is enabled
    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 0;  // ? EOC0 is trigger for ADCINT1
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // Clears respective flag bit in the ADCINTFLG register
    AdcaRegs.ADCINTSEL3N4.all = 0x00; // ADC Interrupt 3 and 4 Selection Register

    /*hakansrc*/
    AdccRegs.ADCINTSEL1N2.all = 0x00; // ADC Interrupt 1 and 2 Selection Register
    AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 0; // No further ADCINT2 pulses are generated until ADCINT2 flag (in ADCINTFLG register) is cleared by user
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;    // ADCINT2 is enabled
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 3;  // ? EOC3 is trigger for ADCINT1
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clears respective flag bit in the ADCINTFLG register
    AdccRegs.ADCINTSEL3N4.all = 0x00; // ADC Interrupt 3 and 4 Selection Register

    AdcaRegs.ADCSOCPRICTL.all = 0x00;       // ADC SOC Priority Control Register
    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0; // SOC priority is handled in round robin mode for all channels

    AdcaRegs.ADCINTSOCSEL1.all = 0x00; // ADC Interrupt SOC Selection 1 Register
    AdcbRegs.ADCINTSOCSEL1.all = 0x00; // ADC Interrupt SOC Selection 1 Register
    AdccRegs.ADCINTSOCSEL1.all = 0x00; // ADC Interrupt SOC Selection 1 Register
    AdcdRegs.ADCINTSOCSEL1.all = 0x00; // ADC Interrupt SOC Selection 1 Register
    /*
     AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 1; // ADCINT1 will trigger SOC0
     AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 1; // ADCINT1 will trigger SOC1
     AdcaRegs.ADCINTSOCSEL1.bit.SOC2 = 1; // ADCINT1 will trigger SOC2
     AdcaRegs.ADCINTSOCSEL1.bit.SOC3 = 1; // ADCINT1 will trigger SOC3
     AdcaRegs.ADCINTSOCSEL1.bit.SOC4 = 1; // ADCINT1 will trigger SOC4
     AdcaRegs.ADCINTSOCSEL1.bit.SOC5 = 1; // ADCINT1 will trigger SOC5
     AdcaRegs.ADCINTSOCSEL1.bit.SOC6 = 1; // ADCINT1 will trigger SOC6
     AdcaRegs.ADCINTSOCSEL1.bit.SOC7 = 1; // ADCINT1 will trigger SOC7
     */

    AdcaRegs.ADCINTSOCSEL2.all = 0x00; // ADC Interrupt SOC Selection 2 Register
    AdcbRegs.ADCINTSOCSEL2.all = 0x00; // ADC Interrupt SOC Selection 2 Register
    AdccRegs.ADCINTSOCSEL2.all = 0x00; // ADC Interrupt SOC Selection 2 Register
    AdcdRegs.ADCINTSOCSEL2.all = 0x00; // ADC Interrupt SOC Selection 2 Register
    /*
     AdcaRegs.ADCINTSOCSEL2.bit.SOC8 = 1; // ADCINT1 will trigger SOC8
     AdcaRegs.ADCINTSOCSEL2.bit.SOC9 = 1; // ADCINT1 will trigger SOC9
     AdcaRegs.ADCINTSOCSEL2.bit.SOC10 = 1; // ADCINT1 will trigger SOC10
     AdcaRegs.ADCINTSOCSEL2.bit.SOC11 = 1; // ADCINT1 will trigger SOC11
     AdcaRegs.ADCINTSOCSEL2.bit.SOC12 = 1; // ADCINT1 will trigger SOC12
     AdcaRegs.ADCINTSOCSEL2.bit.SOC13 = 1; // ADCINT1 will trigger SOC13
     AdcaRegs.ADCINTSOCSEL2.bit.SOC14 = 1; // ADCINT1 will trigger SOC14
     AdcaRegs.ADCINTSOCSEL2.bit.SOC15 = 1; // ADCINT1 will trigger SOC15
     */

    // AdcaRegs.ADCSOC0CTL.bit.TRIGSEL: SOC0 Trigger Source Select.
    // Along with the SOC0 field in the ADCINTSOCSEL1 register, this bit field configures which trigger will set the SOC0 flag
    // in the ADCSOCFLG1 register to initiate a conversion to start once priority is given to it
    // AdcaRegs.ADCSOC0CTL.bit.CHSEL: SOC0 Channel Select.
    // Selects the channel to be converted when SOC0 is received by the ADC
    // AdcaRegs.ADCSOC0CTL.bit.ACQPS: SOC0 Acquisition Prescale
    // Temp Sensor
    AnalogSubsysRegs.TSNSCTL.bit.ENABLE = 1; // Temperature Sensor Enable
    AdcaRegs.ADCSOC13CTL.all = 0x0000;    // ADC SOC13 Control Register
    AdcaRegs.ADCSOC13CTL.bit.TRIGSEL = 7; // (7) ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC13CTL.bit.CHSEL = 13;   // Single-ended ADCIN13
    AdcaRegs.ADCSOC13CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide

    // Vdc_M3_adc
    AdcaRegs.ADCSOC0CTL.all = 0x0000;    // ADC SOC0 Control Register
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;   // Single-ended ADCINA0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Vdc_M1_adc
    AdcaRegs.ADCSOC1CTL.all = 0x0000;    // ADC SOC1 Control Register
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;   // Single-ended ADCINA1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    //AdcaRegs.ADCSOC2CTL.all = 0x0000;    // ADC SOC2 Control Register
    //AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;   // Single-ended ADCINA2
    //AdcaRegs.ADCSOC2CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    // Is_M3_PhC_adc
    AdcaRegs.ADCSOC3CTL.all = 0x0000;    // ADC SOC3 Control Register
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;   // Single-ended ADCINA3
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M1_PhA_adc
    AdcaRegs.ADCSOC4CTL.all = 0x0000;    // ADC SOC4 Control Register
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;   // Single-ended ADCINA4
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M2_PhB_adc
    AdcaRegs.ADCSOC5CTL.all = 0x0000;    // ADC SOC5 Control Register
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;   // Single-ended ADCINA5
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M3_PhB_adc
    AdcaRegs.ADCSOC14CTL.all = 0x0000;    // ADC SOC14 Control Register
    AdcaRegs.ADCSOC14CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC14CTL.bit.CHSEL = 14;   // Single-ended ADCIN14
    AdcaRegs.ADCSOC14CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M3_PhA_adc
    AdcaRegs.ADCSOC15CTL.all = 0x0000;    // ADC SOC15 Control Register
    AdcaRegs.ADCSOC15CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcaRegs.ADCSOC15CTL.bit.CHSEL = 15;   // Single-ended ADCIN15
    AdcaRegs.ADCSOC15CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M4_PhB_adc
    AdcbRegs.ADCSOC0CTL.all = 0x0000;    // ADC SOC0 Control Register
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;   // Single-ended ADCINB0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M4_PhC_adc
    AdcbRegs.ADCSOC1CTL.all = 0x0000;    // ADC SOC1 Control Register
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;   // Single-ended ADCINB1
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M2_PhC_adc
    AdcbRegs.ADCSOC2CTL.all = 0x0000;    // ADC SOC2 Control Register
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;   // Single-ended ADCINB2
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M4_PhA_adc
    AdcbRegs.ADCSOC3CTL.all = 0x0000;    // ADC SOC3 Control Register
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;   // Single-ended ADCIB3
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    //AdcbRegs.ADCSOC4CTL.all = 0x0000;    // ADC SOC4 Control Register
    //AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcbRegs.ADCSOC4CTL.bit.CHSEL = 4;   // Single-ended ADCINB4
    //AdcbRegs.ADCSOC4CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    //AdcbRegs.ADCSOC5CTL.all = 0x0000;    // ADC SOC5 Control Register
    //AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcbRegs.ADCSOC5CTL.bit.CHSEL = 5;   // Single-ended ADCINB5
    //AdcbRegs.ADCSOC5CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    // Is_M2_PhA_adc
    AdccRegs.ADCSOC2CTL.all = 0x0000;    // ADC SOC2 Control Register
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;   // Single-ended ADCINC2
    AdccRegs.ADCSOC2CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M1_PhC_adc
    AdccRegs.ADCSOC3CTL.all = 0x0000;    // ADC SOC3 Control Register
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 2; // ADCTRIG5 - ePWM2, ADCSOCA
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;   // Single-ended ADCINC3
    AdccRegs.ADCSOC3CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Is_M1_PhB_adc
    AdccRegs.ADCSOC4CTL.all = 0x0000;    // ADC SOC4 Control Register
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;   // Single-ended ADCINC4
    AdccRegs.ADCSOC4CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    //AdccRegs.ADCSOC5CTL.all = 0x0000;    // ADC SOC5 Control Register
    //AdccRegs.ADCSOC5CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdccRegs.ADCSOC5CTL.bit.CHSEL = 5;   // Single-ended ADCINC5
    //AdccRegs.ADCSOC5CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    // Vdc_M4_adc
    AdcdRegs.ADCSOC0CTL.all = 0x0000;    // ADC SOC0 Control Register
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;   // Single-ended ADCIND0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    // Vdc_M2_adc
    AdcdRegs.ADCSOC1CTL.all = 0x0000;    // ADC SOC1 Control Register
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;   // Single-ended ADCIND1
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = 30; // Sample window is 1 system clock cycle wide
    //AdcdRegs.ADCSOC2CTL.all = 0x0000;    // ADC SOC2 Control Register
    //AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;   // Single-ended ADCIND2
    //AdcdRegs.ADCSOC2CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    //AdcdRegs.ADCSOC3CTL.all = 0x0000;    // ADC SOC3 Control Register
    //AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;   // Single-ended ADCIND3
    //AdcdRegs.ADCSOC3CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    //AdcdRegs.ADCSOC4CTL.all = 0x0000;    // ADC SOC4 Control Register
    //AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;   // Single-ended ADCIND4
    //AdcdRegs.ADCSOC4CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide
    //AdcdRegs.ADCSOC5CTL.all = 0x0000;    // ADC SOC5 Control Register
    //AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = 7; // ADCTRIG5 - ePWM2, ADCSOCA
    //AdcdRegs.ADCSOC5CTL.bit.CHSEL = 5;   // Single-ended ADCIND5
    //AdcdRegs.ADCSOC5CTL.bit.ACQPS = 30;   // Sample window is 1 system clock cycle wide

    //DELAY_US(1000);

    EDIS;

}

void Setup_EQEP(void)
{
    // ARC-H-50-3600-TTL-6-3M-10-FC
    // ARC: Optik, H: Hollow, 50: 50mm, 3600: Resolution, TTL: 5VDC supply, 6: A, An, B, Bn, Z, Zn
    // 1: A  - Yellow
    // 2: Bn - White
    // 3: +V - Red
    // 4: 0V - Black
    // 5: An - Blue
    // 6: B  - Green
    // 7: Zn - Grey
    // 8: Z  - Pink
    // 9: GND - Shield

    // QEPI: Gated to A and B (zero marker)
    // A leads B, forward direction (quadrature clock mode)

    /*the formula will be X/(t(k)-t(k-1)) at low  speeds, can be used with UPEVNT */
    /*the formula will be (x(k)-x(k-1))/T at high speeds, can be used with eqep unit timer or CAPCLK */

    // added by hakansrc
    EQep1Regs.QUPRD = 40000;        // Unit Timer for 100Hz at 200 MHz SYSCLKOUT

    // Quadrature Decoder Unit (QDU) Registers
    EQep1Regs.QDECCTL.all = 0x00;     // Quadrature Decoder Control
    EQep1Regs.QDECCTL.bit.QSRC = 0; // Position-counter source selection: Quadrature count mode (QCLK = iCLK, QDIR = iDIR)
    // hakansrc QSRC=2 girmis -- "0"
    EQep1Regs.QDECCTL.bit.SOEN = 0;   // Disable position-compare sync output
    EQep1Regs.QDECCTL.bit.SPSEL = 1; // Strobe pin is used for sync output: Don't care
    EQep1Regs.QDECCTL.bit.XCR = 0; // External Clock Rate: 2x resolution: Count the rising/falling edge
    EQep1Regs.QDECCTL.bit.SWAP = 0; // CLK/DIR Signal Source for Position Counter: Quadrature-clock inputs are not swapped
    EQep1Regs.QDECCTL.bit.IGATE = 0;  // Disable gating of Index pulse
    EQep1Regs.QDECCTL.bit.QAP = 0;    // QEPA input polarity: No effect
    EQep1Regs.QDECCTL.bit.QBP = 0;    // QEPB input polarity: No effect
    EQep1Regs.QDECCTL.bit.QIP = 0;    // QEPI input polarity: No effect
    EQep1Regs.QDECCTL.bit.QSP = 0;    // QEPS input polarity: No effect

    // Position Counter and Control Unit (PCCU) Registers
    EQep1Regs.QEPCTL.all = 0x00;      // QEP Control
    EQep1Regs.QEPCTL.bit.FREE_SOFT = 0; // Emulation mode: Position counter stops immediately on emulation suspend
    // hakansrc FREE_SOFT=2 devam etsin demis
    EQep1Regs.QEPCTL.bit.PCRM = 0;   // Position counter reset on an index event
    EQep1Regs.QEPCTL.bit.IEI = 2; // Initializes the position counter on the rising edge of the QEPI signal
    // hakansrc IEI=0 nedeni bilinmiyor
    EQep1Regs.QEPCTL.bit.IEL = 1; // Latches position counter on rising edge of the index signal
    // hakansrc IEL=0 nedeni bilinmiyor
    EQep1Regs.QEPCTL.bit.QPEN = 0; // Reset the eQEP peripheral internal operating flags/read-only registers.
    EQep1Regs.QEPCTL.bit.QCLM = 0; // QEP capture latch mode: Latch on position counter read by CPU
    // hakansrc QCLM=1 latch on unit timeout
    EQep1Regs.QEPCTL.bit.UTE = 1;    // QEP unit timer enable: Enable unit timer
    EQep1Regs.QEPCTL.bit.WDE = 1;     // Enable the eQEP watchdog timer

    EQep1Regs.QPOSINIT = 0; // Initial QPOSCNT , QPOSCNT set to zero on index event (or strobe or software if desired)
    EQep1Regs.QPOSMAX = 14399;       // Max value of QPOSCNT

    // Quadrature edge-capture unit for low-speed measurement (QCAP)
    EQep1Regs.QCAPCTL.all = 0x00;
    EQep1Regs.QCAPCTL.bit.CEN = 1;    // eQEP capture unit is enabled
    EQep1Regs.QCAPCTL.bit.CCPS = 0; // eQEP capture timer clock prescaler: // CAPCLK = SYSCLKOUT/1
    EQep1Regs.QCAPCTL.bit.UPPS = 0; // Unit position event prescaler: UPEVNT = QCLK/1 , QCLK is triggered in every rising or falling edge of A or B
    // UPPS reiz veri important

    // Position Compare Control
    EQep1Regs.QPOSCTL.all = 0x0000;	  // Position Compare Control: Disabled
    // hakansrc enable etmis: PCE=1, PCSPW=0xFFF
    /*
     EQep1Regs.QPOSCTL.bit.PCSHDW = 0;   // shadow disabled
     EQep1Regs.QPOSCTL.bit.PCLOAD = 0;   // does not matter, shadow already disabled
     EQep1Regs.QPOSCTL.bit.PCPOL = 0;    // polarity of sync output is set to high pulse output
     EQep1Regs.QPOSCTL.bit.PCE = 1;      // position compare enable
     EQep1Regs.QPOSCTL.bit.PCSPW = 0xFFF;// Select-position-compare sync output pulse width, 4096 * 4 * SYSCLKOUT cycles
     */

    // QEP Interrupt Control (EQEPxINT)
    // Eleven interrupt events (PCE, PHE, QDC, WTO, PCU, PCO, PCR, PCM, SEL, IEL and UTO) can be generated.
    EQep1Regs.QEINT.all = 0x00;
    EQep1Regs.QEINT.bit.WTO = 1;      // Watchdog time out interrupt enabled
    // added by hakansrc
    EQep1Regs.QEINT.bit.UTO = 1; // unit timeout interrupt enabled, check QCAPCTL.bit.CCPS (NOTE: can be used for speed calculations at high speeds,check technical reference manual page 1996)
    // UTO currently not used
    EQep1Regs.QEINT.bit.IEL = 1;        // Index event latch interrupt enabled
    // IEL currently not used

    // added by hakansrc
    EQep1Regs.QFLG.all = 0;             // Interrupts are flagged here
    EQep1Regs.QCLR.all = 0;             // QEP Interrupt Clear

    // added by hakansrc
    EQep1Regs.QCTMR = 0; // This register provides time base for edge capture unit. 16 bit
    EQep1Regs.QCPRD = 0; // This register holds the period count value between the last successive eQEP position events
    EQep1Regs.QCTMRLAT = 0; // QCTMR latch register, latching can be stopped by clearing QEPCTL[QCLM] register
    EQep1Regs.QCPRDLAT = 0; // QCPRD latch register, latching can be stopped by clearing QEPCTL[QCLM] register

    /*
     // Registers to be watched
     EQep1Regs.QPOSCNT   // Position counter: This counter acts as a position integrator whose count value is proportional to position from a give reference point
     EQep1Regs.QPOSINIT  // Position counter init: contains the position value that is used to initialize the position counter based on external strobe or index event
     EQep1Regs.QPOSMAX   // Maximum Position Count: contains the maximum position counter value.
     EQep1Regs.QPOSCMP   // Position Compare: is compared with the position counter (QPOSCNT) to generate sync output and/or interrupt on compare match
     EQep1Regs.QPOSILAT  // Index Position Latch: The position-counter value is latched into this register on an index event as defined by the QEPCTL[IEL] bits.
     EQep1Regs.QPOSLAT   // Position Latch: The position-counter value is latched into this register on a unit time out event.
     EQep1Regs.QUTMR	    // QEP Unit Timer: This register acts as time base for unit time event generation. When this timer value matches the unit time period value a unit time event is generated.
     EQep1Regs.QUPRD     // QEP unit period: contains the period count for the unit timer to generate periodic unit time events.
     EQep1Regs.QWDTMR    // Watchdog timer: time base for the watchdog to detect motor stalls
     EQep1Regs.QWDPRD	// Watchdog period: contains the time-out count for the eQEP peripheral	watch dog timer
     EQep1Regs.QCTMR	 	// QEP Capture Timer: This register provides time base for edge capture unit
     EQep1Regs.QCPRD	    // QEP Capture Period: This register holds the period count value between the last successive eQEP position events

     EQep1Regs.QFLG.bit.INT   // Global interrupt status flag
     EQep1Regs.QCLR.bit.INT   // Global interrupt clear flag
     EQep1Regs.QFLG.bit.WTO	 // Watchdog timeout interrupt flag
     EQep1Regs.QCLR.bit.WTO = 1;	 // Clear watchdog timeout interrupt flag

     EQep1Regs.QEPSTS.bit.UPEVNT	 // 1h (R/W) = Unit position event detected. Write 1 to clear
     EQep1Regs.QEPSTS.bit.QDF 	 // Quadrature direction flag: 1=Clockwise
     */

    EQep1Regs.QEPCTL.bit.QPEN = 1;    // eQEP position counter is enabled

}

void InitEpwm1(void)
{
    EPwm1Regs.TBCTL.all = 0x00;
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm1Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
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

    EPwm1Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm1Regs.CMPB.half.CMPB = EPwm1Regs.TBPRD/2;    // Set Compare B value

    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm1Regs.TBCTL2.all = 0x00;
    EPwm1Regs.CMPCTL2.all = 0x00;
    EPwm1Regs.DBCTL.all = 0x00;
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBCTL.bit.POLSEL = 2;
    EPwm1Regs.DBFED = dead_time / sysclk_period;
    EPwm1Regs.DBRED = dead_time / sysclk_period;
    EPwm1Regs.DBCTL2.all = 0x00;

    EPwm1Regs.ETSEL.all = 0x00;
    EPwm1Regs.ETSEL.bit.INTSEL = 1; // When TBCTR == 0
    EPwm1Regs.ETSEL.bit.INTEN = 1;  // Enable INT
    EPwm1Regs.ETPS.all = 0x00;
    EPwm1Regs.ETPS.bit.INTPRD = 1;  // Generate INT on first event

}

void InitEpwm2(void)
{

    EPwm2Regs.TBCTL.all = 0x00;
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm2Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm2Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
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

    EPwm2Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm2Regs.CMPB.half.CMPB = EPwm2Regs.TBPRD/2;    // Set Compare B value

    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm2Regs.TBCTL2.all = 0x00;
    EPwm2Regs.CMPCTL2.all = 0x00;
    EPwm2Regs.DBCTL.all = 0x00;
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBFED = dead_time / sysclk_period;
    EPwm2Regs.DBRED = dead_time / sysclk_period;
    EPwm2Regs.DBCTL2.all = 0x00;

    EPwm2Regs.ETSEL.all = 0x00;
    EPwm2Regs.ETSEL.bit.SOCAEN = 1;  // Enable SOCA generation
    EPwm2Regs.ETSEL.bit.SOCASEL = 2;  // Generate SOCA when CTR = PRD
    //EPwm2Regs.ETSEL.bit.INTEN = 1;
    //EPwm2Regs.ETSEL.bit.INTSEL = 1;
    EPwm2Regs.ETPS.bit.SOCAPRD = 1;   // Interrupt on the first event
    //EPwm2Regs.ETPS.bit.INTPRD = 1;

}

void InitEpwm3(void)
{

    EPwm3Regs.TBCTL.all = 0x00;
    EPwm3Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm3Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm3Regs.TBCTR = 0x0000;          // Clear counter

    EPwm3Regs.CMPCTL.all = 0x00;
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm3Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm3Regs.AQCTLA.all = 0x00;
    EPwm3Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm3Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm3Regs.AQCTLB.all = 0x00;
    //EPwm3Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm3Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm3Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm3Regs.CMPB.half.CMPB = EPwm3Regs.TBPRD/2;    // Set Compare B value

    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm3Regs.TBCTL2.all = 0x00;
    EPwm3Regs.CMPCTL2.all = 0x00;
    EPwm3Regs.DBCTL.all = 0x00;
    EPwm3Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm3Regs.DBCTL.bit.POLSEL = 2;
    EPwm3Regs.DBFED = dead_time / sysclk_period;
    EPwm3Regs.DBRED = dead_time / sysclk_period;
    EPwm3Regs.DBCTL2.all = 0x00;

    EPwm3Regs.ETSEL.all = 0x00;

}

void InitEpwm4(void)
{

    EPwm4Regs.TBCTL.all = 0x00;
    EPwm4Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm4Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm4Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm4Regs.TBCTR = 0x0000;          // Clear counter

    EPwm4Regs.CMPCTL.all = 0x00;
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm4Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm4Regs.AQCTLA.all = 0x00;
    EPwm4Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm4Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm4Regs.AQCTLB.all = 0x00;
    //EPwm4Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm4Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm4Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm4Regs.CMPB.half.CMPB = EPwm4Regs.TBPRD/2;    // Set Compare B value

    EPwm4Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm4Regs.TBCTL2.all = 0x00;
    EPwm4Regs.CMPCTL2.all = 0x00;
    EPwm4Regs.DBCTL.all = 0x00;
    EPwm4Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm4Regs.DBCTL.bit.POLSEL = 2;
    EPwm4Regs.DBFED = dead_time / sysclk_period;
    EPwm4Regs.DBRED = dead_time / sysclk_period;
    EPwm4Regs.DBCTL2.all = 0x00;

    EPwm4Regs.ETSEL.all = 0x00;

}

void InitEpwm5(void)
{

    EPwm5Regs.TBCTL.all = 0x00;
    EPwm5Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm5Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm5Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm5Regs.TBCTR = 0x0000;          // Clear counter

    EPwm5Regs.CMPCTL.all = 0x00;
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm5Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm5Regs.AQCTLA.all = 0x00;
    EPwm5Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm5Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm5Regs.AQCTLB.all = 0x00;
    //EPwm5Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm5Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm5Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm5Regs.CMPB.half.CMPB = EPwm5Regs.TBPRD/2;    // Set Compare B value

    EPwm5Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm5Regs.TBCTL2.all = 0x00;
    EPwm5Regs.CMPCTL2.all = 0x00;
    EPwm5Regs.DBCTL.all = 0x00;
    EPwm5Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm5Regs.DBCTL.bit.POLSEL = 2;
    EPwm5Regs.DBFED = dead_time / sysclk_period;
    EPwm5Regs.DBRED = dead_time / sysclk_period;
    EPwm5Regs.DBCTL2.all = 0x00;

    EPwm5Regs.ETSEL.all = 0x00;

}

void InitEpwm6(void)
{

    EPwm6Regs.TBCTL.all = 0x00;
    EPwm6Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm6Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm6Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm6Regs.TBCTR = 0x0000;          // Clear counter

    EPwm6Regs.CMPCTL.all = 0x00;
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm6Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm6Regs.AQCTLA.all = 0x00;
    EPwm6Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm6Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm6Regs.AQCTLB.all = 0x00;
    //EPwm6Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm6Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm6Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm6Regs.CMPB.half.CMPB = EPwm6Regs.TBPRD/2;    // Set Compare B value

    EPwm6Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm6Regs.TBCTL2.all = 0x00;
    EPwm6Regs.CMPCTL2.all = 0x00;
    EPwm6Regs.DBCTL.all = 0x00;
    EPwm6Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm6Regs.DBCTL.bit.POLSEL = 2;
    EPwm6Regs.DBFED = dead_time / sysclk_period;
    EPwm6Regs.DBRED = dead_time / sysclk_period;
    EPwm6Regs.DBCTL2.all = 0x00;

    EPwm6Regs.ETSEL.all = 0x00;

}

void InitEpwm7(void)
{

    EPwm7Regs.TBCTL.all = 0x00;
    EPwm7Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm7Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm7Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm7Regs.TBCTR = 0x0000;          // Clear counter

    EPwm7Regs.CMPCTL.all = 0x00;
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm7Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm7Regs.AQCTLA.all = 0x00;
    EPwm7Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm7Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm7Regs.AQCTLB.all = 0x00;
    //EPwm7Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm7Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm7Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm7Regs.CMPB.half.CMPB = EPwm7Regs.TBPRD/2;    // Set Compare B value

    EPwm7Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm7Regs.TBCTL2.all = 0x00;
    EPwm7Regs.CMPCTL2.all = 0x00;
    EPwm7Regs.DBCTL.all = 0x00;
    EPwm7Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm7Regs.DBCTL.bit.POLSEL = 2;
    EPwm7Regs.DBFED = dead_time / sysclk_period;
    EPwm7Regs.DBRED = dead_time / sysclk_period;
    EPwm7Regs.DBCTL2.all = 0x00;

    EPwm7Regs.ETSEL.all = 0x00;

}

void InitEpwm8(void)
{

    EPwm8Regs.TBCTL.all = 0x00;
    EPwm8Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm8Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm8Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm8Regs.TBCTR = 0x0000;          // Clear counter

    EPwm8Regs.CMPCTL.all = 0x00;
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm8Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm8Regs.AQCTLA.all = 0x00;
    EPwm8Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm8Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm8Regs.AQCTLB.all = 0x00;
    //EPwm8Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm8Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm8Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm8Regs.CMPB.half.CMPB = EPwm8Regs.TBPRD/2;    // Set Compare B value

    EPwm8Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm8Regs.TBCTL2.all = 0x00;
    EPwm8Regs.CMPCTL2.all = 0x00;
    EPwm8Regs.DBCTL.all = 0x00;
    EPwm8Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm8Regs.DBCTL.bit.POLSEL = 2;
    EPwm8Regs.DBFED = dead_time / sysclk_period;
    EPwm8Regs.DBRED = dead_time / sysclk_period;
    EPwm8Regs.DBCTL2.all = 0x00;

    EPwm8Regs.ETSEL.all = 0x00;

}

void InitEpwm9(void)
{

    EPwm9Regs.TBCTL.all = 0x00;
    EPwm9Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm9Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm9Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm9Regs.TBCTR = 0x0000;          // Clear counter

    EPwm9Regs.CMPCTL.all = 0x00;
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = 1;         //only active registers are used
    //EPwm9Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm9Regs.AQCTLA.all = 0x00;
    EPwm9Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm9Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm9Regs.AQCTLB.all = 0x00;
    //EPwm9Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm9Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm9Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm9Regs.CMPB.half.CMPB = EPwm9Regs.TBPRD/2;    // Set Compare B value

    EPwm9Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm9Regs.TBCTL2.all = 0x00;
    EPwm9Regs.CMPCTL2.all = 0x00;
    EPwm9Regs.DBCTL.all = 0x00;
    EPwm9Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm9Regs.DBCTL.bit.POLSEL = 2;
    EPwm9Regs.DBFED = dead_time / sysclk_period;
    EPwm9Regs.DBRED = dead_time / sysclk_period;
    EPwm9Regs.DBCTL2.all = 0x00;

    EPwm9Regs.ETSEL.all = 0x00;

}

void InitEpwm10(void)
{

    EPwm10Regs.TBCTL.all = 0x00;
    EPwm10Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm10Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm10Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm10Regs.TBCTR = 0x0000;          // Clear counter

    EPwm10Regs.CMPCTL.all = 0x00;
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = 1;        //only active registers are used
    //EPwm10Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm10Regs.AQCTLA.all = 0x00;
    EPwm10Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm10Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm10Regs.AQCTLB.all = 0x00;
    //EPwm10Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm10Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm10Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm10Regs.CMPB.half.CMPB = EPwm10Regs.TBPRD/2;    // Set Compare B value

    EPwm10Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm10Regs.TBCTL2.all = 0x00;
    EPwm10Regs.CMPCTL2.all = 0x00;
    EPwm10Regs.DBCTL.all = 0x00;
    EPwm10Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm10Regs.DBCTL.bit.POLSEL = 2;
    EPwm10Regs.DBFED = dead_time / sysclk_period;
    EPwm10Regs.DBRED = dead_time / sysclk_period;
    EPwm10Regs.DBCTL2.all = 0x00;

    EPwm10Regs.ETSEL.all = 0x00;

}

void InitEpwm11(void)
{

    EPwm11Regs.TBCTL.all = 0x00;
    EPwm11Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm11Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm11Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm11Regs.TBCTR = 0x0000;          // Clear counter

    EPwm11Regs.CMPCTL.all = 0x00;
    EPwm11Regs.CMPCTL.bit.SHDWAMODE = 1;        //only active registers are used
    //EPwm11Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm11Regs.AQCTLA.all = 0x00;
    EPwm11Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm11Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm11Regs.AQCTLB.all = 0x00;
    //EPwm11Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm11Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm11Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm11Regs.CMPB.half.CMPB = EPwm11Regs.TBPRD/2;    // Set Compare B value

    EPwm11Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm11Regs.TBCTL2.all = 0x00;
    EPwm11Regs.CMPCTL2.all = 0x00;
    EPwm11Regs.DBCTL.all = 0x00;
    EPwm11Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm11Regs.DBCTL.bit.POLSEL = 2;
    EPwm11Regs.DBFED = dead_time / sysclk_period;
    EPwm11Regs.DBRED = dead_time / sysclk_period;
    EPwm11Regs.DBCTL2.all = 0x00;

    EPwm11Regs.ETSEL.all = 0x00;

}

void InitEpwm12(void)
{

    EPwm12Regs.TBCTL.all = 0x00;
    EPwm12Regs.TBCTL.bit.CTRMODE = 2;   // Count up and douwn
    EPwm12Regs.TBCTL.bit.CLKDIV = 0;    // TBCLOK = EPWMCLOCK/(128*10) = 78125Hz
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm12Regs.TBPRD = sysclk_frequency / (switching_frequency * 2);
    EPwm12Regs.TBCTR = 0x0000;          // Clear counter

    EPwm12Regs.CMPCTL.all = 0x00;
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = 1;        //only active registers are used
    //EPwm12Regs.CMPCTL.bit.SHDWBMODE = 1;//only active registers are used

    EPwm12Regs.AQCTLA.all = 0x00;
    EPwm12Regs.AQCTLA.bit.CAU = 2; //set high
    EPwm12Regs.AQCTLA.bit.CAD = 1; //set low
    //EPwm12Regs.AQCTLB.all = 0x00;
    //EPwm12Regs.AQCTLB.bit.CBU = 1; //set low
    //EPwm12Regs.AQCTLB.bit.CBD = 2; //set high

    EPwm12Regs.CMPA.half.CMPA = EPwm12Regs.TBPRD / 2;    // Set compare A value
    //EPwm12Regs.CMPB.half.CMPB = EPwm12Regs.TBPRD/2;    // Set Compare B value

    EPwm12Regs.TBPHS.half.TBPHS = 0x0000;          // Phase is 0

    EPwm12Regs.TBCTL2.all = 0x00;
    EPwm12Regs.CMPCTL2.all = 0x00;
    EPwm12Regs.DBCTL.all = 0x00;
    EPwm12Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm12Regs.DBCTL.bit.POLSEL = 2;
    EPwm12Regs.DBFED = dead_time / sysclk_period;
    EPwm12Regs.DBRED = dead_time / sysclk_period;
    EPwm12Regs.DBCTL2.all = 0x00;

    EPwm12Regs.ETSEL.all = 0x00;

}

#if VBYFCONTROLENABLE
void VBYFSetup(void)
{
    /*WARNING: call this function before initializing interrupts*/
    /*fMotorFundamentalFrequency&fAmplitudeModulationIndex are updated inside the GPIO interrupt service routines(isr_Increment_freq,isr_Decrement_freq)
	and those values are applied to the motor when spwm_counter is set to zero (i.e. when the fundamental sinusoidal at 0 degree)*/
    fFundamentalFrequencyToBeApplied = fFrequencyInitial;
    VbyF_ma_ToBeApplied = fFundamentalFrequencyToBeApplied * (fVrated / fFrequencySaturation) * (sqrt(3) / (fVdc * 0.612));
    fMotorFundamentalFrequency = fFundamentalFrequencyToBeApplied;
    fAmplitudeModulationIndex = VbyF_ma_ToBeApplied;
    EALLOW;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0; // GPIO32 = input /*BUTTON for frequency increment*/
    PieVectTable.XINT1_INT = &isr_Increment_freq;
    XintRegs.XINT1CR.bit.ENABLE = 1;   /*enable XINT interrupt*/
    XintRegs.XINT1CR.bit.POLARITY = 1; /*rising edge trigger*/
    GPIO_SetupXINT1Gpio(32);           /*set GPIO32 as the source of the trigger*/
    EDIS;
    /*hakansrc: WARNING -- donot merge these EDIS and EALLOW*/
    EALLOW;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 0; // GPIO19 = input /*BUTTON for frequency decrement*/
    PieVectTable.XINT2_INT = &isr_Decrement_freq;
    XintRegs.XINT2CR.bit.ENABLE = 1;   /*enable XINT interrupt*/
    XintRegs.XINT2CR.bit.POLARITY = 1; /*rising edge trigger*/
    GPIO_SetupXINT2Gpio(19);           /*set GPIO19 as the source of the trigger*/
    EDIS;

    /**/
}
__interrupt void isr_Increment_freq(void) /*ISR for increment*/
{
    //Xint1Count++;
    /*
    if((fFundamentalFrequencyToBeApplied+fVbyfFreqSteps)>fFrequencySaturation)
    {
        fFundamentalFrequencyToBeApplied = fFrequencySaturation;
    }
    else
    {
        fFundamentalFrequencyToBeApplied = fFundamentalFrequencyToBeApplied + fVbyfFreqSteps;
    }
    VbyF_ma_ToBeApplied = fFundamentalFrequencyToBeApplied*(fVrated/fFrequencySaturation)*(sqrt(3)/(fVdc*0.612));
     */
    uiArtirFlag = 1;
    uiAzaltFlag = 0;
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
__interrupt void isr_Decrement_freq(void) /*ISR for decrement*/
{
    //Xint2Count++;
    /*
    if((fFundamentalFrequencyToBeApplied-fVbyfFreqSteps)<fFrequencyInitial)
    {
        fFundamentalFrequencyToBeApplied = fFrequencyInitial;
    }
    else
    {
        fFundamentalFrequencyToBeApplied = fFundamentalFrequencyToBeApplied - fVbyfFreqSteps;

    }
    VbyF_ma_ToBeApplied = fFundamentalFrequencyToBeApplied*(fVrated/fFrequencySaturation)*(sqrt(3)/(fVdc*0.612));
*/
    uiArtirFlag = 0;
    uiAzaltFlag = 1;
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
#endif
