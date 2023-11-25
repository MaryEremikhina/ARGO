#ifndef ___WIFI_MQTT_H___
#define ___WIFI_MQTT_H___


#define WIFI_IS_OFF 0x0
#define WIFI_IS_ON 0x1
#define MQTT_IS_OFF 0x0
#define MQTT_IS_ON 0x1

bool get_wifiMode();
void setWifiMode(bool new_wifiMode);

bool get_mqttMode();
void setMqttMode(bool new_mqttMode);

bool check_wifiConnection();
bool check_mqtt_connection();

void mqtt_log(String msg);

void mqtt_tick();
void mqtt_routine();

void stop_wifi();
void setup_wifi();
void mqtt_reconnect();

void mqtt_send_sensorData(String msg);

#endif