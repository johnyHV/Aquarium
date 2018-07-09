#include "variable.h"

//--------------------------------------
// Definicia konstant

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                        // LCD display

time_date odklad_filter = {0, 0, 0, 0, 0, 0, 0};                // premenna pre ulozenie dalsieho startu filtra
time_date posun_filter = {0, 15, 0, 0, 0, 0, 0};                // posun o 15min
time_date end_LCD_svetlo = { 0, 0, 0, 0, 0, 0, 0};              // cas ukoncenia podsvietenia LCD


time_date start_svetlo =  {10, 0, 0, 0, 0, 0, 0, 0, 1};          // cas zaciatku svetla
time_date end_svetlo =    {20, 0, 0, 0, 0, 0, 0, 2, 3};            // cas ukoncenia svetla
time_date krmenie =       { 0, 0, 0, 0, 0, 0, 0, 4, 5};                 // zaciatok krmenia

uint8_t krmenie_otacky = 0;                    // pocet otacok krmidla
uint8_t krmenie_otacky_status = 0;

#define refresh_cycle 5                                         // konstanta poctu opakovania pre refresh casu z RTC
#define button_delay 200                                        // konstatna pre cakanie na znova stlacenie tlacidla

Servo servo1;

//--------------------------------------
// definicia premennych
time_date time_actuall;
uint8_t refresh_time = 0;
bool krmenie_flag = false;
uint8_t servo_position = 180;
uint8_t servo_position_cycle = 0;
