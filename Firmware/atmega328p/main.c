/*
* Author: Michael Albert
* Für externen 8Mhz Quarz
Änderungen:
V1.01	: 09.08.2010 michael@albert-hetzles.de Setzen der Ausgänge in den Interrupt gelegt
V1.02.b1: 10.08.2010 michael@albert-hetzles.de Wird kein LCD Display verwendet wird der Status über LED's ausgegeben.
V1.02.b2: 11.08.2010 michael@albert-hetzles.de Sekunden seit dem letzten DCF77 Komplettempfang werden mitgezählt
V1.02.b3: 17.08.2010 michael@albert-hetzles.de Pullup Widerstand DCF77 Input ein.
                                               Bugs bei LED Ausgabe beseitigt
V1.02.b4  20.08.2010 michael@albert-hetzles.de DCF77: Bug beim Überlauf des Timer1 beseitig. Für einen gesetzten Interrupt (TOIE1) muß auch die jeweilige Routine (SIG_OVERFLOW1) definiert sein.
												ansonsten restartet der controller
V1.02.b5  20.08.2010 michael@albert-hetzles.de Für die Konvertierung von iDCF77LastReceivedDataPaketInSeconds itoa -> ltoa
V1.02.b6  22.08.2010 michael@albert-hetzles.de Bug im Endanschlag (0xffff-> 0xffffffff) beim hochzählen von iDCF77LastReceivedDataPaketInSeconds behoben
V1.02.rc1 22.08.2010 michael@albert-hetzles.de Reihenfolge der LED Ausgänge nochmal angepasst
V1.02	  26.08.2010 michael@albert-hetzles.de V1.02 = V1.02.rc1, mit WinAVR 20100110 kompiliert
V1.03	  26.08.2010 michael@albert-hetzles.de Bug behoben beimn LED enzeigen  && ! bLCDDisplayOn hinzugefügt
V1.04	  23.11.2010 michael@albert-hetzles.de Bug beim Kommando nst behoben, syncuhr wurde weitergezählt
											   Neue fReadUART Funktion mit STRG-C	
V1.05	  06.12.2010 michael@albert-hetzles.de Bug im  Protokoll gd7 behoben		
V1.06b2   25.10.2011 michael@albert-hetzles.de Trotz bDCF77=false wurde syncronisiert		
          20.12.2012 michael@albert-hetzles.de Peridodenzeit der Impulse beim synchronieren änderbar, default 1sec	
V1.06b4	  24.02.2013 michael@albert-hetzles.de Bug!!! Inkonsitenz Schaltplan und Programm OUTPUT_PULSE_EVEN PB0 -> PD6	   
V1.07	  06.03.2013 michael@albert-hetzles.de 1.07=1.06b4	
V1.08b1   01.03.2014 michael@albert-hetzles.de Bug in fInitLEDIOPorts Zeile 1573 	PORTD -> PORTC geändert
V1.08     22.06.2014 michael@albert-hetzles.de V1.08b1 -> V1.08
V1.09b1   01.08.2014 michael@albert-hetzles.de Portierung auf atmega328p - uart lib, ISR Routinen
V1.09b2   05.09.2014 michael@albert-hetzles.de Max. Pulsedauer auf 8000ms erhöht, Pulse Delay wird dynamisch anhand der Pulseweite ermittelt DelayInSec=PulseweiteIn100MS*2/10+1
V1.09b3   05.09.2014 michael@albert-hetzles.de Pulse Delay wird dynamisch anhand der Pulseweite ermittelt DelayInSec=(PulseweiteIn100MS+1)/10+1, Bug interval wurde immer noch auf SYNC_SECONDS_TO_WAIT_TO_NEXT_MINUTE
                                               Interval wird beim setzen des spw kommandos mitausgeben
V1.09     17.09.2014 michael@albert-hetzles.de V1.09b3 -> V1.09WW											   
V2.00b1   22.10.2018 michael@albert-hetzles.de Umstellung WinAVR -> Linux avr-gcc. Anpassung an avr-gcc 4.9.2 (Debian 9 stretch). Benötigte pakete apt-get install gcc-avr avr-libc
V2.00b2   13.09.2019 michael@albert-hetzles.de Debug funktion für DCF77
*/
/* TODO INTERRUPT */
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lcd.h>
#include "uart.h"
#include "terminal.h"

// Language
// #define LANG_EN
#define LANG_DE
// Version
char sFirmwareVersion[17]="V2.00b2";


// Toogles a LED/Bit at Port and pin
#define TOGGLE_PIN(_port,_pin){((_port) & (1<<(_pin)))?((_port)&=~(1<<(_pin))):((_port)|=(1<<(_pin)));}
// Set LED at Port and pin = VOltage to High 5V
#define SET_LED(_port,_pin){((_port)|=(1<<(_pin)));}
// Clear LED on at Port and pin = VOltage to Low 0V  
#define CLEAR_LED(_port,_pin){((_port)&=~((1<<(_pin))));}
// Timer 1 STOP
#define TIMER0_STOP {TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));TIMSK0 &= ~(1<<TOIE0);TCNT0=0;}
#define TIMER0_START_DIV8 {TCNT0=0;TCCR0B |= (1<<CS00);TIMSK0 |= (1<<TOIE0);}

// Serial BUAD Rate
#define BAUD_RATE 9600
// Pulseweite Client Uhren
#define DEFAULT_PULSE_WIDTH_IN100MS 4
// Min Pulsweite
#define MIN_PULSE_WIDTH_IN100MS 2
// Max Pulsweite
#define MAX_PULSE_WIDTH_IN100MS 80
// Ausgang ungerade Minute
#define OUTPUT_PULSE_ODD PD7
// Ausgang gerade Minute
#define OUTPUT_PULSE_EVEN PD6
// Ausgang gerade Minute an
#define SET_PULSE_ODD {PORTD|=(1<<OUTPUT_PULSE_ODD);}
// Ausgang ungerade Minute an
#define SET_PULSE_EVEN {PORTD|=(1<<OUTPUT_PULSE_EVEN);}
// Ausgang ungerade Minute aus
#define RESET_PULSE_ODD {PORTD&=~(1<<OUTPUT_PULSE_ODD);}
// Ausgang gerade Minute aus
#define RESET_PULSE_EVEN {PORTD&=~(1<<OUTPUT_PULSE_EVEN);}

// Eingang für die Netzspannungsausfallerkennung
#define POWER_SUPPLY_ACTIVE_PIN PD3
// Macro zur Abfrage Netzspannungsausfallerkennung
#define POWER_SUPPLY_ACTIVE ((PIND) & (1<<POWER_SUPPLY_ACTIVE_PIN))
// Eingang für die LCD Display Abschaltung
#define LCD_DISPLAY_ACTIVE_PIN PD4
// Macro zur Abfrage ob LCD Display an
#define LCD_DISPLAY_ACTIVE ((PIND) & (1<<LCD_DISPLAY_ACTIVE_PIN))


// 1HZ Blink Ausgang
#define PIN_1HZ_BLINK PD5

// Ausgänge für LED's definieren
// Ausgang wenn DCF77 Daten empfangen werden
// Blinken=warte auf Daten, leuchten=Empfange Daten, aus=DCF77 Off
// Uhr syncronisiert
#define PIN_CLOCK_IS_SYNCING PC5
// DCF77 Health Status Leuchtet=DCF77 letzter DCF77 Empfang <1h; blinkt=DCF77 letzter DCF77 Empfang <24h;aus DCF77 letzter DCF77 Empfang >24h 
#define PIN_LED_DCF77_HEALTH_STATE PC4
// Gültiges DCF77 Paket empfangen
#define PIN_LED_DCF77_RETURN_STATE_OK PC3
// Ungültiges DCF77 Paket empfangen
#define PIN_LED_DCF77_RETURN_STATE_ERROR PC2
// Empfangsstatus blink=warte auf Daten, leuchtet=Empfange Daten
#define PIN_LED_DCF77_CURRENT_STATUS PC1




// For bool variables
#define TRUE 1
#define FALSE 0
// UART, ECHO zurückschreiben
#define UART_ECHO 1

// Begin Konstanten für DCF77
#define TIMER1_STOP {TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));TIMSK1 &= ~(1<<TOIE1);}
#define TIMER1_START_DIV1024 {TCNT1=0;TCCR1B |= (1<<CS12) | (1<<CS10);TIMSK1 |= (1<<TOIE1);}
/* Toleranz Ranges */
// Counter läuft mit 8Mhz / 1024 = 7812/Counter increments/sekunde
// DCF77 Starterkennung letzte Sekunde low pegel 1,7-1,8 Sekunden je ob 58Bit high oder low (ca 14452 +/- 600) low
// 14853 bei Parität Datum=1 14062 bei Parität Datum=0 (Bit 58), Mittel also 14452
// Startpausecycle = 14452, Toleranz +/- 600
#define DCF_START_CYCLE_MIN 13852
#define DCF_START_CYCLE_MAX 15052
// DCF77 Highpegel, ca 0,2sec Highpegel(1562 Toleranz+/-250) 
// Highcycle = 1562, TOleranz +/- 250
#define DCF_SIGNAL_HIGH_CYCLE_Min 1312
#define DCF_SIGNAL_HIGH_CYCLE_MAX 1812
// DCF77 Highpegel, ca 0,1sec Highpegel(781 Toleranz+/-150) 
// Highcycle = 781, TOleranz +/- 150
#define DCF_SIGNAL_LOW_CYCLE_MIN 631
#define DCF_SIGNAL_LOW_CYCLE_MAX 931
// Überprüfung ob High und low Signalpegel ca 7812 ergeben Toleranz 400
// Pulse+Pause +/- Toleranzen von High und low
// 7812 +/- (250+150)
#define DCF_PULSE_AND_PAUSE_CYCLE_SUM_MIN 7412
#define DCF_PULSE_AND_PAUSE_CYCLE_SUM_MAX 8212
// Zeit für die die DCF Daten gültig sind
#define DCF77_IS_VALID_FOR_SECONDS 59
// EIngangspin für das DCF77 Signal, ISR0, PD2
#define DCF77_INPUT_PIN PD2
// Pegel für DCF In
#define HIGH 1
#define LOW 0
// End Konstanten für DCF77
#define DCF_DEBUG FALSE
// Wie oft die Displayseite wechseln
#define CHANGE_DISPLAY_PAGE_IN_SEC 5

// Max Input chars für die Konsole
#define MAX_INPUT_CHARACTERS 33

// Wieviel Sekunden soll beim synchronisieren gewartet werden bis der Minutenzeiger weiter gestellt wird
#define SYNC_SECONDS_TO_WAIT_TO_NEXT_MINUTE 10
// Berechnet den Abstand Pulse
// #define CALC_PULSE_INTERVAL_LENGTH_IN_SEC(_PULSEWIDTHIN100MS)((_PULSEWIDTHIN100MS*2/10)+1)
#define CALC_PULSE_INTERVAL_LENGTH_IN_SEC(_PULSEWIDTHIN100MS)((_PULSEWIDTHIN100MS+1)/10+1)
// Datatype für Uhren(Clientzeit, Synczeit), bReady4Sync wird nur für die synczeit gebraucht
// um anzuzeigen das ein sync durchzuführen ist. 
typedef struct{
	uint8_t iHour;
	uint8_t iMinute;
	uint8_t iSecond;
	uint8_t bReady4Sync;
	uint8_t bIncTime;
} TTime;
// Begin Strukturen für DCF77
// DCF77 Datatype, iIsValidForSeconds gibt die Zahl der Sekunden an für die die DCF Zeit gültig ist
// wird jede Sekunde um 1 verringert bis 0. 
// Wird nach erfolgreichem Empfang auf DCF77_IS_VALID_FOR_SECONDS 
// gesetzt.
// iDCF77LastResult= der letzte Return Code, return codes sind in eDCF77ReturnCodes definiert
// iDCF77Status= Der aktuelle Status, definiert in eDCF77StatusCodes
typedef struct{
	uint8_t iYear;
	uint8_t iMonth;
	uint8_t iDay;
	uint8_t iWeekday;
	uint8_t iHour;
	uint8_t iMinute;
	uint8_t iSecond;
	uint8_t bMEZ;
	uint8_t iIsValidForSeconds;
	uint8_t iDCF77LastResult;
	uint8_t iDCF77Status;
	uint32_t iDCF77LastReceivedDataPaketInSeconds;
} TDCF77DateTime;

enum eDCF77StatusCodes{
	DCF77_INIT, DCF77_SYNCING, DCF77_WAIT_FOR_SYNCING
};
enum eDCF77ReturnCodes{
	DCF77_RECEIVE_COMPLETE_SUCCESSFULLY, DCF77_PARITY_ERROR_HOUR, DCF77_PARITY_ERROR_MINUTE, DCF77_PARITY_ERROR_DATE, DCF77_CYCLE_ERROR, DCF77_PULSE_RANGE_ERROR, DCF77_NO_STATUS
};
#if DCF_DEBUG
// DCF77 Pulse Debug 
typedef struct{
	uint16_t  iPulseWidth;
	uint16_t  iPauseWidth;
	uint8_t DCF77LastResult;
	uint8_t DCF77Status;
} T_DCF77Debug;

// DCF77 Debug
volatile T_DCF77Debug tDCF77Debug  = {.iPulseWidth=0, .iPauseWidth=0, .DCF77LastResult=DCF77_NO_STATUS, .DCF77Status=DCF77_INIT};
// DCF77 Debug. Ausgabe von Puls und Pauseweite auf Display
bool bDCF77Debug=false;
#endif


