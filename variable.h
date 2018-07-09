#ifndef __VARIABLE_H
#define __VARIABLE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTC.h" // DS1307

/*
   Arduino Leonardo
  A0 – D18
  A1 – D19
  A2 – D20
  A3 – D21
  A4 – D22
  A5 – D23
  A6 – D4
  A7 – D6
  A8 – D8
  A9 – D9
  A10 – D10
  A11 – D12

   Arduino UNO
  Pin 14 = Analog in 0
  Pin 15 = Analog in 1
  Pin 16 = Analog in 2
  Pin 17 = Analog in 3
  Pin 18 = Analog in 4
  Pin 19 = Analog in 5
*/

//---------------------------------------
// Definitia konstant pre I/O
#define r_svetlo 2      // vystup pre rele svetla  A4
#define r_filter 3      // vystup pre rele filtra  A5

#define B_OFF_FILTER 14        // tlacidlo pre prve oneskorenie
#define B_KRMENIE     15
#define B_ENTER 16        // tlacidlo pre zmenu stavu svetla

#define l_status 7      // led status operacie
#define l_filter 6      // led status vypnuteho filtra
#define l_start 4       // led status upravy startu casu
#define l_end 5         // led status upravy koncu casu

#define LCD_svetlo 17   // pin pre podsvietenie LCD displeja

//--------------------------------------
// Definicia konstant

extern LiquidCrystal lcd;                        // LCD display
extern time_date odklad_filter;                       // premenna pre ulozenie dalsieho startu filtra
extern time_date start_svetlo;          // cas zaciatku svetla
extern time_date end_svetlo;            // cas ukoncenia svetla
extern time_date end_LCD_svetlo;               // cas ukoncenia podsvietenia LCD
extern time_date posun_filter;             // posun o 15min

#define refresh_cycle 5                                         // konstanta poctu opakovania pre refresh casu z RTC
#define button_delay 200                                        // konstatna pre cakanie na znova stlacenie tlacidla
#define LCD_DELAY     30

//--------------------------------------
// definicia premennych
extern time_date time_actuall;
extern uint8_t refresh_time;

#endif
