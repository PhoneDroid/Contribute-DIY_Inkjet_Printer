
#include <Arduino.h>

//define variables

#include <Wire.h>
#define tool_0 5
#define tool_1 4
#define tool_2 3
#define tool_3 2

boolean

    drop_atFrequency = true ,
    drop_onDemand = false ,

    auto_shoot_0 ,
    auto_shoot_1 ,
    auto_shoot_2 ,
    auto_shoot_3 ,

    shoot_0 ,
    shoot_1 ,
    shoot_2 ,
    shoot_3 ,

    idle = true ;


unsigned long
    saved_time_0 ,
    saved_time_1 ;


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


void demandDropping ();


/**
 *  @brief  Received I2C messages determine which tool bit is set
 */

void onMessage ( int ){

    const byte message =
        lastMessage();


    if(bitRead(message,0))
        shoot_0 = true;

    if(bitRead(message,1))
        shoot_1 = true;

    if(bitRead(message,2))
        shoot_2 = true;

    if(bitRead(message,3))
        shoot_3 = true;

    if(shoot_0 || shoot_1 || shoot_2 || shoot_3)
        demandDropping();

    auto_shoot_0 = bitRead(message,4);
    auto_shoot_1 = bitRead(message,5);
    auto_shoot_2 = bitRead(message,6);
    auto_shoot_3 = bitRead(message,7);
}


void dropAtFrequency ();
void dropOnDemand ();

void loop (){

    dropAtFrequency();

    if(drop_onDemand)
        dropOnDemand();
}


void drop ( boolean state ){

    if(shoot_0)
        digitalWrite(tool_0,state);

    if(shoot_1)
        digitalWrite(tool_1,state);

    if(shoot_2)
        digitalWrite(tool_2,state);

    if(shoot_3)
        digitalWrite(tool_3,state);
}



void demandDropping (){

    drop(true);

    saved_time_1 = millis();
    drop_onDemand = true;
}


/**
 *  @brief  Eject drops on demand
 */

void dropOnDemand (){

    const long delta =
        millis() - saved_time_1;

    if(delta < 5)
        return;

    drop(false);

    if(delta < 50)
        return;

    drop_onDemand = false;

    shoot_0
    = shoot_1
    = shoot_2
    = shoot_3
    = false;
}




/**
 *  @brief  Eject drops at a set frequency
 */

void dropAtFrequency (){

    const long delta =
        millis() - saved_time_0;


    if(drop_atFrequency){

        if(delta <= 50){
            drop_atFrequency = false;
            return;
        }

        if(auto_shoot_0)
            digitalWrite(tool_0,HIGH);

        if(auto_shoot_1)
            digitalWrite(tool_1,HIGH);

        if(auto_shoot_2)
            digitalWrite(tool_2,HIGH);

        if(auto_shoot_3)
            digitalWrite(tool_3,HIGH);

        return;
    }


    if (delta >= 100){
        drop_atFrequency = true;
        saved_time_0 = millis();
        return;
    }

    if(auto_shoot_0)
        digitalWrite(tool_0,LOW);

    if(auto_shoot_1)
        digitalWrite(tool_1,LOW);

    if(auto_shoot_2)
        digitalWrite(tool_2,LOW);

    if(auto_shoot_3)
        digitalWrite(tool_3,LOW);
}
