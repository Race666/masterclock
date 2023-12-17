EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Mutteruhr Mainboard"
Date ""
Rev "1.00"
Comp "Michael Albert"
Comment1 "Mutteruhr Mainboard mit USB Interface"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U2
U 1 1 60265A60
P 4850 3000
F 0 "U2" H 4206 3046 50  0000 R CNN
F 1 "ATmega328-PU" H 4206 2955 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 4850 3000 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 4850 3000 50  0001 C CNN
	1    4850 3000
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Switching:GL2576-5.0TB5T U1
U 1 1 602674EE
P 1700 3800
F 0 "U1" H 1700 4167 50  0000 C CNN
F 1 "GL2576-5.0TB5T" H 1700 4076 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-5_Vertical" H 1700 3550 50  0001 L CIN
F 3 "http://www.dianyuan.com/bbs/u/54/437861181916300.pdf" H 1700 3800 50  0001 C CNN
	1    1700 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR01
U 1 1 60267D0E
P 850 3500
F 0 "#PWR01" H 850 3350 50  0001 C CNN
F 1 "+24V" H 865 3673 50  0000 C CNN
F 2 "" H 850 3500 50  0001 C CNN
F 3 "" H 850 3500 50  0001 C CNN
	1    850  3500
	1    0    0    -1  
$EndComp
$Comp
L Interface_USB:MCP2200-I-SO U4
U 1 1 60269A22
P 4150 8450
F 0 "U4" H 3450 7650 50  0000 C CNN
F 1 "MCP2200-I-SO" H 3700 7550 50  0000 C CNN
F 2 "Package_SO:SOIC-20W_7.5x12.8mm_P1.27mm" H 4150 7300 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/200022228D.pdf" H 4150 7450 50  0001 C CNN
	1    4150 8450
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_A J4
U 1 1 6026A536
P 5950 7950
F 0 "J4" H 6007 8417 50  0000 C CNN
F 1 "USB_A" H 6007 8326 50  0000 C CNN
F 2 "Connector_USB:USB_B_OST_USB-B1HSxx_Horizontal" H 6100 7900 50  0001 C CNN
F 3 " ~" H 6100 7900 50  0001 C CNN
	1    5950 7950
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 6026B9C6
P 950 1900
F 0 "J1" H 868 2117 50  0000 C CNN
F 1 "Power 24V" H 868 2026 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 950 1900 50  0001 C CNN
F 3 "~" H 950 1900 50  0001 C CNN
	1    950  1900
	-1   0    0    -1  
$EndComp
$Comp
L Driver_Motor:L298HN U5
U 1 1 6026EF69
P 11750 3700
F 0 "U5" H 11750 4581 50  0000 C CNN
F 1 "L298HN" H 11750 4490 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-15_P2.54x2.54mm_StaggerOdd_Lead4.58mm_Vertical" H 11800 3050 50  0001 L CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00000240.pdf" H 11900 3950 50  0001 C CNN
	1    11750 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 60273C2D
P 6350 2550
F 0 "Y1" H 6350 2818 50  0000 C CNN
F 1 "8Mhz" H 6350 2300 50  0000 C CNN
F 2 "Crystal:Crystal_HC49-U_Vertical" H 6350 2550 50  0001 C CNN
F 3 "~" H 6350 2550 50  0001 C CNN
	1    6350 2550
	-1   0    0    1   
$EndComp
$Comp
L Device:L L1
U 1 1 60275B93
P 2700 3900
F 0 "L1" V 2890 3900 50  0000 C CNN
F 1 "100uH" V 2799 3900 50  0000 C CNN
F 2 "Inductor_THT:L_Radial_D8.7mm_P5.00mm_Fastron_07HCP" H 2700 3900 50  0001 C CNN
F 3 "~" H 2700 3900 50  0001 C CNN
	1    2700 3900
	0    -1   -1   0   
$EndComp
$Comp
L Isolator:6N138 U3
U 1 1 60276838
P 6750 4450
F 0 "U3" H 6750 4917 50  0000 C CNN
F 1 "6N138" H 6750 4826 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 7040 4150 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/HCPL2731-D.pdf" H 7040 4150 50  0001 C CNN
	1    6750 4450
	-1   0    0    -1  
$EndComp
$Comp
L Diode:1N62xxCA D7
U 1 1 602792F3
P 13400 3650
F 0 "D7" H 13400 3867 50  0000 C CNN
F 1 "1N62xxCA" H 13400 3776 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 13400 3450 50  0001 C CNN
F 3 "https://www.vishay.com/docs/88301/15ke.pdf" H 13400 3650 50  0001 C CNN
	1    13400 3650
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4004 D3
U 1 1 6027AFE6
P 12850 3300
F 0 "D3" H 12850 3517 50  0000 C CNN
F 1 "1N4004" H 12850 3426 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 12850 3125 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 12850 3300 50  0001 C CNN
	1    12850 3300
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4004 D4
U 1 1 6027BE50
P 12600 3300
F 0 "D4" H 12600 3517 50  0000 C CNN
F 1 "1N4004" H 12600 3426 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 12600 3125 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 12600 3300 50  0001 C CNN
	1    12600 3300
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4004 D5
U 1 1 6027C682
P 12600 4100
F 0 "D5" H 12600 4317 50  0000 C CNN
F 1 "1N4004" H 12600 4226 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 12600 3925 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 12600 4100 50  0001 C CNN
	1    12600 4100
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4004 D6
U 1 1 6027D293
P 12850 4100
F 0 "D6" H 12850 4317 50  0000 C CNN
F 1 "1N4004" H 12850 4226 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 12850 3925 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 12850 4100 50  0001 C CNN
	1    12850 4100
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J5
U 1 1 6027E405
P 13850 3600
F 0 "J5" H 13930 3592 50  0000 L CNN
F 1 "Uhr" H 13930 3501 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 13850 3600 50  0001 C CNN
F 3 "~" H 13850 3600 50  0001 C CNN
	1    13850 3600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x04 J2
