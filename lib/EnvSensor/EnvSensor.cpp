#include "EnvSensor.h"

EnvSensor::EnvSensor() {
    currentTemp = 0.0f;
    currentHum = 0.0f;
    currentPressure = 0.0f;
}

void EnvSensor::begin() {
    // M5Stack Basic の Port A (I2C) のピン番号
    const uint8_t SDA_PIN = 21;
    const uint8_t SCL_PIN = 22;
    const uint32_t I2C_FREQ = 400000;

    // ENV III の I2Cアドレス
    const uint8_t SHT30_ADDR = 0x44;
    const uint8_t QMP6988_ADDR = 0x70;

    Wire.begin(SDA_PIN, SCL_PIN, I2C_FREQ);

    // センサーの初期化
    if (!sht30.begin(&Wire, SHT30_ADDR, SDA_PIN, SCL_PIN, I2C_FREQ)) {
        Serial.println("SHT30 initialization failed.");
    }
    
    if (!qmp6988.begin(&Wire, QMP6988_ADDR, SDA_PIN, SCL_PIN, I2C_FREQ)) {
        Serial.println("QMP6988 initialization failed.");
    }
}

void EnvSensor::update() {
    sht30.update();
    qmp6988.update();

    currentTemp = sht30.cTemp;
    currentHum = sht30.humidity;
    currentPressure = qmp6988.pressure / 100.0f; // Pa から hPa に変換
}

float EnvSensor::getTemperature() {
    return currentTemp;
}

float EnvSensor::getHumidity() {
    return currentHum;
}

float EnvSensor::getPressure() {
    return currentPressure;
}