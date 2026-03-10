#include "DisplayUi.h"

DisplayUi::DisplayUi() : sprite(&M5.Display) {
    // コンストラクタでの初期化が必要な場合はここに記述
}

void DisplayUi::begin() {
    // スプライトの設定
    // M5Stack Basicの画面サイズ(320x240)に合わせてメモリを確保
    sprite.createSprite(320, 240);
    sprite.setTextDatum(top_left); // テキストの描画基準を左上に
}

void DisplayUi::draw(String time, String weather, float temp, float hum, String acState) {
    // 1. スプライト（裏画面）を黒で塗りつぶす
    sprite.fillScreen(TFT_BLACK);

    // 2. 枠線や固定のタイトルを描画
    sprite.drawRect(0, 0, 320, 240, TFT_WHITE);
    sprite.drawFastHLine(0, 40, 320, TFT_WHITE); // 上部の区切り線
    
    // タイトル
    sprite.setTextColor(TFT_CYAN);
    sprite.setFont(&fonts::FreeSansBold12pt7b);
    sprite.setCursor(10, 10);
    sprite.print("Smart AC Controller");

    // 3. 変化する情報（時刻・天気）の描画
    sprite.setTextColor(TFT_WHITE);
    sprite.setFont(&fonts::FreeSans12pt7b);
    
    sprite.setCursor(10, 60);
    sprite.printf("Time: %s", time.c_str());
    
    sprite.setCursor(10, 95);
    sprite.printf("Weather: %s", weather.c_str());

    // 4. センサー情報（温度・湿度）の描画
    sprite.setTextColor(TFT_YELLOW);
    sprite.setCursor(10, 130);
    sprite.printf("Temp: %.1f C", temp);
    
    sprite.setCursor(160, 130);
    sprite.printf("Hum: %.1f %%", hum);

    // 5. エアコンの動作状況の描画
    // 状態によって文字色を変える
    if (acState.indexOf("COOL") >= 0) {
        sprite.setTextColor(TFT_BLUE);
    } else if (acState.indexOf("HEAT") >= 0) {
        sprite.setTextColor(TFT_RED);
    } else {
        sprite.setTextColor(TFT_LIGHTGRAY); // OFFの時など
    }
    
    sprite.setFont(&fonts::FreeSansBold18pt7b); // 少し大きなフォントで
    sprite.setCursor(10, 180);
    sprite.printf("AC: %s", acState.c_str());

    // 6. 最後にスプライト（裏画面）を実際のディスプレイに転送する
    sprite.pushSprite(0, 0);
}