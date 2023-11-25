#ifndef ___SD_CARD_H___
#define ___SD_CARD_H___

#include "myEnvSensors.h"

void sd_card_init();
void file_init();
void saveDataFrame(envSensorData df);
void sendDataFrame();

/*int get_DB_index();
void reset_DB();
envSensorData get_df_fromDB_byIndex(int i);
void db_saveDataFrame(envSensorData df);*/

#endif