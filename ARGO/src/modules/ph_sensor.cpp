#include <Arduino.h>

#include "settings.h"
#include "ph_sensor.h"

//////////////////////////////////////////////////////////////////////////////
////////////////////////////  PH-METER  /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void ph_init() {
  pinMode(PH_PIN, INPUT);
}

float get_ph() {
  int phRaw = analogRead(PH_PIN);
  float phValue = (float)phRaw * 3.3 / 4096 / 6; //convert the analog into millivolt
  return 3.5 * phValue; //convert the millivolt into pH value
}