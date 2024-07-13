---
title: "&lt;BRD_APPTWELITE&gt;"
author: "Mono Wireless Inc."
description: "&lt;BRD_APPTWELITE&gt;"
---

# \<BRD\_APPTWELITE>

This board BEHAVIOR assumes the same wiring as the standard application App\_Twelite. It has constant definitions and readout functions for M1-M3 and BPS pins.

## Constants

The following constants are defined. `BRD_APPTWELITE::PIN_DI1`.

```cpp
static const uint8_t PIN_DI1 = mwx::PIN_DIGITAL::DIO12;
static const uint8_t PIN_DI2 = mwx::PIN_DIGITAL::DIO13;
static const uint8_t PIN_DI3 = mwx::PIN_DIGITAL::DIO11;
static const uint8_t PIN_DI4 = mwx::PIN_DIGITAL::DIO16;

static const uint8_t PIN_DO1 = mwx::PIN_DIGITAL::DIO18;
static const uint8_t PIN_DO2 = mwx::PIN_DIGITAL::DIO19;
static const uint8_t PIN_DO3 = mwx::PIN_DIGITAL::DIO4;
static const uint8_t PIN_DO4 = mwx::PIN_DIGITAL::DIO9;

static const uint8_t PIN_M1 = mwx::PIN_DIGITAL::DIO10;
static const uint8_t PIN_M2 = mwx::PIN_DIGITAL::DIO2;
static const uint8_t PIN_M3 = mwx::PIN_DIGITAL::DIO3;
static const uint8_t PIN_BPS = mwx::PIN_DIGITAL::DIO17;

static const uint8_t PIN_AI1 = mwx::PIN_ANALOGUE::A1;
static const uint8_t PIN_AI2 = mwx::PIN_ANALOGUE::A3;
static const uint8_t PIN_AI3 = mwx::PIN_ANALOGUE::A2;
static const uint8_t PIN_AI4 = mwx::PIN_ANALOGUE::A4;
```



## Methods

Methods are provided to obtain the values of the DIP SW (M1 M2 M3 BPS) pins.

```cpp
inline uint8_t get_M1()
inline uint8_t get_M2()
inline uint8_t get_M3()
inline uint8_t get_BPS()
inline uint8_t get_DIPSW_BM()
```

The return value is not HIGH or LOW, but `0` means that the switch is not set (HIGH side) and `1` means that the switch is set (LOW side).

`get_DIPSW_BM()` returns the value of M1, M2, M3 and BPS pins in order from bit0.

{% hint style="warning" %}
This value is checked at system startup and is not updated by operating the switch thereafter.
{% endhint %}
