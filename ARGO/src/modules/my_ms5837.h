#ifndef ___MY_MS5837_H___
#define ___MY_MS5837_H___


void ms5837_update();
void ms5837_init();
float update_depth();

float get_pressure();
float get_temperature();
float get_depth();
float get_altitude();

#endif