#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTC.h" // DS1307
#include "variable.h"
#include "system.h"

//--------------------------------------

void setup() {
  // initial UART
  Serial.begin(9600);
  Serial.println(F("Start"));
  //time_actuall = readTime();
  //printTime(time_actuall);

  // initial I2C
  //Wire.begin();
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  
  // Initial LCD
  lcd.begin(20, 4);
  

  //lcd.setCursor(19,4);

  // read data EEPROM
  //read_data_eeprom();

  //initial I/O
  /*pinMode(r_svetlo, OUTPUT);
  pinMode(r_filter, OUTPUT);
  pinMode(l_status, OUTPUT);
  pinMode(l_filter, OUTPUT);
  pinMode(l_start, OUTPUT);
  pinMode(l_end, OUTPUT);
  pinMode(LCD_svetlo, OUTPUT);
  pinMode(B_OFF_FILTER, INPUT);
  pinMode(B_KRMENIE, INPUT);
  pinMode(B_ENTER, INPUT);

  svetlo_control();
  filter_on();
  LCD_svetlo_off();

  digitalWrite(l_status, LOW);
  digitalWrite(l_filter, LOW);
  digitalWrite(l_start, LOW);
  digitalWrite(l_end, LOW);
*/
}

void loop() {
/*
  filter_on();
  filter_off();
  svetlo_control();
  LCD_svetlo_off();
  change_config();

  // refresh time
  if (refresh_time == refresh_cycle) {
    time_actuall = readTime();
    refresh_time = 0;

    // ochrana proti zblbnutiu RTC
    if (time_actuall.sec > 59) {
      time_actuall.sec = 59;
      setTime(time_actuall);
    }
  }
  refresh_time++;

  print_time_LCD(time_actuall);
  delay(button_delay);
*/
/*
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();

    if (incomingByte == 'a')
      svetlo_on();
    if (incomingByte == 's')
      svetlo_off();
    if (incomingByte == 'd')
      filter_on();
    if (incomingByte == 'f')
      filter_off();
    if (incomingByte == 'g')
      printTime(time);
    if (incomingByte == 'h') {
      posun(odklad_1);
      filter_off();
      Serial.print(odklad.hour);
      Serial.print(F(":"));
      Serial.println(odklad.min);
    }
    if (incomingByte == 'j') {
      posun(odklad_2);
      filter_off();
      Serial.print(F("Znova zapnutie filtra o: "));
      Serial.print(odklad.hour);
      Serial.print(F(":"));
      Serial.println(odklad.min);
    }
    if (incomingByte == 'k') {
      setTimeUart();
    }
    if (incomingByte == 'l') {
      Serial.println(F("Setting Time"));
      time_date set_time_ = {15, 56, 00, 4, 12, 6, 14};
      setTime(set_time_);

    }
    if (incomingByte == 'q') {
      Serial.print(digitalRead(14));
      Serial.print(digitalRead(15));
      Serial.print(digitalRead(16));
    }
    if (incomingByte == 'w') {
      EEPROM.write(0, 1);
      EEPROM.write(1, 1);
      EEPROM.write(2, 1);
      EEPROM.write(3, 1);

      time_date start_svetlo = {10, 0, 0, 0, 0, 0, 0, 0, 1};   // cas zaciatku svetla
      time_date end_svetlo = {20, 0, 0, 0, 0, 0, 0, 2, 3};     // cas ukoncenia svetla
      Serial.println(F("Write"));
      write_data_eeprom();
      Serial.println(F("Read"));
      read_data_eeprom();
    }
  }
  */
}

