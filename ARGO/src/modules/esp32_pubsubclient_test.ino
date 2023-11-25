#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

#include "buoyancy.h"
#include "myEnvSensors.h"

#define SERIAL_SPEED 115200

#define WIFI_SSID "G"
#define WIFI_PSWD "12344321"

#define MQTT_HOST "192.168.57.128"
#define MQTT_PORT 1883
#define MQTT_USER "admin"
#define MQTT_PSWD "12344321"
#define MQTT_DEVICE_ID "esp32_argo-1"

#define MQTT_TEST_TOPIC "test"
#define HELLO_MSG "Hello World!!!"

#define MQTT_SET_SERVO "setServo"

void callback(char* topic, byte* message, unsigned int length);
 
WiFiClient espClient;
PubSubClient client(MQTT_HOST,
     MQTT_PORT,
     callback,
     espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


bool mqtt_connect(){
 bool result = 0;
 if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PSWD)) {
  client.publish(MQTT_TEST_TOPIC, HELLO_MSG);
  client.subscribe(MQTT_TEST_TOPIC);
  client.subscribe(MQTT_SET_SERVO);
  result = 1;
 }
 return result;
}


void mqtt_reconnect() {
  bool fl = 0;
  while (!fl) {
    Serial.print("Attempting MQTT connection...");
    
    if (mqtt_connect()) {
   fl = 1;
      Serial.println("connected!");
      Serial.println("subscribed!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}




void setup_wifi() {
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}






void callback(char* topic, byte* message, unsigned int length) {
 String topicStr = topic;
 //String msgStr = (char*)message;  
  
 Serial.print("Message arrived on topic: ");
 Serial.print(topic);
 Serial.print(". Message: ");
 String messageTemp;

 for (int i = 0; i < length; i++) {
  Serial.print((char)message[i]);
  messageTemp += (char)message[i];
 }

 String msgStr = messageTemp;
 Serial.println();
  
 if (topicStr == MQTT_SET_SERVO){
  Serial.print("Received Servo command:");
  Serial.println(msgStr);
  if (msgStr == "Up"){
   Serial.println("Floating Up!");
   buoyancyServo_float();
  }
  else if (msgStr == "Down"){
   Serial.println("Diving Down!");
   buoyancyServo_dive();
  }
  else{
   Serial.println("Wrong servo-command ");
  }
 }
}


#define ENV_SENSOR_PERIOD_MS 1000


void setup() {
  Serial.begin(SERIAL_SPEED);
  setup_wifi();
  
  mqtt_reconnect();
  
  envSensors_init();
  buoyancyServo_init();
  enders_init();
}

void loop() {
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();

  
  static uint32_t envSensorLasttime = millis();
  uint32_t now = millis();
  if (fabs(now - envSensorLasttime) >= ENV_SENSOR_PERIOD_MS) { //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Костыль Розы! abs() заменила на fabs()
	envSensorData dataStamp = get_envData_stamp();
	envSensorsUpdate(dataStamp);
	String msg = envSensorData_toString(dataStamp);
	Serial.print(msg);
	client.publish(MQTT_TEST_TOPIC, msg.c_str());
	envSensorLasttime = millis();
  }
}