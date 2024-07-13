// -*- coding:utf-8-unix -*-
/**
 * @file   ActEx_AQM0802.cpp
 * @author alnasl
 * @date   Thu Jun 16 14:45:57 2022
 *
 * @brief  Act sample for using AQM0802 / AQM1602 LCD modules built with the Sitronix ST7032 LCD controller
 *
 * Copyright (C) 2022 Mono Wireless Inc. All Rights Reserved.
 * Released under MW-OSSLA-*J,*E (MONO WIRELESS OPEN SOURCE SOFTWARE LICENSE AGREEMENT).
 */

// Include Files //////////////////////////////////////////////////////////////

#include <TWELITE>
#include "AQM0802/AQM0802.hpp"    // AQM0802 local library for TWELITE


// Macro Definitions //////////////////////////////////////////////////////////


// Function Prototypes ////////////////////////////////////////////////////////


// Global Objects /////////////////////////////////////////////////////////////

AQM0802 display;                // AQM0802 Display object
uint16_t count;                 // Count number (Maximum: 5 digits)
bool use_mwx_stream;            // Using mwx::stream or not
st7032_module_type_e type;      // Using AQM0802 or AQM1602


// Setup and Loop Procedure ///////////////////////////////////////////////////

/**
 * @fn      setup
 * @brief   the setup procedure (called on boot)
 *
 * @param   none
 * @return  none
 */
void setup() {
    // Select module type
    type = TYPE_AQM0802;
    //type = TYPE_AQM1602;
    switch (type) {
    case TYPE_AQM0802: {
        Serial.printfmt("ActEx_AQM0802: Using AQM0802\n");
        break;
    }
    case TYPE_AQM1602: {
        Serial.printfmt("ActEx_AQM0802: Using AQM1602\n");
        break;
    }
    default:
        break;
    }

    // Setup the display (with prefered contrast: 23 (AQM0802) or 31 (AQM1602))
    display.begin(type);

    // Setup the display (with optional contrast: 0~63)
    // int8_t contrast = 25;
    // display.begin(type, contrast);

    // Select printing style
    use_mwx_stream = false;
    //use_mwx_stream = true;
    if (use_mwx_stream) {
        Serial << "ActEx_AQM0802: Using mwx::stream" << mwx::crlf;      // Cpp-style
    } else {
        Serial.printfmt("ActEx_AQM0802: Using printf\n");               // C-style
    }

    // Print initial messages
    switch (type) {
    case TYPE_AQM0802: {
        if (use_mwx_stream) {
            display << "ActExtra AQM0802" << mwx::crlf;     // Cpp-style
        } else {
            display.printf("ActExtra AQM0802\n");           // C-style
        }
        break;
    }
    case TYPE_AQM1602: {
        if (use_mwx_stream) {
            display << "ActExtra AQM1602" << mwx::crlf;     // Cpp-style
        } else {
            display.printf("ActExtra AQM1602\n");           // C-style
        }
        break;
    }
    default:
        break;
    }

    delay(2000);

    switch (type) {
    case TYPE_AQM0802: {
        if (use_mwx_stream) {
            display << "-Start!-" << mwx::crlf;     // Cpp-style
        } else {
            display.printf("-Start!-\n");           // C-style
        }
        break;
    }
    case TYPE_AQM1602: {
        if (use_mwx_stream) {
            display << "    -Start!-" << mwx::crlf;     // Cpp-style
        } else {
            display.printf("    -Start!-\n");           // C-style
        }
        break;
    }
    default:
        break;
    }

    // Initialize the count number
    count = 0;

    // Begin the Timer0 with 1Hz
    Timer0.begin(1);
}

/**
 * @fn      loop
 * @brief   the loop procedure (called every event)
 *
 * @param   none
 * @return  none
 */
void loop() {
    if (Timer0.available()) {
        // Increment the count number and print it
        switch (type) {
        case TYPE_AQM0802: {
            if (use_mwx_stream) {
                display << format("%5d[s]", ++count) << mwx::crlf; // Cpp-style
            } else {
                display.printf("%5d[s]\n", ++count);               // C-style
            }
            break;
        }
        case TYPE_AQM1602: {
            if (use_mwx_stream) {
                display << format("Elapsed:%5d[s]", ++count) << mwx::crlf; // Cpp-style
            } else {
                display.printf("Elapsed:%5d[s]\n", ++count);               // C-style
            }
            break;
        }
        default:
            break;
        }
    }
}