U 1 1 6028067A
P 8350 4450
F 0 "J2" H 8430 4442 50  0000 L CNN
F 1 "DCF77 Empfänger" H 8430 4351 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-4-5.08_1x04_P5.08mm_Horizontal" H 8350 4450 50  0001 C CNN
F 3 "~" H 8350 4450 50  0001 C CNN
	1    8350 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR09
U 1 1 60530F49
P 3200 3500
F 0 "#PWR09" H 3200 3350 50  0001 C CNN
F 1 "+5V" H 3215 3673 50  0000 C CNN
F 2 "" H 3200 3500 50  0001 C CNN
F 3 "" H 3200 3500 50  0001 C CNN
	1    3200 3500
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N5820 D1
U 1 1 605318AE
P 2050 1450
F 0 "D1" H 2050 1233 50  0000 C CNN
F 1 "1N5820" H 2050 1324 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 2050 1275 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88526/1n5820.pdf" H 2050 1450 50  0001 C CNN
	1    2050 1450
	-1   0    0    1   
$EndComp
$Comp
L Device:Fuse F1
U 1 1 60532474
P 1750 1450
F 0 "F1" V 1553 1450 50  0000 C CNN
F 1 "Fuse" V 1644 1450 50  0000 C CNN
F 2 "Fuse:Fuse_Bourns_MF-RHT500" V 1680 1450 50  0001 C CNN
F 3 "~" H 1750 1450 50  0001 C CNN
	1    1750 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 1900 1300 1900
Wire Wire Line
	1300 1900 1300 1450
Wire Wire Line
	2200 1450 2400 1450
Wire Wire Line
	2400 1450 2400 1350
Wire Wire Line
	1150 2000 1300 2000
Wire Wire Line
	1300 1450 1550 1450
$Comp
L Device:CP C1
U 1 1 6053B169
P 850 4000
F 0 "C1" H 968 4046 50  0000 L CNN
F 1 "1000uF / 35V" H 968 3955 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 888 3850 50  0001 C CNN
F 3 "~" H 850 4000 50  0001 C CNN
	1    850  4000
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR06
U 1 1 6053C5BB
P 2400 1350
F 0 "#PWR06" H 2400 1200 50  0001 C CNN
F 1 "+24V" H 2415 1523 50  0000 C CNN
F 2 "" H 2400 1350 50  0001 C CNN
F 3 "" H 2400 1350 50  0001 C CNN
	1    2400 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  3850 850  3700
Wire Wire Line
	850  3700 1200 3700
Connection ~ 850  3700
Wire Wire Line
	850  3700 850  3500
$Comp
L power:GND #PWR02
U 1 1 6053D2BA
P 850 4450
F 0 "#PWR02" H 850 4200 50  0001 C CNN
F 1 "GND" H 855 4277 50  0000 C CNN
F 2 "" H 850 4450 50  0001 C CNN
F 3 "" H 850 4450 50  0001 C CNN
	1    850  4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6053D612
P 1150 4450
F 0 "#PWR03" H 1150 4200 50  0001 C CNN
F 1 "GND" H 1155 4277 50  0000 C CNN
F 2 "" H 1150 4450 50  0001 C CNN
F 3 "" H 1150 4450 50  0001 C CNN
	1    1150 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  4150 850  4450
Wire Wire Line
	1150 4450 1150 3900
Wire Wire Line
	1150 3900 1200 3900
$Comp
L Diode:1N5822 D2
U 1 1 6053E743
P 2500 4250
F 0 "D2" V 2454 4330 50  0000 L CNN
F 1 "1N5822" V 2545 4330 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 2500 4075 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88526/1n5820.pdf" H 2500 4250 50  0001 C CNN
	1    2500 4250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 6053F22F
P 1700 4450
F 0 "#PWR05" H 1700 4200 50  0001 C CNN
F 1 "GND" H 1705 4277 50  0000 C CNN
F 2 "" H 1700 4450 50  0001 C CNN
F 3 "" H 1700 4450 50  0001 C CNN
	1    1700 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4100 1700 4450
Wire Wire Line
	2200 3900 2500 3900
Wire Wire Line
	2500 3900 2500 4100
$Comp
L power:GND #PWR07
U 1 1 605402D2
P 2500 4450
F 0 "#PWR07" H 2500 4200 50  0001 C CNN
F 1 "GND" H 2505 4277 50  0000 C CNN
F 2 "" H 2500 4450 50  0001 C CNN
F 3 "" H 2500 4450 50  0001 C CNN
	1    2500 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 4400 2500 4450
$Comp
L Device:CP C3
U 1 1 605410A2
P 3200 4200
F 0 "C3" H 3318 4246 50  0000 L CNN
F 1 "4700uF / 10V" V 3300 4300 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P7.50mm" H 3238 4050 50  0001 C CNN
F 3 "~" H 3200 4200 50  0001 C CNN
	1    3200 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 6054317E
P 2850 4200
F 0 "C2" H 2965 4246 50  0000 L CNN
F 1 "1uF" V 2950 4300 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W6.0mm_P5.00mm" H 2888 4050 50  0001 C CNN
F 3 "~" H 2850 4200 50  0001 C CNN
	1    2850 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 60543B4C
P 2850 4450
F 0 "#PWR08" H 2850 4200 50  0001 C CNN
F 1 "GND" H 2855 4277 50  0000 C CNN
F 2 "" H 2850 4450 50  0001 C CNN
F 3 "" H 2850 4450 50  0001 C CNN
	1    2850 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 60543DF0
P 3200 4450
F 0 "#PWR010" H 3200 4200 50  0001 C CNN
F 1 "GND" H 3205 4277 50  0000 C CNN
F 2 "" H 3200 4450 50  0001 C CNN
F 3 "" H 3200 4450 50  0001 C CNN
	1    3200 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3700 2850 3700
Wire Wire Line
	3200 3500 3200 3700
Connection ~ 3200 3700
Wire Wire Line
	2850 4350 2850 4450
$Comp
L Device:R R1
U 1 1 605481B9
P 1550 1800
F 0 "R1" H 1620 1846 50  0000 L CNN
F 1 "50k" H 1620 1755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1480 1800 50  0001 C CNN
F 3 "~" H 1550 1800 50  0001 C CNN
	1    1550 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 605491BF
