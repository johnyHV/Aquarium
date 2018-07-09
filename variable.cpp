#include "variable.h"

//--------------------------------------
// Definicia konstant

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                        // LCD display

time_date odklad_filter = {0, 0, 0, 0, 0, 0, 0};                // premenna pre ulozenie dalsieho startu filtra
time_date posun_filter = {0, 15, 0, 0, 0, 0, 0};                // posun o 15min

time_date start_svetlo = {10, 0, 0, 0, 0, 0, 0, 0, 2};          // cas zaciatku svetla
time_date end_svetlo = {20, 0, 0, 0, 0, 0, 0, 4, 6};            // cas ukoncenia svetla
time_date end_LCD_svetlo = {0, 0, 0, 0, 0, 0, 0};               // cas ukoncenia podsvietenia LCD

#define refresh_cycle 5                                         // konstanta poctu opakovania pre refresh casu z RTC
#define button_delay 200                                        // konstatna pre cakanie na znova stlacenie tlacidla

//--------------------------------------
// definicia premennych
time_date time_actuall;
uint8_t refresh_time = 0;

