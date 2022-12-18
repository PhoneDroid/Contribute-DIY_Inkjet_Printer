
#include <Arduino.h>

//define variables

#include <Wire.h>
#define tool_0 5
#define tool_1 4
#define tool_2 3
#define tool_3 2

boolean shoot_0 = false;
boolean shoot_1 = false;
boolean shoot_2 = false;
boolean shoot_3 = false;

boolean auto_shoot_0 = false;
boolean auto_shoot_1 = false;
boolean auto_shoot_2 = false;
boolean auto_shoot_3 = false;

enum Auto_Drop {on_0, off_0};
Auto_Drop auto_drop;

enum Drop_On_Demand {idle_1, on_1, off_1};
Drop_On_Demand drop_on_demand;

unsigned long saved_time_0;
unsigned long saved_time_1;


void onMessage ( int );


/**
 *  1. Start Serial
 *  2. Start I2C
 *  3. Set Pin Types
 */

void setup (){

    Serial.begin(9600);

    Wire.begin(9);
    Wire.onReceive(onMessage);

    pinMode(tool_0,OUTPUT);
    pinMode(tool_1,OUTPUT);
    pinMode(tool_2,OUTPUT);
    pinMode(tool_3,OUTPUT);
}


byte lastMessage (){

    byte message;

    while ( Wire.available() )
        message = Wire.read();

    return message;
}


//read i2c message
//check which tool bit is set

void onMessage ( int ){

    const byte message =
        lastMessage();


    if(bitRead(message,0)){
        drop_on_demand = on_1;
        shoot_0 = true;
    }

    if(bitRead(message,1)){
        drop_on_demand = on_1;
        shoot_1 = true;
    }

    if(bitRead(message,2)){
        drop_on_demand = on_1;
        shoot_2 = true;
    }

    if(bitRead(message,3)){
        drop_on_demand = on_1;
        shoot_3 = true;
    }

    auto_shoot_0 = bitRead(message,4);
    auto_shoot_1 = bitRead(message,5);
    auto_shoot_2 = bitRead(message,6);
    auto_shoot_3 = bitRead(message,7);
}


void dropAtFrequency ();
void dropOnDemand ();

void loop (){
    dropAtFrequency();
    dropOnDemand();
}


/**
 *  @brief  Eject drops on demand
 */

void dropOnDemand (){

    switch ( drop_on_demand ){
    case idle_1:

        saved_time_1 = millis();

        shoot_0 = false;
        shoot_1 = false;
        shoot_2 = false;
        shoot_3 = false;

        return;
    case on_1:

        if(millis() - saved_time_1 < 5){

            if(shoot_0)
                digitalWrite(tool_0,HIGH);

            if(shoot_1)
                digitalWrite(tool_1,HIGH);

            if(shoot_2)
                digitalWrite(tool_2,HIGH);

            if(shoot_3)
                digitalWrite(tool_3,HIGH);

        } else {
            drop_on_demand = off_1;
        }

        return;
    case off_1:

        if (millis() - saved_time_1 < 50){

            if(shoot_0)
                digitalWrite(tool_0,LOW);

            if(shoot_1)
                digitalWrite(tool_1,LOW);

            if(shoot_2)
                digitalWrite(tool_2,LOW);

            if(shoot_3)
                digitalWrite(tool_3,LOW);

        } else {
            drop_on_demand = idle_1;
        }

        return;
    }
}


/**
 *  @brief  Eject drops at a set frequency
 */

void dropAtFrequency (){

    switch ( auto_drop ){
    case on_0 :

        if(millis() - saved_time_0 < 50){

            if(auto_shoot_0)
                digitalWrite(tool_0, HIGH);

            if(auto_shoot_1)
                digitalWrite(tool_1, HIGH);

            if(auto_shoot_2)
                digitalWrite(tool_2, HIGH);

            if(auto_shoot_3)
                digitalWrite(tool_3, HIGH);
        } else {
            auto_drop = off_0;
        }

        return;
    case off_0:

        if (millis() - saved_time_0 < 100){

            if(auto_shoot_0)
                digitalWrite(tool_0,LOW);

            if(auto_shoot_1)
                digitalWrite(tool_1,LOW);

            if(auto_shoot_2)
                digitalWrite(tool_2,LOW);

            if(auto_shoot_3)
                digitalWrite(tool_3,LOW);

        } else {
            auto_drop = on_0;
            saved_time_0 = millis();
        }

        return;
    }
}