P 1550 2300
F 0 "R2" H 1620 2346 50  0000 L CNN
F 1 "8.2k" H 1620 2255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1480 2300 50  0001 C CNN
F 3 "~" H 1550 2300 50  0001 C CNN
	1    1550 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 605496BF
P 1950 2300
F 0 "C4" H 2068 2346 50  0000 L CNN
F 1 "10uF / 10V" H 2068 2255 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 1988 2150 50  0001 C CNN
F 3 "~" H 1950 2300 50  0001 C CNN
	1    1950 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 60549C74
P 1550 2600
F 0 "#PWR011" H 1550 2350 50  0001 C CNN
F 1 "GND" H 1555 2427 50  0000 C CNN
F 2 "" H 1550 2600 50  0001 C CNN
F 3 "" H 1550 2600 50  0001 C CNN
	1    1550 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 60549FB9
P 1950 2600
F 0 "#PWR012" H 1950 2350 50  0001 C CNN
F 1 "GND" H 1955 2427 50  0000 C CNN
F 2 "" H 1950 2600 50  0001 C CNN
F 3 "" H 1950 2600 50  0001 C CNN
	1    1950 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 6054A168
P 1300 2600
F 0 "#PWR04" H 1300 2350 50  0001 C CNN
F 1 "GND" H 1305 2427 50  0000 C CNN
F 2 "" H 1300 2600 50  0001 C CNN
F 3 "" H 1300 2600 50  0001 C CNN
	1    1300 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2000 1300 2600
Wire Wire Line
	1550 2450 1550 2600
Wire Wire Line
	1950 2600 1950 2450
Wire Wire Line
	1550 1650 1550 1450
Connection ~ 1550 1450
Wire Wire Line
	1550 1450 1600 1450
Wire Wire Line
	1550 1950 1550 2050
Wire Wire Line
	1550 2050 1950 2050
Wire Wire Line
	1950 2050 1950 2150
Connection ~ 1550 2050
Wire Wire Line
	1550 2050 1550 2150
Text Label 1750 2050 0    50   ~ 0
Spannungsausfallerkennung
Wire Wire Line
	6200 2500 6200 2550
Wire Wire Line
	6500 2400 6500 2550
$Comp
L Device:C_Variable C5
U 1 1 60552D50
P 6200 2750
F 0 "C5" H 6050 2600 50  0000 L CNN
F 1 "3-60pF" H 6000 2200 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W5.0mm_P5.00mm" H 6200 2750 50  0001 C CNN
F 3 "~" H 6200 2750 50  0001 C CNN
	1    6200 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 60553E8F
P 6500 2750
F 0 "C6" H 6600 2600 50  0000 L CNN
F 1 "18pF" H 6615 2705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W2.0mm_P2.50mm" H 6538 2600 50  0001 C CNN
F 3 "~" H 6500 2750 50  0001 C CNN
	1    6500 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2600 6500 2550
Connection ~ 6500 2550
Wire Wire Line
	6200 2600 6200 2550
Connection ~ 6200 2550
$Comp
L power:GND #PWR0101
U 1 1 605569D9
P 6200 3000
F 0 "#PWR0101" H 6200 2750 50  0001 C CNN
F 1 "GND" H 6205 2827 50  0000 C CNN
F 2 "" H 6200 3000 50  0001 C CNN
F 3 "" H 6200 3000 50  0001 C CNN
	1    6200 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 60557187
P 6500 3000
F 0 "#PWR0102" H 6500 2750 50  0001 C CNN
F 1 "GND" H 6505 2827 50  0000 C CNN
F 2 "" H 6500 3000 50  0001 C CNN
F 3 "" H 6500 3000 50  0001 C CNN
	1    6500 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2900 6200 3000
Wire Wire Line
	6500 3000 6500 2900
Wire Wire Line
	5450 2500 6200 2500
Wire Wire Line
	5450 2400 6500 2400
$Comp
L power:+5V #PWR0103
U 1 1 6055CCD6
P 4850 1300
F 0 "#PWR0103" H 4850 1150 50  0001 C CNN
F 1 "+5V" H 4865 1473 50  0000 C CNN
F 2 "" H 4850 1300 50  0001 C CNN
F 3 "" H 4850 1300 50  0001 C CNN
	1    4850 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 1300 4850 1400
Wire Wire Line
	4850 1400 4950 1400
Wire Wire Line
	4950 1400 4950 1500
Connection ~ 4850 1400
Wire Wire Line
	4850 1400 4850 1500
$Comp
L power:GND #PWR0104
U 1 1 60560292
P 3500 6500
F 0 "#PWR0104" H 3500 6250 50  0001 C CNN
F 1 "GND" H 3505 6327 50  0000 C CNN
F 2 "" H 3500 6500 50  0001 C CNN
F 3 "" H 3500 6500 50  0001 C CNN
	1    3500 6500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x07_Odd_Even J6
U 1 1 60568301
P 7600 2900
F 0 "J6" H 7650 3417 50  0000 C CNN
F 1 "Display" H 7650 3326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x07_P2.54mm_Vertical" H 7600 2900 50  0001 C CNN
F 3 "~" H 7600 2900 50  0001 C CNN
	1    7600 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 6056A5D6
P 7000 2700
F 0 "#PWR016" H 7000 2450 50  0001 C CNN
F 1 "GND" H 7005 2527 50  0000 C CNN
F 2 "" H 7000 2700 50  0001 C CNN
F 3 "" H 7000 2700 50  0001 C CNN
	1    7000 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2700 7000 2600
Wire Wire Line
	7000 2600 7400 2600
$Comp
L power:+5V #PWR018
U 1 1 6056B751
P 8100 2500
F 0 "#PWR018" H 8100 2350 50  0001 C CNN
F 1 "+5V" H 8115 2673 50  0000 C CNN
F 2 "" H 8100 2500 50  0001 C CNN
F 3 "" H 8100 2500 50  0001 C CNN
	1    8100 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 2600 8100 2600
Wire Wire Line
	8100 2600 8100 2500
