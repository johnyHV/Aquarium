#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTC.h"
#include "variable.h"

void LCD_svetlo_on();
void LCD_svetlo_off();

void filter_on();
void filter_off();
void filter_off_basic();

void filter_off_time(time_date tmp);

void svetlo_control();

void print_time_LCD(time_date tmp);

void change_config();
bool change_config_update_time_hour(time_date *tmp);
bool change_config_update_time_min(time_date *tmp);
bool change_config_update_value(uint8_t *tmp);
bool change_config_update_bool(uint8_t *tmp);

void status_led();

void write_data_eeprom();
void read_data_eeprom();

bool button_read(uint8_t ID);

bool krmenie_control();
bool servo_control();

#endif
