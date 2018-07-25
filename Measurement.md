## How to Measure

#### Current Measurement
There are five different methods to measure device current. </br>
* Shunt Resistor
* Rogowski Coil
* Hall sensor & Current transformer
* PCB coupled magnetic measurement design
* Current Probe

##### Pros and Cons of Shunt Resistor
+ Simple method
+ Voltage probe is enough to measures
- Special HBW shunt resistor is required because for standard chip resistors the resistance increases as the frequency increases due to the skin effect and proximity effect; the total impedance of the resistor increases with frequency because of the series inductance(1)
+ Coaxial shunt resistors have BW up to 2 GHz
- Coaxial shunt resistors are rare and expensive
- Protection should be taken into consideration
- The power is limited (2W)
- Increase in parasitic inductance on power loop

##### Pros and Cons of Rogowski Coil
+ Simple method
+ Large currents can be measure safely
+ Isolated
- Too low bandwidth (max 30 MHz)

##### Pros and Cons of Hall Sensor & Current transformer
+ Average simplicity
+ Saturation problem, so large amount of currents cannot be measured
+ Mutuality changes with frequencies, compensation is required

##### Pros and Cons of PCB coupled magnetic measurement design(1)
- Complex solution
+ Cheap
+ More research
+ Frequency dependent compensation algorithm is required
+ HBW voltage probe is enough

##### Pros and Cons of Current Probe
- Requires high BW current probe (>350 MHz) so its expensive
+ Very simple

##### References
(1) https://ieeexplore.ieee.org/document/8025825/

#### Voltage Measurement

Voltage measurement is simpler than current measurement. A voltage probe with BW larger than 350 MHz is enough. </br>
On the PCB, the vias/holes should be placed close to the DUT.
