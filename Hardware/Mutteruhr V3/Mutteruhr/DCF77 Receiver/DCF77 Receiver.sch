EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "DCF 77 Empfänger"
Date ""
Rev ""
Comp "Michael Albert"
Comment1 "DCF 77 Empfänger mit symmetrischer Übertragung"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Screw_Terminal_01x04 J1
U 1 1 60282049
P 1600 2350
F 0 "J1" H 1518 2667 50  0000 C CNN
F 1 "Anschlußklemme" H 1518 2576 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-4-5.08_1x04_P5.08mm_Horizontal" H 1600 2350 50  0001 C CNN
F 3 "~" H 1600 2350 50  0001 C CNN
	1    1600 2350
	-1   0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U1
U 1 1 60283CCC
P 3800 2000
F 0 "U1" H 3800 2367 50  0000 C CNN
F 1 "LM358" H 3800 2276 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 3800 2000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 3800 2000 50  0001 C CNN
	1    3800 2000
	-1   0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U1
U 2 1 6028742D
P 3800 3100
F 0 "U1" H 3800 3467 50  0000 C CNN
F 1 "LM358" H 3800 3376 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 3800 3100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 3800 3100 50  0001 C CNN
	2    3800 3100
	-1   0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U1
U 3 1 6028B1C6
P 4050 4250
F 0 "U1" H 4008 4296 50  0000 L CNN
F 1 "LM358" H 4008 4205 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 4050 4250 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 4050 4250 50  0001 C CNN
	3    4050 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 6028DCC2
P 2050 1950
F 0 "#PWR01" H 2050 1800 50  0001 C CNN
F 1 "+5V" H 2065 2123 50  0000 C CNN
F 2 "" H 2050 1950 50  0001 C CNN
F 3 "" H 2050 1950 50  0001 C CNN
	1    2050 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 6028E51A
P 3950 4800
F 0 "#PWR07" H 3950 4550 50  0001 C CNN
F 1 "GND" H 3955 4627 50  0000 C CNN
F 2 "" H 3950 4800 50  0001 C CNN
F 3 "" H 3950 4800 50  0001 C CNN
	1    3950 4800
	1    0    0    -1  
$EndComp
$Comp
L Diode:ZPDxx D1
U 1 1 6028F7E2
P 5250 2950
F 0 "D1" V 5204 3030 50  0000 L CNN
F 1 "Z3.3V" V 5295 3030 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P10.16mm_Horizontal" H 5250 2775 50  0001 C CNN
F 3 "http://diotec.com/tl_files/diotec/files/pdf/datasheets/zpd1" H 5250 2950 50  0001 C CNN
	1    5250 2950
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 60290F45
P 7250 2650
F 0 "J2" H 7222 2624 50  0000 R CNN
F 1 "DCF Modul" H 7222 2533 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7250 2650 50  0001 C CNN
F 3 "~" H 7250 2650 50  0001 C CNN
	1    7250 2650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 602920B3
P 2300 2800
F 0 "#PWR02" H 2300 2550 50  0001 C CNN
F 1 "GND" H 2305 2627 50  0000 C CNN
F 2 "" H 2300 2800 50  0001 C CNN
F 3 "" H 2300 2800 50  0001 C CNN
	1    2300 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3800 3950 3950
Wire Wire Line
	3950 4550 3950 4800
$Comp
L power:+5V #PWR06
U 1 1 602968C6
P 3950 3800
F 0 "#PWR06" H 3950 3650 50  0001 C CNN
F 1 "+5V" H 3965 3973 50  0000 C CNN
F 2 "" H 3950 3800 50  0001 C CNN
F 3 "" H 3950 3800 50  0001 C CNN
	1    3950 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2250 2300 2800
Wire Wire Line
	1800 2250 2300 2250
Wire Wire Line
	1800 2350 2050 2350
Wire Wire Line
	2050 2350 2050 1950
Wire Wire Line
	1800 2550 2900 2550
Wire Wire Line
	2900 2550 2900 2000
Wire Wire Line
	2900 2000 3500 2000
$Comp
L Device:R R4
U 1 1 602B3054
P 5250 2200
F 0 "R4" H 5320 2246 50  0000 L CNN
F 1 "390" H 5320 2155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5180 2200 50  0001 C CNN
F 3 "~" H 5250 2200 50  0001 C CNN
	1    5250 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 602B36D8
