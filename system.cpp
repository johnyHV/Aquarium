#include "system.h"

/**

   @info Zapnutie podsvietenia LCD displeja
   @param
*/
void LCD_svetlo_on() {
  Serial.println("LCD ON");
  digitalWrite(LCD_svetlo, HIGH);

  if (time_actuall.sec > 59 - LCD_DELAY) {
    if (time_actuall.min == 59) {
      end_LCD_svetlo.sec = LCD_DELAY - (time_actuall.sec - LCD_DELAY);
      end_LCD_svetlo.min = 0;

      if (time_actuall.hour == 23)
        end_LCD_svetlo.hour = 0;
      else
        end_LCD_svetlo.hour = time_actuall.hour + 1;

    } else {
      end_LCD_svetlo.sec = LCD_DELAY - (60 - time_actuall.sec);
      end_LCD_svetlo.min = time_actuall.min + 1;
      end_LCD_svetlo.hour = time_actuall.hour;
    }
  } else {
    end_LCD_svetlo.sec = time_actuall.sec + LCD_DELAY;
    end_LCD_svetlo.min = time_actuall.min;
    end_LCD_svetlo.hour = time_actuall.hour;
  }

  Serial.print(time_actuall.hour);
  Serial.print(":");
  Serial.print(time_actuall.min);
  Serial.print(":");
  Serial.println(time_actuall.sec);

  Serial.print(end_LCD_svetlo.hour);
  Serial.print(":");
  Serial.print(end_LCD_svetlo.min);
  Serial.print(":");
  Serial.println(end_LCD_svetlo.sec);
}

/**

   @info Vypnutie podsvietenia LCD displeja
   @param
*/
void LCD_svetlo_off() {
  // kontrola vypnutia podsvietenia LCD
  if ((end_LCD_svetlo.hour == time_actuall.hour) && (end_LCD_svetlo.min <= time_actuall.min)
      && (end_LCD_svetlo.sec <= time_actuall.sec)) {
    Serial.println("LCD OFF");
    digitalWrite(LCD_svetlo, LOW);
  }
}

/**

   @info zapne filter
   @return void
*/
void filter_on() {

  // kontrola zapnutia filtra
  if ((odklad_filter.hour == time_actuall.hour) && (odklad_filter.min == time_actuall.min)) {
    digitalWrite(r_filter, true);
    digitalWrite(l_filter, false);
    digitalWrite(l_status, false);
    Serial.print(F("Filter ON - "));
    Serial.print(time_actuall.hour);
    Serial.print(F(":"));
    Serial.println(time_actuall.min);
  }
}

/**

   @info vypne filter
   @return void
*/
void filter_off() {

  // nacitanie stavu pre prvy odklad filtra
  if ((button_read(B_OFF_FILTER))) {
    filter_off_basic();
  }
}

void filter_off_basic() {

  LCD_svetlo_on();
  filter_off_time(posun_filter);

  digitalWrite(r_filter, false);
  digitalWrite(l_filter, true);
  Serial.print(F("Filter OFF - "));
  Serial.print(time_actuall.hour);
  Serial.print(F(":"));
  Serial.println(time_actuall.min);

}

/**

   @info posunie cas podla tlacidla
   @param time_date cas posunu
*/
void filter_off_time(time_date tmp) {

  if ((60 - time_actuall.min) <= tmp.min) {
    odklad_filter.hour = tmp.hour++;
    odklad_filter.min = tmp.min - (60 - time_actuall.min);
  } else {
    odklad_filter.min = time_actuall.min + tmp.min;
  }

  if ((time_actuall.hour + tmp.hour) >= 24) {
    odklad_filter.hour = (time_actuall.hour + tmp.hour) - 24;
  } else {
    odklad_filter.hour = time_actuall.hour + tmp.hour;
  }

  digitalWrite(r_filter, false);
  digitalWrite(l_filter, true);
  Serial.print(F("Filter OFF - "));
  Serial.print(time_actuall.hour);
  Serial.print(F(":"));
  Serial.println(time_actuall.min);
}