$Comp
L power:+5V #PWR015
U 1 1 6056E051
P 7000 2150
F 0 "#PWR015" H 7000 2000 50  0001 C CNN
F 1 "+5V" H 7015 2323 50  0000 C CNN
F 2 "" H 7000 2150 50  0001 C CNN
F 3 "" H 7000 2150 50  0001 C CNN
	1    7000 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2150 7000 2300
$Comp
L Device:R_POT_TRIM RV1
U 1 1 6056F429
P 7000 2450
F 0 "RV1" H 6930 2496 50  0000 R CNN
F 1 "2k" H 6930 2405 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_ACP_CA6-H2,5_Horizontal" H 7000 2450 50  0001 C CNN
F 3 "~" H 7000 2450 50  0001 C CNN
	1    7000 2450
	1    0    0    -1  
$EndComp
Connection ~ 7000 2600
Wire Wire Line
	7400 2700 7250 2700
Wire Wire Line
	7250 2700 7250 2450
Wire Wire Line
	7250 2450 7150 2450
Wire Wire Line
	7400 2900 7400 3000
Wire Wire Line
	7900 3000 7900 2900
$Comp
L power:GND #PWR014
U 1 1 6057361C
P 6950 3150
F 0 "#PWR014" H 6950 2900 50  0001 C CNN
F 1 "GND" H 6955 2977 50  0000 C CNN
F 2 "" H 6950 3150 50  0001 C CNN
F 3 "" H 6950 3150 50  0001 C CNN
	1    6950 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 60573901
P 8450 3150
F 0 "#PWR019" H 8450 2900 50  0001 C CNN
F 1 "GND" H 8455 2977 50  0000 C CNN
F 2 "" H 8450 3150 50  0001 C CNN
F 3 "" H 8450 3150 50  0001 C CNN
	1    8450 3150
	1    0    0    -1  
$EndComp
Connection ~ 7900 3000
Connection ~ 7400 3000
Text Label 8050 2700 0    50   ~ 0
RS
Wire Wire Line
	5450 3200 5750 3200
Text Label 5650 3200 0    50   ~ 0
RS
Wire Wire Line
	5450 3100 5750 3100
Text Label 5650 3100 0    50   ~ 0
RW
Wire Wire Line
	7400 2800 7200 2800
Text Label 7200 2800 0    50   ~ 0
RW
Wire Wire Line
	7900 2700 8150 2700
Wire Wire Line
	7900 2800 8150 2800
Wire Wire Line
	5450 3000 5750 3000
Text Label 8050 2800 0    50   ~ 0
E
Text Label 5650 3000 0    50   ~ 0
E
Wire Wire Line
	6950 3150 6950 3000
Wire Wire Line
	6950 3000 7400 3000
Wire Wire Line
	8450 3000 8450 3150
Wire Wire Line
	7900 3000 8450 3000
Wire Wire Line
	7400 3100 7200 3100
Wire Wire Line
	7400 3200 7200 3200
Wire Wire Line
	7900 3100 8150 3100
Wire Wire Line
	7900 3200 8150 3200
Wire Wire Line
	5450 2700 5750 2700
Wire Wire Line
	5450 2800 5750 2800
Wire Wire Line
	5450 2900 5750 2900
Text Label 7200 3100 0    50   ~ 0
D4
Text Label 5650 2900 0    50   ~ 0
D4
Text Label 8050 3100 0    50   ~ 0
D5
Text Label 5650 2800 0    50   ~ 0
D5
Text Label 5650 2700 0    50   ~ 0
D6
Text Label 7200 3200 0    50   ~ 0
D6
Wire Wire Line
	5450 2000 5750 2000
Text Label 5650 2000 0    50   ~ 0
D7
Text Label 8050 3200 0    50   ~ 0
D7
Text Notes 5950 3100 1    50   ~ 0
HD44780
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J3
U 1 1 605941C7
P 4200 6600
F 0 "J3" H 4250 6917 50  0000 C CNN
F 1 "Ext. Seriell Adapter" H 4250 6826 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 4200 6600 50  0001 C CNN
F 3 "~" H 4200 6600 50  0001 C CNN
	1    4200 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3500 5750 3500
Wire Wire Line
	5450 3600 5750 3600
Text Label 5600 3500 0    50   ~ 0
RxD
Text Label 5600 3600 0    50   ~ 0
TxD
$Comp
L power:+5V #PWR013
U 1 1 605A0E3A
P 4750 6450
F 0 "#PWR013" H 4750 6300 50  0001 C CNN
F 1 "+5V" H 4765 6623 50  0000 C CNN
F 2 "" H 4750 6450 50  0001 C CNN
F 3 "" H 4750 6450 50  0001 C CNN
	1    4750 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 6500 4000 6500
Wire Wire Line
	4500 6500 4750 6500
Wire Wire Line
	4750 6500 4750 6450
Wire Wire Line
	4500 6600 4750 6600
Wire Wire Line
	4000 6600 3700 6600
Text Label 3700 6600 0    50   ~ 0
TxD
Text Label 4650 6600 0    50   ~ 0
RxD
NoConn ~ 4000 6700
NoConn ~ 4500 6700
$Comp
L power:GND #PWR017
U 1 1 605AC65C
P 4150 9400
F 0 "#PWR017" H 4150 9150 50  0001 C CNN
F 1 "GND" H 4155 9227 50  0000 C CNN
F 2 "" H 4150 9400 50  0001 C CNN
F 3 "" H 4150 9400 50  0001 C CNN
	1    4150 9400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 9250 4150 9400
$Comp
L Device:Crystal Y2
U 1 1 605B0349
P 5400 9200
F 0 "Y2" H 5400 9468 50  0000 C CNN
F 1 "12Mhz" H 5400 8950 50  0000 C CNN
F 2 "Crystal:Crystal_HC49-U_Vertical" H 5400 9200 50  0001 C CNN
F 3 "~" H 5400 9200 50  0001 C CNN
	1    5400 9200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 605B1C90
P 5650 9500
F 0 "C8" H 5750 9350 50  0000 L CNN
F 1 "27pF" H 5765 9455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W2.0mm_P2.50mm" H 5688 9350 50  0001 C CNN
F 3 "~" H 5650 9500 50  0001 C CNN
	1    5650 9500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 605B2262
