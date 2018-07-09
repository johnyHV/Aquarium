#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Servo.h>
#include "RTC.h" // DS1307
#include "variable.h"
#include "system.h"

//--------------------------------------

void setup() {
  // initial UART
  Serial.begin(9600);
  Serial.println(F("Start"));

  // initial I2C
  Wire.begin();

  // time_date set_time_ = {10, 01, 00, 1, 8, 7, 18};
  // setTime(set_time_);

  time_actuall = readTime();
  printTime(time_actuall);

  // Initial LCD
  lcd.begin(20, 4);

  //read data EEPROM
  read_data_eeprom();

  //initial I/O
  pinMode(r_svetlo, OUTPUT);
  pinMode(r_filter, OUTPUT);
  pinMode(l_status, OUTPUT);
  pinMode(l_filter, OUTPUT);
  pinMode(LCD_svetlo, OUTPUT);
  pinMode(B_OFF_FILTER, INPUT);
  pinMode(B_KRMENIE, INPUT);
  //pinMode(B_ENTER, INPUT);

  digitalWrite(r_filter, HIGH);
  svetlo_control();
  LCD_svetlo_off();

  digitalWrite(l_status, LOW);
  digitalWrite(l_filter, LOW);

  pinMode(SERVO_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(servo_position);

}

void loop() {

  filter_on();
  filter_off();
  svetlo_control();
  LCD_svetlo_off();

  if (refresh_time == refresh_cycle)
    krmenie_control();

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
}

