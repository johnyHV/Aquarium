#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTC.h" // DS1307

/*
 * Arduino Leonardo
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

 * Arduino UNO
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
#define off_1 14        // tlacidlo pre prve oneskorenie
#define off_2 15        // tlacidlo pre druhe oneskorenie18
#define enter 16        // tlacidlo pre zmenu stavu svetla
#define l_status 7      // led status operacie
#define l_filter 6      // led status vypnuteho filtra
#define l_start 4       // led status upravy startu casu
#define l_end 5         // led status upravy koncu casu
#define LCD_svetlo 17   // pin pre podsvietenie LCD displeja

//--------------------------------------
// Definicia konstant

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);                        // LCD display
time_date odklad = {0, 0, 0, 0, 0, 0, 0};                       // premenna pre ulozenie dalsieho startu filtra
time_date start_svetlo = {10, 0, 0, 0, 0, 0, 0, 0, 2};          // cas zaciatku svetla
time_date end_svetlo = {20, 0, 0, 0, 0, 0, 0, 4, 6};            // cas ukoncenia svetla
time_date set_time = {0, 0, 0, 0, 0, 0, 0};			// cas nastavenia
time_date end_LCD_svetlo = {0, 0, 0, 0, 0, 0, 0};               // cas ukoncenia podsvietenia LCD
//static time_date LCD_svetlo = {0, 0, 30, 0, 0, 0, 0};           // svetlo svieti 30s
static time_date odklad_1 = {0, 15, 0, 0, 0, 0, 0};             // posun o 15min
static time_date odklad_2 = {0, 30, 0, 0, 0, 0, 0};             // posun o 1hod
#define refresh_cycle 5                                         // konstanta poctu opakovania pre refresh casu z RTC
#define button_delay 200                                        // konstatna pre cakanie na znova stlacenie tlacidla

//--------------------------------------
// definicia premennych
time_date time;
bool s_svetlo = false;
bool s_filter = false;
bool s_led = false;
bool s_LCD_svetlo = false;  // pomocna premenna pre informaciu o zapnuti svetla
uint8_t refresh_time = 0;

// funkcie
void svetlo_on();
void svetlo_off();
void filter_on();
void filter_off();
void status_led();
void posun(time_date tmp);
void print_time_LCD(time_date tmp);
char convert_number(uint8_t data);
void read_data_eeprom();
void write_data_eeprom();
void LCD_svetlo_on();
void LCD_svetlo_off();

//--------------------------------------

void setup() {
    // initial UART
    Serial.begin(9600);
    Serial.println(F("Start"));
    time = readTime();
    printTime(time);

    // initial I2C
    Wire.begin();

    // Initial LCD
    lcd.begin(16, 2);

    // read data EEPROM
    read_data_eeprom();
    
    //initial I/O
    pinMode(r_svetlo, OUTPUT);
    pinMode(r_filter, OUTPUT);
    pinMode(l_status, OUTPUT);
    pinMode(l_filter, OUTPUT);
    pinMode(l_start, OUTPUT);
    pinMode(l_end, OUTPUT);
    pinMode(LCD_svetlo, OUTPUT);
    pinMode(off_1, INPUT);
    pinMode(off_2, INPUT);
    pinMode(enter, INPUT);
    
    svetlo_off();
    filter_on();
    LCD_svetlo_off();
    
    digitalWrite(l_status, false);
    digitalWrite(l_filter, LOW);
    digitalWrite(l_start, LOW);
    digitalWrite(l_end, LOW);

}

void loop() {
    // refresh time 
    if (refresh_time == refresh_cycle) {
        time = readTime();
        refresh_time = 0;
    }

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
            EEPROM.write(0,1);
            EEPROM.write(1,1);
            EEPROM.write(2,1);
            EEPROM.write(3,1);
            
            time_date start_svetlo = {10, 0, 0, 0, 0, 0, 0, 0, 1};   // cas zaciatku svetla
            time_date end_svetlo = {20, 0, 0, 0, 0, 0, 0, 2, 3};     // cas ukoncenia svetla    
            Serial.println(F("Write"));
            write_data_eeprom();
            Serial.println(F("Read"));
            read_data_eeprom();
        }
    }

    // kontrola zapnutia svetla
    if (((start_svetlo.hour <= time.hour) && (end_svetlo.hour > time.hour)) && ((start_svetlo.min <= time.min) && (end_svetlo.min <= time.min))) {
        if (!s_svetlo)
            svetlo_on();
    } else {
        if (s_svetlo)
            svetlo_off();
    }

    // kontrola vypnutia podsvietenia LCD
    if ((end_LCD_svetlo.hour == time.hour) && (end_LCD_svetlo.min <= time.min) 
            && (end_LCD_svetlo.sec <= time.sec) && s_LCD_svetlo) {
        LCD_svetlo_off();
    }
    
    // kontrola zapnutia filtra
    if ((odklad.hour == time.hour) && (odklad.min == time.min)) {
        if (!s_filter)
            filter_on();
    }

    // nacitanie vstupu pre zmenu stavu svetla
    if (digitalRead(enter)) {
        status_led();
        Serial.println(F("Enter button"));
        delay(button_delay);
        status_led();
        bool slucka = true;
	set_time.hour = time.hour;
	set_time.min = time.min;
        LCD_svetlo_on();

        // nastavenie casu zaciatku
        do {
            time = readTime();
            print_time_LCD(time);

            digitalWrite(l_start, true);
            if (digitalRead(off_2)) {
                if ((start_svetlo.hour <= 22) || (start_svetlo.hour <= 0)) {
                    start_svetlo.hour++;
                } else {
                    start_svetlo.hour = 0;
                }
            }
            if (digitalRead(off_1)) {
                if ((start_svetlo.hour <= 22) || (start_svetlo.hour <= 0)) {
                    start_svetlo.hour--;
                } else {
                    start_svetlo.hour = 0;
                }
            }

            if (digitalRead(enter)) {
                status_led();
                slucka = false;
                digitalWrite(l_start, false);
            }

            delay(button_delay);
            status_led();
        } while (slucka);


        slucka = true;
        do {
            time = readTime();
            print_time_LCD(time);

            // nastavenie casu konca
            digitalWrite(l_end, true);
            if (digitalRead(off_2)) {
                if ((end_svetlo.hour <= 22) || (end_svetlo.hour <= 0)) {
                    end_svetlo.hour++;
                } else {
                    end_svetlo.hour = 0;
                }
            }
            if (digitalRead(off_1)) {
                if ((end_svetlo.hour <= 22) || (end_svetlo.hour <= 0)) {
                    end_svetlo.hour--;
                } else {
                    end_svetlo.hour = 0;
                }
            }

            if (digitalRead(enter)) {
                slucka = false;
                digitalWrite(l_end, false);
            }

            delay(button_delay);
            status_led();
        } while (slucka);
	
	slucka = true;
        do {
            time = readTime();
            print_time_LCD(time);

            // nastavenie casu hodiny
            digitalWrite(l_start, true);
            if (digitalRead(off_2)) {
                if ((set_time.hour <= 22) || (set_time.hour <= 0)) {
                    set_time.hour++;
                } else {
                    set_time.hour = 0;
                }
            }
            if (digitalRead(off_1)) {
                if ((set_time.hour <= 22) || (set_time.hour <= 0)) {
                    set_time.hour--;
                } else {
                    set_time.hour = 0;
                }
            }

            if (digitalRead(enter)) {
                slucka = false;
                digitalWrite(l_start, false);
            }

            setTime(set_time);

            delay(button_delay);
            status_led();
        } while (slucka);
        
        slucka = true;
        do {
            time = readTime();
            print_time_LCD(time);

            // nastavenie casu minuty
            digitalWrite(l_end, true);
            if (digitalRead(off_2)) {
                if ((set_time.min <= 58) || (set_time.min <= 0)) {
                    set_time.min++;
                } else {
                    set_time.min = 0;
                }
            }
            if (digitalRead(off_1)) {
                if ((set_time.min <= 58) || (set_time.min <= 0)) {
                    set_time.min--;
                } else {
                    set_time.min = 0;
                }
            }

            if (digitalRead(enter)) {
                slucka = false;
                digitalWrite(l_end, false);
            }

            setTime(set_time);

            delay(button_delay);
            status_led();
        } while (slucka);
        
        write_data_eeprom();
    }

    // nacitanie stavu pre prvy odklad filtra
    if ((digitalRead(off_1))) {
        status_led();
        posun(odklad_1);
        filter_off();
        Serial.println(F("OFF1 button"));
        delay(button_delay);
        status_led();
        LCD_svetlo_on();
    }

    //nacitanie stavu pre druhy odklad filtra
    if (digitalRead(off_2)) {
        status_led();
        posun(odklad_2);
        filter_off();
        Serial.println(F("OFF2 button"));
        delay(button_delay);
        status_led();
        LCD_svetlo_on();
    }

    refresh_time++;
    print_time_LCD(time);
    delay(button_delay);
}

/**
 *
 * @info zapne svetlo
 * @return void
 */