// EEPROM Speicher für Uhrzeit
uint8_t ee_iHourClientTime EEMEM = 0;
// An 0xFF wird beim auslesen erkannt das noch keine Sicherung der Uhr stattgefunden hat
uint8_t	ee_iMinuteClientTime EEMEM = 0xFF;
uint8_t ee_iPowerFaultHour EEMEM = 0;
uint8_t ee_iPowerFaultMinute EEMEM = 0xFF;
uint8_t ee_iPowerFaultDay EEMEM = 0xFF;
uint8_t ee_iPowerFaultMonth EEMEM = 0;
uint8_t ee_iPowerFaultYear EEMEM = 0;
uint8_t ee_iPowerFaultWeekday EEMEM = 0;
uint8_t ee_bMEZ EEMEM = TRUE;
uint8_t ee_bDCF77 EEMEM = FALSE;
uint8_t ee_bSyncMode24h EEMEM = FALSE;
uint8_t ee_bClientClockPaused EEMEM = FALSE;
// Pulsweite der Ausgänge
uint8_t ee_iPulsWidthIn100ms EEMEM = 4;

// String constant programspace, Data space reicht dafür nicht :-(
// German
#if defined LANG_DE
// Version
char sClockName[17] = "RA Mutteruhr";
char sPrompt[6]="mu:> ";
const char prgsInitHelp[] PROGMEM ="\r\nHilfe mit \x1b[37;1m?\x1b[0;37m oder \x1b[37;1mhelp\x1b[0;37m und mit return anschliessen.\r\n";
const char prgsHelp[] PROGMEM ="\
 \x1b[37;1mVerfuegbare Kommandos:\r\n\
 \x1b[37;1mdcf \x1b[0;37m-> Synchronisation mit DCF77 ja|nein.\r\n\
 \x1b[37;1msmt \x1b[0;37m-> Mutteruhr auf Zeit von Nebenuhren stellen.\r\n\
 \x1b[37;1mnst \x1b[0;37m-> Nebenuhren auf exakt diese Zeit stellen(Nicht im DCF77 Modus).\r\n\
 \x1b[37;1mast \x1b[0;37m-> Nebenuhren auf diese Zeit stellen. Zeit die dafuer benötigt wird, wird hinzuaddiert (Nicht im DCF77 Modus).\r\n\
 \x1b[37;1mpst \x1b[0;37m-> Mutter- und Nebenuhren anhalten.(Synchronisation funktioniert weiter).\r\n\
 \x1b[37;1mssm \x1b[0;37m-> Syncmodus 12h/24h setzen.\r\n\
 \x1b[37;1mspw \x1b[0;37m-> Ausgangsimpulsweite aendern(200-8000ms).\r\n\
 \x1b[37;1mstat \x1b[0;37m-> Gesamtstatus anzeigen.\r\n";
const char prgsFirm[] PROGMEM = "\x1b[37;1mFirmware\x1b[21C: \x1b[0;37m";
const char prgsTimeOnClients[] PROGMEM = "\x1b[37;1mZeit auf Nebenuhren\x1b[10C: \x1b[0;37m";
const char prgsMode[] PROGMEM = "\r\n\x1b[37;1mStatus\x1b[23C: \x1b[0;37m";
const char prgsLastPowerF[] PROGMEM = "\x1b[37;1mLetzter Spannungsausfall\x1b[5C: \x1b[0;37m";
const char prgsLastPowerFClientTime[] PROGMEM = "\x1b[37;1mZeit auf Nebenuhren bei\r\nletztem Spannungsausfall\x1b[5C: \x1b[0;37m";
const char prgsDCF77[] PROGMEM = "\r\n\x1b[37;1mDCF77\x1b[24C: \x1b[0;37m";
const char prgsDCF77LastRec[] PROGMEM = "aktiviert\r\n\x1b[37;1mLetztes DCF77 Empfangsdatum\x1b[2C: \x1b[0;37m";
const char prgsDCF77LastRes[] PROGMEM = "\r\n\x1b[37;1mDCF77 Letztes Ergebnis\x1b[7C: \x1b[0;37m";
const char prgsDCF77LastStat[] PROGMEM = "\r\n\x1b[37;1mDCF77 Status\x1b[17C: \x1b[0;37m";
const char prgsDCF77LastData[] PROGMEM = "\r\n\x1b[37;1mDCF77 letzter Komplettempfang: \x1b[0;37m";
const char prgsSyncMode[] PROGMEM = "\r\n\x1b[37;1mSynchronisationsmodus\x1b[8C: \x1b[0;37m";
const char prgsLCDDisplayIs[] PROGMEM = "\x1b[37;1mLCD Display ist\x1b[14C: \x1b[0;37m";
const char prgsOutputPulseWidth[] PROGMEM = "\x1b[37;1mAusgangsimpulsweite\x1b[10C: \x1b[0;37m";
const char prgsOutputPulseInterval[] PROGMEM = "\x1b[37;1mAusgangsimpulsinterval\x1b[7C: \x1b[0;37m";
const char prgsSCTQuest[] PROGMEM = "Zeit auf den Nebenuhren (xx:xx): ";
const char prgsNCTQuest[] PROGMEM = "Stelle Nebenuhren exakt auf (xx:xx): ";
const char prgsACTQuest[] PROGMEM = "Stelle Nebenuhren auf (xx:xx): ";
const char prgsACTNCTErrorMsg[] PROGMEM = "Bitte DCF77 zuerst deaktivieren!\r\n";
const char prgsPCTStat[] PROGMEM = "Nebenuhr(en) ";
const char prgsPCTQuestPaused[] PROGMEM =  "laufen. Anhalten ? (j|n): ";
const char prgsPCTQuestContinue[] PROGMEM = "sind angehalten. Starten ? (j|n): ";
const char prgsDCF77NoRec[] PROGMEM = "Noch keine DCF77 Daten empfangen.";
const char prgsDCF77Head[] PROGMEM = "DCF Sychnronistaion ist ";
const char prgsDCF77QuestOn[] PROGMEM = "an. Ausschalten? (j|n): ";
const char prgsDCF77QuestOff[] PROGMEM = "aus. Einschalten? (j|n): ";
const char prgsSetDCF77On[] PROGMEM = "DCF77 Synchronisation ein.\r\n";
const char prgsSetDCF77Off[] PROGMEM = "DCF77 Synchronisation aus.\r\n";
const char prgsSyncMode24hHead[] PROGMEM = "Syncmodus ist ";
const char prgsSyncMode24hQuest12[] PROGMEM = "24 Stunden. Auf 12 Stunden setzen? (j|n): ";
const char prgsSyncMode24hQuest24[] PROGMEM = "12 Stunden. Auf 24 Stunden setzen? (j|n): ";
const char prgsSyncMode24h[] PROGMEM = "24 Stunden";
const char prgsSyncMode12h[] PROGMEM = "12 Stunden";
const char prgsSetSyncMode24hOn[] PROGMEM = "24h Stunden Modus aktiviert.\r\n";
const char prgsSetSyncMode24hOff[] PROGMEM = "12h Stunden Modus aktiviert.\r\n";
const char prgsUnknownCommand[] PROGMEM = "Kommando existiert nicht.\r\n";
const char prgsInvalidTime[] PROGMEM = "Ungueltiges Zeitformat\r\n";
const char prgsSetContinue[] PROGMEM = "Uhr gestartet.\r\n";
const char prgsSetHalted[] PROGMEM = "Uhr angehalten.\r\n";
const char prgsPrintok[] PROGMEM = "ok\r\n";
const char prgsReturnOK[] PROGMEM = "OK\r\n";
const char prgsReturnERROR[] PROGMEM = "ERROR\r\n";
const char prgsCurrentBit[] PROGMEM = ", gerade empfangenes Bit: ";
const char prgsMinutesLeft[] PROGMEM = " Minute(n) zu synchronisieren";
const char prgsSyncingTo[] PROGMEM = "synchronisiere Nebenuhren auf ";
const char prgsWrongInput[] PROGMEM = "Falsche Eingabe.\r\n";
const char prgsHalted[] PROGMEM = "->pause";
const char prgsLCDOn[] PROGMEM = "Aktiv";
const char prgsLCDOff[] PROGMEM = "Aus";
const char prgsPulsWidthQuest[] PROGMEM = "Ausgangsimpulsweite aendern (in 100ms, moegliche Werte: 2|3|4|....|80): ";
const char prgsPulsWidthSetOutPre[] PROGMEM = "Ausgangsimpulsweite wurde auf     :";
const char prgsPulsWidthSetOutPost[] PROGMEM = "00ms gesetzt.\r\n";
const char prgsPulsIntervalSetOutPre[] PROGMEM = "Ausgangsimpulseinterval wurde auf: ";
const char prgsPulsIntervalSetOutPost[] PROGMEM = "s gesetzt.\r\n";
const char prgsSeconds[] PROGMEM = " Sekunden";
// LCD Display Ausgaben:
const char sTimeOnSlaves[14] = "Nebenuhrzeit:";
const char sSyncingClocks[17] = "Synchronisiere..";
const char sDCF77mode[17]="DCF77 Syncmodus:";
const char sDCF77State[15]="DCF77 Status: ";
const char sDCF77StateON[6] = "Aktiv";
const char sMinutesLeft[10] =" Minuten.";
const char sDCF77StateOff[12]="deaktiviert";
const char sDCF77LastResult[16] = "DCF Auswertung:";
const char sDCF77SyncStatus[16] = "DCF77 Empfange:";
const char sSyncronisationMode[12] ="Sync Modus:";
const char sPowerSupplyDownLine1[16]="Stromversorgung";
const char sPowerSupplyDownLine2[13]="ausgefallen!";
// Weekdays long
const char prgsSundayLong[] PROGMEM = "Sonntag";
const char prgsMondayLong[] PROGMEM = "Montag";
const char prgsTuesdayLong[] PROGMEM = "Dienstag";
const char prgsWednesdayLong[] PROGMEM = "Mittwoch";
const char prgsThursdayLong[] PROGMEM = "Donnerstag";
const char prgsFridayLong[] PROGMEM = "Freitag";
const char prgsSaturdayLong[] PROGMEM = "Samstag";
// Weekdays short
const char prgsSundayShort[] PROGMEM = "So";
const char prgsMondayShort[] PROGMEM = "Mo";
const char prgsTuesdayShort[] PROGMEM = "Di";
const char prgsWednesdayShort[] PROGMEM = "Mi";
const char prgsThursdayShort[] PROGMEM = "Do";
const char prgsFridayShort[] PROGMEM = "Fr";
const char prgsSaturdayShort[] PROGMEM = "Sa";
// DCF77 Errormessages
const char prgsDCF77ErrMsgOK[] PROGMEM ="ok";
const char prgsDCF77ErrMsgParityHour[] PROGMEM ="ERR-Par. Stunde";
const char prgsDCF77ErrMsgParityMinute[] PROGMEM ="ERR-Par. Minute";
const char prgsDCF77ErrMsgParityDate[] PROGMEM ="ERR-Par. Datum";
const char prgsDCF77ErrMsgCycleTime[] PROGMEM ="ERR-DauerPeriode";
const char prgsDCF77ErrMsgPulse[] PROGMEM ="ERR-Pulsdauer";
const char prgsDCF77ErrMsgNoState[] PROGMEM ="Kein Status";
// DCF77 StatusStrings
const char prgsDCF77StateInit[] PROGMEM = "Initialsiere";
const char prgsDCF77StateSyncing[] PROGMEM = "Empfange Daten";
const char prgsDCF77StateWait4Sync[] PROGMEM = "Warte auf Daten";
const char prgsDCF77StateUnknown[] PROGMEM = "Unbekannt";
// Input keys
char sKeyYes[2]="j";
char sKeyNo[2]="n";
#else
// English Version
char sClockName[17] = "RA Motherclock";
char sPrompt[6]="mc:> ";
char prgsInitHelp[] PROGMEM ="\r\nEnter \x1b[37;1m?\x1b[0;37m or \x1b[37;1mhelp\x1b[0;37m and hit return for help\r\n";
char prgsHelp[] PROGMEM ="\
 \x1b[37;1mAvaliable commands:\r\n\
 \x1b[37;1mdcf \x1b[0;37m-> Switch DCF77 syncmode on|off.\r\n\
 \x1b[37;1msmt \x1b[0;37m-> Sets the master clock to time on slaves.\r\n\
 \x1b[37;1mnst \x1b[0;37m-> Forwards the slaves to exact this time(Not in DCF77 mode).\r\n\
 \x1b[37;1mast \x1b[0;37m-> Forwards the slaves to this time. Time elapsed for the task will be added(Not in DCF77 mode).\r\n\
 \x1b[37;1mpst \x1b[0;37m-> Switch slave clock pause mode on|off.(sync for dcf77, nct and act are still working).\r\n\
 \x1b[37;1mssm \x1b[0;37m-> Set 12h/24h Syncmode.\r\n\
 \x1b[37;1mspw \x1b[0;37m-> Change output pulsewidth(200-8000ms).\r\n\
 \x1b[37;1mstat \x1b[0;37m-> Show the current state of the clock.\r\n";
