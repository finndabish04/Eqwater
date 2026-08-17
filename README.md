This is the first draft of a README for Finn and Ginger's water quality monitoring code.

As of Aug. 16 2026, this code is designed to be compatible with an ESP32-C6 chip, specifically with the DFRobot firebeetle C6 v2. 
The main components of this project are the sensors, SD card module, and the power-limiting sections. These sections all require major edits if not a complete overhaul. The calibration code almost certainly requires an overhaul. 

Sensors used:
- DFRobot pH sensor (industrial grade. not lab grade)
- Atlas scientific conductivity sensor (K 1.0 I think
- DS18B20 temperature sensor, waterproof

Sensors to add:
- TDS sensor (potentially replace conductivity? cheaper)
- Perhaps a new pH sensor, depending on how consistent the current one behaves during testing

There is a world in which we no longer need to worry about power-cycling for this project, as power to recharge the 6600 mAH liPo batteries might be provided by a turbine of some sort. This plan is currently in progress, and we hope to use the power-cycling code as a backup option as it is what caused a lot of problems during the testing phase of the first prototypes (summer 2026). 
