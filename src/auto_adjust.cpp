#include <M5Unified.h>
#include "EnvSensor.h"
#include "DisplayUi.h"

EnvSensor envSensor;
DisplayUi displayUi;

unsigned long lastUpdate = 0;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    // クラスの初期化
    envSensor.begin();
    displayUi.begin();
}

void loop() {
    M5.update();

    // 2秒ごとにセンサーから値を取得して画面を更新
    if (millis() - lastUpdate > 2000) {
        lastUpdate = millis();

        envSensor.update();
        float t = envSensor.getTemperature();
        float h = envSensor.getHumidity();

        // テスト用のダミーデータ（時刻、天気、AC状態）
        String dummyTime = "12:34";
        String dummyWeather = "Clear";
        
        // 温度に応じて表示を変えてみるテスト
        String dummyAcState = "OFF";
        if (t >= 28.0) dummyAcState = "COOL 28C";
        if (t <= 24.0) dummyAcState = "HEAT 24C";

        // 画面に描画
        displayUi.draw(dummyTime, dummyWeather, t, h, dummyAcState);
    }
}