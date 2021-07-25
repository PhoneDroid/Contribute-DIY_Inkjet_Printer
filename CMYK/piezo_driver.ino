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
enum DoD_0 {idle_0, on_0, off_0};
DoD_0 dod_0;
enum DoD_1 {idle_1, on_1, off_1};
DoD_1 dod_1;
unsigned long saved_time_0;
unsigned long saved_time_1;
byte i2c_message;


//Start serial, start i2c, set L298N pins to output
void setup() {
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  pinMode(tool_0, OUTPUT);
  pinMode(tool_1, OUTPUT);
  pinMode(tool_2, OUTPUT);
  pinMode(tool_3, OUTPUT);
}


//read i2c message
//check which tool bit is set
void receiveEvent() {
  while (Wire.available()) {
    i2c_message = Wire.read();
  }

  if (bitRead(i2c_message, 0) == 1 ) {
    dod_1 = on_1;
    shoot_0 = true;
  }
  if (bitRead(i2c_message, 1) == 1 ) {
    dod_1 = on_1;
    shoot_1 = true;
  }
  if (bitRead(i2c_message, 2) == 1 ) {
    shoot_2 = true;
    dod_1 = on_1;
  }
  if (bitRead(i2c_message, 3) == 1 ) {
    shoot_3 = true;
    dod_1 = on_1;
  }
  if (bitRead(i2c_message, 4) == 1 ) {
    dod_0 = on_0;
  }
  if (bitRead(i2c_message, 4) == 0 ) {
    dod_0 = idle_0;
  }
}


//to start the printhead from printer menu
void loop() {
  switch (dod_0) {
    case idle_0:
      break;
    case on_0:
      if (millis() - saved_time_0 < 100) {
        digitalWrite(tool_0, HIGH);
        digitalWrite(tool_1, HIGH);
        digitalWrite(tool_2, HIGH);
        digitalWrite(tool_3, HIGH);
      }
      else {
        dod_0 = off_0;
      }
      break;
    case off_0:
      if (millis() - saved_time_0 < 200) {
        digitalWrite(tool_0, LOW);
        digitalWrite(tool_1, LOW);
        digitalWrite(tool_2, LOW);
        digitalWrite(tool_3, LOW);
      }
      else
      {
        dod_0 = on_0;
        saved_time_0 = millis();
      }
      break;
  }


  // eject drop on demand
  switch (dod_1) {
    case idle_1:
      saved_time_1 = millis();
      shoot_0 = false;
      shoot_1 = false;
      shoot_2 = false;
      shoot_3 = false;
      break;
    case on_1:
      if (millis() - saved_time_1 < 5) {
        if (shoot_0 == true) {
          digitalWrite(tool_0, HIGH);
        }
        if (shoot_1 == true) {
          digitalWrite(tool_1, HIGH);
        }
        if (shoot_2 == true) {
          digitalWrite(tool_2, HIGH);
        }
        if (shoot_3 == true) {
          digitalWrite(tool_3, HIGH);
        }
      }
      else {
        dod_1 = off_1;
      }
      break;
    case off_1:
      if (millis() - saved_time_1 < 50) {
        if (shoot_0 == true) {
          digitalWrite(tool_0, LOW);
        }
        if (shoot_1 == true) {
          digitalWrite(tool_1, LOW);
        }
        if (shoot_2 == true) {
          digitalWrite(tool_2, LOW);
        }
        if (shoot_3 == true) {
          digitalWrite(tool_3, LOW);
        }
      }
      else
      {
        dod_1 = idle_1;
      }
      break;
  }
}
