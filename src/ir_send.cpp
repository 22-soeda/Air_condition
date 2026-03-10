#include <M5Unified.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin =26; 
IRsend irsend(kIrLedPin);

// 取得していただいた電源ON/OFF（トグル）のRawデータ
const uint16_t rawPowerToggle[271] = {
  3482, 3484,  870, 894,  844, 896,  844, 2640,  842, 2638,  842, 2636,  846, 2638,  
  846, 2634,  846, 2634,  846, 894,  844, 896,  846, 2638,  842, 2634,  844, 2640,  
  842, 2640,  840, 2640,  842, 2640,  844, 892,  846, 896,  846, 2634,  844, 896,  
  844, 898,  840, 896,  844, 898,  842, 896,  842, 898,  842, 896,  844, 2638,  
  844, 896,  846, 892,  848, 892,  846, 894,  846, 896,  3456, 3508,  844, 900,  
  840, 896,  844, 2638,  844, 2638,  842, 2638,  846, 2634,  846, 2636,  842, 2636,  
  846, 894,  844, 900,  842, 2638,  842, 2640,  842, 2638,  846, 2636,  844, 2634,  
  846, 2636,  846, 894,  844, 898,  842, 2638,  840, 896,  844, 896,  844, 898,  
  840, 898,  844, 894,  844, 898,  846, 894,  846, 2634,  846, 896,  846, 894,  
  844, 896,  844, 900,  840, 900,  3452, 3510,  842, 13954, 3456, 3508,  840, 900,  
  840, 896,  846, 2638,  840, 898,  846, 892,  848, 894,  844, 896,  846, 894,  
  846, 896,  844, 896,  844, 2636,  844, 896,  844, 898,  840, 898,  842, 898,  
  842, 898,  842, 896,  844, 2640,  840, 2638,  846, 894,  844, 2638,  844, 2636,  
  844, 898,  840, 896,  844, 900,  840, 2638,  844, 2640,  840, 896,  846, 2634,  
  846, 2634,  846, 894,  846, 896,  3456, 3506,  846, 894,  844, 896,  842, 2640,  
  842, 898,  846, 894,  846, 896,  842, 896,  846, 894,  846, 894,  844, 896,  
  844, 2636,  844, 896,  844, 898,  840, 900,  840, 898,  842, 898,  842, 898,  
  840, 2640,  840, 2638,  846, 894,  844, 2636,  846, 2636,  844, 898,  840, 896,  
  846, 898,  840, 2638,  842, 2642,  840, 898,  844, 2636,  842, 2638,  846, 894,  
  842, 896,  3458, 3506,  844
};

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    irsend.begin();
    
    // UIの描画
    M5.Display.setTextSize(3);
    M5.Display.fillScreen(BLACK);
    M5.Display.setCursor(10, 80);
    M5.Display.println("AC Power Test");
    M5.Display.setTextSize(2);
    M5.Display.setCursor(10, 140);
    M5.Display.println("Touch to Toggle");
}

void loop() {
    M5.update();

    // 画面がタッチされたら送信
    if (M5.Touch.getCount() > 0 && M5.Touch.getDetail(0).wasPressed()) {
        M5.Display.fillScreen(ORANGE);
        M5.Display.setCursor(60, 100);
        M5.Display.setTextSize(4);
        M5.Display.println("SEND!");
        
        // 配列のサイズを計算して送信 (38kHz)
        uint16_t dataSize = sizeof(rawPowerToggle) / sizeof(rawPowerToggle[0]);
        irsend.sendRaw(rawPowerToggle, dataSize, 38);
        
        delay(800); // 誤って連続送信しないためのウェイト
        
        // 元の画面に戻す
        M5.Display.fillScreen(BLACK);
        M5.Display.setTextSize(3);
        M5.Display.setCursor(10, 80);
        M5.Display.println("AC Power Test");
        M5.Display.setTextSize(2);
        M5.Display.setCursor(10, 140);
        M5.Display.println("Touch to Toggle");
    }
}