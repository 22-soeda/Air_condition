#include <M5Unified.h>
#include "EnvSensor.h"
#include "DisplayUi.h"
#include "WeatherClock.h"
#include "IrController.h"
#include "wifi_config.h"

// -----------------------------------------
// 各クラスのインスタンス化
// -----------------------------------------
EnvSensor envSensor;
DisplayUi displayUi;
WeatherClock weatherClock;
IrController acController(26);

// -----------------------------------------
// 状態管理用の変数
// -----------------------------------------
enum ACState { OFF, COOLING, HEATING };
ACState currentAcState = OFF;

unsigned long lastUpdate = 0;              // ループ更新用のタイマー
unsigned long inRangeStartTime = 0;        // 適温範囲に入った時刻
bool isTemperatureInRange = false;         // 現在適温範囲にいるかどうかのフラグ

// 10秒に1回ロジックを判定・画面更新する
const unsigned long UPDATE_INTERVAL = 10000; 

// 30分 = 30 * 60 * 1000 ミリ秒
const unsigned long TARGET_KEEP_TIME = 1800000; 

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    Serial.begin(115200);
    Serial.println("System Starting...");

    // UIの初期化と待機画面
    displayUi.begin();
    displayUi.draw("--:--", "Connecting...", 0.0, 0.0, "WAIT");

    // 各モジュールの初期化
    Serial.println("Init EnvSensor...");
    envSensor.begin();

    Serial.println("Init IR Controller...");
    acController.begin();

    Serial.println("Init WiFi & Time...");
    weatherClock.begin(WIFI_SSID, WIFI_PASS, OWM_API_KEY);

    // 起動時は念のためエアコンを停止状態にする
    acController.sendStop();
    currentAcState = OFF;

    Serial.println("Setup Complete!");
}

void loop() {
    M5.update();
    
    // Wi-Fiによる時刻と天気のバックグラウンド更新
    weatherClock.update();

    // 10秒ごとにセンサー値の取得とエアコン制御ロジックを実行
    if (millis() - lastUpdate > UPDATE_INTERVAL) {
        lastUpdate = millis();

        // 1. センサー値の更新
        envSensor.update();
        float temp = envSensor.getTemperature();
        float hum = envSensor.getHumidity();

        // 2. エアコンの自動制御ロジック
        if (temp >= 28.0) {
            // 28℃以上なら冷房ON
            if (currentAcState != COOLING) {
                acController.sendCool28();
                currentAcState = COOLING;
            }
            isTemperatureInRange = false;

        } else if (temp <= 24.0) {
            // 24℃以下なら暖房ON
            if (currentAcState != HEATING) {
                acController.sendHeat24();
                currentAcState = HEATING;
            }
            isTemperatureInRange = false;

        } else if (temp > 24.0 && temp < 28.0) {
            // 25℃〜27℃の適温範囲内に入った場合
            if (!isTemperatureInRange) {
                // 初めて範囲に入った瞬間、タイマーをスタート
                inRangeStartTime = millis();
                isTemperatureInRange = true;
                Serial.println("Temperature reached target range. Timer started.");
            } else {
                // すでに範囲内にいる場合、30分経過したかチェック
                if (millis() - inRangeStartTime >= TARGET_KEEP_TIME) {
                    // 30分経過し、かつエアコンが動いていれば停止する
                    if (currentAcState != OFF) {
                        acController.sendStop();
                        currentAcState = OFF;
                        Serial.println("30 minutes passed in target range. AC Stopped.");
                    }
                }
            }
        }

        // 3. 画面の描画更新
        String timeStr = weatherClock.getTimeString();
        String weatherStr = weatherClock.getWeatherString();
        
        String stateStr = "OFF";
        if (currentAcState == COOLING) stateStr = "COOL 28C";
        if (currentAcState == HEATING) stateStr = "HEAT 24C";

        displayUi.draw(timeStr, weatherStr, temp, hum, stateStr);
    }
}