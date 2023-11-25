#include "myEnvSensors.h"

#include "tds_sensor.h"
#include "my_ms5837.h"
#include "ds12b20.h"
#include "turbidity.h"
#include "ph_sensor.h"

#include "settings.h"


#define ___DEBUG_MODE___



envSensorData envSensorData_stamp;

envSensorData get_envData_stamp()
{
  return envSensorData_stamp;
};

//////////////////////////////////////////////////////////////////////////////
///////////////////   COMMON SENSOR FUNCTIONS   //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void envSensors_init() {
  tds_init();
  ds12b20_init();
  ms5837_init();
  ph_init();
}

void envSensorsUpdate(envSensorData &data) {
  data.dsTemp = ds12b20_getTemp();

  ms5837_update();
  data.msPressure = get_pressure();
  data.msTemperature = get_temperature();
  data.msDepth = get_depth();
  data.msAltitude = get_altitude();
  
  
  data.tdsNormConductivity = get_normConductivity();
  data.tdsEC = get_EC(data.tdsNormConductivity);
  data.tdsTDS = get_TDS(data.tdsEC);

  data.PH = get_ph();
  data.Turbidity = get_turbidity();
  
}

void envSensorData_print(envSensorData &data) {
  Serial.print((String)"Temp C: " + data.dsTemp + " deg C\n" +
               "Pressure: " + data.msPressure + " mbar\n" +
               "Temperature: " + data.msTemperature + " deg C\n" +
               "Depth: " + data.msDepth + " m\n" +
               "Altitude: " + data.msAltitude + " m above mean sea level\n" +
               "S = " + data.tdsNormConductivity + " мСм/см\n" +
               "EC = " + data.tdsEC + " мСм/см\n" +
               "TDS = " + data.tdsTDS + " мг/л\n" +
               "PH = " + data.PH + "\n" +
               "Turbidity = " + data.Turbidity + " мг/л\n");
}


String envSensorData_toString(envSensorData &data) {
  String msg = "";
  msg += (String)"Temp C: " + data.dsTemp + " deg C\n" +
         "Pressure: " + data.msPressure + " mbar\n" +
         "Temperature: " + data.msTemperature + " deg C\n" +
         "Depth: " + data.msDepth + " m\n" +
         "Altitude: " + data.msAltitude + " m above mean sea level\n" +
         "S = " + data.tdsNormConductivity + " мСм/см\n" +
         "EC = " + data.tdsEC + " мСм/см\n" +
         "TDS = " + data.tdsTDS + " мг/л\n" +
         "PH = " + data.PH + "\n" +
         "Turbidity = " + data.Turbidity + " мг/л\n";
  return msg;
}



void print_dataframe_csvHeader(){
  String msg = "";
  msg += (String)"Temp" + ";" + "Pressure" + ";" +
          "Temperature" + ";" + "Depth" + ";" +
          "Altitude" + ";" + "S" + ";" + "EC" + ";" +
          "TDS" + ";" + "PH" + ";" + "Turbidity";
  //DPRINT("Dataframe headers: ");
  DPRINTLN(msg);
}

String dataframe_csvHeader_toString(){
  String msg = "";
  msg += (String)"Temp" + ";" + "Pressure" + ";" +
          "Temperature" + ";" + "Depth" + ";" +
          "Altitude" + ";" + "S" + ";" + "EC" + ";" +
          "TDS" + ";" + "PH" + ";" + "Turbidity";
  //DPRINT("Dataframe headers: ");
  return msg;
}


void print_dataframe_csvData(envSensorData &data){
  String msg = "";
  msg += (String)data.dsTemp + ";" + 
          data.msPressure + ";" +
          data.msTemperature + ";" + 
          data.msDepth + ";" +
          data.msAltitude + ";" + 
          data.tdsNormConductivity + ";" + 
          data.tdsEC  + ";" +
          data.tdsTDS + ";" + 
          data.PH + ";" + 
          data.Turbidity + ";";
  DPRINTLN(msg);
}

String dataframe_csvData_toString(envSensorData &data){
  String msg = "";
  msg += (String)data.dsTemp + ";" + 
          data.msPressure + ";" +
          data.msTemperature + ";" + 
          data.msDepth + ";" +
          data.msAltitude + ";" + 
          data.tdsNormConductivity + ";" + 
          data.tdsEC  + ";" +
          data.tdsTDS + ";" + 
          data.PH + ";" + 
          data.Turbidity + ";";
  return msg;
}

String get_sensorMsg(){
  envSensorData dataStamp = get_envData_stamp();
  envSensorsUpdate(dataStamp);
  return dataframe_csvData_toString(dataStamp);
}