P 6100 2950
F 0 "R5" H 6170 2996 50  0000 L CNN
F 1 "47k" H 6170 2905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6030 2950 50  0001 C CNN
F 3 "~" H 6100 2950 50  0001 C CNN
	1    6100 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 602B4C5B
P 2900 2850
F 0 "R1" H 2970 2896 50  0000 L CNN
F 1 "R" H 2970 2805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2830 2850 50  0001 C CNN
F 3 "~" H 2900 2850 50  0001 C CNN
	1    2900 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 2450 3350 3100
Wire Wire Line
	3350 3100 3500 3100
Wire Wire Line
	1800 2450 3350 2450
Wire Wire Line
	2900 2550 2900 2700
Connection ~ 2900 2550
$Comp
L Device:LED D2
U 1 1 602B5CE7
P 2900 3300
F 0 "D2" V 2939 3182 50  0000 R CNN
F 1 "LED" V 2848 3182 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 2900 3300 50  0001 C CNN
F 3 "~" H 2900 3300 50  0001 C CNN
	1    2900 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 3150 2900 3000
$Comp
L power:GND #PWR09
U 1 1 602B6CB7
P 4700 3350
F 0 "#PWR09" H 4700 3100 50  0001 C CNN
F 1 "GND" H 4705 3177 50  0000 C CNN
F 2 "" H 4700 3350 50  0001 C CNN
F 3 "" H 4700 3350 50  0001 C CNN
	1    4700 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3450 2900 3700
$Comp
L Device:R R2
U 1 1 602BD234
P 4700 2200
F 0 "R2" H 4770 2246 50  0000 L CNN
F 1 "22k" H 4770 2155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4630 2200 50  0001 C CNN
F 3 "~" H 4700 2200 50  0001 C CNN
	1    4700 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 602BD952
P 4700 2950
F 0 "R3" H 4770 2996 50  0000 L CNN
F 1 "10k" H 4770 2905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4630 2950 50  0001 C CNN
F 3 "~" H 4700 2950 50  0001 C CNN
	1    4700 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 602BF561
P 4700 1800
F 0 "#PWR08" H 4700 1650 50  0001 C CNN
F 1 "+5V" H 4715 1973 50  0000 C CNN
F 2 "" H 4700 1800 50  0001 C CNN
F 3 "" H 4700 1800 50  0001 C CNN
	1    4700 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 602BF919
P 5250 1800
F 0 "#PWR010" H 5250 1650 50  0001 C CNN
F 1 "+5V" H 5265 1973 50  0000 C CNN
F 2 "" H 5250 1800 50  0001 C CNN
F 3 "" H 5250 1800 50  0001 C CNN
	1    5250 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 602BFD78
P 5700 2950
F 0 "C3" H 5818 2996 50  0000 L CNN
F 1 "10uF" H 5818 2905 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 5738 2800 50  0001 C CNN
F 3 "~" H 5700 2950 50  0001 C CNN
	1    5700 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1800 4700 2050
Wire Wire Line
	5250 2050 5250 1800
$Comp
L power:GND #PWR03
U 1 1 602C1B9E
P 2900 3700
F 0 "#PWR03" H 2900 3450 50  0001 C CNN
F 1 "GND" H 2905 3527 50  0000 C CNN
F 2 "" H 2900 3700 50  0001 C CNN
F 3 "" H 2900 3700 50  0001 C CNN
	1    2900 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 602C1EBF
P 5250 3350
F 0 "#PWR011" H 5250 3100 50  0001 C CNN
F 1 "GND" H 5255 3177 50  0000 C CNN
F 2 "" H 5250 3350 50  0001 C CNN
F 3 "" H 5250 3350 50  0001 C CNN
	1    5250 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3000 4300 3000
Wire Wire Line
	4300 2100 4100 2100
Wire Wire Line
	4700 2350 4700 2550
Wire Wire Line
	4700 2550 4300 2550
Wire Wire Line
	4300 2100 4300 2550
Connection ~ 4700 2550
Wire Wire Line
	4700 2550 4700 2800
Connection ~ 4300 2550
Wire Wire Line
	4300 2550 4300 3000
