#ifndef __VARIABLE_H
#define __VARIABLE_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTC.h"
#include <Servo.h>

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
#define r_svetlo 6      // vystup pre rele svetla  A4
#define r_filter 7      // vystup pre rele filtra  A5

#define B_OFF_FILTER  14        // tlacidlo pre prve oneskorenie
#define B_KRMENIE     15
#define B_ENTER       A7        // tlacidlo pre zmenu stavu svetla

#define SERVO_PIN     10

#define l_status 8      // led status operacie
#define l_filter 9      // led status vypnuteho filtra

#define LCD_svetlo 13   // pin pre podsvietenie LCD displeja

#define E_krmenie_otacky 10
#define E_krmenie_status 11

//--------------------------------------
// Definicia konstant

extern LiquidCrystal lcd;                        // LCD display
extern time_date odklad_filter;                       // premenna pre ulozenie dalsieho startu filtra
extern time_date start_svetlo;          // cas zaciatku svetla
extern time_date end_svetlo;            // cas ukoncenia svetla
extern time_date end_LCD_svetlo;               // cas ukoncenia podsvietenia LCD
extern time_date posun_filter;             // posun o 15min
extern time_date krmenie;                 // zaciatok krmenia
extern uint8_t krmenie_otacky;                    // pocet otacok krmidla
extern uint8_t krmenie_otacky_status;
extern uint8_t krmenie_status;

#define refresh_cycle 5                                         // konstanta poctu opakovania pre refresh casu z RTC
#define button_delay 200                                        // konstatna pre cakanie na znova stlacenie tlacidla
#define LCD_DELAY     30

extern Servo servo1;

//--------------------------------------
// definicia premennych
extern time_date time_actuall;
extern uint8_t refresh_time;
extern bool krmenie_flag;
extern uint8_t servo_position;
extern uint8_t servo_position_cycle;

#endif