P 5100 9500
F 0 "C7" H 5200 9350 50  0000 L CNN
F 1 "27pF" H 5215 9455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W2.0mm_P2.50mm" H 5138 9350 50  0001 C CNN
F 3 "~" H 5100 9500 50  0001 C CNN
	1    5100 9500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 605B27AF
P 5100 9800
F 0 "#PWR020" H 5100 9550 50  0001 C CNN
F 1 "GND" H 5105 9627 50  0000 C CNN
F 2 "" H 5100 9800 50  0001 C CNN
F 3 "" H 5100 9800 50  0001 C CNN
	1    5100 9800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 605B2C1A
P 5650 9800
F 0 "#PWR021" H 5650 9550 50  0001 C CNN
F 1 "GND" H 5655 9627 50  0000 C CNN
F 2 "" H 5650 9800 50  0001 C CNN
F 3 "" H 5650 9800 50  0001 C CNN
	1    5650 9800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 9050 5100 9050
Wire Wire Line
	5100 9050 5100 9200
Wire Wire Line
	5250 9200 5100 9200
Connection ~ 5100 9200
Wire Wire Line
	5100 9200 5100 9350
Wire Wire Line
	5550 9200 5650 9200
Wire Wire Line
	5650 9200 5650 9350
Wire Wire Line
	5050 8850 5650 8850
Wire Wire Line
	5650 8850 5650 9200
Connection ~ 5650 9200
Wire Wire Line
	5650 9650 5650 9800
Wire Wire Line
	5100 9650 5100 9800
$Comp
L Device:R R3
U 1 1 605BFE37
P 2850 7850
F 0 "R3" V 2643 7850 50  0000 C CNN
F 1 "1k" V 2734 7850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2780 7850 50  0001 C CNN
F 3 "~" H 2850 7850 50  0001 C CNN
	1    2850 7850
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR023
U 1 1 605C0E7D
P 2400 7750
F 0 "#PWR023" H 2400 7600 50  0001 C CNN
F 1 "+5V" H 2415 7923 50  0000 C CNN
F 2 "" H 2400 7750 50  0001 C CNN
F 3 "" H 2400 7750 50  0001 C CNN
	1    2400 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 7750 2400 7850
Wire Wire Line
	2400 7850 2700 7850
Wire Wire Line
	3000 7850 3250 7850
Wire Wire Line
	3250 7950 2950 7950
Wire Wire Line
	3250 8050 2950 8050
Text Label 2950 7950 0    50   ~ 0
TxD
Text Label 2950 8050 0    50   ~ 0
RxD
Wire Wire Line
	4050 7650 4050 7550
Wire Wire Line
	5650 7550 5650 7750
Wire Wire Line
	5650 7950 5400 7950
Wire Wire Line
	5400 7950 5400 7850
Wire Wire Line
	5400 7850 5050 7850
Wire Wire Line
	5050 8050 5650 8050
Wire Wire Line
	4050 7550 5650 7550
Text Notes 3350 7050 0    50   ~ 0
Interne USB Schnittstelle (optional)
$Comp
L Device:R R4
U 1 1 605EA748
P 2900 8950
F 0 "R4" V 2693 8950 50  0000 C CNN
F 1 "560" V 2784 8950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2830 8950 50  0001 C CNN
F 3 "~" H 2900 8950 50  0001 C CNN
	1    2900 8950
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 605EAC8F
P 2900 9050
F 0 "R5" V 3000 9050 50  0000 C CNN
F 1 "560" V 3100 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2830 9050 50  0001 C CNN
F 3 "~" H 2900 9050 50  0001 C CNN
	1    2900 9050
	0    1    1    0   
$EndComp
Wire Wire Line
	3050 8950 3250 8950
Wire Wire Line
	3250 9050 3050 9050
$Comp
L Device:C C9
U 1 1 605F0FED
P 4250 7350
F 0 "C9" H 4350 7200 50  0000 L CNN
F 1 "1uF" H 4365 7305 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W6.0mm_P5.00mm" H 4288 7200 50  0001 C CNN
F 3 "~" H 4250 7350 50  0001 C CNN
	1    4250 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 7650 4250 7500
$Comp
L power:GND #PWR025
U 1 1 605F93F8
P 4600 7350
F 0 "#PWR025" H 4600 7100 50  0001 C CNN
F 1 "GND" H 4605 7177 50  0000 C CNN
F 2 "" H 4600 7350 50  0001 C CNN
F 3 "" H 4600 7350 50  0001 C CNN
	1    4600 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 7200 4250 7100
Wire Wire Line
	4250 7100 4600 7100
Wire Wire Line
	4600 7100 4600 7350
$Comp
L Device:LED D9
U 1 1 605FCD25
P 2500 8750
F 0 "D9" V 2539 8632 50  0000 R CNN
F 1 "LED" V 2448 8632 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm_FlatTop" H 2500 8750 50  0001 C CNN
F 3 "~" H 2500 8750 50  0001 C CNN
	1    2500 8750
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D8
U 1 1 605FDD50
P 2050 8750
F 0 "D8" V 2089 8632 50  0000 R CNN
F 1 "LED" V 1998 8632 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm_FlatTop" H 2050 8750 50  0001 C CNN
F 3 "~" H 2050 8750 50  0001 C CNN
	1    2050 8750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 8950 2500 8950
Wire Wire Line
	2500 8950 2500 8900
Wire Wire Line
	2050 8900 2050 9050
Wire Wire Line
	2050 9050 2750 9050
$Comp
L power:+5V #PWR022
U 1 1 606046A7
P 2050 8400
F 0 "#PWR022" H 2050 8250 50  0001 C CNN
F 1 "+5V" H 2065 8573 50  0000 C CNN
F 2 "" H 2050 8400 50  0001 C CNN
F 3 "" H 2050 8400 50  0001 C CNN
	1    2050 8400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR024