void svetlo_control() {

  // kontrola zapnutia svetla
  if (((start_svetlo.hour <= time_actuall.hour) && (end_svetlo.hour > time_actuall.hour)) && ((start_svetlo.min <= time_actuall.min) && (end_svetlo.min <= time_actuall.min))) {
    Serial.print(F("Svetlo ON - "));
    digitalWrite(r_svetlo, true);
  } else {
    Serial.print(F("Svetlo OFF - "));
    digitalWrite(r_svetlo, false);
  }

  Serial.print(time_actuall.hour);
  Serial.print(F(":"));
  Serial.println(time_actuall.min);
}

/**

   @info Vypis dat na displej
   @param time_date aktualny cas
*/
void print_time_LCD(time_date tmp) {

  // ------------------------
  // vypis aktualneho casu
  lcd.setCursor(0, 0);
  if (tmp.hour < 10) {
    lcd.print(F("0"));
    lcd.setCursor(1, 0);
    lcd.print(tmp.hour);
  } else
    lcd.print(tmp.hour);

  lcd.setCursor(2, 0);
  lcd.print(F(":"));

  lcd.setCursor(3, 0);
  if (tmp.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(4, 0);
    lcd.print(tmp.min);
  } else
    lcd.print(tmp.min);

  lcd.setCursor(5, 0);
  lcd.print(F(":"));

  lcd.setCursor(6, 0);
  if (tmp.sec < 10) {
    lcd.print(F("0"));
    lcd.setCursor(7, 0);
    lcd.print(tmp.sec);
  } else
    lcd.print(tmp.sec);

  // ------------------------
  // vypis stavu filtra
  lcd.setCursor(11, 0);
  lcd.print(F("F-"));

  if (digitalRead(r_filter) == true) {
    lcd.setCursor(13, 0);
    lcd.print(F("ON   "));
  } else {
    lcd.setCursor(13, 0);
    if (odklad_filter.hour < 10) {
      lcd.print(F("0"));
      lcd.setCursor(14, 0);
      lcd.print(odklad_filter.hour);
    }
    else
      lcd.print(odklad_filter.hour);

    lcd.setCursor(15, 0);
    lcd.print(F(":"));
    lcd.setCursor(16, 0);
    if (odklad_filter.min < 10) {
      lcd.print(F("0"));
      lcd.setCursor(17, 0);
      lcd.print(odklad_filter.min);
    } else
      lcd.print(odklad_filter.min);
  }

  // ------------------------
  // vypis casu svetla
  lcd.setCursor(0, 1);
  lcd.print(F("Sz-"));

  lcd.setCursor(3, 1);
  if (start_svetlo.hour < 10) {
    lcd.print(0);
    lcd.setCursor(4, 1);
    lcd.print(start_svetlo.hour);
  } else
    lcd.print(start_svetlo.hour);

  lcd.setCursor(5, 1);
  lcd.print(F(":"));

  lcd.setCursor(6, 1);
  if (start_svetlo.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(7, 1);
    lcd.print(start_svetlo.min);
  } else
    lcd.print(start_svetlo.min);

  lcd.setCursor(11, 1);
  lcd.print(F("Sv-"));

  lcd.setCursor(14, 1);
  if (end_svetlo.hour < 10) {
    lcd.print(F("0"));
    lcd.setCursor(15, 1);
    lcd.print(end_svetlo.hour);
  } else
    lcd.print(end_svetlo.hour);

  lcd.setCursor(16, 1);
  lcd.print(F(":"));

  lcd.setCursor(17, 1);
  if (end_svetlo.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(18, 1);
    lcd.print(end_svetlo.min);
  } else
    lcd.print(end_svetlo.min);


  // ------------------------
  // vypis krmenia
  // ------------------------
  lcd.setCursor(0, 2);
  lcd.print(F("Kz-"));

  lcd.setCursor(3, 2);
  if (krmenie.hour < 10) {
    lcd.print(0);
    lcd.setCursor(4, 2);
    lcd.print(krmenie.hour);
  } else
    lcd.print(krmenie.hour);

  lcd.setCursor(5, 2);
  lcd.print(F(":"));

  lcd.setCursor(6, 2);
  if (krmenie.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(7, 2);
    lcd.print(krmenie.min);
  } else
    lcd.print(krmenie.min);

  lcd.setCursor(11, 2);
  lcd.print(F("Ko-"));

  lcd.setCursor(14, 2);
  lcd.print(krmenie_otacky);

  // ------------------------
  // Nastavenie
  // ------------------------
  lcd.setCursor(0, 3);
  lcd.print(F("Ka-"));

  lcd.setCursor(3, 3);
  if (krmenie_status)
  {
    lcd.print(F("Off"));
  }
  else
  {
    lcd.print(F("On "));
  }

  // ------------------------
  // nastavenia filtra
  lcd.setCursor(11, 3);
  lcd.print(F("Fc-"));

  lcd.setCursor(14, 3);
  if (posun_filter.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(15, 3);
    lcd.print(posun_filter.min);
  }
  else
    lcd.print(posun_filter.min);
    
  lcd.setCursor(16, 3);
  lcd.print("min");

  //lcd.setCursor(8,1);
  //lcd.write(0xFF);

}