void svetlo_on() {

    s_svetlo = true;
    digitalWrite(r_svetlo, false);
    Serial.print(F("Svetlo ON - "));
    Serial.print(time.hour);
    Serial.print(F(":"));
    Serial.println(time.min);
}

/**
 *
 * @info vypne svetlo
 * @return void
 */
void svetlo_off() {

    s_svetlo = false;
    digitalWrite(r_svetlo, true);
    Serial.print(F("Svetlo OFF - "));
    Serial.print(time.hour);
    Serial.print(F(":"));
    Serial.println(time.min);
}

/**
 *
 * @info zapne filter
 * @return void
 */
void filter_on() {

    s_filter = true;
    digitalWrite(r_filter, true);
    digitalWrite(l_filter, false);
    Serial.print(F("Filter ON - "));
    Serial.print(time.hour);
    Serial.print(F(":"));
    Serial.println(time.min);
}

/**
 *
 * @info vypne filter
 * @return void
 */
void filter_off() {

    s_filter = false;
    digitalWrite(r_filter, false);
    digitalWrite(l_filter, true);
    Serial.print(F("Filter OFF - "));
    Serial.print(time.hour);
    Serial.print(F(":"));
    Serial.println(time.min);
}

/**
 *
 * @info zisti stav ledky, a zneguje ho
 * @return void
 */
