#ifndef DISPLAYUI_H
#define DISPLAYUI_H

#include <M5Unified.h>

class DisplayUi {
private:
    // チラつき防止用のスプライト（仮想画面）
    M5Canvas sprite;

public:
    DisplayUi();
    void begin();
    
    // 画面に情報を描画する関数
    void draw(String time, String weather, float temp, float hum, String acState);
};

#endif