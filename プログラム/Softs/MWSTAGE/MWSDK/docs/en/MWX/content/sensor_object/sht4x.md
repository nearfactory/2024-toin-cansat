---
title: "SHT4x - Temperature and humidity sensor"
author: "Mono Wireless Inc."
description: "SHT4x - Temperature and humidity sensor"
---
# SHT4x - Temperature and humidity sensor

A temperature/humidity sensor using the I2C bus.

{% hint style="warning" %}
Available only when board BEHAVIOR `<ARIA>` is loaded. Procedures of common methods except `begin()` are executed in board BEHAVIOR.
{% endhint %}



## Process flow

1.`Wire.begin()`: Initialize bus
2.`.begin()`: Start sensor operation 3.
3. wait a few ms
4. `.available()` becomes `true
5.`.get_temp(), .get_humid()`: read values



## Procedures required for operation

### Wire bus

Before calling the `begin()` method, the Wire must be put into operation by `Wire.begin()`.



### Procedures when returning from sleep

Keep the Wire bus in operation just before sleep (the Wire is automatically restored after returning from sleep).



## Methods

### get\_temp(), get\_temp\_cent()

```cpp
double get_temp()
int16_t get_temp_cent()
```

Read temperature. `get_temp()` returns a value in °C and `get_temp_cent()` returns an integer value that is 100 times the value in °C.

On error, values between -32760 and -32768 are returned.



### get\_humid(), get\_humid\_per\_dmil()

```cpp
double get_humid()
int16_t get_humid_per_dmil()
```

Reads the humidity. `get_humid()` returns an integer value in % and `get_humid_per_dmil()` returns an integer value in % times 100.

On error, values between -32760 and -32768 are returned.



## 共通メソッド

### setup()

```cpp
void setup() 
```

Allocates and initializes memory space for the sensor.



### begin(), end()

```cpp
void begin()
void end()
```

Starts acquiring a sensor. Wait about 5 ms before reading the sensor value.

Does not support `end()`.



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

In the case of a sensor with wait processing, `arg1` is given `E_EVENT_TICK_TIMER` or `E_EVENT_START_UP` to indicate the elapse of time. After calling this method, if the required time has elapsed, it will be available and the sensor value can be read.



### available()

```cpp
bool available()
```

Returns `true` when the sensor satisfies the readout condition.



### probe()

```cpp
bool probe()
```

Returns `true` when the sensor is connected.

