# Sinusoidal Charger with Optimized Frequency Tracking for Li-ion Batteries

The purpose of this thesis is to develop a sinusoidal charger that can track the optimized frequency of Li-ion batteries.
In this study, the optimized AC impedance frequency tracking is firstly carried out for various types of lithium-ion batteries with different aging degrees to achieve the goal of shortening the charging time and increasing the charging efficiency. 
The digital control core of the charger developed in this thesis uses the F280049C digital signal controller (DSC) produced by Texas Instruments and adopts the Powell algorithm to quickly search for the best sinusoidal charging current frequency when the battery state-of-charge (SOC) is 0%, 15% and 30%, which ensures the lowest impedance during charging.
The proposed prototyping circuit uses field effect transistors as voltage-to-current converters and utilizes digital finite impulse response filters and proportional-integral controllers to stabilize the charging current. 
The charger can provide a maximum charging power of 21W. The experimental results show that the proposed sinusoidal charger can shorten the charging time by 3.66%, reduce the charging loss by 1.08% and reduce the temperature rise by 27.16% compared with the traditional constant current and constant voltage charging method.
Keyword: Sinusoidal Charging, Li-ion Batteries, Fast Charging technique, On-line Optimized Impedance Tracking

[Thesis link](https://ndltd.ncl.edu.tw/cgi-bin/gs32/gsweb.cgi/login?o=dwebmge)

# Outlook
![Front side](https://github.com/HabonRoof/SRC-Charging/blob/main/image/outlook.PNG)
!{Baack side](https://github.com/HabonRoof/SRC-Charging/blob/main/image/outlook2.PNG)

# File disceiption
#### AD5940_BATImpedance
- ADICuP3029 project, to control and calculate real/imaginary part of impedance. Include FFT calculation.
- Source code for AD5940 battery EIS measurment module (CN0510), modified and origin by Analog Device .Inc.
#### KiCad
- Schematic file and PCB file for main board, which offer CN0510, F280049C Launch Pad and AD9833 waveform generator module a base.
#### RippleCharging
- The main CCS project folder, control the behavior of sinusoidal charger, waveform generator, MOSFET relay, EIS measurement module.
#### ad5940lib
- Companion library of AD5940_BATImpedance folder
