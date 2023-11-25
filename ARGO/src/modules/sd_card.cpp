
#include <SPI.h>
#include <SD.h>

#include "myEnvSensors.h"
#include "settings.h"
#include "wifi_mqtt.h"

#include "sd_card.h"

File myFile;

void sd_card_init(){
    pinMode(PIN_CHIP, OUTPUT);
    while (!SD.begin(PIN_CHIP)) {
        DPRINTLN("Card failed, or not present");
        // Если что-то пошло не так, завершаем работу:
        delay(100);
    }
    DPRINTLN("card initialized.");
}

void file_init(){
    DPRINTLN(F("esp32.csv doesn't exist. Creating esp32.csv file..."));
    // create a new file by opening a new file and immediately close it
    myFile = SD.open("/esp32.csv", FILE_WRITE);
    myFile.println(dataframe_csvHeader_toString());
    //myFile.println("Temperature; ;Pressure;Temperature;Depth;Altitude; ;PH; ;NormConductivity;EC;TDS")
    DPRINTLN("file was opened");
    myFile.close();
}

void saveDataFrame(envSensorData df){
    if (SD.exists("/esp32.csv")){
        myFile = SD.open("/esp32.csv", FILE_WRITE);
        myFile.println(dataframe_csvData_toString(df));
        DPRINTLN("data was received");
    }
    else {
        DPRINTLN(F("esp32.csv doesn't exist on SD Card."));
    }
}

void sendDataFrame(){
    if (myFile) {
        String msg;
        while (myFile.available()) {
            String data = String(myFile.read()); // read characters one by one from Micro SD Card
            //DPRINT(data); // print the character to Serial Monitor
            if (data == "\n") {
                mqtt_send_sensorData(msg);
                msg = "";
            }
            else {
                msg += data;
            }
        }
        mqtt_send_sensorData(msg);
        myFile.close();
    } else {
        Serial.print(F("SD Card: Issue encountered while attempting to open the file esp32.txt"));
    }
}


/*envSensorData dataBuffer[300];

int index_db = 0;

void db_saveDataFrame(envSensorData df){
    dataBuffer[index_db] = df;
    index_db++;
}

envSensorData get_df_fromDB_byIndex(int i){
    return dataBuffer[0];
}

void reset_DB(){
    index_db=0;
}

int get_DB_index(){
    return index_db;
}*/
