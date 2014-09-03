#include "RTC.h"

//---------------------------------------
// Definitia konstant pre I/O
#define r_svetlo 2  // vystup pre rele svetla
#define r_filter 3  // vystup pre rele filtra
#define l_status 7  // led status operacie
#define off_1 4     // tlacidlo pre prve oneskorenie
#define off_2 5     // tlacidlo pre druhe oneskorenie
#define svetlo 6    // tlacidlo pre zmenu stavu svetla

//--------------------------------------
// Definicia konstant

time_date odklad = {0,0,0,0,0,0,0};
time_date start_svetlo = {8,0,0,0,0,0,0};
time_date end_svetlo = {18,0,0,0,0,0,0};
#define odklad_1 15         // konstanta prec prvy casovy odklad
#define odklad_2 60         // konstanta pre druhy casovy odklad
#define refresh_cycle 25    // konstanta poctu opakovania pre refresh casu z RTC

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

//--------------------------------------
void setup() {
    
    //initial I/O
    pinMode(r_svetlo,OUTPUT);
    pinMode(r_filter,OUTPUT);
    pinMode(l_status,OUTPUT);
    pinMode(off_1,INPUT);
    pinMode(off_2,INPUT);
    pinMode(svetlo,INPUT);            
   
    digitalWrite(l_status,LOW);
    digitalWrite(r_svetlo,LOW);
    digitalWrite(r_filter,HIGH);
    s_filter = true;
    
  // initial I2C
  Wire.begin();

}

void loop() {
    // refresh time 
    if (refresh_time == refresh_cycle) {
        time = readTime();
        refresh_time = 0;
    }
    
    // kontrola zapnutia svetla
    if ((start_svetlo.hour == time.hour ) && (start_svetlo.min == time.min)) {
        if (!s_svetlo)
            svetlo_on();
    }
    
    // kontrola vypnutia svetla
    if ((end_svetlo.hour == time.hour) && (end_svetlo.min == time.min)) {
        if (s_svetlo)
            svetlo_off();
    }
    
    // kontrola zapnutia filtra
    if ((odklad.hour == time.hour) && (odklad.min == time.min)) {
        if (!s_filter)
            filter_on();
    } 
    
    // nacitanie vstupu pre zmenu stavu svetla
    if (digitalRead(svetlo)) {
        status_led();
        
        if (s_svetlo)
            svetlo_off();
        else
            svetlo_on();
    }
    
    // nacitanie stavu pre prvy odklad filtra
    if (digitalRead(off_1)) {
        status_led();
        
        if ((59 - time.min) <=  odklad_1){
            odklad.hour = time.hour++;
            odklad.min = 15 - (59 - time.min);
        }
        else {
            odklad.min = time.min + odklad_1;
            odklad.hour = time.hour;
        }
        filter_off();
    }
    
    //nacitanie stavu pre druhy odklad filtra
    if (digitalRead(off_2)) {
        status_led();
        
        odklad.hour = time.hour + odklad_2;
        odklad.min = 0;
        odklad.sec = 0;
        filter_off();
    }
    
    delay(1000);
    refresh_time++;
    status_led();
}

/**
 *
 * @info zapne svetlo
 * @return void
 */
void svetlo_on() {
    s_svetlo = true;
    digitalWrite(r_svetlo,true);
}

/**
 *
 * @info vypne svetlo
 * @return void
 */
void svetlo_off() {
    s_svetlo = false;
    digitalWrite(r_svetlo,false);
}

/**
 *
 * @info zapne filter
 * @return void
 */
void filter_on(){
    s_filter = true;
    digitalWrite(r_filter,true);
}

/**
 *
 * @info vypne filter
 * @return void
 */
void filter_off() {
    s_filter = false;
    digitalWrite(r_filter,false);
}

/**
 *
 * @info zisti stav ledky, a zneguje ho
 * @return void
 */
void status_led() {
    if (s_led){
        s_led = false;
        digitalWrite(l_status,false);
    }
    else {
        s_led = true;
        digitalWrite(l_status,true);
    }
}