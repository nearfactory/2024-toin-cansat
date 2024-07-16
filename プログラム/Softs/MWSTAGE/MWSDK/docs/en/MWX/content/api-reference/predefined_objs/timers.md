---
title: Timer0 .. 4
author: "Mono Wireless Inc."
description: Timers, PWM (mwx::periph_timer)
---

# Timer0 .. 4

The timer has two functions: to generate a software interrupt at a specified period, and to output a PWM at a specified period. 5 timers in total are available in the TWELITE radio module, from 0...4.

The name of the built-in object is `Timer0..4`, but will be referred to as `TimerX` on this page.

## Methods

### setup()

```cpp
void setup()
```

Initializes the timer. This call allocates the necessary memory space.



### begin()

```cpp
void begin(uint16_t u16Hz, bool b_sw_int = true, bool b_pwm_out = false)
```

Starts a timer; the first parameter is the period of the timer in Hz; setting the second parameter to `true` enables software interrupts; setting the third parameter to `true `true` enables PWM output.

{% hint style="info" %}
You can change the frequency with `change_hz()`. The `change_hz()` allows more detailed specification than the `begin()` specification.

The duty ratio of PWM output can be changed with `change_duty()`.

To describe the processing of the interrupt handler, specify it in the [application behavior](../behavior/README.md) definition.
{% endhint %}



### end()

```cpp
void end()
```

Stops the timer operation.



### available()

```cpp
inline bool available()
```

It becomes `true` at `loop()` immediately after a timer interrupt occurs, and becomes `false` when `loop()` ends.



### change\_duty()

```cpp
void change_duty(uint16_t duty, uint16_t duty_max = 1024)
```

Set the duty ratio. the first parameter specifies the duty ratio (a small value makes the average of the waveform closer to the GND level, a large value makes it closer to the Vcc level). the second parameter specifies the maximum duty ratio value of the duty ratio.

{% hint style="warning" %}
It is recommended that `duty_max` be one of `1024,4096,16384`.

The internal calculation of the count value involves division, but only for these three values is bit shifting used, while for other values, a computationally expensive division process is performed.
{% endhint %}



### change\_hz()

```cpp
void change_hz(uint16_t hz, uint16_t mil = 0) 
```

Sets the frequency of the timer; the second parameter is an integer with three decimal places for the frequency. For example, to set the frequency to 10.4 Hz, specify `hz=10, mil=400`.
