EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L basic:BatteryHolder B2
U 1 1 61A2689C
P 2500 2650
F 0 "B2" H 2173 2646 50  0000 R CNN
F 1 "BatteryHolder" H 2173 2555 50  0000 R CNN
F 2 "basic:BatteryHolder-Single" H 2500 2650 50  0001 C CNN
F 3 "" H 2820 2325 50  0001 C CNN
	1    2500 2650
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:INA282-Amplifier_Current U?
U 1 1 61A2B1F0
P 4050 1550
AR Path="/61A2B1F0" Ref="U?"  Part="1" 
AR Path="/61A249B2/61A2B1F0" Ref="U6"  Part="1" 
F 0 "U6" H 4250 1350 50  0000 L CNN
F 1 "INA282" H 4250 1250 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4050 1550 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina282.pdf" H 4050 1550 50  0001 C CNN
	1    4050 1550
	1    0    0    -1  
$EndComp
Text GLabel 3750 1450 0    50   Input ~ 0
Rs2+
Text GLabel 3750 1650 0    50   Input ~ 0
Rs2-
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A2B1F8
P 3950 2000
AR Path="/61A2B1F8" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A2B1F8" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 3950 1750 50  0001 C CNN
F 1 "GND" H 3955 1827 50  0000 C CNN
F 2 "" H 3950 2000 50  0001 C CNN
F 3 "" H 3950 2000 50  0001 C CNN
	1    3950 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 1150 3950 1250
Wire Wire Line
	3950 1850 3950 1950
Wire Wire Line
	4050 1850 4050 1950
Wire Wire Line
	4050 1950 3950 1950
Connection ~ 3950 1950
Wire Wire Line
	3950 1950 3950 2000
Wire Wire Line
	4150 2000 4150 1850
$Comp
L CVModule-rescue:+5V-power #PWR?
U 1 1 61A2B206
P 3950 1150
AR Path="/61A2B206" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A2B206" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 3950 1000 50  0001 C CNN
F 1 "+5V" H 3965 1323 50  0000 C CNN
F 2 "" H 3950 1150 50  0001 C CNN
F 3 "" H 3950 1150 50  0001 C CNN
	1    3950 1150
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:+5V-power #PWR?
U 1 1 61A2B20C
P 4150 2000
AR Path="/61A2B20C" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A2B20C" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 4150 1850 50  0001 C CNN
F 1 "+5V" H 4165 2173 50  0000 C CNN
F 2 "" H 4150 2000 50  0001 C CNN
F 3 "" H 4150 2000 50  0001 C CNN
	1    4150 2000
	-1   0    0    1   
$EndComp
$Comp
L CVModule-rescue:R_Small-Device R?
U 1 1 61A2ECB7
P 2500 1550
AR Path="/61A2ECB7" Ref="R?"  Part="1" 
AR Path="/61A249B2/61A2ECB7" Ref="R15"  Part="1" 
F 0 "R15" H 2559 1596 50  0000 L CNN
F 1 "13m" H 2559 1505 50  0000 L CNN
F 2 "Resistor_SMD:R_2010_5025Metric_Pad1.40x2.65mm_HandSolder" H 2500 1550 50  0001 C CNN
F 3 "~" H 2500 1550 50  0001 C CNN
	1    2500 1550
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:R_Small-Device R?
U 1 1 61A2ECBD
P 2950 1550
AR Path="/61A2ECBD" Ref="R?"  Part="1" 
AR Path="/61A249B2/61A2ECBD" Ref="R34"  Part="1" 
F 0 "R34" H 3009 1596 50  0000 L CNN
F 1 "13m" H 3009 1505 50  0000 L CNN
F 2 "Resistor_SMD:R_2010_5025Metric_Pad1.40x2.65mm_HandSolder" H 2950 1550 50  0001 C CNN
F 3 "~" H 2950 1550 50  0001 C CNN
	1    2950 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1650 2950 1800
