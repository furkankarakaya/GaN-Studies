### List of Parameters for Characterization

The following parameters are critical for GaN characterization. </br>

##### Gate-Source Driver Parameters
* Gate Driver Topology (Bootstrap or Isolated)
* Gate Driver Voltage Level (Vpulse)
* Turn-on Resistance (Ron)
* Turn-off Resistance (Roff)
* Gate Driver Inductance (Lg-ex)
* Gate Signal Rise and Fall times (tr,tf)

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
* Channel conductivity
* Junction Temperature (Tj)

##### Circuit Parameters
* Voltage Level (Vdc)
* Load Current Level (I-load)
* Loop Inductance (Lp)

##### Topological Parameters
* Parallel connections

#### Discussions and Comments
* Vpulse can be varied for both high (6V nominal) and low (-3V nominal) levels. It can be used to obtain the static characterization of the device and finding the optimum negative bias for the minimization of dead time loss and false turn-on effect.
* Ron-Roff can be varied easily for characterization.
* There are two possibilities for Lg-ex:
  1. Accurate modeling using analytical and/or FEA methods (Comsol etc.) for the designed PCB.
  2. Variable inductance using multi-layer PCBs and interchangable connections for characterization.
* There are several elements in the gate drive path which may introduce delay to the digital pwm signal. A complete analysis may be done.
* Cgs dependent (variable) model is not usually provided. An analytical study can be applied for its characterization.
* A mathematical modeling approach (state space etc.) will be studied ASAP.
* A closed loop model including thermal model (GaN + all other elements), loss model and GaN model on Simulink will be studied. It can be applied to the IMMD later.
* A research on measuring the temperatures of GaN (including its junction) will be conducted.
* DC busbar designs on PCB with variable inductance can be considered.
* Parallel operation will be discussed later.
* Care should be taken for measurements (temperature, current etc.).
* Filtering (common mode etc.) on the gate driver isolated supply can be considered on the PCB.

#### Some notes the behaviour of GaN and potential research topics
* Active turn-on, passive turn-on etc. can be investigated experimentally. A mode-by-mode analysis on the oscilloscope waveforms can be applied.
* State trajectories on experimental waveforms can be studied.
* Care should be given to the gate driver delay skew when designing the PCB.
* Optimum negative bias voltage can be studied concerning the false turn-on phenomena (device has very low threshold).
* The negative absolute minimum of the GaN device gate should also be considered for spikes caused by miller effect.
* Some precautions can be taken on the gate (diode + TVS etc.) on the PCB.
* A comparative analysis with switching loss and dv/dt (and di/dt) rates can be studied with variable resistances (and/or other parameters ???).
* In addition to the trade-off between loss and spikes, study on critical frequencies (to avoid), sustained oscillation possibilities etc. can be conducted and verified experimentally (apart from the almost linear relation between these parameters). This will be easier when a mathematical model is developed.
* For the aforomentioned critical frequcies, some damping measures can be considered. Effect of ferrite bead can be investigated. Although it may introduce additional inductance to the gate-source path, its relatively high impedance on high frequencies (which are around possible gate oscillation frequencies) will help damping the oscillations.  This should be included on both Simulink model and state space model.
* RC snubber across gate-source ??? Not very common.
*
