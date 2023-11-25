#include <Arduino.h>

#include "modules/ota.h"
#include "modules/settings.h"
#include "modules/buoyancy.h"
#include "modules/wifi_mqtt.h"
#include "modules/myEnvSensors.h"

#include "modules/mqtt_handlers.h"

#include "modules/sd_card.h"

#define OTA_DISABLED false;
#define OTA_ENABLED true;

bool ota_state = OTA_ENABLED;


void print_sensorData(String msg){
  Serial.println(msg);
}

bool serialPrintSensorData = false; 

void send_msg(String msg){
  if (serialPrintSensorData){
      print_sensorData(msg);
    }
    if (get_mqttMode() == MQTT_IS_ON){
      mqtt_send_sensorData(msg);
    }
}

void update_sensorData(){
  envSensorData dataStamp = get_envData_stamp();
  envSensorsUpdate(dataStamp);
  
  if (get_mqttMode() == MQTT_IS_ON){
    String msg = dataframe_csvData_toString(dataStamp);
    send_msg(msg);
  }
  else{
    sendDataFrame();
  }
  /*
    #ifdef ___DEBUG___
      msg = envSensorData_toString(dataStamp);
      DPRINTLN(msg);
    #endif
  */
}


void mqtt_send_startSettings(){
  String msg = (String) "Diving time = " + get_DivingTime() + 
               "Swimming time = " + get_SwimmingTime() + 
               "FloattingTime = " + get_FloattingTime() + 
               "Target depth = " + get_targetDepth();
  mqtt_log(msg);
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  //Serial.println(sizeof(envSensorData));
  setup_wifi();
  ota_init();
  mqtt_reconnect();
  
  enders_init();
  envSensors_init();
  buoyancyServo_init();

  send_msg(dataframe_csvHeader_toString());

  mqtt_send_startSettings();
}


#define WAITING_STAGE      0
#define STARTING_STAGE     1
#define START_DIVING_STAGE 2
#define DIVING_STAGE       3
#define SWIMMING_STAGE     4
#define START_FLOATTING_STAGE    5
#define FLOATTING_STAGE    6
#define DATA_SEND          7

int stage = WAITING_STAGE;


uint32_t start_time = 0;
uint32_t start_floatting_time = 0;

void launch(){
  switch (stage){
    case STARTING_STAGE:{
      mqtt_send_startSettings();
      if (get_DivingTime() > 0 &&  
          get_SwimmingTime() > 0 &&
          get_FloattingTime() > 0 &&
          get_targetDepth() > 0){

        mqtt_log("Settings are OK");
        set_floattingDepth(get_depth());
        mqtt_log((String)"Floating depth = " + get_floattingDepth());
        mqtt_log("Starting..");

        //setMqttMode(0);
        //ota_state = 0;
        //stage = DIVING_STAGE;
        stage = START_DIVING_STAGE;
      }
      else{
        mqtt_log("Wrong settings. Cannot start!");  
        stage = WAITING_STAGE;
        set_startStatus(false);
      }
    }
    case  START_DIVING_STAGE:{
      start_time = millis();
      Serial.println("Diving Down!");
      mqtt_log("Start Diving Down!");  ////////////////////////////////////////////////////////////////////////////
      buoyancyServo_dive();
      stage = DIVING_STAGE;
    }
    case  DIVING_STAGE:{
      mqtt_log("Diving...");
      uint32_t now = millis();
      if (fabs(now - start_time) >= get_DivingTime()){
        stage = START_FLOATTING_STAGE;
        mqtt_log("diving is over!");
      }
    }
    case  START_FLOATTING_STAGE:{
      //delay(get_SwimmingTime());
      mqtt_log("start floatting");
      start_floatting_time = millis();
      buoyancyServo_float();
      stage = FLOATTING_STAGE;
    }
    case  FLOATTING_STAGE:{
      mqtt_log("floatting");
      uint32_t now = millis();
      if (fabs(now - start_floatting_time) >= get_FloattingTime()){
        mqtt_log("Floatting time is over!");
        DPRINTLN("Floatting time is over!");
        stage = DATA_SEND;
      }
    }
    case DATA_SEND:{
      mqtt_log("Start data sending!");
      sendDataFrame();
      DPRINTLN("FINISHED!!!!!!\n\n");
      stage = WAITING_STAGE;
    }
    /*case  DATA_SEND:{
      mqtt_log("Start data sending!");
      int DB_size = get_DB_index();
      envSensorData df;
      
      DPRINTLN("Data sending...");
      for (int i=0; i<DB_size;i++){
        df = get_df_fromDB_byIndex(i);
        String msg = dataframe_csvData_toString(df);
        send_msg(msg);
        DPRINTLN(msg);
      }
      reset_DB();        
      /////////////////////////////////  НАДО ВСТАВИТЬ ОТПРАВКУ ДАННЫХ С СД  /////////////////////////////////////

      DPRINTLN("FINISHED!!!!!!\n\n");
      stage = WAITING_STAGE;
    }*/
    case  WAITING_STAGE:{
      setMqttMode(1);
      ota_state = 1;
      delay(500);
      mqtt_log("Waiting stage!");
      DPRINTLN("Waiting stage!");
      set_startStatus(false);
      //stage = WAITING_STAGE;
    }
    default:{
      mqtt_log("wrong stage!");
      Serial.println("Error! WRONG Stage!");
      Serial.println(stage);
      stage = WAITING_STAGE;
      set_startStatus(false);
    }
  }
  


    
    
    set_startStatus(false);
}



void loop() {
  if (get_mqttMode() == MQTT_IS_ON){
    mqtt_routine();
  }
  
  if (ota_state){
    ota_tick();
  }
  

  static uint32_t envSensorLasttime = millis();
  uint32_t now = millis();

  if (fabs(now - envSensorLasttime) >= ENV_SENSORS_TASK_PERIOD_MS) {
    update_sensorData();
    envSensorLasttime = millis();
  }
  if (get_startStatus()){
    stage = STARTING_STAGE;
    launch();
  }
}
