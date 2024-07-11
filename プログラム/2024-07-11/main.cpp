#include <TWELITE>
#include <NWK_SIMPLE>

#include"@lib/SSD1306-master/lib/ssd1306.h"

enum class BTN{
    R = 24,
    U = 25,
    L = 22,
    C = 23,
    D = 19
};

const uint8_t BTN_PIN[5] = {24, 25, 22, 23, 19};

void setup(){
    Serial.begin(115200);
    
    for(int i=13;i<=16;i++) pinMode(i, PIN_MODE::INPUT);
    for(auto p:BTN_PIN) pinMode(p, PIN_MODE::INPUT);

    Serial.println("setup() finish!");
}

void loop(){
    // bool btn[5]{};
    // for(int i=0;i<5;i++) btn[i] = digitalRead(BTN_PIN[i]);
    // for(auto b:btn) Serial.print(b);
    // Serial.println();
    int32_t v = digitalReadBitmap();
    for(int i=0;i<32;i++) Serial.print(v>>i & 0b01);
    Serial.println();
    delay(50);
}