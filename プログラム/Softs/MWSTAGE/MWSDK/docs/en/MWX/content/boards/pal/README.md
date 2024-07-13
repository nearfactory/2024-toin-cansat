---
title: "PAL"
author: "Mono Wireless Inc."
description: "PAL common interface."
---
# PAL

TWELITE PAL's hardware has common parts, and board BEHAVIOR also defines common interfaces for common hardware.



## constants

The following definitions are available

```cpp
static const uint8_t PIN_BTN = 12; // button (as SET)
static const uint8_t PIN_LED = 5;  // LED
static const uint8_t PIN_WDT = 13; // WDT (shall tick every 60sec)

static const uint8_t PIN_D1 = 1; // DIP SW1
static const uint8_t PIN_D2 = 2; // DIP SW2
static const uint8_t PIN_D3 = 3; // DIP SW3
static const uint8_t PIN_D4 = 4; // DIP SW4

static const uint8_t PIN_SNS_EN = 16;
static const uint8_t PIN_SNS_INT = 17;
```

It can be accessed as `PAL_AMB::PIN_BTN`.



## Hardware initialization

```cpp
pinMode(PIN_BTN, INPUT_PULLUP);
pinMode(PIN_LED, OUTPUT_INIT_HIGH);
pinMode(PIN_WDT, OUTPUT_INIT_HIGH);

pinMode(PIN_D1, INPUT_PULLUP);
pinMode(PIN_D2, INPUT_PULLUP);
pinMode(PIN_D3, INPUT_PULLUP);
pinMode(PIN_D4, INPUT_PULLUP);
```

Each pin is initialized as shown in the code above.



### Watchdog Timer

Resets the external watchdog timer at startup, sleep wakeup, and after a certain period of time after startup.

{% hint style="warning" %}
To avoid watchdog timer timeout, TWELITE should be set to wake up within 60 seconds (when using calibrated internal CR timer).
{% endhint %}



## Methods

### set\_led()

```cpp
void set_led(uint8_t mode, uint16_t tick)
```

LED (D1) control.

{% hint style="warning" %}
Do not call this method if you do not want to control with board BEHAVIOR.
{% endhint %}



`mode`は以下のパラメータを取ります。`tick`は点灯時間\[ms]を指定しますが、詳細は`mode`の解説を参照してください。

| Designation | Meaning
| ----------------------- | ---------------------------------------------------------------------------- |
| `LED_TIMER::BLINK` | Blinks the LED. ON/OFF is toggled every \[ms] of time given to `tick`. After returning from sleep, it resets the count and starts from the lit state. |
| `LED_TIMER::ON_RX` | When a packet is received, it turns on for the time given to the `tick`, `LED_TIMER::ON_RX`.                                           |
| `LED_TIMER::ON_TX_COMP` | Turns on for the time given to the `tick` when the transmission is completed.                                              |

{% hint style="info" %}
The setting before the return to sleep is maintained after the return to sleep.
{% endhint %}



### led\_one\_shot()

```cpp
void led_one_shot(uint16_t tick)
```

Turns on the LED for a specified period of time. Cannot be used at the same time as the `set_led()` function.



### get\_D1() .. D4(), get\_DIPSW\_BM()

```cpp
inline uint8_t get_D1()
inline uint8_t get_D2()
inline uint8_t get_D3()
inline uint8_t get_D4()
inline uint8_t get_DIPSW_BM()
```

`get_D1() ... get_D4()` returns `0` when DIP SW is HIGH (switch is up) and `1` when it is LOW (switch is down).

`get_DIPSW_BM()` returns the DIP SW setting as `0..15`. Returns the sum of `SW1==LOW` as `1`, `SW2 == LOW` as `2`, `SW3 == LOW` as `4` and `SW4 == LOW` as `8`.

{% hint style="warning" %}
The opposite of `HIGH(1),LOW(0)` of D1...D4.

This is because the `LOW(0)` side of the DIP SW is set, meaning it has a value of `1`.
{% endhint %}

{% hint style="warning" %}
This value is checked at system startup and is not updated by manipulating the switch thereafter.
{% endhint %}

