#include "lets_use_display.hpp"
#include "font.hpp"
#include <TWELITE>

const byte initCommands[25] = {
    0x00,           // OMAJINAI
    0x81, 0xFF,     // Set Contrast

    0x40,           // 1st Line
    0xA8, 0x1F,     // Set Line Number
    
    0xA1,   // Set Segment Re-map
    0xA6,   // Set Normal/Inverse
    0xC8,   // Set COM Scan Direction
    0xDA, 0x00,     // COM config
    0x2E,   // Stop Scroll
    
    0x20,   // Set Memory Addressing Mode
    0x00,   // Horizontal Addressing Mode
    
    0x21,   // Set Column Address
    0x00,   // SEG 0
    0xFF,   // SEG 127

    0x22,   // Set Page Address
    0x00,   // PAGE 0
    0x07,   // PAGE 7

    0xD5,   // Set Display Clock
    0x80,   // 370 kHz

    0x8D, 0x14,     // Enable Charge Pump   

    0xAF    // Display ON
};
const int initCommandsLength = 25;

void displayInit(byte addr){
    for(int i=0; i<initCommandsLength; i++){
        Wire.beginTransmission(addr);
        Wire.write(initCommands[i]);
        Wire.endTransmission();
        Serial.print("=");
        delay(10);
    }
    Serial.println();
    Serial.println("Display Init Completed Successfully");
}

void displayClear(byte addr){
    // for each PAGE
    for(int page=0; page<8; page++){
        Wire.beginTransmission(0x3C);
        Wire.write(0x00);   // OMAJINAI(Co=0 D/C#=0 Commandを送る)
        Wire.write(0x21);   // Set Column Address
        Wire.write(0);      // Draw Start X=COM
        Wire.write(0xFF);   // SEG 127
        Wire.write(0x22);   // Set Page Address
        Wire.write(page);   // Draw START Y=PAGE
        Wire.write(0x07);   // PAGE8
        Wire.endTransmission();

        // for each COM
        for(int com=0;com<128;com++){
            Wire.beginTransmission(0x3C);
            Wire.write(0x40);// OMAJINAI(Co=0 D/C#=1 GDDRAMにdataを送る)
            Wire.write(0x00);// 0x00=黒で塗りつぶす
            Wire.endTransmission();
        }
    }
    
    Serial.println("Display Cleared");
}

void displayLocate(byte addr, int COM, int PAGE){
    bool err = false;
    if(!(COM >= 0 && COM <= 127)){
        Serial.println("ERROR! Invalid Value for COM=X");
        err = true;
    }
    if(!(PAGE >= 0 && PAGE <= 7)){
        Serial.println("ERROR! Invalid Value for PAGE=Y");
        err = true;
    }
    if(err){
        return;
    }

    Wire.beginTransmission(0x3C);
    Wire.write(0x00);
    Wire.write(0x21);
    Wire.write(COM);
    Wire.write(0xFF);
    Wire.write(0x22);
    Wire.write(PAGE);
    Wire.write(0x07);
    Wire.endTransmission();
}

void displayDrawChar(byte addr, int font){
    for(int i=0;i<8;i++){
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
        Wire.write(FONT[font][i]);// GDDRAMに書き込む
        Wire.endTransmission();
    }
}