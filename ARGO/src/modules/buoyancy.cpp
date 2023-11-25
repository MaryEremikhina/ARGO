#include <ESP32Servo.h>

#include "settings.h"
#include "buoyancy.h"


#define DOWN 0
#define STOP 90
#define UP   180

Servo buoyancyServo;

void enders_init(){
  pinMode(CON_UP_PIN, INPUT_PULLUP);
  pinMode(CON_DOWN_PIN, INPUT_PULLUP);
}

bool check_conUpEnder(){
  //DPRINT(".");
  bool upEnder = digitalRead(CON_UP_PIN);
  //DPRINT(upEnder);
  return upEnder;
}

bool check_conDownEnder(){
  //DPRINT(".");
  bool downEnder = digitalRead(CON_DOWN_PIN);
  //DPRINTLN(downEnder);
  return downEnder;
}

void buoyancyServo_init(){
 buoyancyServo.attach(BUOYANCY_SERVO_PIN, 544, 2400);
}

void buoyancyServo_detach(){
 buoyancyServo.detach();
}  

void buoyancyServo_dive(){
  DPRINT("BuoyancyServo diving start!");
  buoyancyServo.write(UP);
  DPRINT("Waiting for up ender....");
  while (!check_conUpEnder()){
    //DPRINT("Waiting for up ender....");
  }
  DPRINT("Up Ender was clicked!");
  buoyancyServo.write(STOP);
  DPRINT("Servo was stopped!!!");
}

void buoyancyServo_float(){
  DPRINT("BuoyancyServo floating start!");
  buoyancyServo.write(DOWN);
  DPRINT("Waiting for down ender....");
  while (!check_conDownEnder()){
    //DPRINT("Waiting for down ender....");
  }
  DPRINT("Down Ender was clicked!");
  buoyancyServo.write(STOP);
  DPRINT("Servo was stopped!!!");
}