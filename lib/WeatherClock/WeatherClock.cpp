#include "WeatherClock.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // JSONパース用
#include <time.h>

WeatherClock::WeatherClock() {
    currentTime = "--:--";
    currentWeather = "---";
    lastWeatherUpdate = 0;
}

void WeatherClock::begin(const char* ssid, const char* pass, const char* key) {
    apiKey = key;
    
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, pass);
    
    // Wi-Fi接続待ち
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // NTPサーバーと同期して時刻を設定 (JST: 日本標準時 = UTC+9時間 = 32400秒)
    configTime(9 * 3600, 0, "ntp.nict.jp", "time.google.com");
    
    // 初回の時刻取得待ち
    struct tm timeinfo;
    Serial.print("Waiting for NTP time sync");
    while (!getLocalTime(&timeinfo, 5000)) {
        Serial.print(".");
    }
    Serial.println("\nTime synchronized!");

    // 初回の天気取得
    fetchWeather();
}

void WeatherClock::update() {
    // 1. 内部時計（RTC）から現在時刻を更新
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        currentTime = String(timeStr);
    }

    // 2. 指定時間(10分)ごとに天気APIを叩いて更新
    if (millis() - lastWeatherUpdate > WEATHER_UPDATE_INTERVAL) {
        fetchWeather();
        lastWeatherUpdate = millis();
    }
}

void WeatherClock::fetchWeather() {
    if (WiFi.status() != WL_CONNECTED) return;

    HTTPClient http;
    // 大田区南千束付近の緯度(35.602)・経度(139.694)を指定
    String url = "http://api.openweathermap.org/data/2.5/weather?lat=35.602&lon=139.694&appid=" + apiKey + "&units=metric";
    
    http.begin(url);
    int httpCode = http.GET(); // APIにリクエスト送信

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            // 天気のメイン状態 (Clear, Rain, Clouds, Snow など) を抽出
            const char* mainWeather = doc["weather"][0]["main"];
            currentWeather = String(mainWeather);
            Serial.println("Weather updated: " + currentWeather);
        } else {
            Serial.println("JSON Parse failed.");
        }
    } else {
        Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}

String WeatherClock::getTimeString() {
    return currentTime;
}

String WeatherClock::getWeatherString() {
    return currentWeather;
}