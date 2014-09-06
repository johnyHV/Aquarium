#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTC.h"

//---------------------------------------
// Definitia konstant pre I/O
#define r_svetlo 2  // vystup pre rele svetla
#define r_filter 3  // vystup pre rele filtra
#define l_status 13  // led status operacie
#define l_filter 1  // led status vypnuteho filtra
#define off_1 4     // tlacidlo pre prve oneskorenie
#define off_2 5     // tlacidlo pre druhe oneskorenie
#define enter 6     // tlacidlo pre zmenu stavu svetla

//--------------------------------------
// Definicia konstant

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
time_date odklad = {0, 0, 0, 0, 0, 0, 0};
static time_date start_svetlo = {8, 0, 0, 0, 0, 0, 0};
static time_date end_svetlo = {18, 0, 0, 0, 0, 0, 0};
static time_date odklad_1 = {0, 15, 0, 0, 0, 0, 0}; // posun o 15min
static time_date odklad_2 = {1, 0, 0, 0, 0, 0, 0};  // posun o 1hod
#define refresh_cycle 25                            // konstanta poctu opakovania pre refresh casu z RTC

//--------------------------------------
// definicia premennych
time_date time;
bool s_svetlo = false;
bool s_filter = false;
bool s_led = false;
uint8_t refresh_time = 0;

// funkcie
void svetlo_on();
void svetlo_off();
void filter_on();
void filter_off();
void status_led();
void posun(time_date tmp);

//--------------------------------------

void setup() {
    // initial UART
    Serial.begin(9600);
    Serial.println("Start");
    time = readTime();
    printTime(time);

    // initial I2C
    Wire.begin();
    
    // Initial LCD
    lcd.begin(16, 2);
    
    //initial I/O
    pinMode(r_svetlo, OUTPUT);
    pinMode(r_filter, OUTPUT);
    pinMode(l_status, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(off_1, INPUT);
    pinMode(off_2, INPUT);
    pinMode(enter, INPUT);

    digitalWrite(l_status, LOW);
    svetlo_off();
    filter_on();

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
            Serial.print(":");
            Serial.println(odklad.min);
        }
        if (incomingByte == 'j') {
            posun(odklad_2);
            filter_off();
            Serial.print("Znova zapnutie filtra o: ");
            Serial.print(odklad.hour);
            Serial.print(":");
            Serial.println(odklad.min);
        }
        if (incomingByte == 'k') {
            setTimeUart();
        }
        if (incomingByte == 'l') {
            Serial.println("Setting Time");
            time_date set_time_ = {23, 58, 00, 4, 4, 9, 14};
            setTime(set_time_);

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

    // kontrola zapnutia filtra
    if ((odklad.hour == time.hour) && (odklad.min == time.min)) {
        if (!s_filter)
            filter_on();
    }

    /*
    // nacitanie vstupu pre zmenu stavu svetla
    if (!digitalRead(svetlo)) {
        status_led();

        if (s_svetlo)
            svetlo_off();
        else
            svetlo_on();
    }

    // nacitanie stavu pre prvy odklad filtra
    if ((!digitalRead(off_1))) {
        status_led();
        posun(odklad_1);
        filter_off();
    }

    //nacitanie stavu pre druhy odklad filtra
    if (!digitalRead(off_2)) {

        status_led();
        posun(odklad_2);
        filter_off();
    }

     */

    delay(1000);
}

/**
 *
 * @info zapne svetlo
 * @return void
 */
void svetlo_on() {

    s_svetlo = true;
    digitalWrite(r_svetlo, false);
    Serial.print("Svetlo ON - ");
    Serial.print(time.hour);
    Serial.print(":");
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
    Serial.print("Svetlo OFF - ");
    Serial.print(time.hour);
    Serial.print(":");
    Serial.println(time.min);
}

/**
 *
 * @info zapne filter
 * @return void
 */
void filter_on() {

    s_filter = true;
    digitalWrite(r_filter, false);
    digitalWrite(l_filter, true);
    Serial.print("Filter ON - ");
    Serial.print(time.hour);
    Serial.print(":");
    Serial.println(time.min);
}

/**
 *
 * @info vypne filter
 * @return void
 */
void filter_off() {

    s_filter = false;
    digitalWrite(r_filter, true);
    digitalWrite(l_filter, false);
    Serial.print("Filter OFF - ");
    Serial.print(time.hour);
    Serial.print(":");
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