Wire Wire Line
	2950 1450 2950 1350
Wire Wire Line
	2500 1350 2500 1450
Wire Wire Line
	2500 1650 2500 1800
Wire Wire Line
	2500 1800 2950 1800
Connection ~ 2500 1800
Text GLabel 2500 1800 0    50   Input ~ 0
Rs2+
Text GLabel 2500 1350 0    50   Input ~ 0
Rs2-
$Comp
L basic:IOPad P?
U 1 1 61A38DA6
P 5750 1350
AR Path="/61A38DA6" Ref="P?"  Part="1" 
AR Path="/61A249B2/61A38DA6" Ref="P3"  Part="1" 
F 0 "P3" H 5692 1135 50  0000 C CNN
F 1 "+12Vin" H 5692 1226 50  0000 C CNN
F 2 "basic:IOPad" H 5750 1250 50  0001 C CNN
F 3 "" H 5750 1350 50  0001 C CNN
	1    5750 1350
	-1   0    0    1   
$EndComp
$Comp
L basic:IOPad P?
U 1 1 61A38DAC
P 5750 1750
AR Path="/61A38DAC" Ref="P?"  Part="1" 
AR Path="/61A249B2/61A38DAC" Ref="P4"  Part="1" 
F 0 "P4" H 5692 1535 50  0000 C CNN
F 1 "GND" H 5692 1626 50  0000 C CNN
F 2 "basic:IOPad" H 5750 1650 50  0001 C CNN
F 3 "" H 5750 1750 50  0001 C CNN
	1    5750 1750
	-1   0    0    1   
$EndComp
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A38DB2
P 5900 1750
AR Path="/61A38DB2" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A38DB2" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 5900 1500 50  0001 C CNN
F 1 "GND" V 5905 1622 50  0000 R CNN
F 2 "" H 5900 1750 50  0001 C CNN
F 3 "" H 5900 1750 50  0001 C CNN
	1    5900 1750
	0    -1   -1   0   
$EndComp
$Comp
L CVModule-rescue:L7805-Regulator_Linear U?
U 1 1 61A38DB8
P 7100 1350
AR Path="/61A38DB8" Ref="U?"  Part="1" 
AR Path="/61A249B2/61A38DB8" Ref="U11"  Part="1" 
F 0 "U11" H 7100 1592 50  0000 C CNN
F 1 "L7805" H 7100 1501 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 7125 1200 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 7100 1300 50  0001 C CNN
	1    7100 1350
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:C_Small-Device C?
U 1 1 61A38DBE
P 6500 1600
AR Path="/61A38DBE" Ref="C?"  Part="1" 
AR Path="/61A249B2/61A38DBE" Ref="C5"  Part="1" 
F 0 "C5" H 6592 1646 50  0000 L CNN
F 1 "0.33u" H 6592 1555 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 6500 1600 50  0001 C CNN
F 3 "~" H 6500 1600 50  0001 C CNN
	1    6500 1600
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:C_Small-Device C?
U 1 1 61A38DC4
P 7650 1600
AR Path="/61A38DC4" Ref="C?"  Part="1" 
AR Path="/61A249B2/61A38DC4" Ref="C10"  Part="1" 
F 0 "C10" H 7742 1646 50  0000 L CNN
F 1 "100n" H 7742 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 7650 1600 50  0001 C CNN
F 3 "~" H 7650 1600 50  0001 C CNN
	1    7650 1600
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:+5V-power #PWR?
U 1 1 61A38DCA
P 7650 1350
AR Path="/61A38DCA" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A38DCA" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 7650 1200 50  0001 C CNN
F 1 "+5V" H 7665 1523 50  0000 C CNN
F 2 "" H 7650 1350 50  0001 C CNN
F 3 "" H 7650 1350 50  0001 C CNN
	1    7650 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 1350 7650 1350
Wire Wire Line
	7650 1500 7650 1350
