#include "MS5837.h"

#include "settings.h"
#include "my_ms5837.h"



//////////////////////////////////////////////////////////////////////////////
////////////////////////////   MS5837   //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

MS5837 ms5837_sensor;

float get_pressure(){
    return ms5837_sensor.pressure();
}

float get_temperature(){
    return ms5837_sensor.temperature();
}

float get_depth(){
    return ms5837_sensor.depth();
}

float get_altitude(){
    return ms5837_sensor.altitude();
}


void ms5837_init() {
  while (!ms5837_sensor.init()) {
    DPRINTLN("Init failed!");
    DPRINTLN("Are SDA/SCL connected correctly?");
    DPRINTLN("Blue Robotics Bar30: White=SDA, Green=SCL");
    DPRINTLN("\n\n\n");
    delay(MS5837_ERR_INIT_TIMEOUT);
    #ifdef ___LAUNCH_WITHOUT_MS5837___
      break;
    #endif  
  }
}

void ms5837_update() {
  ms5837_sensor.setFluidDensity(997);
  ms5837_sensor.read();
}

float update_depth() {
  ms5837_update();
  return ms5837_sensor.depth();

}