const char prgsFirm[] PROGMEM = "\x1b[37;1mFirmware\x1b[21C: \x1b[0;37m";
const char prgsTimeOnClients[] PROGMEM = "\x1b[37;1mTime on slaves\x1b[15C: \x1b[0;37m";
const char prgsMode[] PROGMEM = "\r\n\x1b[37;1mMode\x1b[25C: \x1b[0;37m";
const char prgsDCF77LastRec[] PROGMEM = "on\r\n\x1b[37;1mDCF77 Last received date\x1b[5C: \x1b[0;37m";
const char prgsDCF77LastRes[] PROGMEM = "\r\n\x1b[37;1mDCF77 Last result\x1b[12C: \x1b[0;37m";
const char prgsDCF77LastStat[] PROGMEM = "\r\n\x1b[37;1mDCF77 State\x1b[18C: \x1b[0;37m";
const char prgsDCF77LastData[] PROGMEM = "\r\n\x1b[37;1mDCF77 last complete receive\x1b[2C: \x1b[0;37m";
const char prgsDCF77[] PROGMEM = "\r\n\x1b[37;1mDCF77\x1b[24C: \x1b[0;37m";
const char prgsLastPowerF[] PROGMEM = "\x1b[37;1mLast Powerfault\x1b[14C: \x1b[0;37m";
const char prgsLastPowerFClientTime[] PROGMEM = "\x1b[37;1mSlavetime at last powerfault : \x1b[0;37m";
const char prgsSyncMode[] PROGMEM = "\r\n\x1b[37;1mSyncmode\x1b[21C: \x1b[0;37m";
const char prgsLCDDisplayIs[] PROGMEM = "\x1b[37;1mLCD Display is\x1b[15C: \x1b[0;37m";
const char prgsOutputPulseWidth[] PROGMEM = "\x1b[37;1mOutput pulse width\x1b[11C: \x1b[0;37m";
const char prgsOutputPulseInterval[] PROGMEM = "\x1b[37;1mOutput pulse interval\x1b[8C: \x1b[0;37m";
const char prgsSCTQuest[] PROGMEM = "Enter current time on slaves (xx:xx): ";
const char prgsNCTQuest[] PROGMEM = "Set slaves exact to (xx:xx): ";
const char prgsACTQuest[] PROGMEM = "Adjust slaves to (xx:xx): ";
const char prgsACTNCTErrorMsg[] PROGMEM = "Disable DCF77 first!\r\n";
const char prgsPCTStat[] PROGMEM = "Slave clocks is currently ";
const char prgsPCTQuestPaused[] PROGMEM =  "running. Set halt? (y|n): ";
const char prgsPCTQuestContinue[] PROGMEM = "halted. Continue? (y|n): ";
const char prgsDCF77NoRec[] PROGMEM = "No DCF77 data received yet.";
const char prgsDCF77Head[] PROGMEM = "DCF is ";
const char prgsDCF77QuestOn[] PROGMEM = "on. Switch off? (y|n): ";
const char prgsDCF77QuestOff[] PROGMEM = "off. Switch on? (y|n): ";
const char prgsSetDCF77On[] PROGMEM = "Set dcf77 on.\r\n";
const char prgsSetDCF77Off[] PROGMEM = "Set dcf77 off.\r\n";
const char prgsSyncMode24hHead[] PROGMEM = "Syncmode is ";
const char prgsSyncMode24hQuest12[] PROGMEM = "24h. Set to 12h? (y|n): ";
const char prgsSyncMode24hQuest24[] PROGMEM = "12h. Set to 24h? (y|n): ";
const char prgsSyncMode24h[] PROGMEM = "24 Hours";
const char prgsSyncMode12h[] PROGMEM = "12 Hours";
const char prgsSetSyncMode24hOn[] PROGMEM = "24h syncmode enabled.\r\n";
const char prgsSetSyncMode24hOff[] PROGMEM = "12h syncmode enabled.\r\n";
const char prgsUnknownCommand[] PROGMEM = "unknown command\r\n";
const char prgsInvalidTime[] PROGMEM = "Invalid time\r\n";
const char prgsSetContinue[] PROGMEM = "Set continue.\r\n";
const char prgsSetHalted[] PROGMEM = "Set halted.\r\n";
const char prgsPrintok[] PROGMEM = "ok\r\n";
const char prgsReturnOK[] PROGMEM = "OK\r\n";
const char prgsReturnERROR[] PROGMEM = "ERROR\r\n";
const char prgsCurrentBit[] PROGMEM = ", current bit: ";
const char prgsMinutesLeft[] PROGMEM = " minutes left";
const char prgsSyncingTo[] PROGMEM = "syncing clients to ";
const char prgsWrongInput[] PROGMEM = "Wrong imput.\r\n";
const char prgsHalted[] PROGMEM = "->halted";
const char prgsLCDOn[] PROGMEM = "On";
const char prgsLCDOff[] PROGMEM = "Off";
const char prgsPulsWidthQuest[] PROGMEM = "Change outputpulsewidth(in 100ms, possible values: 2|3|4|....|80) to: ";
const char prgsPulsWidthSetOutPre[] PROGMEM = "Output pulse width is set to   : ";
const char prgsPulsWidthSetOutPost[] PROGMEM = "00ms\r\n";
const char prgsPulsIntervalSetOutPre[] PROGMEM = "Output pulse interval is set to: ";
const char prgsPulsIntervalSetOutPost[] PROGMEM = "s\r\n";
const char prgsSeconds[] PROGMEM = " seconds";
// LCD Display Ausgaben:
const char sTimeOnSlaves[16] = "Time on slaves:";
const char sSyncingClocks[17] = "Syncing clock...";
const char sDCF77mode[17]="DCF77 Syncmode: ";
const char sDCF77State[14]="DCF77 State: ";
const char sDCF77StateON[3] = "On";
const char sMinutesLeft[11] =" min left.";
const char sDCF77StateOff[4]="Off";
const char sDCF77LastResult[17] = "DCF last result:";
const char sDCF77SyncStatus[15] = "DCF receiving:";
const char sSyncronisationMode[11] ="Sync mode:";
const char sPowerSupplyDownLine1[13]="POWER SUPPLY";
const char sPowerSupplyDownLine2[5]="DOWN";
// Weekdays long
const char prgsSundayLong[] PROGMEM = "Sunday";
const char prgsMondayLong[] PROGMEM = "Monday";
const char prgsTuesdayLong[] PROGMEM = "Thuesday";
const char prgsWednesdayLong[] PROGMEM = "Wednesday";
const char prgsThursdayLong[] PROGMEM = "Thursday";
const char prgsFridayLong[] PROGMEM = "Friday";
const char prgsSaturdayLong[] PROGMEM = "Saturday";
// Weekdays short
const char prgsSundayShort[] PROGMEM = "Sun";
const char prgsMondayShort[] PROGMEM = "Mon";
const char prgsTuesdayShort[] PROGMEM = "Tue";
const char prgsWednesdayShort[] PROGMEM = "Wed";
const char prgsThursdayShort[] PROGMEM = "Thu";
const char prgsFridayShort[] PROGMEM = "Fri";
const char prgsSaturdayShort[] PROGMEM = "Sat";
// DCF77 Errormessages
const char prgsDCF77ErrMsgOK[] PROGMEM ="ok";
const char prgsDCF77ErrMsgParityHour[] PROGMEM ="ERR-Parity hour";
const char prgsDCF77ErrMsgParityMinute[] PROGMEM ="ERR-Parity min";
const char prgsDCF77ErrMsgParityDate[] PROGMEM ="ERR-Parity date";
const char prgsDCF77ErrMsgCycleTime[] PROGMEM ="ERR-Cycletime";
const char prgsDCF77ErrMsgPulse[] PROGMEM ="ERR-Pulsetime";
const char prgsDCF77ErrMsgNoState[] PROGMEM ="No state";
// DCF77 StatusStrings
const char prgsDCF77StateInit[] PROGMEM = "init";
const char prgsDCF77StateSyncing[] PROGMEM = "receiving data";
const char prgsDCF77StateWait4Sync[] PROGMEM = "wait 4 data";
const char prgsDCF77StateUnknown[] PROGMEM = "unknown";
// Input keys
char sKeyYes[2]="y";
char sKeyNo[2]="n";
#endif

/*
char prgsSyncMode24hHead[] PROGMEM = "Syncmodus ist ";
char prgsSyncMode24hQuest12[] PROGMEM = "24h. Auf 12h setzen? (y|n): ";
char prgsSyncMode24hQuest24[] PROGMEM = "12h. Auf 24h setzen? (y|n): ";
char prgsSetSyncMode24hOn[] PROGMEM = "24h Stunden Modus aktiviert.\r\n";
char prgsSetSyncMode24hOff[] PROGMEM = "12h Stunden Modus aktiviert.\r\n";
*/
//char *aWeekDays[]={"Sunday,Monday,Thuesday,Wednesday,Thirsday,Friday,Saturday"};
// End Strukturen für DCF77

// functions prototypes
static void fExecuteEverySecond(void);
static void fExecuteEvery10telSecond(void);
static void fUpdateTimeString(char *psTime,TTime *ptClientTime);
static void fInitIoports(void);
static void fInitLEDIOPorts(void);
static void fInitCounter(void);
int fReadUART(char *psInput,uint8_t iMaxInCharacters);
static int fConvertInput2Time(char *psIn,TTime *ptTime);
static int fTimeDiffFor12hScope(TTime *ttIsTime, TTime *ttNewTime);
static int fTimeDiffFor24hScope(TTime *ttIsTime, TTime *ttNewTime);
static void fIncClientTime(TTime *ttClientTime,uint8_t bIncMinutes);
// DCF Funktionen
static void fInitDCF77(void);
static void fDisableDCF77(void);
static uint8_t fParityOdd(uint8_t aBitField[], uint8_t iStartField, uint8_t iStopField);
static void fUpdateDCF77DateTimeString(char *psDCF77DateTime,TDCF77DateTime *ptDCF77DateInfo);
static void fGetLastDCF77ResultString(uint8_t iRes,char *psErrMsg);
static void fGetLastDCF77StatusString(uint8_t iRes,char *psStatMsg);
static void fGetShortDayString(uint8_t iDayNumber,char *psShortDay);
static void fUInt8To2CharStr(uint8_t iNum, char *s2CharStr);
static void fLCDPutStringCenter(char *sOut,uint8_t iLineNumber);
/* static void fCopyStringCenter(char *sString, char *sOut, uint8_t iMaxStrLength); */
static void fGetCenterAndFilledUpString(char *sIn, char *sOut,uint8_t iLength);
static uint8_t fSplitCommandFromParameter(char *psInput, char* psCommand, char* psParameter);
static uint8_t fCheckAndConvertPulsWidth(char *sIn,uint8_t *iOut);
/* static void fPrintIntToUART(char *sIn,int16_t iIn); */ 
// end functions prototypes

TTime stClientTime={0,0,0,FALSE,TRUE}, tDisplayLastPowerFault={0,0,0,FALSE,FALSE} ,tDisplayClientTimeLastPowerFault={0,0,0,FALSE,FALSE};
TTime stNewTimeForClients={0,0,0,FALSE,FALSE};

char sTime[17]="--:--:--";
char sDCFDateTime[34]="---------- --.--.---- --:-- ----";
char sInput[16]="";
char sCommand[6]="";
char sParameter[10];
// Serieller Prompt

/* Position im Terminalmenu
0=Commandprompt
1=Eingabe der Zeit auf den Client/Nebenuhren
2=Eingabe neue Zeit für Clients
.....
*/
uint8_t iShellPosition=0;
/* DisplayPage
0=Client time
...
*/
enum eShellMenuPosiotion{ 
	SHELL_MENU_PROMPT=0
};
uint8_t iDisplayPage=0;

// DisplayUpdate, wird in der function fExecuteEvery10telSecond gesetzt
volatile uint8_t bUpdateDisplay=FALSE;
uint8_t iDisplayPageSecCount=0;
// Temp Variable für Displayausgabe
char sTemp[LCD_DISP_LENGTH+1]="";
char sOutputLine[LCD_DISP_LENGTH+1]="";
char sDisplayLine1[LCD_DISP_LENGTH+1]="";
char sDisplayLine2[LCD_DISP_LENGTH+1]="";
uint8_t iTemp=0;
// Steuerzeichen für die PC Kopplung
char sPCControlCharEndBlock[2] = ";";
char sPCControlCharEndParameter[2] = ",";
// Zeitdifferenz zwischen Client und Synctime, global wg. Displayausgabe
int iTimeDiff=0;

// Werden bei Pulse auf iPulsWidthIn100ms gesetzt und dann wieder herunter gezählt
volatile uint8_t iPulseWidthEven=0;
volatile uint8_t iPulseWidthOdd=0;
// Pulsweite
uint8_t iPulsWidthIn100ms=4;
// Länge eines Pulsintervall
uint8_t iPulsIntervalLengthInSec=1;

// DCF77 Zeit
TDCF77DateTime tDCF77DateTime={0,0,0,0,0,0,0,TRUE,0,DCF77_NO_STATUS,DCF77_INIT,0xffffffff}, tDisplayLastPowerFaultFull={0,0,0,0,0,0,0,TRUE,0,DCF77_NO_STATUS,DCF77_INIT,0xffffffff};
//tDCF77DateTime = {0,0,0,0,0,0,0,0,DCF77_NO_STATUS,DCF77_INIT};
// Bitfeld für die DCF Daten, Global da für Displayausgabe
static uint8_t aDCFData[60];
// Zeiger auf das aktuell zu enpfangende Bit
static uint8_t iDCF77BitPointer=0;

// Wird ein DCF77 Empfänger benutzt?
bool bDCF77 = false;
// 24h Syncmodus?
bool bSyncMode24h = false;
// LCD Display On?
bool bLCDDisplayOn = true;


