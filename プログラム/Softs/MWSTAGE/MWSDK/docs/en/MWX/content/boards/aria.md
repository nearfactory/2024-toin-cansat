---
title: "&lt;ARIA&gt;"
author: "Mono Wireless Inc."
description: "&lt;ARIA&gt;"
---
# \<ARIA>

Board BEHAVIOR for the peripheral on [TWELITE ARIA](https://mono-wireless.com/jp/products/twelite-aria/index.html).

It can handle acceleration sensors, magnetic sensors, and LEDs on the board.

* Temperature and humidity sensor
* Magnetic sensor
* LED

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<ARIA>();
}
```



## Temperature/Humidity Sensor

The member object (sns\_SHT4x) of [SHT4x sensor](../sensor\_object/sht4x.md) member object (sns\_SHT4x) is defined.



## Magnetic sensor

The sensor of the open/close sensor pal is a magnetic sensor, which is the only input for the interrupt of the two signal lines.

```cpp
const uint8_t CUE::PIN_SNS_NORTH = 16;
const uint8_t CUE::PIN_SNS_OUT1 = 16;
const uint8_t CUE::PIN_SNS_SOUTH = 8;
const uint8_t CUE::PIN_SNS_OUT2 = 8;
```

The `PAL_MAG::PIN_SNS_NORTH` is interrupted when the sensor detects an N-pole and the `PAL_MAG::PIN_SNS_SOUTH` is interrupted when the sensor detects an N-pole.



The following settings should be made before sleep

```cpp
pinMode(CUE::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
pinMode(CUE::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);
```



Check the IO of the waking factor upon waking.

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(CUE::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(CUE::PIN_SNS_SOUTH);
```



## LED

### set\_led()

```cpp
void set_led(uint8_t mode, uint16_t tick)
```

LED (D1) control.

{% hint style="warning" %}
Do not call this method if you do not want to control with board BEHAVIOR.
{% endhint %}



The `mode` takes the following parameters. `tick` specifies the lighting time \[ms], please refer to the description of `mode` for details.

| specification | meaning |
| ----------------------- | ---------------------------------------------------------------------------- |
| `LED_TIMER::BLINK` | Blinks the LED. ON/OFF is toggled every time `tick` is given. After returning from sleep, it resets the count and starts from the lit state. |
| `LED_TIMER::ON_RX` | When a packet is received, it turns on for the time given to the `tick`, `LED_TIMER::ON_RX`.                                           |
| `LED_TIMER::ON_TX_COMP` | Turns on for the time given to the `tick` when the transmission is completed.                                              |

{% hint style="info" %}
The setting before the return to sleep is maintained after the return to sleep.
{% endhint %}



### led\_one\_shot()

```cpp
void led_one_shot(uint16_t tick)
```

Turns on the LED for a specified period of time. Cannot be used with `set_led()` function at the same time.



## watchdog timer

Resets the external watchdog timer at startup, sleep wakeup, and after a certain period of time after startup.

{% hint style="warning" %}
Wake TWELITE within 60 seconds (when using calibrated internal CR timer) to avoid watchdog timer timeout.
{% endhint %}