void change_config()
{
  // nacitanie vstupu pre zmenu stavu svetla
  if (button_read(B_ENTER))
  {
    Serial.println(F("Enter change config"));

    digitalWrite(l_filter, HIGH);
    posun_filter.hour = 0;
    change_config_update_time_min(&posun_filter);

    digitalWrite(l_filter, LOW);
    change_config_update_bool(krmenie_status);

    digitalWrite(l_filter, HIGH);
    change_config_update_value(&krmenie_otacky);

    digitalWrite(l_filter, LOW);
    change_config_update_time_hour(&krmenie);
    digitalWrite(l_filter, HIGH);
    change_config_update_time_min(&krmenie);

    digitalWrite(l_filter, LOW);
    change_config_update_time_hour(&start_svetlo);
    digitalWrite(l_filter, HIGH);
    change_config_update_time_min(&start_svetlo);

    digitalWrite(l_filter, LOW);
    change_config_update_time_hour(&end_svetlo);
    digitalWrite(l_filter, HIGH);
    change_config_update_time_min(&end_svetlo);

    time_actuall = readTime();
    digitalWrite(l_filter, LOW);
    change_config_update_time_hour(&time_actuall);
    digitalWrite(l_filter, HIGH);
    change_config_update_time_min(&time_actuall);

    setTime(time_actuall);
    write_data_eeprom();

    digitalWrite(l_status, LOW);
    digitalWrite(l_filter, LOW);
  }
}

bool change_config_update_time_hour(time_date *tmp)
{
  status_led();
  delay(button_delay);
  LCD_svetlo_on();

  do {
    print_time_LCD(time_actuall);
    if ((tmp->hour <= 22) || (tmp->hour <= 0)) {
      if (button_read(B_KRMENIE)) {
        tmp->hour++;
      }
      if (button_read(B_OFF_FILTER)) {
        tmp->hour--;
      }
    } else {
      tmp->hour = 0;
    }

    if (button_read(B_ENTER)) {
      status_led();
      return true;
    }

    delay(button_delay);
    status_led();
  } while (true);
}

bool change_config_update_time_min(time_date *tmp)
{
  status_led();
  delay(button_delay);
  LCD_svetlo_on();

  do {
    print_time_LCD(time_actuall);

    // nastavenie casu minuty
    if ((tmp->min <= 58) || (tmp->min <= 0)) {
      if (button_read(B_KRMENIE)) {
        tmp->min++;
      }
      if (button_read(B_OFF_FILTER)) {
        tmp->min--;
      }
    } else {
      tmp->min = 0;
    }

    if (button_read(B_ENTER)) {
      status_led();
      return true;
    }

    delay(button_delay);
    status_led();
  } while (true);
}

bool change_config_update_value(uint8_t *tmp)
{
  status_led();
  delay(button_delay);
  LCD_svetlo_on();

  do {
    print_time_LCD(time_actuall);

    // nastavenie casu minuty
    if ((krmenie_otacky <= 255) || (krmenie_otacky <= 0)) {
      if (button_read(B_KRMENIE)) {
        krmenie_otacky++;
      }
      if (button_read(B_OFF_FILTER)) {
        krmenie_otacky--;
      }
    } else {
      krmenie_otacky = 0;
    }

    if (button_read(B_ENTER)) {
      status_led();
      return true;
    }

    delay(button_delay);
    status_led();
  } while (true);
}

bool change_config_update_bool(uint8_t *tmp)
{
  status_led();
  delay(button_delay);
  LCD_svetlo_on();

  do {
    print_time_LCD(time_actuall);

    // nastavenie casu minuty

    if (button_read(B_KRMENIE)) {
      krmenie_status = true;
    }
    if (button_read(B_OFF_FILTER)) {
      krmenie_status = false;
    }

    if (button_read(B_ENTER)) {
      status_led();
      return true;
    }

    delay(button_delay);
    status_led();
  } while (true);
}