int main(void){
	while(! eeprom_is_ready());
	// Gespeicherte Clientzeit aus eeprom lesen
	stClientTime.iMinute=eeprom_read_byte(&ee_iMinuteClientTime);
	if(stClientTime.iMinute>=0 && stClientTime.iMinute<=59){
		stClientTime.iHour=eeprom_read_byte(&ee_iHourClientTime);
		if(!(stClientTime.iHour>=0 && stClientTime.iHour<=23)){
			stClientTime.iHour=0;
			stClientTime.iMinute=0;
		}
	}
	else{
		stClientTime.iHour=0;
		stClientTime.iMinute=0;
	}
	// Pause für Clients
	if(eeprom_read_byte(&ee_bClientClockPaused)){
		stClientTime.bIncTime=FALSE;
	}
	else{
		stClientTime.bIncTime=TRUE;
	}
	bDCF77=eeprom_read_byte(&ee_bDCF77);
	bSyncMode24h=eeprom_read_byte(&ee_bSyncMode24h);
	iPulsWidthIn100ms=eeprom_read_byte(&ee_iPulsWidthIn100ms);
	if(!(iPulsWidthIn100ms>=MIN_PULSE_WIDTH_IN100MS && iPulsWidthIn100ms<=MAX_PULSE_WIDTH_IN100MS)){
		iPulsWidthIn100ms=DEFAULT_PULSE_WIDTH_IN100MS;
	}
	iPulsIntervalLengthInSec=CALC_PULSE_INTERVAL_LENGTH_IN_SEC(iPulsWidthIn100ms);
	//Interrupts On
	sei();
	uart_init(UART_BAUD_SELECT(BAUD_RATE,F_CPU));
	uart_puts(TERM_RESET);
	uart_puts(TERM_FG_WHITE_BRIGHT);
	uart_puts("\x1b[2J");
	uart_puts(sClockName);
	uart_puts(" ");
	uart_puts(sFirmwareVersion);
	uart_puts("\r\n");
	uart_puts(TERM_RESET);
	// 1. Prompt ausgeben
	uart_puts(sPrompt);
	// IO port init
	fInitIoports();
	// Couter init
	fInitCounter();
	// End Init
	if(bDCF77){fInitDCF77();};
	// Display
	if(LCD_DISPLAY_ACTIVE){
		// Display init	
		lcd_init(LCD_DISP_ON);
		lcd_gotoxy(0,0);
		lcd_clrscr();
		bLCDDisplayOn=true;
		fLCDPutStringCenter(sClockName,0);
		//lcd_puts("Race Masterclock");
		//lcd_gotoxy(0,1);
		//lcd_puts(sFirmwareVersion);	
		fLCDPutStringCenter(sFirmwareVersion,1);
	}
	else{
		bLCDDisplayOn=false;
		fInitLEDIOPorts();
	}
	for(;;){
		if(POWER_SUPPLY_ACTIVE){
		//if(TRUE){
			/******** Begin Display Ausgabe ********/
			/* Begin Strings für die AUsgabe erzeugen */
				if (iDisplayPage==0){
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
					fUpdateTimeString(sTime,&stClientTime);
					//lcd_puts("TIME ON CLIENTS:");
					// fLCDPutStringCenter(sTimeOnSlaves,0);
					fGetCenterAndFilledUpString((char*)sTimeOnSlaves,sDisplayLine1,sizeof(sDisplayLine1)-1);
					//lcd_gotoxy(0,1);
					//lcd_puts(sTime);
					//fLCDPutStringCenter(sTime,1);
					fGetCenterAndFilledUpString(sTime,sDisplayLine2,sizeof(sDisplayLine2)-1);
					//fGetCenterAndFilledUpString()
				}
				if(iDisplayPage==1){
					if(stNewTimeForClients.bReady4Sync){
						//lcd_clrscr();
						//lcd_gotoxy(0,0);
						//lcd_puts("Syncing clock...");
						//fLCDPutStringCenter(sSyncingClocks,0);
						fGetCenterAndFilledUpString((char*)sSyncingClocks,sDisplayLine1,sizeof(sDisplayLine1)-1);
						//lcd_gotoxy(0,1);
						itoa(iTimeDiff,sOutputLine,10);
						//lcd_puts(sOutputLine);
						//lcd_puts(" min left.");
						strcat(sOutputLine,sMinutesLeft);
						// fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
					else{
						iDisplayPage++;
					}
				}
				if(iDisplayPage==2){
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
					//lcd_puts("DCF77: ");
					// fLCDPutStringCenter(sDCF77mode,0);
					fGetCenterAndFilledUpString((char*)sDCF77mode,sDisplayLine1,sizeof(sDisplayLine1)-1);
					if(bDCF77){
						//lcd_gotoxy(0,1);
						fGetCenterAndFilledUpString((char*)sDCF77StateON,sDisplayLine2,sizeof(sDisplayLine2)-1);
						//strcpy(sOutputLine,sDCF77StateON);
						//lcd_puts(sOutputLine);
						//fLCDPutStringCenter(sOutputLine,1);
					}
					else{
						//lcd_gotoxy(0,1);
						//lcd_puts("OFF");
						//fLCDPutStringCenter(sDCF77StateOff,1);
						fGetCenterAndFilledUpString((char*)sDCF77StateOff,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
				}	
				if(iDisplayPage==3){
					//lcd_clrscr();
					//lcd_gotoxy(0,0);
					//lcd_puts("DCF77 STATUS: ");
					//fLCDPutStringCenter(sDCF77State,0);
					fGetCenterAndFilledUpString((char*)sDCF77State,sDisplayLine1,sizeof(sDisplayLine1)-1);
					if(bDCF77){
						fGetLastDCF77StatusString(tDCF77DateTime.iDCF77Status,sTemp);
						//strcpy(sOutputLine,sTemp);
						//lcd_puts(sOutputLine);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sTemp,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
					else{
						iDisplayPage++;
					}
				}				
				if(iDisplayPage==4){
					if(bDCF77){
						//lcd_clrscr();
						//lcd_gotoxy(0,0);
						//lcd_puts("DCF LAST RESULT:");
						//fLCDPutStringCenter(sDCF77LastResult,0);
						fGetCenterAndFilledUpString((char*)sDCF77LastResult,sDisplayLine1,sizeof(sDisplayLine1)-1);
						fGetLastDCF77ResultString(tDCF77DateTime.iDCF77LastResult,sOutputLine);
						//lcd_gotoxy(0,1);
						//lcd_puts(sOutputLine);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}			
					else{
						iDisplayPage++;
					}
				}
				if(iDisplayPage==5){
					if(bDCF77 && tDCF77DateTime.iDCF77Status==DCF77_SYNCING && iDCF77BitPointer>=1 && iDCF77BitPointer<=59){
						//lcd_clrscr();
						//fLCDPutStringCenter(sDCF77SyncStatus,0);
						fGetCenterAndFilledUpString((char*)sDCF77SyncStatus,sDisplayLine1,sizeof(sDisplayLine1)-1);
						strcpy(sOutputLine,"BIT:");
						fUInt8To2CharStr(iDCF77BitPointer-1,sTemp);
						strcat(sOutputLine,sTemp);
						strcat(sOutputLine,"=");
						if(aDCFData[iDCF77BitPointer-1]==HIGH){
							//strcat(sOutputLine,"HIGH");
							strcat(sOutputLine,"1");
						}
						else{
							//strcat(sOutputLine,"LOW");
							strcat(sOutputLine,"0");
						}
						//fGetLastDCF77ResultString(tDCF77DateTime.iDCF77LastResult,sOutputLine);
						//lcd_gotoxy(0,1);
						//lcd_puts(sOutputLine);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}			
					else{
						iDisplayPage++;
					}
				}
				if(iDisplayPage==6){
					if(bDCF77 && tDCF77DateTime.iIsValidForSeconds>0){
						//lcd_clrscr();
						strcpy(sOutputLine,"DCF77:");
						fUInt8To2CharStr(tDCF77DateTime.iHour,sTemp);
						strcat(sOutputLine,sTemp);
						strcat(sOutputLine,":");
						fUInt8To2CharStr(tDCF77DateTime.iMinute,sTemp);
						strcat(sOutputLine,sTemp);
						strcat(sOutputLine," ");
						if(tDCF77DateTime.bMEZ){
							strcat(sOutputLine,"MEZ");
						}
						else{
							strcat(sOutputLine,"MESZ");
						}
						//fLCDPutStringCenter(sOutputLine,0);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine1,sizeof(sDisplayLine1)-1);
						fGetShortDayString(tDCF77DateTime.iWeekday,sOutputLine);
						strcat(sOutputLine," - ");
						fUInt8To2CharStr(tDCF77DateTime.iDay,sTemp);
						strcat(sOutputLine,sTemp);
						strcat(sOutputLine,".");
						fUInt8To2CharStr(tDCF77DateTime.iMonth,sTemp);
						strcat(sOutputLine,sTemp);
						strcat(sOutputLine,".");
						fUInt8To2CharStr(tDCF77DateTime.iYear,sTemp);
						strcat(sOutputLine,sTemp);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
					else{
						iDisplayPage++;
					}
				}	
				if(iDisplayPage==7){	
					//lcd_clrscr();
					//fLCDPutStringCenter(sSyncronisationMode,0);
					fGetCenterAndFilledUpString((char*)sSyncronisationMode,sDisplayLine1,sizeof(sDisplayLine1)-1);
					if(bSyncMode24h){
						strcpy_P(sOutputLine,prgsSyncMode24h);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
					else{
						strcpy_P(sOutputLine,prgsSyncMode12h);
						//fLCDPutStringCenter(sOutputLine,1);
						fGetCenterAndFilledUpString(sOutputLine,sDisplayLine2,sizeof(sDisplayLine2)-1);
					}
					
				}
				if(iDisplayPage>=8){
					iDisplayPage=0;
				}
			/* EndStrings für die AUsgabe erzeugen */
			// Strings auf Display ausgeben
			if(bUpdateDisplay && bLCDDisplayOn){
				lcd_gotoxy(0,0);
				lcd_puts(sDisplayLine1);
				lcd_gotoxy(0,1);
				lcd_puts(sDisplayLine2);
				bUpdateDisplay=FALSE;
			}
			// Kein LCD Display -> LED Ausgabe
			else if(bUpdateDisplay && ! bLCDDisplayOn){
				// LED's Updaten
				if(bDCF77){
					if(tDCF77DateTime.iDCF77Status==DCF77_INIT){
						CLEAR_LED(PORTC,PIN_LED_DCF77_CURRENT_STATUS);
					}
					else if(tDCF77DateTime.iDCF77Status==DCF77_SYNCING){
						SET_LED(PORTC,PIN_LED_DCF77_CURRENT_STATUS);
					}
					else if(tDCF77DateTime.iDCF77Status==DCF77_WAIT_FOR_SYNCING){
						TOGGLE_PIN(PORTC,PIN_LED_DCF77_CURRENT_STATUS);
					}
					else{
						CLEAR_LED(PORTC,PIN_LED_DCF77_CURRENT_STATUS);
					}
					if( (tDCF77DateTime.iDCF77LastResult==DCF77_PARITY_ERROR_HOUR)||
						(tDCF77DateTime.iDCF77LastResult==DCF77_PARITY_ERROR_MINUTE)||
						(tDCF77DateTime.iDCF77LastResult==DCF77_CYCLE_ERROR)||
						(tDCF77DateTime.iDCF77LastResult==DCF77_PULSE_RANGE_ERROR)||
						(tDCF77DateTime.iDCF77LastResult==DCF77_PARITY_ERROR_HOUR)){
						SET_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_ERROR);
						CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_OK);
					}
					else if((tDCF77DateTime.iDCF77LastResult==DCF77_RECEIVE_COMPLETE_SUCCESSFULLY)&&(tDCF77DateTime.iIsValidForSeconds>0)){
					//else if(tDCF77DateTime.iIsValidForSeconds>0){
						CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_ERROR);
						SET_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_OK);
					}
					else{
						CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_OK);
						CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_ERROR);
					}
					if(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds>86400){
						CLEAR_LED(PORTC,PIN_LED_DCF77_HEALTH_STATE);
					} 
					else if((tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds<=86400)&&(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds>3600)){
						TOGGLE_PIN(PORTC,PIN_LED_DCF77_HEALTH_STATE);
					}
					else{
						SET_LED(PORTC,PIN_LED_DCF77_HEALTH_STATE);
					}
				}
				else{
					CLEAR_LED(PORTC,PIN_LED_DCF77_CURRENT_STATUS);
					CLEAR_LED(PORTC,PIN_LED_DCF77_HEALTH_STATE);
					CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_OK);
					CLEAR_LED(PORTC,PIN_LED_DCF77_RETURN_STATE_ERROR);
				}
				if(stNewTimeForClients.bReady4Sync==TRUE){
					SET_LED(PORTC,PIN_CLOCK_IS_SYNCING);
					//
				}
				else{
					CLEAR_LED(PORTC,PIN_CLOCK_IS_SYNCING);
				}
				bUpdateDisplay=FALSE;
			}
			/******** End Display Ausgabe ********/

			/*************** Begin Serielles Userinterface ****************/
			/*************** Begin Shell ****************/
			if(fReadUART(sInput,sizeof(sInput))){
				if(strcmp(sInput,"\3")==0){
					iShellPosition=SHELL_MENU_PROMPT;
					uart_puts("STRG-C\n\r");
					sInput[0]='\0';
				}
				// 0= Kommandoeingabe, Hauptmenu
				else if(fSplitCommandFromParameter(sInput,sCommand,sParameter)){
					//Direct Command and parameter
					if (strcmp(sCommand,"_smt")==0){
						if (fConvertInput2Time(sParameter,&stClientTime)){
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}
					}
					else if (strcmp(sCommand,"_ast")==0){
						if (fConvertInput2Time(sParameter,&stNewTimeForClients)){
							stNewTimeForClients.bReady4Sync=TRUE;
							stNewTimeForClients.bIncTime=TRUE;
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}					
					}
					else if (strcmp(sCommand,"_nst")==0){
						if (fConvertInput2Time(sParameter,&stNewTimeForClients)){
							stNewTimeForClients.bIncTime=FALSE;
							stNewTimeForClients.bReady4Sync=TRUE;
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}					
					}					
					else if (strcmp(sCommand,"_dcf")==0){
						if (strcmp(sParameter,"1")==0){
							bDCF77=true;
							eeprom_write_byte(&ee_bDCF77,TRUE);
							fInitDCF77();
							uart_puts_p(prgsReturnOK);
						}
						else if (strcmp(sParameter,"0")==0){
							bDCF77=false;
							eeprom_write_byte(&ee_bDCF77,FALSE);
							fDisableDCF77();
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}
						
					}				
					else if (strcmp(sCommand,"_ssm")==0){
						if (strcmp(sParameter,"1")==0){
							bSyncMode24h=true;
							eeprom_write_byte(&ee_bSyncMode24h,TRUE);
							uart_puts_p(prgsReturnOK);
						}
						else if (strcmp(sParameter,"0")==0){
							bSyncMode24h=false;
							eeprom_write_byte(&ee_bSyncMode24h,FALSE);
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}
						
					}	
					else if (strcmp(sCommand,"_spw")==0){
						if(fCheckAndConvertPulsWidth(sParameter,&iTemp)){
							iPulsWidthIn100ms=iTemp;
							iPulsIntervalLengthInSec=CALC_PULSE_INTERVAL_LENGTH_IN_SEC(iPulsWidthIn100ms);
							eeprom_write_byte(&ee_iPulsWidthIn100ms,iTemp);
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}
					}
					else if (strcmp(sCommand,"_pst")==0){
						if (strcmp(sParameter,"1")==0){
							stClientTime.bIncTime=FALSE;
							eeprom_write_byte(&ee_bClientClockPaused,TRUE);
							uart_puts_p(prgsReturnOK);
						}
						else if (strcmp(sParameter,"0")==0){
							stClientTime.bIncTime=TRUE;
							eeprom_write_byte(&ee_bClientClockPaused,FALSE);
							uart_puts_p(prgsReturnOK);
						}
						else{
							uart_puts_p(prgsReturnERROR);
						}
						
					}
					#if DCF_DEBUG
					else if (strcmp(sInput,"_dbgdcf")==0){
						if(bDCF77Debug==true && bDCF77==true)
						{
							bDCF77Debug=false;
							uart_puts_p(prgsReturnOK);
						}
						else if(bDCF77==true)
						{
						    bDCF77Debug=true;	
						    uart_puts_p(prgsReturnOK);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}	
					#endif										
					else if (strcmp(sCommand,"_stat")==0){
						// Get Version
						if (strcmp(sParameter,"gv")==0){
						// Block 0 Clockname,Firmwareversion
							strcpy(sOutputLine,sClockName);
							uart_puts(sOutputLine);
							strcpy(sOutputLine,sPCControlCharEndParameter);
							strcat(sOutputLine,sFirmwareVersion);
							strncat(sOutputLine,sPCControlCharEndBlock,2);
							uart_puts(sOutputLine);
							uart_puts_p(prgsReturnOK);
						}
						// Get Slavetime
						else if (strcmp(sParameter,"gst")==0){
						// Block 1 SlaveTimes
							// Hour, Minute, Second
							itoa(stClientTime.iHour,sTemp,10);
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stClientTime.iMinute,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stClientTime.iSecond,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);
							uart_puts(sOutputLine);
							uart_puts_p(prgsReturnOK);
						}
						// Get New Slavetime
						else if (strcmp(sParameter,"gnt")==0){
						// Block 2 New Clienttimes
							// Hour, Minute, Second
							itoa(stNewTimeForClients.iHour,sTemp,10);
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stNewTimeForClients.iMinute,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stNewTimeForClients.iSecond,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);
							uart_puts(sOutputLine);
							uart_puts_p(prgsReturnOK);
						}
						// Get DCF77 Time
						else if (strcmp(sParameter,"gd7")==0){
						// Block 3 DCF77
							// Year, Month, day, weekday, Hour, Minute, Second, MEZ, Valid4Seconds, LastResult, Status, LetzterKomplett Empfang in Sekunden 
							itoa(tDCF77DateTime.iYear,sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iMonth,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							itoa(tDCF77DateTime.iDay,sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iWeekday,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iHour,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							itoa(tDCF77DateTime.iMinute,sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iSecond,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.bMEZ,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							itoa(tDCF77DateTime.iIsValidForSeconds,sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iDCF77LastResult,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(tDCF77DateTime.iDCF77Status,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							ltoa(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds,sTemp,10);
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);							
							uart_puts(sOutputLine);		
							uart_puts_p(prgsReturnOK);
						}
						// Get DCF77 Bitarray
						else if (strcmp(sParameter,"gda")==0){
						// Block 4 DCF77 Bitarray
							// Bitpointer, Bit 0....59
							itoa(iDCF77BitPointer,sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							for(iTemp=0;iTemp<=59;iTemp++){
								itoa(aDCFData[iTemp],sTemp,10);	
								strcpy(sOutputLine,sTemp);
								if(iTemp!=59){
									strncat(sOutputLine,sPCControlCharEndParameter,2);
								}
								uart_puts(sOutputLine);
							}
							uart_puts(sPCControlCharEndBlock);	
							uart_puts_p(prgsReturnOK);
						}
						// Get Lastpowerfault
						else if (strcmp(sParameter,"glp")==0){
						// Block 5 Last Powerfault
							// Year, Month, Day, Weekday, Hour, Minute, MEZ
							itoa(eeprom_read_byte(&ee_iPowerFaultYear),sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_iPowerFaultMonth),sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_iPowerFaultDay),sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							itoa(eeprom_read_byte(&ee_iPowerFaultWeekday),sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_iPowerFaultHour),sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_iPowerFaultMinute),sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_bMEZ),sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);							
							uart_puts(sOutputLine);	
							uart_puts_p(prgsReturnOK);
						}
						else if (strcmp(sParameter,"ggs")==0){
						// Block 6 Get Global State
							// Clock Paused; New Clock Time Inc; Ready4Sync; iTimeDiff; DCF77 On;SyncModus 24h;Ausgang Pulsweite
							itoa(stClientTime.bIncTime,sTemp,10);
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stNewTimeForClients.bIncTime,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(stNewTimeForClients.bReady4Sync,sTemp,10);
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							uart_puts(sOutputLine);
							itoa(iTimeDiff,sTemp,10);
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(bDCF77,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(bSyncMode24h,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(iPulsWidthIn100ms,sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);							
							uart_puts(sOutputLine);
							uart_puts_p(prgsReturnOK);
						}
						else if (strcmp(sParameter,"gls")==0){
						// Block 7 SlaveTime at Last Powerfault 
							// Hour, Minute
							itoa(eeprom_read_byte(&ee_iHourClientTime),sTemp,10);	
							strcpy(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndParameter,2);
							itoa(eeprom_read_byte(&ee_iMinuteClientTime),sTemp,10);	
							strcat(sOutputLine,sTemp);
							strncat(sOutputLine,sPCControlCharEndBlock,2);							
							uart_puts(sOutputLine);	
							uart_puts_p(prgsReturnOK);
						}						
						else{
							uart_puts_p(prgsReturnERROR);
						}
						// End
					}
				}
				else{
					// Shell
					if (iShellPosition==SHELL_MENU_PROMPT){
						if (strcmp(sInput,"smt")==0){
							// Eingabe der aktuellen Zeit auf den Clients
							iShellPosition=1;
							uart_puts_p(prgsSCTQuest);
							//uart_puts("Enter current time on clients:\r\n");
						}
						else if (strcmp(sInput,"ATZ")==0){
							uart_puts(TERM_FG_WHITE_BRIGHT);	
							uart_puts(sClockName);
							uart_puts("\r\n");
							uart_puts_p(prgsFirm);
							uart_puts(sFirmwareVersion);		 
							uart_puts("\r\n");
							uart_puts_p(prgsInitHelp);
						}
						else if (strcmp(sInput,"nst")==0){
							if(! bDCF77){
								// Eingabe der aktuellen Zeit auf den Clients, Synctime bleibt stehen(wenn auf Clients eine definierte Zeit eingestellt werden muß)
								iShellPosition=2;
								uart_puts_p(prgsNCTQuest);
								//uart_puts("Enter current time on clients:\r\n");
							}
							else{
								uart_puts_p(prgsACTNCTErrorMsg);
							}
								
						}
						else if (strcmp(sInput,"ast")==0){
							if(! bDCF77){
								// Eingabe der aktuellen Zeit auf den Clients, Synctime läuft weiter(wenn auf Clients die Zeit gestellt werden soll)
								iShellPosition=3;
								uart_puts_p(prgsACTQuest);
							}
							else{
								uart_puts_p(prgsACTNCTErrorMsg);
							}
						}					
						else if (strcmp(sInput,"dcf")==0){
							// DCF on/off
							iShellPosition=4;
							//uart_puts("Set DCF on ? (y|n): ");
							uart_puts_p(prgsDCF77Head);
							if (bDCF77){uart_puts_p(prgsDCF77QuestOn);}else{uart_puts_p(prgsDCF77QuestOff);}
							//uart_puts("Enter current time on clients:\r\n");
						}		
						else if (strcmp(sInput,"pst")==0){
							// Pause für die Clients?
							iShellPosition=5;
							//uart_puts("Set DCF on ? (y|n): ");
							uart_puts_p(prgsPCTStat);
							if (stClientTime.bIncTime){uart_puts_p(prgsPCTQuestPaused);}else{uart_puts_p(prgsPCTQuestContinue);}
							//uart_puts("Enter current time on clients:\r\n");
						}				
						else if (strcmp(sInput,"ssm")==0){
							// DCF on/off
							iShellPosition=6;
							uart_puts_p(prgsSyncMode24hHead);
							if (bSyncMode24h){uart_puts_p(prgsSyncMode24hQuest12);}else{uart_puts_p(prgsSyncMode24hQuest24);}
						}							
						else if ((strcmp(sInput,"help")==0)||(strcmp(sInput,"?")==0)){
							/*uart_puts("\x1b[33mdcf -> \x1b[37mSwitch the DCF77 ");
							uart_puts("on off.\r\n");*/
							uart_puts_p(prgsHelp);
						}
						
						else if (strcmp(sInput,"spw")==0){
							// Ändern der Impulsweite der Ausgänge 
							iShellPosition=7;
							uart_puts_p(prgsPulsWidthQuest);
						}	
						else if (strcmp(sInput,"stat")==0){
								uart_puts(TERM_FG_WHITE_BRIGHT);	
								uart_puts(sClockName);
								uart_puts("\r\n");
								uart_puts_p(prgsFirm);
								uart_puts(sFirmwareVersion);		 
								uart_puts("\r\n");
								fUpdateTimeString(sTime,&stClientTime);
								uart_puts_p(prgsTimeOnClients);
								uart_puts(sTime);
								uart_puts_p(prgsMode);
								if(stNewTimeForClients.bReady4Sync){
									fUpdateTimeString(sTime,&stNewTimeForClients);
									uart_puts_p(prgsSyncingTo);	
									uart_puts(sTime);	
									itoa(iTimeDiff,sTemp,10);
									uart_puts(" ");
									uart_puts(sTemp); 
									uart_puts_p(prgsMinutesLeft);
								}
								else{
									uart_puts("normal");
								}
								uart_puts_p(prgsSyncMode);
								if(bSyncMode24h){
									uart_puts_p(prgsSyncMode24h);
								}
								else{
									uart_puts_p(prgsSyncMode12h);
								}
								uart_puts_p(prgsDCF77);
								if(bDCF77){
									uart_puts_p(prgsDCF77LastRec);
									if(tDCF77DateTime.iDay!=0){
										fUpdateDCF77DateTimeString(sDCFDateTime,&tDCF77DateTime);		
										uart_puts(sDCFDateTime);
									}
									else{
										uart_puts_p(prgsDCF77NoRec);
									}
									uart_puts_p(prgsDCF77LastRes);
									fGetLastDCF77ResultString(tDCF77DateTime.iDCF77LastResult,sTemp);
									uart_puts(sTemp);
									uart_puts_p(prgsDCF77LastStat);
									fGetLastDCF77StatusString(tDCF77DateTime.iDCF77Status,sTemp);
									uart_puts(sTemp);
									if(tDCF77DateTime.iDCF77Status==DCF77_SYNCING && iDCF77BitPointer>0){
										uart_puts_p(prgsCurrentBit);
										itoa(iDCF77BitPointer-1,sTemp,10);
										uart_puts(sTemp);
										if(aDCFData[iDCF77BitPointer-1]==HIGH){
											//strcat(sOutputLine,"HIGH");
											uart_puts("=1");
										}
										else{
											//strcat(sOutputLine,"LOW");
											uart_puts("=0");
										}										
									}
									if(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds!=0xffffffff){
										uart_puts_p(prgsDCF77LastData);
										ltoa(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds,sTemp,10);
										uart_puts(sTemp);
										uart_puts_p(prgsSeconds);
									}
									uart_puts("\r\n");
								}
								else{
									uart_puts(sDCF77StateOff);
									uart_puts("\r\n");
								}
								if(eeprom_read_byte(&ee_iPowerFaultMinute)!=0xFF){
									uart_puts_p(prgsLastPowerF);
									if(eeprom_read_byte(&ee_iPowerFaultDay)!=0xFF){
										tDisplayLastPowerFaultFull.iMinute=eeprom_read_byte(&ee_iPowerFaultMinute);
										tDisplayLastPowerFaultFull.iHour=eeprom_read_byte(&ee_iPowerFaultHour);
										tDisplayLastPowerFaultFull.iDay=eeprom_read_byte(&ee_iPowerFaultDay);
										tDisplayLastPowerFaultFull.iMonth=eeprom_read_byte(&ee_iPowerFaultMonth);
										tDisplayLastPowerFaultFull.iYear=eeprom_read_byte(&ee_iPowerFaultYear);
										tDisplayLastPowerFaultFull.bMEZ=eeprom_read_byte(&ee_bMEZ);
										tDisplayLastPowerFaultFull.iWeekday=eeprom_read_byte(&ee_iPowerFaultWeekday);
										fUpdateDCF77DateTimeString(sDCFDateTime,&tDisplayLastPowerFaultFull);
										uart_puts(sDCFDateTime);
									}
									else{
										tDisplayLastPowerFault.iMinute=eeprom_read_byte(&ee_iPowerFaultMinute);
										tDisplayLastPowerFault.iHour=eeprom_read_byte(&ee_iPowerFaultHour);
										tDisplayLastPowerFault.iSecond=0;
										fUpdateTimeString(sTemp,&tDisplayLastPowerFault);
										uart_puts(sTemp);
									}
									uart_puts("\r\n");
								}
								if(eeprom_read_byte(&ee_iMinuteClientTime)!=0xFF){
									uart_puts_p(prgsLastPowerFClientTime);
									tDisplayClientTimeLastPowerFault.iMinute=eeprom_read_byte(&ee_iMinuteClientTime);
									tDisplayClientTimeLastPowerFault.iHour=eeprom_read_byte(&ee_iHourClientTime);
									tDisplayClientTimeLastPowerFault.iSecond=0;
									fUpdateTimeString(sTemp,&tDisplayClientTimeLastPowerFault);
									uart_puts(sTemp);
									uart_puts("\r\n");
								}
								uart_puts_p(prgsLCDDisplayIs);
								if(bLCDDisplayOn){
									uart_puts_p(prgsLCDOn);
								}
								else{
									uart_puts_p(prgsLCDOff);
								}
								uart_puts("\r\n");
								uart_puts_p(prgsOutputPulseWidth);
								itoa(iPulsWidthIn100ms,sTemp,10);
								uart_puts(sTemp);
								uart_puts("00ms\r\n");
								uart_puts_p(prgsOutputPulseInterval);
								itoa(iPulsIntervalLengthInSec,sTemp,10);
								uart_puts(sTemp);
								uart_puts("sec\r\n");
								
						}
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsUnknownCommand);
							uart_puts(TERM_RESET);
						}
					}
					// 1= Eingabe Zeit auf den Cliebt/Nebenuhren
					else if (iShellPosition==1){
						if (fConvertInput2Time(sInput,&stClientTime)){
							uart_puts(TERM_FG_GREEN_BRIGHT);
							uart_puts_p(prgsPrintok);
							uart_puts(TERM_RESET);
						}
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsInvalidTime);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}
					else if (iShellPosition==2){
						if (fConvertInput2Time(sInput,&stNewTimeForClients)){
							uart_puts(TERM_FG_GREEN_BRIGHT);
							uart_puts_p(prgsPrintok);
							uart_puts(TERM_RESET);
							stNewTimeForClients.bIncTime=FALSE;
							stNewTimeForClients.bReady4Sync=TRUE;
						}
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsInvalidTime);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}
					else if (iShellPosition==3){
						if (fConvertInput2Time(sInput,&stNewTimeForClients)){
							uart_puts(TERM_FG_GREEN_BRIGHT);
							uart_puts_p(prgsPrintok);
							uart_puts(TERM_RESET);
							stNewTimeForClients.bReady4Sync=TRUE;
							stNewTimeForClients.bIncTime=TRUE;
						}
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsInvalidTime);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}			
					else if (iShellPosition==4){
						if(strcmp(sInput,sKeyYes)==0){
							if(! bDCF77){
								uart_puts(TERM_FG_GREEN_BRIGHT);
								uart_puts_p(prgsSetDCF77On);
								uart_puts(TERM_RESET);
								bDCF77=true;
								eeprom_write_byte(&ee_bDCF77,TRUE);
								fInitDCF77();
							}
							else{
								uart_puts(TERM_FG_GREEN_BRIGHT);
								uart_puts_p(prgsSetDCF77Off);
								uart_puts(TERM_RESET);
								bDCF77=false;
								eeprom_write_byte(&ee_bDCF77,FALSE);
								fDisableDCF77();
							}
						}
						else if(strcmp(sInput,sKeyNo)==0){
							// Nothing
						} 
						else{
							uart_puts_p(prgsWrongInput);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}
					else if (iShellPosition==5){
						if(strcmp(sInput,sKeyYes)==0){
							if(! stClientTime.bIncTime){
								uart_puts(TERM_FG_WHITE_BRIGHT);
								uart_puts_p(prgsSetContinue);
								uart_puts(TERM_RESET);
								stClientTime.bIncTime=TRUE;
								eeprom_write_byte(&ee_bClientClockPaused,FALSE);
							}
							else{
								uart_puts(TERM_FG_WHITE_BRIGHT);
								uart_puts_p(prgsSetHalted);
								uart_puts(TERM_RESET);
								stClientTime.bIncTime=FALSE;
								eeprom_write_byte(&ee_bClientClockPaused,TRUE);
							}
						}
						else if(strcmp(sInput,sKeyNo)==0){
							// Nothing
						} 
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsWrongInput);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}	
					else if (iShellPosition==6){
						if(strcmp(sInput,sKeyYes)==0){
							if(! bSyncMode24h){
								uart_puts(TERM_FG_GREEN_BRIGHT);
								uart_puts_p(prgsSetSyncMode24hOn);
								uart_puts(TERM_RESET);
								bSyncMode24h=true;
								eeprom_write_byte(&ee_bSyncMode24h,TRUE);
							}
							else{
								uart_puts(TERM_FG_GREEN_BRIGHT);
								uart_puts_p(prgsSetSyncMode24hOff);
								uart_puts(TERM_RESET);
								bSyncMode24h=false;
								eeprom_write_byte(&ee_bSyncMode24h,FALSE);
							}
						}
						else if(strcmp(sInput,sKeyNo)==0){
							// Nothing
						} 
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsWrongInput);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}
					
					else if (iShellPosition==7){
						if(fCheckAndConvertPulsWidth(sInput,&iTemp)){
							iPulsWidthIn100ms=iTemp;
							iPulsIntervalLengthInSec=CALC_PULSE_INTERVAL_LENGTH_IN_SEC(iPulsWidthIn100ms);
							eeprom_write_byte(&ee_iPulsWidthIn100ms,iTemp);
							uart_puts(TERM_FG_GREEN_BRIGHT);
							uart_puts_p(prgsPulsWidthSetOutPre);
							itoa(iPulsWidthIn100ms,sTemp,10);
							uart_puts(sTemp);
							uart_puts_p(prgsPulsWidthSetOutPost);
							uart_puts_p(prgsPulsIntervalSetOutPre);
							itoa(iPulsIntervalLengthInSec,sTemp,10);
							uart_puts(sTemp);
							uart_puts_p(prgsPulsIntervalSetOutPost);							
							uart_puts(TERM_RESET);
						}
						else{
							uart_puts(TERM_FG_RED_BRIGHT);
							uart_puts_p(prgsWrongInput);
							uart_puts(TERM_RESET);
						}
						iShellPosition=SHELL_MENU_PROMPT;
					}
					else{
						iShellPosition=SHELL_MENU_PROMPT;
					}
				}
				strcpy(sInput,"");
				sCommand[0]='\0';
				sParameter[0]='\0';
				if (iShellPosition==SHELL_MENU_PROMPT){
					uart_puts(sPrompt);
				}
			}
		}
		/*************** End Shell ****************/

		/*************** End Serielles Userinterface ****************/
	}
	return(0);
}
// END MAIN
static void fUpdateDCF77DateTimeString(char *psDCF77DateTime,TDCF77DateTime *ptDCF77DateInfo){
		char sTmp[10]="";
		//char sDCFDateTime[34]="---------- --.--.---- --:-- ----";
		switch(ptDCF77DateInfo->iWeekday){
			case 1: strcpy_P(psDCF77DateTime,prgsMondayLong);
				break;
			case 2: strcpy_P(psDCF77DateTime,prgsTuesdayLong);
				break;
			case 3: strcpy_P(psDCF77DateTime,prgsWednesdayLong);
				break;
			case 4: strcpy_P(psDCF77DateTime,prgsThursdayLong);
				break;
			case 5: strcpy_P(psDCF77DateTime,prgsFridayLong);
				break;
			case 6: strcpy_P(psDCF77DateTime,prgsSaturdayLong);
				break;
			case 7: strcpy_P(psDCF77DateTime,prgsSundayLong);
				break;
			default:strcpy(psDCF77DateTime,"---");
		}
		strcat(psDCF77DateTime," ");
		fUInt8To2CharStr(ptDCF77DateInfo->iDay,sTmp);
		strcat(psDCF77DateTime,sTmp);
		strcat(psDCF77DateTime,".");
		fUInt8To2CharStr(ptDCF77DateInfo->iMonth,sTmp);
		strcat(psDCF77DateTime,sTmp);
		strcat(psDCF77DateTime,".");
		itoa(ptDCF77DateInfo->iYear,sTmp,10);		
		if(ptDCF77DateInfo->iYear<10){
			strcat(psDCF77DateTime,"200");
			strcat(psDCF77DateTime,sTmp);
		}
		else{
			strcat(psDCF77DateTime,"20");
			strcat(psDCF77DateTime,sTmp);
		}
		strcat(psDCF77DateTime," ");			
		fUInt8To2CharStr(ptDCF77DateInfo->iHour,sTmp);
		strcat(psDCF77DateTime,sTmp);
		strcat(psDCF77DateTime,":");
		fUInt8To2CharStr(ptDCF77DateInfo->iMinute,sTmp);
		strcat(psDCF77DateTime,sTmp);
		strcat(psDCF77DateTime," ");					
		if(ptDCF77DateInfo->bMEZ){
			strcat(psDCF77DateTime,"MEZ");
		}
		else{
			strcat(psDCF77DateTime,"MESZ");
		}

}
static void fUInt8To2CharStr(uint8_t iNum, char *s2CharStr){
		char sTmp[4]="";
		itoa(iNum,sTmp,10);
		if(iNum<10){
			strcpy(s2CharStr,"0");
			strcat(s2CharStr,sTmp);
		}
		else{
			strcpy(s2CharStr,sTmp);
		}
}
static void fUpdateTimeString(char *psTime,TTime *ptClientTime){
		char sTmp[3]="";
		fUInt8To2CharStr(ptClientTime->iHour,sTmp);
		strcpy(psTime,sTmp);
		strcat(psTime,":");
		fUInt8To2CharStr(ptClientTime->iMinute,sTmp);
		strcat(psTime,sTmp);
		strcat(psTime,":");
		fUInt8To2CharStr(ptClientTime->iSecond,sTmp);
		strcat(psTime,sTmp);
		if(! stClientTime.bIncTime){
			strcat_P(psTime,prgsHalted);
		}
}

