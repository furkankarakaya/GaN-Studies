### List of Parameters for Characterization

The following parameters are critical for GaN characterization. </br>

The effect of highlighted parameters will be characterized. Remaining parameters are out of scope due to the technical incapability.

##### Gate-Source Driver Parameters
* Gate Driver Topology (Bootstrap or Isolated)
* <mark>Gate Driver Voltage Level (Vpulse)</mark>
* <mark>Turn-on Resistance (Ron)</mark>
* <mark>Turn-off Resistance (Roff)</mark>
* <mark>Gate Driver Inductance (Lg-ex)</mark>
* <mark>Gate Signal Rise and Fall times (tr,tf)</mark>

##### GaN Parameters
* Drain-Source Capacitance(Cds)
* Gate-Drain Capacitance(Cgd)
* Gate-Source Capacitance(Cgs)
* Drain Inductance (Ld-in)
* Source Inductance (Ls-in)
* Kelvin Source Inductance (Lss-in)
* Gate Inductance (Lg-in)
* Drain Resistance (Rd-in)
* Source Resistance (Rs-in)
* Gate Resistance (Rg-in)
* Channel Conductivity
* <mark>Junction Temperature (Tj)</mark>
* <mark>Threshold Voltage</mark>

##### Circuit Parameters
* <mark>Voltage Level (Vdc)</mark>
* <mark>Load Current Level (I-load)</mark>
* <mark>Loop Inductance (Lp)</mark>

##### Topological Parameters
* <mark>Parallel connections</mark>
* <mark>Three Phase Inverter</mark>
* <mark>Variable Parameter Half Bridge</mark>

#### Discussions and Comments
* Vpulse can be varied for both high (6V nominal) and low (-3V nominal) levels. It can be used to obtain the static characterization of the device and finding the optimum negative bias for the minimization of dead time loss and false turn-on effect.
* <mark>Ron-Roff can be varied easily for characterization.</mark>
* There are two possibilities for Lg-ex:
  1. <mark>Accurate modeling using analytical and/or FEA methods (Comsol etc.) for the designed PCB.
  2. <mark>Variable inductance using multi-layer PCBs and interchangable connections for characterization.
* <mark>There are several elements in the gate drive path which may introduce delay to the digital pwm signal. A complete analysis may be done. (Cuma Tartışılabilir - Schmidt Trigger)
* Cgs dependent (variable) model is not usually provided. An analytical study can be applied for its characterization. <mark>Should be discussed.</mark>
* <mark>A mathematical modeling approach (state space etc.) is going on.</mark>
* <mark>A closed loop model including thermal model (GaN + all other elements), loss model and GaN model on Simulink will be studied. It can be applied to the IMMD later.</mark>
* <mark>A research on measuring the temperatures of GaN (including its junction) will be conducted.</mark>
* <mark>DC busbar designs on PCB with variable inductance can be considered.</mark>
* <mark>Parallel operation will be discussed later.</mark>
* <mark>Care should be taken for measurements (temperature, current etc.).</mark>
* <mark>Filtering (common mode etc.) on the gate driver isolated supply can be considered on the PCB.</mark>

#### Some notes the behaviour of GaN and potential research topics
* <mark>Active turn-on, passive turn-on etc. can be investigated experimentally. A mode-by-mode analysis on the oscilloscope waveforms can be applied.</mark>
* <mark>State trajectories on experimental waveforms can be studied.</mark>
* <mark>Care should be given to the gate driver delay skew when designing the PCB.</mark>
* Optimum negative bias voltage can be studied concerning the false turn-on phenomena (device has very low threshold).
* The negative absolute minimum of the GaN device gate should also be considered for spikes caused by miller effect.
* Some precautions can be taken on the gate (diode + TVS etc.) on the PCB.
* A comparative analysis with switching loss and dv/dt (and di/dt) rates can be studied with variable resistances (and/or other parameters ???). <mark> Should be discussed in detail</mark>
* <mark>In addition to the trade-off between loss and spikes, study on critical frequencies (to avoid), sustained oscillation possibilities etc. can be conducted and verified experimentally (apart from the almost linear relation between these parameters). This will be easier when a mathematical model is developed.
* <mark>For the aforementioned critical frequencies, some damping measures can be considered. Effect of ferrite bead can be investigated. Although it may introduce additional inductance to the gate-source path, its relatively high impedance on high frequencies (which are around possible gate oscillation frequencies) will help damping the oscillations.  This should be included on both Simulink model and state space model.
* RC snubber across gate-source ??? Not very common.
