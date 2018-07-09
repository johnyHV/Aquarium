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
  if ((digitalRead(B_OFF_FILTER))) {
    status_led();
    filter_off_time(posun_filter);

    digitalWrite(r_filter, false);
    digitalWrite(l_filter, true);
    Serial.print(F("Filter OFF - "));
    Serial.print(time_actuall.hour);
    Serial.print(F(":"));
    Serial.println(time_actuall.min);
  }
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
    digitalWrite(r_svetlo, false);
  } else {
    Serial.print(F("Svetlo OFF - "));
    digitalWrite(r_svetlo, true);
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
/*
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

  // vypis stavu filtra
  lcd.setCursor(9, 0);
  lcd.print(F("F-"));

  if (s_filter == true) {
    lcd.setCursor(11, 0);
    lcd.print(F("ON   "));
  } else {
    lcd.setCursor(11, 0);
    if (odklad.hour < 10) {
      lcd.print(F("0"));
      lcd.setCursor(12, 0);
      lcd.print(odklad.hour);
    }
    else
      lcd.print(odklad.hour);

    lcd.setCursor(13, 0);
    lcd.print(F(":"));
    lcd.setCursor(14, 0);
    if (odklad.min < 10) {
      lcd.print(F("0"));
      lcd.setCursor(15, 0);
      lcd.print(odklad.min);
    } else
      lcd.print(odklad.min);
  }

  // vypis casu svetla
  lcd.setCursor(0, 1);
  lcd.print(F("Z-"));

  lcd.setCursor(2, 1);
  if (start_svetlo.hour < 10) {
    lcd.print(0);
    lcd.setCursor(3, 1);
    lcd.print(start_svetlo.hour);
  } else
    lcd.print(start_svetlo.hour);

  lcd.setCursor(4, 1);
  lcd.print(F(":"));

  lcd.setCursor(5, 1);
  if (start_svetlo.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(6, 1);
    lcd.print(start_svetlo.min);
  } else
    lcd.print(start_svetlo.min);

  lcd.setCursor(9, 1);
  lcd.print(F("K-"));

  lcd.setCursor(11, 1);
  if (end_svetlo.hour < 10) {
    lcd.print(F("0"));
    lcd.setCursor(12, 1);
    lcd.print(end_svetlo.hour);
  } else
    lcd.print(end_svetlo.hour);

  lcd.setCursor(13, 1);
  lcd.print(F(":"));

  lcd.setCursor(14, 1);
  if (end_svetlo.min < 10) {
    lcd.print(F("0"));
    lcd.setCursor(15, 1);
    lcd.print(end_svetlo.min);
  } else
    lcd.print(end_svetlo.min);

  //lcd.setCursor(8,1);
  //lcd.write(0xFF);
  */
}

void change_config()
{
  // nacitanie vstupu pre zmenu stavu svetla
  if (digitalRead(B_ENTER)) {

    change_config_update_time_hour(&start_svetlo);
    change_config_update_time_min(&start_svetlo);

    change_config_update_time_hour(&end_svetlo);
    change_config_update_time_min(&end_svetlo);

    time_date set_time;      // cas nastavenia
    set_time.hour = time_actuall.hour;
    set_time.min = time_actuall.min;
    change_config_update_time_hour(&set_time);
    change_config_update_time_min(&set_time);

    setTime(set_time);
    write_data_eeprom();
  }
}

bool change_config_update_time_hour(time_date *tmp)
{
  status_led();
  delay(button_delay);
  LCD_svetlo_on();

  do {
    time_actuall = readTime();
    print_time_LCD(time_actuall);

    digitalWrite(l_start, true);
    if ((tmp->hour <= 22) || (tmp->hour <= 0)) {
      if (digitalRead(B_KRMENIE)) {
        tmp->hour++;
      }
      if (digitalRead(B_OFF_FILTER)) {
        tmp->hour--;
      }
    } else {
      tmp->hour = 0;
    }

    if (digitalRead(B_ENTER)) {
      status_led();
      digitalWrite(l_start, false);
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
    time_actuall = readTime();
    print_time_LCD(time_actuall);

    // nastavenie casu minuty
    digitalWrite(l_end, true);

    if ((tmp->min <= 58) || (tmp->min <= 0)) {
      if (digitalRead(B_KRMENIE)) {
        tmp->min++;
      }
      if (digitalRead(B_OFF_FILTER)) {
        tmp->min--;
      }
    } else {
      tmp->min = 0;
    }

    if (digitalRead(B_ENTER)) {
      status_led();
      digitalWrite(l_end, false);
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
  EEPROM.write(start_svetlo.a_hour, start_svetlo.hour);
  EEPROM.write(start_svetlo.a_min, start_svetlo.min);

  EEPROM.write(end_svetlo.a_hour, end_svetlo.hour);
  EEPROM.write(end_svetlo.a_min, end_svetlo.min);

  Serial.println(F("Write data to EEPROM"));
}

/**

   @info Citanie dat z EEPROM pamete
   @param
*/
void read_data_eeprom() {

  start_svetlo.hour = EEPROM.read(start_svetlo.a_hour);
  start_svetlo.min = EEPROM.read(start_svetlo.a_min);

  end_svetlo.hour = EEPROM.read(end_svetlo.a_hour);
  end_svetlo.min = EEPROM.read(end_svetlo.a_min);

  Serial.print(start_svetlo.hour);
  Serial.print(F(":"));
  Serial.println(start_svetlo.min);

  Serial.print(end_svetlo.hour);
  Serial.print(F(":"));
  Serial.print(end_svetlo.min);
}