// Init the io ports
static void fInitIoports(void){
	/// Datenrichtung 1= Ausgang, 0=Eingang
	// DDRB |= (1<<OUTPUT_PULSE_EVEN);
	DDRD |= ((1<<OUTPUT_PULSE_ODD)|(1<<OUTPUT_PULSE_EVEN)|(1<<PIN_1HZ_BLINK));
	// ISR1 und PD4 als Eingang
	DDRD &= ~((1<<POWER_SUPPLY_ACTIVE_PIN)|(1<<LCD_DISPLAY_ACTIVE_PIN));
	// Pullups auf PD4 an
	PORTD |= (1<<LCD_DISPLAY_ACTIVE_PIN);
	// ISR1 Fallende Flanke
	// EICRA |= (1<<ISC11);
	// ISR1 Steigende/Fallende Flanke
	EICRA |= (1<<ISC10);
	// Interrupt 1 (PD3)
	EIMSK |= (1<<INT1);
}
static void fInitLEDIOPorts(void){
	PORTC &= ~((1<<PIN_LED_DCF77_CURRENT_STATUS)|(1<<PIN_LED_DCF77_RETURN_STATE_OK)|(1<<PIN_LED_DCF77_RETURN_STATE_ERROR)|(1<<PIN_LED_DCF77_HEALTH_STATE)|(1<<PIN_CLOCK_IS_SYNCING));
	DDRC |= ((1<<PIN_LED_DCF77_CURRENT_STATUS)|(1<<PIN_LED_DCF77_RETURN_STATE_OK)|(1<<PIN_LED_DCF77_RETURN_STATE_ERROR)|(1<<PIN_LED_DCF77_HEALTH_STATE)|(1<<PIN_CLOCK_IS_SYNCING));
}
// init counter
static void fInitCounter(void){
	//CPU Takt/8
	TIMER0_START_DIV8;
}
// SIGNAL(SIG_INTERRUPT1){
ISR(INT1_vect){
   //uint8_t iSRegister;  
   //iSRegister = SREG;   // Statusregister sichern
   //cli();  
	//TOGGLE_LED(PORTB,OUTPUT_PULSE_EVEN);
	// Spannungsausfall Zeit der Clientuhren sichern
	if(! POWER_SUPPLY_ACTIVE){
		// power down
		eeprom_write_byte(&ee_iHourClientTime,stClientTime.iHour);
		eeprom_write_byte(&ee_iMinuteClientTime,stClientTime.iMinute);
		if(tDCF77DateTime.iIsValidForSeconds>0){
			eeprom_write_byte(&ee_iPowerFaultHour,tDCF77DateTime.iHour);
			eeprom_write_byte(&ee_iPowerFaultMinute,tDCF77DateTime.iMinute);
			eeprom_write_byte(&ee_iPowerFaultDay,tDCF77DateTime.iDay);
			eeprom_write_byte(&ee_iPowerFaultMonth,tDCF77DateTime.iMonth);
			eeprom_write_byte(&ee_iPowerFaultYear,tDCF77DateTime.iYear);
			eeprom_write_byte(&ee_iPowerFaultWeekday,tDCF77DateTime.iWeekday);
			eeprom_write_byte(&ee_bMEZ,tDCF77DateTime.bMEZ);
		}
		else{
			eeprom_write_byte(&ee_iPowerFaultHour,stClientTime.iHour);
			eeprom_write_byte(&ee_iPowerFaultMinute,stClientTime.iMinute);
			eeprom_write_byte(&ee_iPowerFaultDay,0xFF);
		}
		if(bLCDDisplayOn){
			lcd_clrscr();			
			fLCDPutStringCenter((char*)sPowerSupplyDownLine1,0);
			fLCDPutStringCenter((char*)sPowerSupplyDownLine2,1);
		}
	}
	else{
		// RESET
		//goto *((void**) 0);
		uart_init(UART_BAUD_SELECT(BAUD_RATE,F_CPU));
		if(bLCDDisplayOn){lcd_init(LCD_DISP_ON);}
		//Powerup
	}
	//SREG = iSRegister;
}
// timer overflow, CPUTakt/8
// SIGNAL(SIG_OVERFLOW0){
ISR(TIMER0_OVF_vect){
	static int iCountTimer0=0;
	// Teiler durch 3125
	if(iCountTimer0<3124){
		iCountTimer0++;
	}
	else{
		iCountTimer0=0;
		// Funktion alle 1/10 Sekunde aufrufen
		fExecuteEvery10telSecond();
	}
}
static void fExecuteEvery10telSecond(void){
	static uint8_t iCountTimer0=0;
	//uint8_t iOverflow=10;
	// Teiler durch 10
	if(iPulseWidthEven>0){
		// Auf Nummer sicher gehen
		RESET_PULSE_ODD;
		SET_PULSE_EVEN;
		iPulseWidthEven--;
	} 
	else{
		RESET_PULSE_EVEN;
	}
	if(iPulseWidthOdd>0){
		// Auf Nummer sicher gehen
		RESET_PULSE_EVEN;
		SET_PULSE_ODD;
		iPulseWidthOdd--;
	} 
	else{
		RESET_PULSE_ODD;
	}	
	if(iCountTimer0<9){
		iCountTimer0++;
	}
	else{
		iCountTimer0=0;
		// Funktion alle 1sec aufrufen
		fExecuteEverySecond();
	}
	if((iCountTimer0 % 5)==0){
		// Toggle 1HZ PIN
		TOGGLE_PIN(PORTD,PIN_1HZ_BLINK);
	}	
}
static void fExecuteEverySecond(void){
	static int iDelayCount=0;
	// Synctime für die Zeit des syncs weiterzählen
	if(stNewTimeForClients.bIncTime){
		if(stNewTimeForClients.iSecond<59){
			stNewTimeForClients.iSecond++;
		}
		else{
			stNewTimeForClients.iSecond=0;
			if (stNewTimeForClients.iMinute<59){
				stNewTimeForClients.iMinute++;
			}
			else{
				stNewTimeForClients.iMinute=0;
				if(stNewTimeForClients.iHour<23){
					stNewTimeForClients.iHour++;
				}
				else{
					stNewTimeForClients.iHour=0;
				}
			}
		}
	}
	// Zeitdifferenz hier berechnen, in fExecuteEverySecond dauerts zu lange
	if(bSyncMode24h){
		iTimeDiff=fTimeDiffFor24hScope(&stClientTime,&stNewTimeForClients);
	}
	else{
		iTimeDiff=fTimeDiffFor12hScope(&stClientTime,&stNewTimeForClients);
	}
	// CLient nur weiterzählen wenn Stromversorgung vorhanden
	if (POWER_SUPPLY_ACTIVE){
		// Sync nötig?
		if(stNewTimeForClients.bReady4Sync && (iTimeDiff!=0)){
			// Die Synczeitnormal weiterzählen
			if(iTimeDiff>0){
				// Zeit aufholen
				// Warten bis iPulsIntervalLengthInSec erreicht
				if(iDelayCount<=0){
					fIncClientTime(&stClientTime,TRUE);
					iDelayCount=iPulsIntervalLengthInSec-1;
				}
				else{
					iDelayCount--;
				}
			}
			else{
				// Zeit warten
			}
		}
		else if((stNewTimeForClients.bReady4Sync) && (iTimeDiff==0)){
				stClientTime.iHour=stNewTimeForClients.iHour;
				stClientTime.iMinute=stNewTimeForClients.iMinute;
				stClientTime.iSecond=stNewTimeForClients.iSecond;
				stNewTimeForClients.bReady4Sync=FALSE;
				stNewTimeForClients.bIncTime=FALSE;
				
		}
		else{
			if(stClientTime.bIncTime){
				// AUf jeden Fall iPulsIntervalLengthInSec warten falls eine sychronisation ansteht nach dem weiterzählen der Uhr
				iDelayCount=iPulsIntervalLengthInSec;
				fIncClientTime(&stClientTime,FALSE);
			}
		}
	}
	// Display umschalten jede CHANGE_DISPLAY_PAGE_IN_SEC te Sekunde
	if(iDisplayPageSecCount>0){
		iDisplayPageSecCount--;
	}
	else{
		iDisplayPageSecCount=CHANGE_DISPLAY_PAGE_IN_SEC;
		iDisplayPage++;
	}
	// Falls DCF77 Zeit gültig, Zeit der Gültigkeit um 1Sekunde verringern
	if(tDCF77DateTime.iIsValidForSeconds>0){tDCF77DateTime.iIsValidForSeconds--;}
	if(tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds<0xffffffff){
		tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds++;
	}
	bUpdateDisplay=TRUE;
}