Wire Wire Line
	5250 2350 5250 2550
Wire Wire Line
	4700 3350 4700 3100
Wire Wire Line
	5250 3100 5250 3350
Connection ~ 5250 2550
Wire Wire Line
	5250 2550 5250 2800
$Comp
L power:GND #PWR012
U 1 1 602CC175
P 5700 3350
F 0 "#PWR012" H 5700 3100 50  0001 C CNN
F 1 "GND" H 5705 3177 50  0000 C CNN
F 2 "" H 5700 3350 50  0001 C CNN
F 3 "" H 5700 3350 50  0001 C CNN
	1    5700 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1900 4500 1900
Wire Wire Line
	4500 1900 4500 3200
Wire Wire Line
	4500 3200 4100 3200
Wire Wire Line
	4500 3200 4500 3900
Wire Wire Line
	4500 3900 6100 3900
Wire Wire Line
	6500 3900 6500 2750
Wire Wire Line
	6500 2750 7050 2750
Connection ~ 4500 3200
Wire Wire Line
	5250 2550 5700 2550
Wire Wire Line
	5700 2800 5700 2550
Connection ~ 5700 2550
Wire Wire Line
	5700 2550 6100 2550
Wire Wire Line
	5700 3100 5700 3350
Wire Wire Line
	6100 2800 6100 2550
Connection ~ 6100 2550
Wire Wire Line
	6100 2550 7050 2550
Wire Wire Line
	6100 3100 6100 3900
Connection ~ 6100 3900
Wire Wire Line
	6100 3900 6500 3900
$Comp
L Device:CP C2
U 1 1 602D1FBC
P 3250 4250
F 0 "C2" H 3368 4296 50  0000 L CNN
F 1 "47uF" H 3368 4205 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 3288 4100 50  0001 C CNN
F 3 "~" H 3250 4250 50  0001 C CNN
	1    3250 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 602D2450
P 3250 3800
F 0 "#PWR04" H 3250 3650 50  0001 C CNN
F 1 "+5V" H 3265 3973 50  0000 C CNN
F 2 "" H 3250 3800 50  0001 C CNN
F 3 "" H 3250 3800 50  0001 C CNN
	1    3250 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 602D2667
P 3250 4800
F 0 "#PWR05" H 3250 4550 50  0001 C CNN
F 1 "GND" H 3255 4627 50  0000 C CNN
F 2 "" H 3250 4800 50  0001 C CNN
F 3 "" H 3250 4800 50  0001 C CNN
	1    3250 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3800 3250 3950
$Comp
L Device:C C1
U 1 1 602D6A29
P 3650 4250
F 0 "C1" H 3765 4296 50  0000 L CNN
F 1 "1uF" H 3765 4205 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W3.5mm_P5.00mm" H 3688 4100 50  0001 C CNN
F 3 "~" H 3650 4250 50  0001 C CNN
	1    3650 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4100 3650 3950
Wire Wire Line
	3650 3950 3250 3950
Connection ~ 3250 3950
Wire Wire Line
	3250 3950 3250 4100
Wire Wire Line
	3250 4400 3250 4550
Wire Wire Line
	3650 4400 3650 4550
Wire Wire Line
	3650 4550 3250 4550
Connection ~ 3250 4550
Wire Wire Line
	3250 4550 3250 4800
$Comp
L power:GND #PWR0101
U 1 1 602DA082
P 6950 3100
F 0 "#PWR0101" H 6950 2850 50  0001 C CNN
F 1 "GND" H 6955 2927 50  0000 C CNN
F 2 "" H 6950 3100 50  0001 C CNN
F 3 "" H 6950 3100 50  0001 C CNN
	1    6950 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 2850 6950 2850
Wire Wire Line
	6950 2850 6950 3100
Wire Wire Line
	7050 2650 6950 2650
Wire Wire Line
	6950 2650 6950 2850
Connection ~ 6950 2850
Text Label 6500 2750 0    50   ~ 0
DCFModuleData
Text Label 6650 2550 0    50   ~ 0
3.3V
Text Label 1950 2450 0    50   ~ 0
Data+
Text Label 1950 2550 0    50   ~ 0
Data-
$EndSCHEMATC
