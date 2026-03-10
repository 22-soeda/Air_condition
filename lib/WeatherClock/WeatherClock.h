#ifndef WEATHERCLOCK_H
#define WEATHERCLOCK_H

#include <Arduino.h>

class WeatherClock {
private:
    String currentTime;
    String currentWeather;
    String apiKey;
    unsigned long lastWeatherUpdate;
    
    // 天気APIを叩く間隔（無料枠制限を超えないように10分に設定）
    const unsigned long WEATHER_UPDATE_INTERVAL = 10 * 60 * 1000; 

    void fetchWeather();

public:
    WeatherClock();
    void begin(const char* ssid, const char* pass, const char* key);
    void update();
    String getTimeString();
    String getWeatherString();
};

#endif