void status_led() {
    if (s_led) {
        s_led = false;
        digitalWrite(l_status, false);
    } else {

        s_led = true;
        digitalWrite(l_status, true);
    }
}

/**
 * 
 * @info posunie cas podla tlacidla
 * @param time_date cas posunu
 */
void posun(time_date tmp) {

    if ((60 - time.min) <= tmp.min) {
        odklad.hour = tmp.hour++;
        odklad.min = tmp.min - (60 - time.min);
    } else {
        odklad.min = time.min + tmp.min;
    }

    if ((time.hour + tmp.hour) >= 24) {
        odklad.hour = (time.hour + tmp.hour) - 24;
    } else {
        odklad.hour = time.hour + tmp.hour;
    }
}

/**
 * 
 * @info Vypis dat na displej
 * @param time_date aktualny cas
 */
void print_time_LCD(time_date tmp) {

    // vypis aktualneho casu
    lcd.setCursor(0, 0);
    if (tmp.hour < 10) {
        lcd.print(F("0"));
        lcd.setCursor(1,0);
        lcd.print(tmp.hour);
    } else
        lcd.print(tmp.hour);
    
    lcd.setCursor(2, 0);
    lcd.print(F(":"));
    
    lcd.setCursor(3,0);
    if (tmp.min < 10) {
        lcd.print(F("0"));
        lcd.setCursor(4,0);
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
            lcd.setCursor(15,0);
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
        lcd.setCursor(3,1);
        lcd.print(start_svetlo.hour);
    } else 
        lcd.print(start_svetlo.hour);

    lcd.setCursor(4, 1);
    lcd.print(F(":"));

    lcd.setCursor(5, 1);
    if (start_svetlo.min < 10) {
        lcd.print(F("0"));
        lcd.setCursor(6,1);
        lcd.print(start_svetlo.min);
    } else
        lcd.print(start_svetlo.min);

    lcd.setCursor(9, 1);
    lcd.print(F("K-"));

    lcd.setCursor(11, 1);
    if (end_svetlo.hour < 10) { 
        lcd.print(F("0"));
        lcd.setCursor(12,1);
        lcd.print(end_svetlo.hour);
    } else
        lcd.print(end_svetlo.hour);

    lcd.setCursor(13, 1);
    lcd.print(F(":"));

    lcd.setCursor(14, 1);
    if (end_svetlo.min < 10) {
        lcd.print(F("0"));
        lcd.setCursor(15,1);
        lcd.print(end_svetlo.min);
    } else
        lcd.print(end_svetlo.min);

    //lcd.setCursor(8,1);
    //lcd.write(0xFF);
}

/**
 * 
 * @info Vycitanie dat z EEPROM pamete
 * @param
 */
void read_data_eeprom(){
    
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

/**
 * 
 * @info Zapis dat do EEPROM pamete
 * @param
 */
void write_data_eeprom(){
    EEPROM.write(start_svetlo.a_hour,start_svetlo.hour);
    EEPROM.write(start_svetlo.a_min,start_svetlo.min);
    
    EEPROM.write(end_svetlo.a_hour,end_svetlo.hour);
    EEPROM.write(end_svetlo.a_min,end_svetlo.min);
    
    Serial.println(F("Write data to EEPROM"));
}

/**
 * 
 * @info Zapnutie podsvietenia LCD displeja
 * @param
 */
void LCD_svetlo_on() {
    Serial.println("LCD ON");
    digitalWrite(LCD_svetlo,HIGH);
    s_LCD_svetlo = true;
    uint8_t delay = 30;
    
    if (time.sec > 59-delay) {
        if (time.min == 59) {
            end_LCD_svetlo.sec = delay - (time.sec - delay);
            end_LCD_svetlo.min = 0;
            
            if (time.hour == 23)
                end_LCD_svetlo.hour = 0;
            else
                end_LCD_svetlo.hour = time.hour + 1;
            
        } else {
            end_LCD_svetlo.sec = delay - (60 - time.sec);
            end_LCD_svetlo.min = time.min +1;
            end_LCD_svetlo.hour = time.hour;
        }
    }else {
        end_LCD_svetlo.sec = time.sec + delay;
        end_LCD_svetlo.min = time.min;
        end_LCD_svetlo.hour = time.hour;
    }
    
    Serial.print(time.hour);
    Serial.print(":");
    Serial.print(time.min);
    Serial.print(":");
    Serial.println(time.sec);
    
    Serial.print(end_LCD_svetlo.hour);
    Serial.print(":");
    Serial.print(end_LCD_svetlo.min);
    Serial.print(":");
    Serial.println(end_LCD_svetlo.sec);
}

/**
 * 
 * @info Vypnutie podsvietenia LCD displeja
 * @param
 */
void LCD_svetlo_off() {
    Serial.println("LCD OFF");
    s_LCD_svetlo = false;
    digitalWrite(LCD_svetlo,LOW);
}