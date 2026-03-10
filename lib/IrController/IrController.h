#ifndef IRCONTROLLER_H
#define IRCONTROLLER_H

#include <IRsend.h>
#include "IrData.h" // Rawデータを読み込み

class IrController {
private:
    IRsend irsend;

public:
    // M5Stack Basic の Port B (送信) は GPIO 26
    IrController(uint16_t pin = 26);
    
    void begin();
    void sendCool28();
    void sendHeat24();
    void sendStop();
};

#endif