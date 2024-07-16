// -*- coding:utf-8-unix -*-
/**
 * @file   AQM0802.hpp
 * @author alnasl
 * @date   Thu Jun 16 14:45:57 2022
 *
 * @brief  Act driver for AQM0802 / AQM1602 LCD modules built with the Sitronix ST7032 LCD controller
 *
 * Copyright (C) 2022 Mono Wireless Inc. All Rights Reserved.
 * Released under MW-OSSLA-*J,*E (MONO WIRELESS OPEN SOURCE SOFTWARE LICENSE AGREEMENT).
 */

#pragma once

// Include Files //////////////////////////////////////////////////////////////

#include <TWELITE>

#include <stdarg.h>             // For printf
#include <stdint.h>             // For uint8_t


// Macro Definitions //////////////////////////////////////////////////////////

/// Display maximum width
#define ST7032_MAX_COLUMNS 16
/// Display maximum height
#define ST7032_MAX_ROWS 2
/// Maximum length of printf() strings
#define ST7032_PRINTF_MAX 64


// Type Definitions ///////////////////////////////////////////////////////////

/// ST7032 control bytes
enum st7032_ctrl_byte_e {
    ST7032_INSTRUCTION = 0x00,
    ST7032_DATA = 0x40
};

// Module type (AQM0802 / AQM1602)
enum st7032_module_type_e {
    TYPE_AQM0802,
    TYPE_AQM1602
};

/// Cursor structure
struct st7032_cursor_s {
    int row;
    int col;
};


// Class Definition ///////////////////////////////////////////////////////////

class AQM0802 : public mwx::stream<AQM0802> {
    /// SUPER: for using mwx::stream
    using SUPER = mwx::stream<AQM0802>;

private:
    /**
     * @brief   I2C slave address
     */
    uint8_t i2c_address;
    /**
     * @brief   Virtual VRAM
     */
    char vvram[ST7032_MAX_ROWS][ST7032_MAX_COLUMNS];
    /**
     * @brief   Current cursor positions (row/col)
     */
    st7032_cursor_s cursor;
    /**
     * @brief   Number of display rows (height)
     */
    uint8_t rows;
    /**
     * @brief   Number of display columns (width)
     */
    uint8_t columns;

public:
    /**
     * @fn      AQM0802
     * @brief   Constructor
     */
    AQM0802();

    /**
     * @fn      ~AQM0802
     * @brief   Destructor
     */
    ~AQM0802() {}

public:
    /**
     * @fn      begin
     * @brief   Setup the display
     *
     * @param   type        Module type (TYPE_AQM0802 or TYPE_AQM1602)
     * @param   contrast    Contrast value (C5-C0 in ST7032). If not set, use preferred value for AQM0802 / AQM1602.
     * @return  none
     */
    void begin(const st7032_module_type_e type = TYPE_AQM0802, const int8_t contrast = -1);

    /**
     * @fn      clear
     * @brief   Clear the display
     *
     * @param   none
     * @return  none
     */
    void clear();

    /**
     * @fn      printf
     * @brief   Print formatted text to the display
     *
     * @param   format  Formatted string
     * @param   ...     Parameters
     * @return  Number of printed characters
     */
    int printf(const char* format, ...);

    /**
     * @fn      putc
     * @brief   Print a character to the display
     *
     * @param   c       Character
     * @return  none
     */
    void putc(const char c);

    /**
     * @fn      move
     * @brief   Move the cursor position (affects printf/putc)
     *
     * @param   y   Cursor position y (row)
     * @param   x   Cursor position x (column)
     * @retval  1   Success
     * @retval  -1  Error (out of range)
     */
    int move(const int y, const int x);

private:
    /**
     * @fn      clearVVRAM
     * @brief   clear the VVRAM
     *
     * @param   none
     * @return  none
     */
    void clearVVRAM();

    /**
     * @fn      moveVVRAMCursorTo
     * @brief   Move the cursor on the VVRAM to the specified position
     *
     * @param   row     Row (y)
     * @param   col     Column (x)
     * @return  none
     */
    void moveVVRAMCursorTo(const uint8_t row, const uint8_t col);

    /**
     * @fn      moveVVRAMCursorForward
     * @brief   Move the cursor on the VVRAM forward
     *
     * @param   none
     * @return  none
     */
    void moveVVRAMCursorForward();

    /**
     * @fn      putVVRAM
     * @brief   Put a character at current cursor position of the VVRAM
     *
     * @param   c       Character to put
     * @return  none
     */
    void putVVRAM(const char c);

private:
    /**
     * @fn      initializeDisplay
     * @brief   Initialize the display
     *
     * @param   type      Module type (TYPE_AQM0802 or TYPE_AQM1602)
     * @param   contrast    Contrast value (C5-C0 in ST7032)
     * @return  none
     */
    void initializeDisplay(const st7032_module_type_e type = TYPE_AQM0802, int8_t contrast = -1);

    /**
     * @fn      updateDisplayWithVVRAM
     * @brief   Update display contents with data in the VVRAM
     *
     * @param   none
     * @return  none
     */
    void updateDisplayWithVVRAM();

    /**
     * @fn      writeDisplay
     * @brief   Write commands to the display
     *
     * @param   ctrl_byte   ST7032 control byte
     * @param   data        ST7032 data
     * @return  none
     */
    void writeDisplay(const st7032_ctrl_byte_e ctrl_byte, const uint8_t data);

    /**
     * @fn      moveDisplayCursorTo
     * @brief   Move the cursor on the display
     *
     * @param   row     Row (y)
     * @param   col     Column (x)
     * @return  none
     */
    void moveDisplayCursorTo(const uint8_t row, const uint8_t col);

public:
    /**
     * @fn      available
     * @brief   Check for input
     * @see     class derived from mwx::stream must have this interface
     *
     * @param   none
     * @return  0   No input (this stream is output only)
     */
    inline int available() {
        return 0;                   // No data to read
    }

    /**
     * @fn      flush
     * @brief   Force output from the buffer, wait for it
     * @see     class derived from mwx::stream must have this interface
     *
     * @param   none
     * @return  none
     */
    inline void flush() {
        this->updateDisplayWithVVRAM();
    }

    /**
     * @fn      read
     * @brief   Read 1 byte from the input stream
     * @see     class derived from mwx::stream must have this interface
     *
     * @param   none
     * @return  -1  No input (this stream is output only)
     */
    inline int read() {
        return -1;                  // No data to read
    }

    /**
     * @fn      write
     * @brief   Write 1 byte to the output stream
     * @see     class derived from mwx::stream must have this interface
     *
     * @param   n   Data to write
     * @return  1   Wrote length
     */
    inline size_t write(int n) {
        this->putc(n);
        return 1;
    }

    /**
     * @fn      vOutput
     * @brief   (static method) Write 1 byte to the output stream
     * @see     class derived from mwx::stream must have this interface
     *
     * @param   out     Character to write
     * @param   vp      Pointer to the class instance
     * @return  none
     */
    static void vOutput(char out, void* vp);
};
