#include "lets_use_display.hpp"
#include "font.hpp"
#include <TWELITE>
#include <NWK_SIMPLE>

const byte initCommands[33] = {
    0x00, 0xA8, 0x3F, // SetMultiplexRatio
    0x00, 0xD3, 0x00, // SetDisplayOffset
    0x80, 0x40,       // SetDisplayStartline
    0x80, 0xA1,       // SetSegmentRemap
    0x80, 0xC8,       // SetCOMOutputScanDirection
    0x00, 0xDA, 0x12, // SetCOMPingsHardwareConfiguration
    0x00, 0x81, 0x7F, // SetCntrastControl
    0x80, 0xA4,       // EntireDisplayON
    0x80, 0xA6,       // SetNormalDisplay
    0x00, 0xD5, 0x80, // SetOscFrequency
    0x00, 0x8D, 0x14, // EnableChargePumpRegulator
    0x80, 0xAF        // Display ON
};
const int initCommandsLength = 33;

void displayInit(byte addr){
    Wire.begin();
    Wire.beginTransmission(addr);
    for(int i=0; i<initCommandsLength; i++){
        
        Wire.write(initCommands[i]);
        // Serial.print("=");
    }
    Wire.endTransmission();
    // Serial.println();
    // Serial.println("Display Init Completed Successfully");
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
    
    // Serial.println("Display Cleared");
}

void displayLocate(byte addr, int COM, int PAGE){
    bool err = false;
    if(!(COM >= 0 && COM <= 127)){
        // Serial.println("ERROR! Invalid Value for COM=X");
        err = true;
    }
    if(!(PAGE >= 0 && PAGE <= 7)){
        // Serial.println("ERROR! Invalid Value for PAGE=Y");
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