// Liest von uart hängt die gelesen Zeichen an psInput an
// Liefert TRUE wenn die Eingabe abgeschlossen wurde, ansonsten 1
int fReadUART(char *psInput,uint8_t iMaxInCharacters){
		//uint8_t iMaxChars=0;
		static char sInput[MAX_INPUT_CHARACTERS];
		//iMaxChars = MAX_INPUT_CHARACTERS<iMaxInCharacters ? MAX_INPUT_CHARACTERS : iMaxInCharacters;
		unsigned int iReceive;
		iReceive=uart_getc();
		// Errors
		if ( iReceive & (UART_FRAME_ERROR | UART_OVERRUN_ERROR | UART_BUFFER_OVERFLOW)  ){
			return(FALSE);
		}
		// No data
		else if(iReceive & UART_NO_DATA){
			return(FALSE);
			// Do nothing
		}
		// Data available
		else{
			// echo char
			// ...until Return Character is 0x0d = 13 is received
			if ((char)iReceive=='\r'){
				if (UART_ECHO){uart_puts("\r\n");}
				strcpy(psInput,sInput);
				sInput[0]='\0';
				return(TRUE);
			}
			// STRG-C
			else if((char)iReceive==3){
				//strcpy(sInput,"");
				strcpy(psInput,"\3");
				sInput[0]='\0';
				return(TRUE);
			}
			else {
				if (UART_ECHO){uart_putc((char)iReceive);}
				// Only if enough space in sInput free
				if(strlen(sInput)<MAX_INPUT_CHARACTERS-1){
					// Concatinate all characters to a string....., we need only the lower byte.
					// Valid character or return??
					if ((((uint8_t)iReceive >= 32)&&((uint8_t)iReceive <= 126)) || ((uint8_t)iReceive==13)){
						strcat(sInput,(char*)&iReceive);
					}
				}
				return(FALSE);
			}
		}
}
static int fConvertInput2Time(char *psIn,TTime *ptTime){
	char *psToken;
	char* pStrErrPos; 
	uint8_t iConvertHour,iConvertMinute;
	psToken=strtok(psIn,":");
	if (psToken){
		iConvertHour=strtol(psToken,&pStrErrPos,10);
		// KOnvertieren nach int ok?
		if (*pStrErrPos=='\0'){
			// Stunde plausibel?
			if ((iConvertHour>=0) && (iConvertHour<=23)){
				psToken=strtok(NULL,":");
				if (psToken){
					iConvertMinute=strtol(psToken,&pStrErrPos,10);
					// KOnvertieren nach int ok?
					if (*pStrErrPos=='\0'){
						if ((iConvertMinute>=0) && (iConvertMinute<=59)){
							ptTime->iHour=iConvertHour;
							ptTime->iMinute=iConvertMinute;
							ptTime->iSecond=0;
							return(TRUE);
						}
						else{
							return(FALSE);
						}
					}
					else{
						return(FALSE);
					}
				}
				else{
					return(FALSE);
				}
			} 
			else{
				return(FALSE);
			}
		}
		else{
			return(FALSE);
		}
	}
	else{
		return(FALSE);
	}
}