U 1 1 60604A4F
P 2500 8400
F 0 "#PWR024" H 2500 8250 50  0001 C CNN
F 1 "+5V" H 2515 8573 50  0000 C CNN
F 2 "" H 2500 8400 50  0001 C CNN
F 3 "" H 2500 8400 50  0001 C CNN
	1    2500 8400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 8400 2050 8600
Wire Wire Line
	2500 8600 2500 8400
Text Notes 2100 8650 0    50   ~ 0
TxD
Text Notes 2550 8650 0    50   ~ 0
RxD
NoConn ~ 3250 8150
NoConn ~ 3250 8250
NoConn ~ 3250 8350
NoConn ~ 3250 8450
NoConn ~ 3250 8550
NoConn ~ 3250 8650
NoConn ~ 3250 8750
NoConn ~ 3250 8850
$Comp
L Device:C C11
U 1 1 606291B1
P 3750 7350
F 0 "C11" H 3850 7200 50  0000 L CNN
F 1 "1uF" H 3865 7305 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W6.0mm_P5.00mm" H 3788 7200 50  0001 C CNN
F 3 "~" H 3750 7350 50  0001 C CNN
	1    3750 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 7550 4050 7100
Wire Wire Line
	4050 7100 3750 7100
Wire Wire Line
	3750 7100 3750 7200
Connection ~ 4050 7550
$Comp
L power:GND #PWR0105
U 1 1 6062CD04
P 3750 7500
F 0 "#PWR0105" H 3750 7250 50  0001 C CNN
F 1 "GND" H 3755 7327 50  0000 C CNN
F 2 "" H 3750 7500 50  0001 C CNN
F 3 "" H 3750 7500 50  0001 C CNN
	1    3750 7500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 6062D3AA
P 5950 8550
F 0 "#PWR0106" H 5950 8300 50  0001 C CNN
F 1 "GND" H 5955 8377 50  0000 C CNN
F 2 "" H 5950 8550 50  0001 C CNN
F 3 "" H 5950 8550 50  0001 C CNN
	1    5950 8550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 8550 5950 8450
Wire Wire Line
	6050 8350 6050 8450
Wire Wire Line
	6050 8450 5950 8450
Connection ~ 5950 8450
Wire Wire Line
	5950 8450 5950 8350
$Comp
L power:GND #PWR0107
U 1 1 606383E7
P 6400 4850
F 0 "#PWR0107" H 6400 4600 50  0001 C CNN
F 1 "GND" H 6405 4677 50  0000 C CNN
F 2 "" H 6400 4850 50  0001 C CNN
F 3 "" H 6400 4850 50  0001 C CNN
	1    6400 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4650 6450 4650
Wire Wire Line
	6400 4650 6400 4850
Wire Wire Line
	5450 3700 5950 3700
Wire Wire Line
	5950 3700 5950 4250
Text Notes 6500 3850 0    50   ~ 0
DCF77 Eingang
Wire Wire Line
	5950 4250 6450 4250
$Comp
L Device:R R6
U 1 1 60677F60
P 7400 4350
F 0 "R6" V 7193 4350 50  0000 C CNN
F 1 "1k" V 7284 4350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7330 4350 50  0001 C CNN
F 3 "~" H 7400 4350 50  0001 C CNN
	1    7400 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 4350 7250 4350
Wire Wire Line
	7550 4350 8150 4350
Wire Wire Line
	7050 4550 7700 4550
Wire Wire Line
	7700 4550 7700 4450
Wire Wire Line
	7700 4450 8150 4450
Text Label 7800 4350 0    50   ~ 0
Data-
Text Label 7800 4450 0    50   ~ 0
Data+
$Comp
L power:+5V #PWR0108
U 1 1 60687C39
P 7050 5100
F 0 "#PWR0108" H 7050 4950 50  0001 C CNN
F 1 "+5V" H 7065 5273 50  0000 C CNN
F 2 "" H 7050 5100 50  0001 C CNN
F 3 "" H 7050 5100 50  0001 C CNN
	1    7050 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:L L2
U 1 1 60688A4D
P 7350 5100
F 0 "L2" V 7540 5100 50  0000 C CNN
F 1 "1mH" V 7449 5100 50  0000 C CNN
F 2 "Inductor_THT:L_Radial_D8.7mm_P5.00mm_Fastron_07HCP" H 7350 5100 50  0001 C CNN
F 3 "~" H 7350 5100 50  0001 C CNN
	1    7350 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C12
U 1 1 60689F01
P 7650 5250
F 0 "C12" H 7750 5100 50  0000 L CNN
F 1 "1uF" H 7765 5205 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W6.0mm_P5.00mm" H 7688 5100 50  0001 C CNN
F 3 "~" H 7650 5250 50  0001 C CNN
	1    7650 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C13
U 1 1 6068A3E2
P 8000 5250
F 0 "C13" H 8118 5296 50  0000 L CNN
F 1 "47uF / 10V" H 8118 5205 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D4.5mm_P2.50mm" H 8038 5100 50  0001 C CNN
F 3 "~" H 8000 5250 50  0001 C CNN
	1    8000 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 5100 7200 5100
Wire Wire Line
	7500 5100 7650 5100
Wire Wire Line
	8000 5100 7650 5100
Connection ~ 7650 5100
Wire Wire Line
	8150 4550 8000 4550
Wire Wire Line
	8000 4550 8000 5100
Connection ~ 8000 5100
$Comp
L power:GND #PWR0109
U 1 1 606AD920
P 8000 5500
F 0 "#PWR0109" H 8000 5250 50  0001 C CNN
F 1 "GND" H 8005 5327 50  0000 C CNN
F 2 "" H 8000 5500 50  0001 C CNN
F 3 "" H 8000 5500 50  0001 C CNN
	1    8000 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 606ADDF4
P 7650 5500
F 0 "#PWR0110" H 7650 5250 50  0001 C CNN
F 1 "GND" H 7655 5327 50  0000 C CNN
F 2 "" H 7650 5500 50  0001 C CNN
F 3 "" H 7650 5500 50  0001 C CNN
	1    7650 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 5500 7650 5400
Wire Wire Line
	8000 5400 8000 5500
