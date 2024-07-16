---
title: "LTR-308ALS - Illuminance sensor"
author: "Mono Wireless Inc."
description: "LTR-308ALS - Illuminance sensor"
---

# LTR-308ALS - Illuminance sensor

Illuminance sensor using I2C bus.

{% hint style="warning" %}
Available only when the board BEHAVIOR `<PAL_AMB>` is loaded. Procedures of common methods except `begin()` are executed in board BEHAVIOR.
{% endhint %}



## Process flow

1. `Wire.begin()`: initialize bus
2.`.begin()`: Start sensor operation 3.
3. wait 50ms
4. `.available()` becomes `true
5. `.get_luminance()`: read value



## Procedures required for operation

### Wire bus

Before calling `.begin()` method, Wire should be put into operation by `Wire.begin()`.



### Procedures when returning from sleep mode

Keep the Wire bus in operation just before sleep (the Wire is automatically restored after returning from sleep).



## Methods

### get\_luminance()

```cpp
uint32_t get_luminance()
```

Returns an integer value of the illuminance `[lx]`.

Returns `-1` on error.



## Common methods

### setup()

```cpp
void setup() 
```

Allocates and initializes a memory area for the sensor.



### begin(), end()

```cpp
void begin()
void end()
```

Starts acquiring a sensor. Wait about 50ms before reading the sensor value.

Does not support `end()`.



### process\_ev().

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

In the case of a sensor with a waiting process, give `arg1` `E_EVENT_TICK_TIMER` or `E_EVENT_START_UP` to signal the passage of time. If the required time has elapsed after calling this method, it becomes available and the sensor value can be read.



### available()

```cpp
bool available()
```

Returns `true` when the sensor satisfies the read condition.



### probe()

```cpp
bool probe()
```

Return `true` when the sensor is connected.
