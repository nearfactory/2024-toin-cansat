---
title: "&lt;MONOSTICK&gt;"
author: "Mono Wireless Inc."
description: "&lt;MONOSTICK&gt;"
---

# \<MONOSTICK>

Board BEHAVIOR for MONOSTICK. It includes procedures for controlling the built-in watchdog timer and lighting LEDs.



## constants

The following definitions will be available

```cpp
const uint8_t PIN_LED = mwx::PIN_DIGITAL::DIO16;  // LED

const uint8_t PIN_WDT = mwx::PIN_DIGITAL::DIO9;     // WDT (shall tick < 1sec)
const uint8_t PIN_WDT_EN = mwx::PIN_DIGITAL::DIO11; // WDT (LO as WDT enabled)

const uint8_t PIN_LED_YELLOW = mwx::PIN_DIGITAL::DO1; // YELLOW LED
```

It can be accessed as `MONOSTICK::PIN_LED`.



## Initialize hardware

```cpp
pinMode(PIN_LED, OUTPUT_INIT_HIGH);
pinMode(PIN_WDT, OUTPUT_INIT_LOW);
pinMode(PIN_WDT_EN, OUTPUT_INIT_LOW);
pinMode(PIN_LED_YELLOW, OUTPUT);
```

Each pin is initialized as shown in the code above.



### Watchdog Timer

Resets the external watchdog timer at startup, sleep wakeup, and after a certain period of time after startup.

{% hint style="warning" %}
Watchdog timer timeout is 1 second.

MONOSTICK does not normally run applications that sleep, in which case it sets `MONOSTICK::PIN_WDT_EN` to HIGH before sleeping.
{% endhint %}



## Methods

### set\_led()

```cpp
void set_led_red(uint8_t mode, uint16_t tick)
void set_led_yellow(uint8_t mode, uint16_t tick)
```

LED (red, yellow) control.

{% hint style="warning" %}
Do not call this method if you do not want to control with board BEHAVIOR.
{% endhint %}

{% hint style="warning" %}
The yellow LED (`MONOSTICK::PIN_LED_YELLOW`) is the SPIMISO pin (semiconductor pin name DO1). This board BEHAVIOR does not include methods and procedures for lighting by PWM control. If necessary, the following description is made.

* Do not call `set_led_yellow()`.
* Initialize PWM output separately after startup; the SPIMISO pin corresponds to PWM3 in the App\_Twelite standard application and can be controlled by the Timer3 class object.
* The PWM output is initialized separately after sleep recovery. At that time, the output setting of DO1 is canceled.
* Before setting PWM, cancel the output setting of DO1. \1}
  `pinMode(PIN_LED_YELLOW, DISABLE_OUTPUT);`
{% endhint %}

{% hint style="warning" %}
The yellow LED (`MONOSTICK::PIN_LED_YELLOW`) cannot be turned on during sleep.
{% endhint %}



`mode` takes the following parameters. The `tick` specifies the lighting time \[ms], please refer to the description of `mode` for details.

| specification | meaning |
| ----------------------- | ---------------------------------------------------------------------------- |
| `LED_TIMER::BLINK` | Blinks the LED. ON/OFF is toggled every time `tick` is given. After returning from sleep, it resets the count and starts from the lit state. |
| `LED_TIMER::ON_RX` | When a packet is received, it turns on for the time given to the `tick`, `LED_TIMER::ON_RX`.                                           |
| `LED_TIMER::ON_TX_COMP` | Turns on for the time given to the `tick` when the transmission is completed.                                              |

{% hint style="info" %}
The setting before the return to sleep is maintained after the return to sleep.
{% endhint %}