Connection ~ 7650 1350
Wire Wire Line
	6800 1350 6500 1350
Wire Wire Line
	6500 1500 6500 1350
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A38DD5
P 7100 1850
AR Path="/61A38DD5" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A38DD5" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 7100 1600 50  0001 C CNN
F 1 "GND" H 7105 1677 50  0000 C CNN
F 2 "" H 7100 1850 50  0001 C CNN
F 3 "" H 7100 1850 50  0001 C CNN
	1    7100 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1700 6500 1850
Wire Wire Line
	6500 1850 7100 1850
Wire Wire Line
	7650 1700 7650 1850
Wire Wire Line
	7650 1850 7100 1850
Connection ~ 7100 1850
Wire Wire Line
	7100 1850 7100 1650
$Comp
L CVModule-rescue:+12V-power #PWR?
U 1 1 61A38DE1
P 5900 1350
AR Path="/61A38DE1" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A38DE1" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 5900 1200 50  0001 C CNN
F 1 "+12V" V 5915 1478 50  0000 L CNN
F 2 "" H 5900 1350 50  0001 C CNN
F 3 "" H 5900 1350 50  0001 C CNN
	1    5900 1350
	0    1    1    0   
$EndComp
$Comp
L CVModule-rescue:+12V-power #PWR?
U 1 1 61A38DE7
P 6500 1350
AR Path="/61A38DE7" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A38DE7" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 6500 1200 50  0001 C CNN
F 1 "+12V" H 6515 1523 50  0000 C CNN
F 2 "" H 6500 1350 50  0001 C CNN
F 3 "" H 6500 1350 50  0001 C CNN
	1    6500 1350
	1    0    0    -1  
$EndComp
Connection ~ 6500 1350
$Comp
L CVModule-rescue:C_Small-Device C?
U 1 1 61A3BEDB
P 8450 1600
AR Path="/61A3BEDB" Ref="C?"  Part="1" 
AR Path="/61A249B2/61A3BEDB" Ref="C11"  Part="1" 
F 0 "C11" H 8542 1646 50  0000 L CNN
F 1 "100n" H 8542 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8450 1600 50  0001 C CNN
F 3 "~" H 8450 1600 50  0001 C CNN
	1    8450 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1300 8450 1500
Wire Wire Line
	8450 1700 8450 1900
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A3BEE3
P 8450 1900
AR Path="/61A3BEE3" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A3BEE3" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 8450 1650 50  0001 C CNN
F 1 "GND" H 8455 1727 50  0000 C CNN
F 2 "" H 8450 1900 50  0001 C CNN
F 3 "" H 8450 1900 50  0001 C CNN
	1    8450 1900
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:+5V-power #PWR?
U 1 1 61A3BEE9
P 8450 1300
AR Path="/61A3BEE9" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A3BEE9" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 8450 1150 50  0001 C CNN
F 1 "+5V" H 8465 1473 50  0000 C CNN
F 2 "" H 8450 1300 50  0001 C CNN
F 3 "" H 8450 1300 50  0001 C CNN
	1    8450 1300
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:LED_Small-Device D?
U 1 1 61A3E949
P 8950 1500
AR Path="/61A3E949" Ref="D?"  Part="1" 
AR Path="/61A249B2/61A3E949" Ref="D2"  Part="1" 
F 0 "D2" V 8996 1430 50  0000 R CNN
F 1 "GREEN" V 8905 1430 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8950 1500 50  0001 C CNN
F 3 "~" V 8950 1500 50  0001 C CNN
	1    8950 1500
	0    -1   -1   0   
$EndComp
$Comp
L CVModule-rescue:R_Small-Device R?
U 1 1 61A3E94F
P 8950 1800
AR Path="/61A3E94F" Ref="R?"  Part="1" 
AR Path="/61A249B2/61A3E94F" Ref="R35"  Part="1" 
F 0 "R35" H 8891 1754 50  0000 R CNN
F 1 "1k" H 8891 1845 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 8950 1800 50  0001 C CNN
F 3 "~" H 8950 1800 50  0001 C CNN
	1    8950 1800
	-1   0    0    1   
