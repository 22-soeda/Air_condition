#ifndef ENVSENSOR_H
#define ENVSENSOR_H

#include <M5Unified.h>
#include <M5UnitENV.h>

class EnvSensor {
private:
    SHT3X sht30;
    QMP6988 qmp6988;
    
    float currentTemp;
    float currentHum;
    float currentPressure;

public:
    EnvSensor();
    void begin();
    void update();
    float getTemperature();
    float getHumidity();
    float getPressure(); // 今後の拡張用に気圧も取得できるようにしておきます
};

#endif