#include <Arduino.h>

#include "ds12b20.h"

#include "settings.h"

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//////////////////////////////////////////////////////////////////////////////
///////////////////////   DS12B20   //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define DS12B20_SENSOR_ID 0

OneWire oneWire(DS12B20_1WIRE_BUS);
DallasTemperature ds12b20_sensor(&oneWire);

void ds12b20_init() {
  Wire.begin();
  ds12b20_sensor.begin();
}

float ds12b20_getTemp() {
  ds12b20_sensor.requestTemperatures();
  return ds12b20_sensor.getTempCByIndex(DS12B20_SENSOR_ID);
}