$EndComp
$Comp
L CVModule-rescue:+5V-power #PWR?
U 1 1 61A3E955
P 8950 1400
AR Path="/61A3E955" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A3E955" Ref="#PWR033"  Part="1" 
F 0 "#PWR033" H 8950 1250 50  0001 C CNN
F 1 "+5V" H 8965 1573 50  0000 C CNN
F 2 "" H 8950 1400 50  0001 C CNN
F 3 "" H 8950 1400 50  0001 C CNN
	1    8950 1400
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A3E95B
P 8950 1900
AR Path="/61A3E95B" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A3E95B" Ref="#PWR036"  Part="1" 
F 0 "#PWR036" H 8950 1650 50  0001 C CNN
F 1 "GND" H 8955 1727 50  0000 C CNN
F 2 "" H 8950 1900 50  0001 C CNN
F 3 "" H 8950 1900 50  0001 C CNN
	1    8950 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 1600 8950 1700
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A50D39
P 3150 2300
AR Path="/61A50D39" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A50D39" Ref="TP7"  Part="1" 
F 0 "TP7" H 3208 2418 50  0000 L CNN
F 1 "V+_DAQ" H 3208 2327 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 3350 2300 50  0001 C CNN
F 3 "~" H 3350 2300 50  0001 C CNN
	1    3150 2300
	0    1    1    0   
$EndComp
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A5198B
P 3150 3500
AR Path="/61A5198B" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A5198B" Ref="TP9"  Part="1" 
F 0 "TP9" H 3208 3618 50  0000 L CNN
F 1 "V-_DAQ" H 3208 3527 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 3350 3500 50  0001 C CNN
F 3 "~" H 3350 3500 50  0001 C CNN
	1    3150 3500
	0    1    1    0   
$EndComp
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A521B5
P 3150 1950
AR Path="/61A521B5" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A521B5" Ref="TP6"  Part="1" 
F 0 "TP6" H 3208 2068 50  0000 L CNN
F 1 "V+_dis" H 3208 1977 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 3350 1950 50  0001 C CNN
F 3 "~" H 3350 1950 50  0001 C CNN
	1    3150 1950
	0    1    1    0   
$EndComp
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A52AC1
P 3150 3150
AR Path="/61A52AC1" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A52AC1" Ref="TP8"  Part="1" 
F 0 "TP8" H 3208 3268 50  0000 L CNN
F 1 "V-_dis" H 3208 3177 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 3350 3150 50  0001 C CNN
F 3 "~" H 3350 3150 50  0001 C CNN
	1    3150 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 3150 3050 3150
Wire Wire Line
	3050 3150 3050 3350
Wire Wire Line
	3050 3500 3150 3500
Wire Wire Line
	2900 3350 3050 3350
Connection ~ 3050 3350
Wire Wire Line
	3050 3350 3050 3500
Wire Wire Line
	3150 1950 3050 1950
Wire Wire Line
	3050 1950 3050 2100
Wire Wire Line
	3050 2100 2900 2100
Wire Wire Line
	3050 2100 3050 2300
Wire Wire Line
	3050 2300 3150 2300
Connection ~ 3050 2100
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A56ECD
P 2500 3600
AR Path="/61A56ECD" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A56ECD" Ref="TP5"  Part="1" 
F 0 "TP5" H 2558 3718 50  0000 L CNN
F 1 "I-_dis" H 2558 3627 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 2700 3600 50  0001 C CNN
F 3 "~" H 2700 3600 50  0001 C CNN
	1    2500 3600
	-1   0    0    1   
