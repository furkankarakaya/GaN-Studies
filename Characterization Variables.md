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
