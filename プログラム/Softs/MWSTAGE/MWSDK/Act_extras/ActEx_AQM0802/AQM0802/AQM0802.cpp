// -*- coding:utf-8-unix -*-
/**
 * @file   AQM0802.cpp
 * @author alnasl
 * @date   Thu Jun 16 14:45:57 2022
 *
 * @brief  Act driver for AQM0802 / AQM1602 LCD modules built with the Sitronix ST7032 LCD controller
 *
 * Copyright (C) 2022 Mono Wireless Inc. All Rights Reserved.
 * Released under MW-OSSLA-*J,*E (MONO WIRELESS OPEN SOURCE SOFTWARE LICENSE AGREEMENT).
 */

// Include Files //////////////////////////////////////////////////////////////

#include "AQM0802.hpp"

#include <TWELITE>
#include <string.h>


// Static Objects /////////////////////////////////////////////////////////////


// Static Methods /////////////////////////////////////////////////////////////


// Public Methods /////////////////////////////////////////////////////////////

AQM0802::AQM0802()
    : SUPER(), i2c_address(0x3E), cursor{0, -1}, rows(2), columns(8)
{
    // Initialize the stream
    SUPER::pvOutputContext = (void*)this; // pass the mwx::strem of instance pointer (used at vOutPut())
}

void AQM0802::begin(const st7032_module_type_e type, const int8_t contrast)
{
    mwx::pnew(*this);           // Placement new: for calling the constructor manually
    this->i2c_address = 0x3E;

    switch (type) {
    case TYPE_AQM0802: {
        this->rows = 2;
        this->columns = 8;
        break;
    }
    case TYPE_AQM1602: {
        this->rows = 2;
        this->columns = 16;
        break;
    }
    default:
        this->rows = 2;
        this->columns = 8;
        break;
    }

    this->clearVVRAM();
    Wire.begin(WIRE_CONF::WIRE_400KHZ, false);
    delay(40);                                  // Wait time > 40ms after VDD stable
    this->initializeDisplay(type, contrast);
}

void AQM0802::clear(void)
{
    this->clearVVRAM();
    this->updateDisplayWithVVRAM();
}

int AQM0802::printf(const char* format, ...)
{
    // Prepare the string
    int wrote_length = 0;
    char string_buffer[ST7032_PRINTF_MAX];
    memset(string_buffer, 0, ST7032_PRINTF_MAX);
    va_list arg;
    va_start(arg, format);
    wrote_length = vsnprintf_(string_buffer, ST7032_PRINTF_MAX, format, arg);
    va_end(arg);
    if (wrote_length < 0) {
        return 0;
    }

    // Write to the VVRAM
    for (int i = 0; i < ST7032_PRINTF_MAX; i++) {
        if (string_buffer[i] == '\0') {
            wrote_length = i + 1;
            break;
        } else {
            this->putVVRAM(string_buffer[i]);
        }
    }

    // Write to the display from the VRAM
    this->updateDisplayWithVVRAM();

    return wrote_length;
}

void AQM0802::putc(const char c)
{
    // Write to the VVRAM
    this->putVVRAM(c);
    // Write to the display from the VRAM
    this->updateDisplayWithVVRAM();
}

int AQM0802::move(const int y, const int x)
{
    if (y < this->rows && x < this->columns ) {
        this->moveVVRAMCursorTo(y, x - 1); // this->cursor is the LATEST character's position
        return 1;
    }
    return -1;
}


// Private Methods ////////////////////////////////////////////////////////////

void AQM0802::clearVVRAM()
{
    // Fill with spaces
    st7032_cursor_s icur;
    for (icur.row = 0; icur.row < this->rows; icur.row++) {
        for (icur.col = 0; icur.col < this->columns; icur.col++) {
            this->vvram[icur.row][icur.col] = ' ';
        }
    }
    // After clearing, the latest positon is [0,-1]
    this->cursor.row = 0;
    this->cursor.col = -1;
}

void AQM0802::moveVVRAMCursorTo(const uint8_t row, const uint8_t col)
{
    // Ignore values bigger than display dimensions
    this->cursor.row = row % this->rows;
    this->cursor.col = col % this->columns;
}

void AQM0802::moveVVRAMCursorForward()
{
    if (this->cursor.col + 1 < this->columns) {
        // Latest char is NOT placed at the tail of the line
        this->cursor.col = this->cursor.col + 1;
    } else {
        if (this->cursor.row + 1 < this->rows) {
            // Latest char is placed at the tail of the line, but it's NOT the final line
            this->cursor.row = this->cursor.row + 1;
            this->cursor.col = 0;
        } else {
            // Latest char is placed at the end of the display
            st7032_cursor_s icur;
            for (icur.row = 0; icur.row < this->rows - 1; icur.row++) {
                for (icur.col = 0; icur.col < this->columns; icur.col++) {
                    this->vvram[icur.row][icur.col] = this->vvram[icur.row + 1][icur.col];
                }
            }
            for (int col = 0; col < this->columns; col++) {
                this->vvram[this->rows - 1][col] = ' ';
            }
            this->cursor.row = this->rows - 1;
            this->cursor.col = 0;
        }
    }
}