$EndComp
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A583D7
P 2500 1150
AR Path="/61A583D7" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A583D7" Ref="TP4"  Part="1" 
F 0 "TP4" H 2558 1268 50  0000 L CNN
F 1 "I+_dis" H 2558 1177 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 2700 1150 50  0001 C CNN
F 3 "~" H 2700 1150 50  0001 C CNN
	1    2500 1150
	1    0    0    -1  
$EndComp
Connection ~ 2500 1350
Wire Wire Line
	2500 1150 2500 1350
Wire Wire Line
	2500 1350 2950 1350
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A596CB
P 4750 1550
AR Path="/61A596CB" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A596CB" Ref="TP11"  Part="1" 
F 0 "TP11" H 4808 1668 50  0000 L CNN
F 1 "Iout_DAQ" H 4808 1577 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 4950 1550 50  0001 C CNN
F 3 "~" H 4950 1550 50  0001 C CNN
	1    4750 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 1550 4350 1550
$Comp
L CVModule-rescue:GND-power #PWR?
U 1 1 61A5B844
P 3900 2950
AR Path="/61A5B844" Ref="#PWR?"  Part="1" 
AR Path="/61A249B2/61A5B844" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 3900 2700 50  0001 C CNN
F 1 "GND" V 3905 2822 50  0000 R CNN
F 2 "" H 3900 2950 50  0001 C CNN
F 3 "" H 3900 2950 50  0001 C CNN
	1    3900 2950
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:TestPoint-Connector TP?
U 1 1 61A5C317
P 3900 2700
AR Path="/61A5C317" Ref="TP?"  Part="1" 
AR Path="/61A249B2/61A5C317" Ref="TP10"  Part="1" 
F 0 "TP10" H 3958 2818 50  0000 L CNN
F 1 "GND_DAQ" H 3958 2727 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 4100 2700 50  0001 C CNN
F 3 "~" H 4100 2700 50  0001 C CNN
	1    3900 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2700 3900 2950
$Comp
L CVModule-rescue:MountingHole-Mechanical H?
U 1 1 61A97B1A
P 5050 2500
AR Path="/61A97B1A" Ref="H?"  Part="1" 
AR Path="/61A249B2/61A97B1A" Ref="H5"  Part="1" 
F 0 "H5" H 5150 2546 50  0000 L CNN
F 1 "MountingHole" H 5150 2455 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 5050 2500 50  0001 C CNN
F 3 "~" H 5050 2500 50  0001 C CNN
	1    5050 2500
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:MountingHole-Mechanical H?
U 1 1 61A97B20
P 5050 2700
AR Path="/61A97B20" Ref="H?"  Part="1" 
AR Path="/61A249B2/61A97B20" Ref="H6"  Part="1" 
F 0 "H6" H 5150 2746 50  0000 L CNN
F 1 "MountingHole" H 5150 2655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 5050 2700 50  0001 C CNN
F 3 "~" H 5050 2700 50  0001 C CNN
	1    5050 2700
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:MountingHole-Mechanical H?
U 1 1 61A97B26
P 5050 2900
AR Path="/61A97B26" Ref="H?"  Part="1" 
AR Path="/61A249B2/61A97B26" Ref="H7"  Part="1" 
F 0 "H7" H 5150 2946 50  0000 L CNN
F 1 "MountingHole" H 5150 2855 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 5050 2900 50  0001 C CNN
F 3 "~" H 5050 2900 50  0001 C CNN
	1    5050 2900
	1    0    0    -1  
$EndComp
$Comp
L CVModule-rescue:MountingHole-Mechanical H?
U 1 1 61A97B2C
P 5050 3100
AR Path="/61A97B2C" Ref="H?"  Part="1" 
AR Path="/61A249B2/61A97B2C" Ref="H8"  Part="1" 
F 0 "H8" H 5150 3146 50  0000 L CNN
F 1 "MountingHole" H 5150 3055 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 5050 3100 50  0001 C CNN
F 3 "~" H 5050 3100 50  0001 C CNN
	1    5050 3100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
