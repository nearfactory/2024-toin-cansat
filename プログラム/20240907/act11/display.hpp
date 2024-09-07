#pragma once

#include<TWELITE>

constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;

class Display
{
protected:
    uint8_t	m_initCommand[33] = {
        0x00, 0xA8, 0x3F,	    // SetMultiplexRatio
        0x00, 0xD3, 0x00,	    // SetDisplayOffset
        0x80, 0x40,			      // SetDisplayStartline
        0x80, 0xA1,			      // SetSegmentRemap
        0x80, 0xC8,			      // SetCOMOutputScanDirection
        0x00, 0xDA, 0x12,	    // SetCOMPingsHardwareConfiguration
        0x00, 0x81, 0x7F,	    // SetCntrastControl
        0x80, 0xA4,			      // EntireDisplayON
        0x80, 0xA6,			      // SetNormalDisplay
        0x00, 0xD5, 0x80,     // SetOscFrequency
        0x00, 0x8D, 0x14,	    // EnableChargePumpRegulator
        0x80, 0xAF            // Display ON
    };
    uint8_t m_addr = 0x3C;

public:
    Display();
    ~Display();

    void init(uint8_t addr);
};

Display::Display()
{
};

Display::~Display()
{
};

void Display::init(uint8_t addr){
    m_addr = addr;
    
    Wire.beginTransmission(m_addr);
    for(auto& cmd:m_initCommand) Wire.write(cmd);
    Wire.endTransmission();

    return;
};