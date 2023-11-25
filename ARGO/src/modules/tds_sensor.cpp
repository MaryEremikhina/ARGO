#include <Arduino.h>

#include "settings.h"
#include "tds_sensor.h"

//////////////////////////////////////////////////////////////////////////////
///////////////////////   TDS SENSOR   ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define ADC_MAX_VALUE 4095

const float   VCC    = 3.3f;                      // Напряжение питания ESR32.
const float   Vccm   = 3.3f;                      // Напряжение питания ОУ модуля        (внутрисхемно используется 3,3В).
const float   Ka     = 1000.0f;                   // Множитель степенной функции         (определяется калибровкой модуля). Можно получить функцией getKa().
const float   Kb     = -5.0f;                     // Степень степенной функции           (определяется калибровкой модуля). Можно получить функцией getKb().
const float   Kt     = 0.02f;                     // Температурный коэффициент           (зависит от состава жидкости).
const float   Kp     = 0.5f;                      // Коэффициент пересчёта               (зависит от состава жидкости).
const float   Kf     = 0.85f;                     // Коэффициент передачи ФВЧ+ФНЧ модуля (зависит от частоты переменного тока используемого для проведения измерений).
const float   T      = 25.0f;                     // Опорная температура в °C.
const float   t      = 25.0f;                     // Температура исследуемой жидкости в °C.


void tds_init() {
  pinMode(TDS_PIN, INPUT);
}

int get_rawTDS() {
  return analogRead(TDS_PIN);
}

// Получаем удельную электропроводность
float get_conductivity(int rawTDS) {
  return float(rawTDS) * VCC / ADC_MAX_VALUE;
}

// Получаем удельную электропроводность
float get_conductivity() {
    int rawTDS = get_rawTDS();
    return float(rawTDS) * VCC / ADC_MAX_VALUE;
}

// Приводим удельную электропроводность в мСм/см
float get_normConductivity(float rawConductivity) {
  return Ka * pow((Vccm - Kf * rawConductivity) / 2, Kb) / 1000;
}

float get_normConductivity() {
    float rawConductivity = get_conductivity();
    return Ka * pow((Vccm - Kf * rawConductivity) / 2, Kb) / 1000;
}

// Получаем минерализацию жидкости в мСм/см
float get_EC(float normConductivity) {
  return normConductivity / (1 + Kt * (t - T));
}

// Получаем загрязнение в мг/л
float get_TDS(float EC) {
  return EC * Kp;
}