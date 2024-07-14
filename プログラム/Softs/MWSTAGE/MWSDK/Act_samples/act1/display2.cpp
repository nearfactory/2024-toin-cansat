#include "display2.hpp"
#include <TWELITE>

void eraseDisplay(uint8_t addr){
    for(uint8_t m=0; m<8; m++){
        Wire.write(0xB0 | (m & 0x07) , 0x10 , 0x00);
    }
}