$Comp
L power:GND #PWR0111
U 1 1 606B8761
P 8150 4800
F 0 "#PWR0111" H 8150 4550 50  0001 C CNN
F 1 "GND" H 8155 4627 50  0000 C CNN
F 2 "" H 8150 4800 50  0001 C CNN
F 3 "" H 8150 4800 50  0001 C CNN
	1    8150 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 4800 8150 4650
Text Label 8000 4550 0    50   ~ 0
5V-DCF
$Comp
L Device:CP C10
U 1 1 606BF594
P 3650 4200
F 0 "C10" H 3768 4246 50  0000 L CNN
F 1 "4700uF / 10V" V 3500 4100 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P7.50mm" H 3688 4050 50  0001 C CNN
F 3 "~" H 3650 4200 50  0001 C CNN
	1    3650 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3700 3650 3700
$Comp
L power:GND #PWR0112
U 1 1 606C5131
P 3650 4450
F 0 "#PWR0112" H 3650 4200 50  0001 C CNN
F 1 "GND" H 3655 4277 50  0000 C CNN
F 2 "" H 3650 4450 50  0001 C CNN
F 3 "" H 3650 4450 50  0001 C CNN
	1    3650 4450
	1    0    0    -1  
$EndComp
Text Notes 3950 4150 1    50   ~ 0
Optional
Wire Wire Line
	5450 3800 5750 3800
Text Label 5600 3800 0    50   ~ 0
Spannungsausfallerkennung
$Comp
L power:+5V #PWR0113
U 1 1 606D2F32
P 11750 2700
F 0 "#PWR0113" H 11750 2550 50  0001 C CNN
F 1 "+5V" H 11765 2873 50  0000 C CNN
F 2 "" H 11750 2700 50  0001 C CNN
F 3 "" H 11750 2700 50  0001 C CNN
	1    11750 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11750 2700 11750 3000
$Comp
L power:+24V #PWR0114
U 1 1 606D8FBA
P 11850 2550
F 0 "#PWR0114" H 11850 2400 50  0001 C CNN
F 1 "+24V" H 11865 2723 50  0000 C CNN
F 2 "" H 11850 2550 50  0001 C CNN
F 3 "" H 11850 2550 50  0001 C CNN
	1    11850 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	11850 2550 11850 3000
Wire Wire Line
	5450 4100 5750 4100
Wire Wire Line
	5450 4200 5750 4200
Text Label 5600 4100 0    50   ~ 0
Ausgang_A
Text Label 5600 4200 0    50   ~ 0
Ausgang_B
Text Label 9900 3200 0    50   ~ 0
Ausgang_A
$Comp
L power:GND #PWR0115
U 1 1 606F8AD6
P 11750 4700
F 0 "#PWR0115" H 11750 4450 50  0001 C CNN
F 1 "GND" H 11755 4527 50  0000 C CNN
F 2 "" H 11750 4700 50  0001 C CNN
F 3 "" H 11750 4700 50  0001 C CNN
	1    11750 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11750 4400 11750 4550
Wire Wire Line
	11550 4400 11550 4550
Wire Wire Line
	11550 4550 11750 4550
Connection ~ 11750 4550
Wire Wire Line
	11750 4550 11750 4700
Wire Wire Line
	11450 4400 11450 4550
Wire Wire Line
	11450 4550 11550 4550
Connection ~ 11550 4550
$Comp
L power:GND #PWR0116
U 1 1 607127B1
P 10900 3950
F 0 "#PWR0116" H 10900 3700 50  0001 C CNN
F 1 "GND" H 10905 3777 50  0000 C CNN
F 2 "" H 10900 3950 50  0001 C CNN
F 3 "" H 10900 3950 50  0001 C CNN
	1    10900 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	11150 3600 10900 3600
Wire Wire Line
	10900 3600 10900 3700
Wire Wire Line
	11150 3700 10900 3700
Connection ~ 10900 3700
Wire Wire Line
	10900 3700 10900 3800
Wire Wire Line
	11150 3800 10900 3800
Connection ~ 10900 3800
Wire Wire Line
	10900 3800 10900 3950
NoConn ~ 12350 3800
NoConn ~ 12350 3900
Wire Wire Line
	12350 3500 12600 3500
Wire Wire Line
	12350 3600 12500 3600
Wire Wire Line
	12500 3600 12500 3800
Wire Wire Line
	12500 3800 12850 3800
Wire Wire Line
	13400 3500 13650 3500
Wire Wire Line
	13650 3500 13650 3600
Connection ~ 13400 3500
Wire Wire Line
	13650 3700 13650 3800
Wire Wire Line
	13650 3800 13400 3800
Connection ~ 13400 3800
Wire Wire Line
	12600 3450 12600 3500
Connection ~ 12600 3500
Wire Wire Line
	12600 3500 13400 3500
Wire Wire Line
	12600 3500 12600 3950
Wire Wire Line
	12850 3450 12850 3800
Connection ~ 12850 3800
Wire Wire Line
	12850 3800 13400 3800
Wire Wire Line
	12850 3800 12850 3950
$Comp
L power:GND #PWR0117
U 1 1 60792F67
P 12600 4450
F 0 "#PWR0117" H 12600 4200 50  0001 C CNN
F 1 "GND" H 12605 4277 50  0000 C CNN
F 2 "" H 12600 4450 50  0001 C CNN
F 3 "" H 12600 4450 50  0001 C CNN
	1    12600 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 6079341D
P 12850 4450
F 0 "#PWR0118" H 12850 4200 50  0001 C CNN
F 1 "GND" H 12855 4277 50  0000 C CNN
F 2 "" H 12850 4450 50  0001 C CNN
F 3 "" H 12850 4450 50  0001 C CNN
	1    12850 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR0119
U 1 1 60793638
P 12850 2950
F 0 "#PWR0119" H 12850 2800 50  0001 C CNN
F 1 "+24V" H 12865 3123 50  0000 C CNN
F 2 "" H 12850 2950 50  0001 C CNN
F 3 "" H 12850 2950 50  0001 C CNN
	1    12850 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+24V #PWR0120