void AQM0802::putVVRAM(const char c)
{
    if (c == '\r') {
        // If the character is a carriage return, skip it
        return;
    }
    if (c == '\n') {
        // If the character is a newline, simply move the cursor to the tail of the line
        moveVVRAMCursorTo(this->cursor.row, this->columns - 1);
    } else {
        // If the character is NOT a newline, move the cursor forward from the latest position
        this->moveVVRAMCursorForward();
        // Then, put the character to the VVRAM
        this->vvram[this->cursor.row][this->cursor.col] = c;
    }
}

void AQM0802::initializeDisplay(const st7032_module_type_e type, int8_t contrast)
{
    // Function Set: 8-bit bus, 2-line, normal font, normal instruction
    this->writeDisplay(ST7032_INSTRUCTION, 0x38);
    // Function Set: 8-bit bus, 2-line, normal font, extension instruction
    this->writeDisplay(ST7032_INSTRUCTION, 0x39);
    // Internal OSC Freq: bias 1/5, freq 183Hz
    this->writeDisplay(ST7032_INSTRUCTION, 0x14);
    // Contrast set
    if (contrast < 0 || 63 < contrast) {
        // Use preferred contrast value
        switch (type) {
        case TYPE_AQM0802: {
            contrast = 23;      // Contrast set: 0b010111 / 0x17
            break;
        }
        case TYPE_AQM1602: {
            contrast = 31;      // Contrast set: 0b011111 / 0x1F
            break;
        }
        default:
            contrast = 23;      // Contrast set: 0b010111 / 0x17
            break;
        }
    }
    // Contrast set (low byte): C3, C2, C1, C0
    this->writeDisplay(ST7032_INSTRUCTION, 0x70 | (contrast & 0x0F));
    // Power/ICON ctrl / Contrast set (high byte): ICON off, booster on(if3.3V), C5, C4
    this->writeDisplay(ST7032_INSTRUCTION, 0x54 | (contrast >> 4 & 0x03));
    // Follower ctrl: follower on, Rab2=1
    this->writeDisplay(ST7032_INSTRUCTION, 0x6C);
    // Wait time > 200ms (for power stable)
    delay(200);
    // Function Set: 8-bit bus, 2-line, normal font, normal instruction
    this->writeDisplay(ST7032_INSTRUCTION, 0x38);
    // Display ON/OFF: display on, cursor disappears, cursor blink off
    this->writeDisplay(ST7032_INSTRUCTION, 0x0C);
    // Clear Display
    this->writeDisplay(ST7032_INSTRUCTION, 0x01);
    // Instruction Execution Time: 1.08ms
    delay(1);
}

void AQM0802::updateDisplayWithVVRAM()
{
    st7032_cursor_s icur;
    for (icur.row = 0; icur.row < this->rows; icur.row++) {
        for (icur.col = 0; icur.col < this->columns; icur.col++) {
            // Move the cursor in the display
            this->moveDisplayCursorTo(icur.row, icur.col);
            // Send data to the display
            this->writeDisplay(ST7032_DATA, static_cast<uint8_t>(this->vvram[icur.row][icur.col]));
        }
    }
}

void AQM0802::writeDisplay(const st7032_ctrl_byte_e ctrl_byte, const uint8_t data)
{
    // All data size is 1Byte (Co=0)
    Wire.beginTransmission(this->i2c_address);
    // Control bytes
    Wire.write(ctrl_byte);
    // Data
    Wire.write(data);
    Wire.endTransmission(true);
    delayMicroseconds(30);      // Wait time > 26.3us (except for special functions such as clear func)
}

void AQM0802::moveDisplayCursorTo(const uint8_t row, const uint8_t col)
{
    uint8_t rounded_row = row % this->rows;
    uint8_t rounded_col = col % this->columns;
    uint8_t ddram_address = 0x80;
    ddram_address |= (rounded_row % 2 == 0) ? 0x00 : 0x40;
    ddram_address |= rounded_col;
    this->writeDisplay(ST7032_INSTRUCTION, ddram_address);
}


void AQM0802::vOutput(char out, void* vp) {
    AQM0802* p_display = reinterpret_cast<AQM0802*>(vp);
    if (p_display != nullptr) {
        p_display->write(out);
    }
}
