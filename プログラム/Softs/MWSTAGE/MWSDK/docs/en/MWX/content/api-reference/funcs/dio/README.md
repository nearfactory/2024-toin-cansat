---
title: DIO General-purpose digital IO
author: "Mono Wireless Inc."
description: API for DIO (General-purpose digital IO)
---
# DIO General-purpose digital IO

The following functions are used for general-purpose digital IO (DIO) operations.

* `pinMode()`
* `digitalWrite()`
* `digitalRead()`
* `attachIntDio()`
* `detachIntDio()`

## Constants

### Pin name and number

| Definition | Name        |
| --------------------------------------- | --------- |
| `const uint8_t PIN_DIGITAL::DIO0 .. 19` | DIO pins 0 to 19 |
| `const uint8_t PIN_DIGITAL::DO0 .. 1`   | DO pin 0,1   |



### Mode of pin（DIO0..19)

The following enumeration values are handled with the type name `E_PIN_MODE`.

| Definition | Pull-up | Name           |
| ------------------------------- | :---: | ------------ |
| `PIN_MODE::INPUT`               |   None   | Input           |
| `PIN_MODE::OUTPUT`              |   None   | Output           |
| `PIN_MODE::INPUT_PULLUP`        |   Yes   | Input           |
| `PIN_MODE::OUTPUT_INIT_HIGH`    |   None   | Output(init HIGH) |
| `PIN_MODE::OUTPUT_INIT_LOW`     |   None   | Output(init LOW)  |
| `PIN_MODE::WAKE_FALLING`        |   None   | Input, raised pin, falling  |
| `PIN_MODE::WAKE_RISING`         |   None   | Input, rising pin, rising  |
| `PIN_MODE::WAKE_FALLING_PULLUP` |   Yes   | Input, raised pin, falling  |
| `PIN_MODE::WAKE_RISING_PULLUP`  |   Yes   | Input, rising pin, rising  |
| `PIN_MODE::DISABLE_OUTPUT`      |   Yes   | return to the input state      |



### Mode of the pin (DO0,1)

The following enumeration values are handled with the type name `E_PIN_MODE`.

| Definition | Name           |
| ---------------------------- | ------------ |
| `PIN_MODE::OUTPUT`           | Contribute           |
| `PIN_MODE::OUTPUT_INIT_HIGH` | Output (initial state HIGH) |
| `PIN_MODE::OUTPUT_INIT_LOW`  | Output (initial state LOW)  |
| `PIN_MODE::DISABLE_OUTPUT`   | Stop setting output     |



### pin_state

The following enumeration values are handled with the type name `E_PIN_STATE`.

| Definition | Value | Name               |
| ----------------- | :-: | ---------------- |
| `PIN_STATE::HIGH` |  1  | HIGH(=Vcc) level |
| `PIN_STATE::LOW`  |  0  | LOW(=GND) level |



### Rising and falling edge of pin

The following enumeration values are handled with the type name `E_PIN_INT_MODE`.

| Definition | Name    |
| ----------------------- | ----- |
| `PIN_INT_MODE::FALLING` | falling edge  |
| `PIN_INT_MODE::RISING`  | rising edge |

