#include <M5Unified.h>
#include <M5UnitENV.h>

// ==========================================
// 設定値の定数定義
// ==========================================
// I2Cピン設定 (CoreS3 Port A: SDA=2, SCL=1)
constexpr uint8_t I2C_SDA_PIN = 2;
constexpr uint8_t I2C_SCL_PIN = 1;

// I2C通信速度 (400kHz)
constexpr uint32_t I2C_FREQ = 400000;

// ENV III センサーのI2Cアドレス
constexpr uint8_t SHT30_ADDR   = 0x44; // 温度・湿度センサー
constexpr uint8_t QMP6988_ADDR = 0x70; // 気圧センサー (修正済)
// ==========================================

// センサーのインスタンスを作成
SHT3X sht30;
QMP6988 qmp6988;

void setup() {
  // M5Stackの初期化
  auto cfg = M5.config();
  M5.begin(cfg);

  // 画面の設定
  M5.Display.setTextSize(3);
  M5.Display.clear(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("ENV III Sensor");

  // I2Cの初期化 (定数を使用)
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ);

  // センサーの初期化 (定数を使用)
  if (!sht30.begin(&Wire, SHT30_ADDR, I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ)) {
    M5.Display.println("SHT30 Error!");
    Serial.println("SHT30 initialization failed.");
  }
  
  if (!qmp6988.begin(&Wire, QMP6988_ADDR, I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ)) {
    M5.Display.println("QMP6988 Error!");
    Serial.println("QMP6988 initialization failed.");
  }

  delay(2000); // エラーメッセージ確認用の待機
}

void loop() {
  // M5Stack自体の状態更新
  M5.update();

  // センサーデータの更新
  sht30.update();
  qmp6988.update();

  // 画面をクリアしてカーソルを左上に戻す
  M5.Display.clear(BLACK);
  M5.Display.setCursor(10, 20);

  // 温度の表示
  M5.Display.printf("Temp: %2.1f C\n", sht30.cTemp);
  
  // 湿度の表示
  M5.Display.setCursor(10, 60);
  M5.Display.printf("Hum:  %2.1f %%\n", sht30.humidity);
  
  // 気圧の表示 (Pa -> hPa)
  M5.Display.setCursor(10, 100);
  M5.Display.printf("Pres: %4.1f hPa\n", qmp6988.pressure / 100.0);

  delay(1000);
}