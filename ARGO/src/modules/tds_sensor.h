#ifndef ___TDS_SENSOR_H___
#define ___TDS_SENSOR_H___

float get_TDS(float EC);
float get_EC(float normConductivity);
float get_normConductivity();
//float get_normConductivity(float rawConductivity);

#endif