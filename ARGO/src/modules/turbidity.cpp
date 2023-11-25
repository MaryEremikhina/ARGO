#include <Arduino.h>

#include "turbidity.h"
#include "settings.h"

/////////////////////////////////////////////////////////////////////////////
//////////////////////  TURBIDITY SENSOR  ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


int get_turbidity(){
  return analogRead(TURBIDITY_PIN);
}