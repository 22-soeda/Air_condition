#include "DisplayUi.h"

DisplayUi::DisplayUi() : sprite(&M5.Display) {
}

void DisplayUi::begin() {
    // メモリ不足を防ぐため、色深度を8bit(256色)に下げてからスプライトを作成します
    sprite.setColorDepth(8); 
    
    // スプライトの作成 (作成できたかどうかのチェック付き)
    if (sprite.createSprite(320, 240) == nullptr) {
        Serial.println("Error: Failed to create sprite! (Out of memory)");
    } else {
        Serial.println("Sprite created successfully.");
    }
    
    sprite.setTextDatum(top_left);
}

void DisplayUi::draw(String time, String weather, float temp, float hum, String acState) {
    // ------------------------------------------
    // デバッグ用：シリアルモニタへ出力
    // ------------------------------------------
    Serial.println("=== Display Update ===");
    Serial.printf("Time    : %s\n", time.c_str());
    Serial.printf("Weather : %s\n", weather.c_str());
    Serial.printf("Temp    : %.1f C\n", temp);
    Serial.printf("Hum     : %.1f %%\n", hum);
    Serial.printf("AC State: %s\n", acState.c_str());
    Serial.println("======================");

    // ------------------------------------------
    // 画面(スプライト)への描画
    // ------------------------------------------
    sprite.fillScreen(TFT_BLACK);

    sprite.drawRect(0, 0, 320, 240, TFT_WHITE);
    sprite.drawFastHLine(0, 40, 320, TFT_WHITE);
    
    sprite.setTextColor(TFT_CYAN);
    sprite.setFont(&fonts::FreeSansBold12pt7b);
    sprite.setCursor(10, 10);
    sprite.print("Smart AC Controller");

    sprite.setTextColor(TFT_WHITE);
    sprite.setFont(&fonts::FreeSans12pt7b);
    
    sprite.setCursor(10, 60);
    sprite.printf("Time: %s", time.c_str());
    
    sprite.setCursor(10, 95);
    sprite.printf("Weather: %s", weather.c_str());

    sprite.setTextColor(TFT_YELLOW);
    sprite.setCursor(10, 130);
    sprite.printf("Temp: %.1f C", temp);
    
    sprite.setCursor(160, 130);
    sprite.printf("Hum: %.1f %%", hum);

    if (acState.indexOf("COOL") >= 0) {
        sprite.setTextColor(TFT_BLUE);
    } else if (acState.indexOf("HEAT") >= 0) {
        sprite.setTextColor(TFT_RED);
    } else {
        sprite.setTextColor(TFT_LIGHTGRAY);
    }
    
    sprite.setFont(&fonts::FreeSansBold18pt7b);
    sprite.setCursor(10, 180);
    sprite.printf("AC: %s", acState.c_str());

    // スプライトを画面に転送
    sprite.pushSprite(0, 0);
}