/* fTimeDiffFor12hScope
Berechnet die Differenz in Minuten zwischen 2 Zeiten auf 12 Stunden Zeigeruhrenbasis, muß aber 0:00-11:59h sein
1. Parameter Istzeit auf den Client Uhren (kann auch 24h Basis sein)
2. Parameter Sollzeit von DCF 77 (kann auch 24h Basis sein)
Return: die Differenz die den Clientuhren aufgeholt werden muß als positive Zahl bzw.
		die Differenz die von den Clientuhren gewarten werden muß als negative Zahl
		Die max Wartezeit kann mit iMaxWait festgelegt werden
*/
static int fTimeDiffFor12hScope(TTime *ttIsTime, TTime *ttNewTime){
	int iIsTimeInMinutes, iNewTimeInMinutes, iDiffMinutes;
	int iMaxWait=61;
	iIsTimeInMinutes=ttIsTime->iMinute;
	iIsTimeInMinutes+=(ttIsTime->iHour*60);
	iNewTimeInMinutes=ttNewTime->iMinute;
	iNewTimeInMinutes+=(ttNewTime->iHour*60);	
	if (iIsTimeInMinutes>720){iIsTimeInMinutes-=720;}
	if (iNewTimeInMinutes>720){iNewTimeInMinutes-=720;}
	if (iIsTimeInMinutes<iNewTimeInMinutes){
		iDiffMinutes=iIsTimeInMinutes+720-iNewTimeInMinutes;
		if (iDiffMinutes<=iMaxWait){
			return(iDiffMinutes*=-1);
		}
		else{
			return(iNewTimeInMinutes-iIsTimeInMinutes);
		}
	}
	else{
		iDiffMinutes=iIsTimeInMinutes-iNewTimeInMinutes;
		if (iDiffMinutes<=iMaxWait){
			return(iDiffMinutes*=-1);
		}
		else{
			return(iNewTimeInMinutes+720-iIsTimeInMinutes);
		}
	}
}
/* fTimeDiffFor24hScope
Berechnet die Differenz in Minuten zwischen 2 Zeiten 
1. Parameter Istzeit auf den Client Uhren 
2. Parameter Sollzeit von DCF 77 
Return: die Differenz die den Clientuhren aufgeholt werden muß als positive Zahl bzw.
		die Differenz die von den Clientuhren gewarten werden muß als negative Zahl
		Die max Wartezeit kann mit iMaxWait festgelegt werden
*/
static int fTimeDiffFor24hScope(TTime *ttIsTime, TTime *ttNewTime){
	int iIsTimeInMinutes, iNewTimeInMinutes, iDiffMinutes;
	int iMaxWait=61;
	iIsTimeInMinutes=ttIsTime->iMinute;
	iIsTimeInMinutes+=(ttIsTime->iHour*60);
	iNewTimeInMinutes=ttNewTime->iMinute;
	iNewTimeInMinutes+=(ttNewTime->iHour*60);	
	if (iIsTimeInMinutes>iNewTimeInMinutes){
		iNewTimeInMinutes+=1440;
	}
	iDiffMinutes=iNewTimeInMinutes-iIsTimeInMinutes;
	if(iDiffMinutes>=(1440-iMaxWait)){
		return((1440-iDiffMinutes)*-1);
	}
	else{
		return(iDiffMinutes);
	}
}
static void fIncClientTime(TTime *ttClientTime,uint8_t bIncMinutes){
	// Sekunden hochzählen nur wenn nicht Minuten ausgewählt sind
	if ((!bIncMinutes) && ttClientTime->iSecond<59){
		ttClientTime->iSecond++;
	}
	else{
		ttClientTime->iSecond=0;
		// Parity Odd? Letztes Bit=1
		if (ttClientTime->iMinute & 1){
			iPulseWidthOdd=iPulsWidthIn100ms;
		}
		// Dann even
		else{
			iPulseWidthEven=iPulsWidthIn100ms;
		}
		if(ttClientTime->iMinute<59){
			ttClientTime->iMinute++;
		}
		else{
			ttClientTime->iMinute=0;
			if (ttClientTime->iHour<23){
				ttClientTime->iHour++;
			}
			else{
				ttClientTime->iHour=0;
			}
		}
	}
}

