#ifndef ___MQTT_HANDLERS_H___
#define ___MQTT_HANDLERS_H___

#include <Arduino.h>

void mqtt_start_handler(String msg);

void mqtt_reboot_handler(String msg);
void mqtt_resetMqtt_handler(String msg);
void mqtt_resetWifi_handler(String msg);

bool get_startStatus();
void set_startStatus(bool new_startStatus);

uint32_t get_DivingTime();
void mqtt_setDivingTime_handler(String msg);
uint32_t get_SwimmingTime();
void mqtt_setSwimmingTime_handler(String msg);
uint32_t get_FloattingTime();
void mqtt_setFloattingTime_handler(String msg);
float get_targetDepth();
void set_targetDepth(float new_targetDepth);
float get_floattingDepth();
void set_floattingDepth(float new_floattingDepth);

void mqtt_setTargetDepth_handler(String msg);
void mqtt_setServo_handler(String msg);




#endif