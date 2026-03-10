#include <Arduino.h>
#include <M5Unified.h>  // M5CoreS3.h ではなく M5Unified.h を推奨
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// Port B のピン設定
// 黄色い線が 8, 白い線が 9 に繋がっています。
// IR Unitの受信素子がどちらに繋がっているか、まずは 9 で試し、ダメなら 8 に変えてください。
const uint16_t kRecvPin = 36; 

const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

void setup() {
    // M5の初期化（これで電源IC AXP2101が起動し、Port Bに5Vが供給されます）
    auto cfg = M5.config();
    M5.begin(cfg);
    
    // 画面に状態を表示（シリアルが見れない時のバックアップ）
    M5.Display.setTextSize(2);
    M5.Display.println("IR Receiver Ready");
    M5.Display.println("Connect to Port B");

    Serial.begin(115200);
    
    irrecv.enableIRIn(); 
    Serial.println("System Ready. Monitoring IR on Pin 9...");
}

void loop() {
    M5.update(); // ボタン操作などの状態更新

    if (irrecv.decode(&results)) {
        Serial.println("--- Signal Detected! ---");
        
        // 画面にも反応を出す
        M5.Display.fillScreen(GREEN);
        delay(100);
        M5.Display.fillScreen(BLACK);
        M5.Display.setCursor(0, 0);
        
        // 解析結果の表示
        String protocolName = typeToString(results.decode_type);
        Serial.print("Protocol: ");
        Serial.println(protocolName);
        M5.Display.printf("Proto: %s\n", protocolName.c_str());

        // 送信用Rawデータの表示
        String sourceCode = resultToSourceCode(&results);
        Serial.println(sourceCode);
        
        irrecv.resume(); 
    }
    yield();
}