static void fInitDCF77(void){
	tDCF77DateTime.iDCF77Status=DCF77_INIT;
	// Als Eingang
	DDRD &= ~(1<<DCF77_INPUT_PIN);
	// Pullup ein
	PORTD |= (1<<DCF77_INPUT_PIN);
	// ISC00 und ISC01 nicht gesetzt = Interrupt bei LowLevel
	// Bei jeder Änderung egal ob fallende oder steigende Flanke einen Interrupt auslösen
	EICRA |= (1<<ISC00);
	//Fallende Flanke
	// EICRA |= (1<<ISC01);
	//Steigende Flanke
	// EICRA |= (1<<ISC01) | (1<<ISC00);
	// Interrupt 0 (PD2)
	EIMSK |= (1<<INT0);
	// oder GICR |= (1<<INT0);
	tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;
	tDCF77DateTime.iDCF77LastResult=DCF77_NO_STATUS;
	//bUpdateDisplay=TRUE;
}
static void fDisableDCF77(void){
	EICRA &= ~(1<<ISC00);
	EIMSK &= ~(1<<INT0);
	TIMER1_STOP;
	// Pullup an
	PORTD |= (1<<DCF77_INPUT_PIN);
}
// Bestimmt durch xor Verkünpfung das Parity aus einer Anzahl Bits aus dem Bitfeld 
static uint8_t fParityOdd(uint8_t aBitField[], uint8_t iStartField, uint8_t iStopField){
	uint8_t iParity, iLoop;
	iParity=aBitField[iStartField];
	for(iLoop=iStartField+1;iLoop<=iStopField;iLoop++){
		iParity^=aBitField[iLoop];
	}
	return(iParity);
}
// SIGNAL(SIG_OVERFLOW1){
ISR(TIMER1_OVF_vect){
	//TOGGLE_LED(PORTB,PB0);
	TIMER1_STOP;
	// Auf Max Couter setzen damit DCF77 Error erkannt wird
	TCNT1=0xffff;
}
//SIGNAL(SIG_INTERRUPT0){
ISR(INT0_vect){
	// Pulsbreite des DCF Signals
	static uint16_t iPulseWidth=0;
	// Pausenbreite des DCF Signals
	static uint16_t iPauseWidth=0;
	// Sum der aktuellen Puls und Pausenbreite
	static uint16_t iDCFCyclePulsePauseSum=0;
	//char sTemp[16]="";
	//char sCounter[16]="";
	// Eingang Low->High Pegel?
	if (PIND & (1<<PIND2)){
		// Counter Stop, Pausenwert lesen
		TIMER1_STOP;
		iPauseWidth=TCNT1;
#if DCF_DEBUG
		tDCF77Debug.iPauseWidth=iPauseWidth;
#endif		
		// Start Counter
		TIMER1_START_DIV1024;
		// Vorheriges Puls + Pausenzeit überprüfen. Bits 0-57
		if (tDCF77DateTime.iDCF77Status==DCF77_SYNCING){
			// Gültiges Signal
			iDCFCyclePulsePauseSum=iPauseWidth+iPulseWidth;
			if (((iDCFCyclePulsePauseSum<DCF_PULSE_AND_PAUSE_CYCLE_SUM_MAX)&& (iDCFCyclePulsePauseSum>DCF_PULSE_AND_PAUSE_CYCLE_SUM_MIN))){
				// Zeit Pulse + Pause Scheint in Ordnung
				// HIGH
			}
			else{
				// Undefiniert Puls + Pause Zeit
				tDCF77DateTime.iDCF77LastResult=DCF77_CYCLE_ERROR;
				tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;
#if DCF_DEBUG				
				tDCF77Debug.DCF77LastResult=DCF77_CYCLE_ERROR;
				tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;
#endif				
			}

		}
		// DCF StartBit(Pause für 1,8sec-1,9sec)
		if (iPauseWidth>(DCF_START_CYCLE_MIN)&&iPauseWidth<(DCF_START_CYCLE_MAX)){
			if(tDCF77DateTime.iIsValidForSeconds>=DCF77_IS_VALID_FOR_SECONDS-3){
				// Letzter Sync erfolgreich? Dann sync setzen
				// 1. Sekunde neue Minute && Sync erfolgreich
				if(bDCF77){
					stNewTimeForClients.iHour=tDCF77DateTime.iHour;
					stNewTimeForClients.iMinute=tDCF77DateTime.iMinute;
					stNewTimeForClients.iSecond=0;
					stNewTimeForClients.bReady4Sync=TRUE;
					stNewTimeForClients.bIncTime=TRUE;
				}
			}
			iDCF77BitPointer=0;
			tDCF77DateTime.iDCF77Status=DCF77_SYNCING;
#if DCF_DEBUG			
			tDCF77Debug.DCF77Status=DCF77_SYNCING;
#endif			
		}
	}
	else{
		// Timer anhalten
		TIMER1_STOP;
		// Pausenzeit speichern
		iPulseWidth=TCNT1;
#if DCF_DEBUG		
		tDCF77Debug.iPulseWidth=iPulseWidth;
#endif		
		// Timer wieder starten
		TIMER1_START_DIV1024
		if(tDCF77DateTime.iDCF77Status==DCF77_SYNCING){
			if ((iPulseWidth>DCF_SIGNAL_HIGH_CYCLE_Min) && (iPulseWidth<DCF_SIGNAL_HIGH_CYCLE_MAX)){
				// OK High 
				aDCFData[iDCF77BitPointer]=HIGH;
			}
			else if ((iPulseWidth>DCF_SIGNAL_LOW_CYCLE_MIN) && (iPulseWidth<DCF_SIGNAL_LOW_CYCLE_MAX)){
				// ok LOW
				aDCFData[iDCF77BitPointer]=LOW;
			}
			else{
				// Undefinierte Puls Zeit
				tDCF77DateTime.iDCF77LastResult=DCF77_PULSE_RANGE_ERROR;
				tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;	
#if DCF_DEBUG				
				tDCF77Debug.DCF77LastResult=DCF77_PULSE_RANGE_ERROR;
				tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;
#endif				
			}
			// 59 Bit Empfangen->fertig (0-58)
			if((tDCF77DateTime.iDCF77Status==DCF77_SYNCING) && (iDCF77BitPointer>=58)){
				// AUswertung
				// Minute Bit 21-27
				// Parity OK?
				//uart_puts("AUSWERTUNG\r\n");
				tDCF77DateTime.bMEZ=aDCFData[18];
				if(fParityOdd(aDCFData,21,27)==aDCFData[28]){
					// Parity ok, bcd -> bin
					tDCF77DateTime.iMinute=(aDCFData[24]<<3)|(aDCFData[23]<<2)|(aDCFData[22]<<1)|(aDCFData[21]);
					tDCF77DateTime.iMinute+=10*((aDCFData[27]<<2)|(aDCFData[26]<<1)|(aDCFData[25]));
				}
				else{
					tDCF77DateTime.iDCF77LastResult=DCF77_PARITY_ERROR_MINUTE;
					tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;	
#if DCF_DEBUG					
					tDCF77Debug.DCF77LastResult=DCF77_PARITY_ERROR_MINUTE;
					tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;
#endif					
				}
				// AUswertung
				// Hour Bit 29-34
				if((fParityOdd(aDCFData,29,34)==aDCFData[35])&&(tDCF77DateTime.iDCF77Status==DCF77_SYNCING)){
					// Parity ok, bcd -> bin
					tDCF77DateTime.iHour=(aDCFData[32]<<3)|(aDCFData[31]<<2)|(aDCFData[30]<<1)|(aDCFData[29]);
					tDCF77DateTime.iHour+=10*((aDCFData[34]<<1)|(aDCFData[33]));
					// Hier den SyncCounter setzen da Uhrzeit vollständig!
				}
				else{
					tDCF77DateTime.iDCF77LastResult=DCF77_PARITY_ERROR_HOUR;
					tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;		
#if DCF_DEBUG						
					tDCF77Debug.DCF77LastResult=DCF77_PARITY_ERROR_HOUR;
					tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;
#endif					
				}
				// AUswertung
				// Datum Bit 36-58
				if((fParityOdd(aDCFData,36,57)==aDCFData[58])&&(tDCF77DateTime.iDCF77Status==DCF77_SYNCING)){
					// Parity ok, bcd -> bin
					// Day
					tDCF77DateTime.iDay=(aDCFData[39]<<3)|(aDCFData[38]<<2)|(aDCFData[37]<<1)|(aDCFData[36]);
					tDCF77DateTime.iDay+=10*((aDCFData[41]<<1)|(aDCFData[40]));
					// Weekday
					tDCF77DateTime.iWeekday=(aDCFData[44]<<2)|(aDCFData[43]<<1)|(aDCFData[42]);
					// Month
					tDCF77DateTime.iMonth=(aDCFData[48]<<3)|(aDCFData[47]<<2)|(aDCFData[46]<<1)|(aDCFData[45]);
					tDCF77DateTime.iMonth+=10*((aDCFData[49]));
					// Year
					tDCF77DateTime.iYear=(aDCFData[53]<<3)|(aDCFData[52]<<2)|(aDCFData[51]<<1)|(aDCFData[50]);
					tDCF77DateTime.iYear+=10*((aDCFData[57]<<3)|(aDCFData[56]<<2)|(aDCFData[55]<<1)|(aDCFData[54]));
					// Wenn bis hier ist alles ok
					tDCF77DateTime.iIsValidForSeconds=DCF77_IS_VALID_FOR_SECONDS;
					tDCF77DateTime.iDCF77LastResult=DCF77_RECEIVE_COMPLETE_SUCCESSFULLY;
					tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;	
#if DCF_DEBUG					
					tDCF77Debug.DCF77LastResult=DCF77_RECEIVE_COMPLETE_SUCCESSFULLY;
					tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;					
#endif					
					tDCF77DateTime.iDCF77LastReceivedDataPaketInSeconds=0;
				}
				else{
					tDCF77DateTime.iDCF77LastResult=DCF77_PARITY_ERROR_DATE;
					tDCF77DateTime.iDCF77Status=DCF77_WAIT_FOR_SYNCING;	
#if DCF_DEBUG					
					tDCF77Debug.DCF77LastResult=DCF77_PARITY_ERROR_DATE;
					tDCF77Debug.DCF77Status=DCF77_WAIT_FOR_SYNCING;							
#endif					
				}
			}
			iDCF77BitPointer++;		
		}
	}
	//bUpdateDisplay=TRUE;
	//uart_puts("UP TRUE");
}

static void fGetLastDCF77StatusString(uint8_t iRes,char *psStatMsg){
	if(tDCF77DateTime.iDCF77Status==DCF77_INIT){
		strcpy_P(psStatMsg,prgsDCF77StateInit);
	}
	else if(tDCF77DateTime.iDCF77Status==DCF77_SYNCING){					
		strcpy_P(psStatMsg,prgsDCF77StateSyncing);
	}
	else if(tDCF77DateTime.iDCF77Status==DCF77_WAIT_FOR_SYNCING){					
		strcpy_P(psStatMsg,prgsDCF77StateWait4Sync);
	}
	else{
		strcpy_P(psStatMsg,prgsDCF77StateUnknown);
	}
}
static void fGetShortDayString(uint8_t iDayNumber,char *psShortDay){
	switch(iDayNumber){
		case 1: strcpy_P(psShortDay,prgsMondayShort);
			break;
		case 2: strcpy_P(psShortDay,prgsTuesdayShort);
			break;
		case 3: strcpy_P(psShortDay,prgsWednesdayShort);
			break;
		case 4: strcpy_P(psShortDay,prgsThursdayShort);
			break;
		case 5: strcpy_P(psShortDay,prgsFridayShort);
			break;
		case 6: strcpy_P(psShortDay,prgsSaturdayShort);
			break;
		case 7: strcpy_P(psShortDay,prgsSundayShort);
			break;
		default:strcpy(psShortDay,"---");			
	}
} 
static void fGetLastDCF77ResultString(uint8_t iRes,char *psErrMsg){
	switch(iRes){
		case DCF77_RECEIVE_COMPLETE_SUCCESSFULLY: strcpy_P(psErrMsg,prgsDCF77ErrMsgOK);
			break;
		case DCF77_PARITY_ERROR_HOUR: strcpy_P(psErrMsg,prgsDCF77ErrMsgParityHour);
			break;
		case DCF77_PARITY_ERROR_MINUTE: strcpy_P(psErrMsg,prgsDCF77ErrMsgParityMinute);
			break;
		case DCF77_PARITY_ERROR_DATE: strcpy_P(psErrMsg,prgsDCF77ErrMsgParityDate);
			break;
		case DCF77_CYCLE_ERROR: strcpy_P(psErrMsg,prgsDCF77ErrMsgCycleTime);
			break;
		case DCF77_PULSE_RANGE_ERROR: strcpy_P(psErrMsg,prgsDCF77ErrMsgPulse);
			break;
		case DCF77_NO_STATUS: strcpy_P(psErrMsg,prgsDCF77ErrMsgNoState);
			break;
	}
} 
static void fLCDPutStringCenter(char *sOut,uint8_t iLineNumber){
	uint8_t iLength, iXStartPos;
	iLength=strlen(sOut);
	iXStartPos=(LCD_DISP_LENGTH-iLength)/2;
	if((iXStartPos>=0 && iXStartPos<(LCD_DISP_LENGTH/2)) && (iLineNumber>=0 && iLineNumber<LCD_LINES)){
		lcd_gotoxy(iXStartPos,iLineNumber);
		lcd_puts(sOut);
	}
}
static void fGetCenterAndFilledUpString(char *sIn, char *sOut,uint8_t iLength){
	int8_t iStartPos=0,iLoop;
	char *pStrpos;
	iStartPos=(iLength-strlen(sIn))/2;
	sOut[0]='\0';
	pStrpos=sOut;
	// Clear String
	for(iLoop=0;iLoop<iLength;iLoop++){strcat(sOut,"\x20");}
	if(iStartPos>=0){
		pStrpos+=iStartPos;
		for(iLoop=0;iLoop<strlen(sIn);iLoop++){
				*pStrpos=sIn[iLoop];
				pStrpos++;
		}
	}
}

/* Not used
static void fCopyStringCenter(char *sString, char *sOut, uint8_t iMaxStrLength){
	uint8_t iLength, iXStartPos,iLoop;
	iLength=strlen(sString);
	iXStartPos=(iMaxStrLength-iLength)/2;
	if((iXStartPos>=0 && iXStartPos<(iMaxStrLength/2))){
		sOut[0]='\0';
		for(iLoop=0;iLoop<iXStartPos;iLoop++){
			strcat(sOut," ");
		}
		if((iXStartPos+iLength)<LCD_DISP_LENGTH){
			strcat(sOut,sString);
		}
	}
}
*/ 
static uint8_t fSplitCommandFromParameter(char *psInput, char* psCommand, char* psParameter){
	char *psToken;
	psToken=strtok(psInput,";");
	strncpy(psCommand,psToken,6);
	psToken=strtok(NULL,";");
	if(psToken != NULL){
		strncpy(psParameter,psToken,10);
		return(TRUE);
	}
	else{
		psParameter[0]='\0';
		return(FALSE);
	}
}
/* not used
static uint8_t fString2UInt8(char *sIn,uint8_t *iOut){
	char *pStrErrPos;
	*iOut=(uint8_t)strtol(sIn,&pStrErrPos,10);
	if (*pStrErrPos=='\0'){
		itoa(*iOut,sTemp,10);
		if(strcmp(sTemp,sIn)==0){
			return(TRUE);
		}
		else{
			return(FALSE);
		}
	}
	else{
		return(FALSE);
	}
}
* */
static uint8_t fCheckAndConvertPulsWidth(char *sIn,uint8_t *iOut){
	char *pStrErrPos;
	long lConvertedNumber;
	lConvertedNumber=strtol(sIn,&pStrErrPos,10);
	if (*pStrErrPos=='\0'){
		if(lConvertedNumber>=MIN_PULSE_WIDTH_IN100MS && lConvertedNumber<=MAX_PULSE_WIDTH_IN100MS){
			*iOut=(uint8_t)lConvertedNumber;
			return(TRUE);
		}
		else{
			return(FALSE);
		}
	}
	else{
		return(FALSE);
	}
}
/* not used
static void fPrintIntToUART(char *sIn,int16_t iIn){
	char sOut[20];
	uart_puts(sIn);
	uart_puts(": ");
	ltoa(iIn,sOut,10);
	uart_puts(sOut);
	uart_puts("\r\n");
}
*/