/**

   @info zisti stav ledky, a zneguje ho
   @return void
*/
void status_led() {
  digitalWrite(l_status, !digitalRead(l_status));
}

/**

   @info Zapis dat do EEPROM pamete
   @param
*/
void write_data_eeprom() {
  EEPROM.write(start_svetlo.E_hour, start_svetlo.hour);
  EEPROM.write(start_svetlo.E_min, start_svetlo.min);

  EEPROM.write(end_svetlo.E_hour, end_svetlo.hour);
  EEPROM.write(end_svetlo.E_min, end_svetlo.min);

  EEPROM.write(krmenie.E_hour, krmenie.hour);
  EEPROM.write(krmenie.E_min, krmenie.min);

  EEPROM.write(posun_filter.E_hour, posun_filter.hour);
  EEPROM.write(posun_filter.E_min, posun_filter.min);

  EEPROM.write(E_krmenie_otacky, krmenie_otacky);

  EEPROM.write(E_krmenie_status, krmenie_status);

  Serial.println(F("Write data to EEPROM"));
  Serial.print(start_svetlo.hour);
  Serial.print(F(":"));
  Serial.println(start_svetlo.min);

  Serial.print(end_svetlo.hour);
  Serial.print(F(":"));
  Serial.println(end_svetlo.min);

  Serial.print(krmenie.hour);
  Serial.print(F(":"));
  Serial.println(krmenie.min);

  Serial.println(krmenie_otacky);

   Serial.print(posun_filter.min);
   Serial.println("min");
}

/**

   @info Citanie dat z EEPROM pamete
   @param
*/
void read_data_eeprom() {

  start_svetlo.hour = EEPROM.read(start_svetlo.E_hour);
  start_svetlo.min = EEPROM.read(start_svetlo.E_min);

  end_svetlo.hour = EEPROM.read(end_svetlo.E_hour);
  end_svetlo.min = EEPROM.read(end_svetlo.E_min);

  krmenie.hour = EEPROM.read(krmenie.E_hour);
  krmenie.min = EEPROM.read(krmenie.E_min);

  posun_filter.hour = EEPROM.read(posun_filter.E_hour);
  posun_filter.min = EEPROM.read(posun_filter.E_min);

  krmenie_otacky = EEPROM.read(E_krmenie_otacky);

  krmenie_status = EEPROM.read(E_krmenie_status);

  Serial.print(start_svetlo.hour);
  Serial.print(F(":"));
  Serial.println(start_svetlo.min);

  Serial.print(end_svetlo.hour);
  Serial.print(F(":"));
  Serial.println(end_svetlo.min);

  Serial.print(krmenie.hour);
  Serial.print(F(":"));
  Serial.println(krmenie.min);

  Serial.println(krmenie_otacky);
}

bool button_read(uint8_t ID)
{
  if (ID == B_ENTER)
  {
    if (analogRead(B_ENTER) < 100)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if (digitalRead(ID) == 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool krmenie_control() {

  // kontrola zapnutia krmenia
  if (((krmenie.hour == time_actuall.hour) && (krmenie.min == time_actuall.min) && (krmenie_status)) || (button_read(B_KRMENIE))) {
    krmenie_flag = true;
    krmenie_otacky_status = 0;
    servo_position_cycle = 2;
    filter_off_basic();
    digitalWrite(l_status, true);
  }

  if (krmenie_flag)
  {
    if (krmenie_otacky_status < krmenie_otacky)
    {
      Serial.println(krmenie_otacky_status);
      if (servo_control())
        krmenie_otacky_status++;
    }

    if (krmenie_otacky_status == krmenie_otacky)
    {
      krmenie_flag = false;
    }
  }
}

bool servo_control()
{
  servo1.write(servo_position);
  Serial.println(servo_position);
  //delay(1000);

  if ((servo_position == 180) && (!servo_position_cycle))
  {
    servo_position_cycle = 2;
    return true;
  }

  if (servo_position == 180)
  {
    servo_position = 0;
    servo_position_cycle--;
    return false;
  }

  if (servo_position == 0)
  {
    servo_position = 180;
    servo_position_cycle--;
    return false;
  }
}

