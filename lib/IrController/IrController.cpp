#include "IrController.h"

// コンストラクタで IRsend にピン番号を渡す
IrController::IrController(uint16_t pin) : irsend(pin) {
}

void IrController::begin() {
    irsend.begin();
    Serial.println("IR Controller Initialized on Pin 26.");
}

void IrController::sendCool28() {
    // キャリア周波数 38kHz で送信
    irsend.sendRaw(rawCool28, rawCool28Size, 38);
    Serial.println("IR Sent: COOL 28C");
}

void IrController::sendHeat24() {
    irsend.sendRaw(rawHeat24, rawHeat24Size, 38);
    Serial.println("IR Sent: HEAT 24C");
}

void IrController::sendStop() {
    irsend.sendRaw(rawStop, rawStopSize, 38);
    Serial.println("IR Sent: AC STOP");
}