U 1 1 60793962
P 12600 2950
F 0 "#PWR0120" H 12600 2800 50  0001 C CNN
F 1 "+24V" H 12615 3123 50  0000 C CNN
F 2 "" H 12600 2950 50  0001 C CNN
F 3 "" H 12600 2950 50  0001 C CNN
	1    12600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	12600 2950 12600 3150
Wire Wire Line
	12850 3150 12850 2950
Wire Wire Line
	12850 4250 12850 4450
Wire Wire Line
	12600 4250 12600 4450
Wire Wire Line
	5400 4000 5450 4000
Connection ~ 5450 4000
Wire Wire Line
	5450 4000 5750 4000
Text Label 5600 4000 0    50   ~ 0
1Hz
NoConn ~ 5450 3300
NoConn ~ 5450 3900
NoConn ~ 4250 1800
NoConn ~ 5450 2100
NoConn ~ 5450 2200
NoConn ~ 5450 2300
NoConn ~ 5450 1800
NoConn ~ 5450 1900
NoConn ~ 6450 4350
NoConn ~ 6450 4550
$Comp
L power:GND #PWR0121
U 1 1 6080A05E
P 4850 4800
F 0 "#PWR0121" H 4850 4550 50  0001 C CNN
F 1 "GND" H 4855 4627 50  0000 C CNN
F 2 "" H 4850 4800 50  0001 C CNN
F 3 "" H 4850 4800 50  0001 C CNN
	1    4850 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4500 4850 4800
$Comp
L Diode:1N4148 D10
U 1 1 60812D54
P 10300 3400
F 0 "D10" H 10600 3500 50  0000 C CNN
F 1 "1N4148" H 10300 3500 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 10300 3225 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 10300 3400 50  0001 C CNN
	1    10300 3400
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148 D11
U 1 1 608146D3
P 10300 3600
F 0 "D11" H 10600 3750 50  0000 C CNN
F 1 "1N4148" H 10300 3750 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 10300 3425 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 10300 3600 50  0001 C CNN
	1    10300 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 60814DCE
P 10550 3750
F 0 "R7" H 10480 3704 50  0000 R CNN
F 1 "4.7k" H 10480 3795 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 10480 3750 50  0001 C CNN
F 3 "~" H 10550 3750 50  0001 C CNN
	1    10550 3750
	-1   0    0    1   
$EndComp
Wire Wire Line
	10450 3400 10550 3400
Wire Wire Line
	10450 3600 10550 3600
Wire Wire Line
	10550 3400 10550 3600
Connection ~ 10550 3400
Wire Wire Line
	10550 3400 11150 3400
Connection ~ 10550 3600
Wire Wire Line
	9900 3200 10050 3200
Wire Wire Line
	9900 3300 9950 3300
Text Label 9900 3300 0    50   ~ 0
Ausgang_B
Wire Wire Line
	10150 3400 10050 3400
Wire Wire Line
	10050 3400 10050 3200
Connection ~ 10050 3200
Wire Wire Line
	10050 3200 11150 3200
Wire Wire Line
	10150 3600 9950 3600
Wire Wire Line
	9950 3600 9950 3300
Connection ~ 9950 3300
Wire Wire Line
	9950 3300 11150 3300
$Comp
L power:GND #PWR0122
U 1 1 6085BBF0
P 10550 3950
F 0 "#PWR0122" H 10550 3700 50  0001 C CNN
F 1 "GND" H 10555 3777 50  0000 C CNN
F 2 "" H 10550 3950 50  0001 C CNN
F 3 "" H 10550 3950 50  0001 C CNN
	1    10550 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 3900 10550 3950
$Comp
L Connector_Generic:Conn_02x01 J7
U 1 1 608664B0
P 5750 4750
F 0 "J7" V 5754 4830 50  0000 L CNN
F 1 "1Hz" V 5845 4830 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5750 4750 50  0001 C CNN
F 3 "~" H 5750 4750 50  0001 C CNN
	1    5750 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	5750 4550 5750 4450
Text Label 5750 4550 0    50   ~ 0
1Hz
$Comp
L power:GND #PWR0123
U 1 1 60871780
P 5750 5100
F 0 "#PWR0123" H 5750 4850 50  0001 C CNN
F 1 "GND" H 5755 4927 50  0000 C CNN
F 2 "" H 5750 5100 50  0001 C CNN
F 3 "" H 5750 5100 50  0001 C CNN
	1    5750 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 5100 5750 5050
Text Notes 13250 3950 0    50   ~ 0
Optional Supressor Diode
$Comp
L Connector_Generic:Conn_02x01 J8
U 1 1 6057B334
P 8050 1550
F 0 "J8" V 8054 1630 50  0000 L CNN
F 1 "LED Display" V 8145 1630 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8050 1550 50  0001 C CNN
F 3 "~" H 8050 1550 50  0001 C CNN
	1    8050 1550
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR026
U 1 1 6057BF93
P 8050 1250
F 0 "#PWR026" H 8050 1100 50  0001 C CNN
F 1 "+5V" H 8065 1423 50  0000 C CNN
F 2 "" H 8050 1250 50  0001 C CNN
F 3 "" H 8050 1250 50  0001 C CNN
	1    8050 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 6057C3CC
P 8050 1950
F 0 "#PWR027" H 8050 1700 50  0001 C CNN
F 1 "GND" H 8055 1777 50  0000 C CNN
F 2 "" H 8050 1950 50  0001 C CNN
F 3 "" H 8050 1950 50  0001 C CNN
	1    8050 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1250 8050 1350
Wire Wire Line
	8050 1950 8050 1850
Wire Wire Line
	2850 3700 2850 3900
Wire Wire Line
	3200 3700 3200 4050
Wire Wire Line
	3200 4350 3200 4450
Connection ~ 2850 3900
Wire Wire Line
	2850 3900 2850 4050
Wire Wire Line
	2500 3900 2550 3900
Connection ~ 2500 3900
Wire Wire Line
	2200 3700 2850 3700
Connection ~ 2850 3700
Wire Wire Line
	3650 4050 3650 3700
Wire Wire Line
	3650 4350 3650 4450
$EndSCHEMATC
