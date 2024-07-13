---
title: "SHT3x - Temperature and humidity sensor"
author: "Mono Wireless Inc."
description: "SHT3x - Temperature and humidity sensor"
---

# SHT3x - Temperature and humidity sensor

Temperature/humidity sensor using I2C bus.

{% hint style="info" %}
This sensor is not used in the TWELITE PAL series. See below for usage examples. \%{% hint="info
[https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30](https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30)
{% endhint %}



## Process flow

1 `Wire.begin()`: Initialize bus
2 `.setup()`: Initialize the sensor
3.`.begin()`: Start operation of the sensor 4.
4.wait()`: wait for a few ms
5.`.available()` becomes `true
6. `.get_temp(), .get_humid()`: read values



## Procedures required for operation

### Wire bus

Before calling `setup()` method, Wire is put into operation by `Wire.begin()`.



### Procedures when returning from sleep mode

Keep the Wire bus in the operating state just before sleep (the Wire is automatically restored after sleep).



### Code Example

```cpp
#include <TWELITE>
#include <SNS_SHT3X>

SNS_SHT3X sns_sht3x; // Declaration of Objects
```

`#include <SNS_SHT3X>` and declaration of `SNS_SHT3X` class object is required.



#### Initialization

```cpp
void setup() {
    Wire.begin();
    sns_sht3x.setup();
}
```



#### Start acquiring sensor values

```cpp
void loop() {

  if(eState == E_STATE::INIT) {
    sns_sht3x.begin();
    eState = E_STATE::CAPTURE;
  }

}
```

Call `.begin()` to start acquiring sensor values. It takes several ms to complete.

In the above `loop()`, the process is designed to branch according to the state variable "eState". ([Reference](../act\_samples/slp\_wk\_and\_tx.md))



#### Waiting for sensor value acquisition

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_sht3x.available()) {
      // Sensor value readout available
    }
  }

}
```

Whether the sensor value is ready or not can be determined by `.available()`.



#### Reading sensor values

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_sht3x.available()) {
      Serial << crlf << "SHT3X:"
            << " T=" << sns_sht3x.get_temp() << 'C'
						<< " H=" << sns_sht3x.get_humid() << '%';
    }
  }

}
```

As soon as the sensor value is ready, the value can be read out.



`.get_temp(), get_humid()` includes floating point operations; you can also get 100x integer values.

```cpp
auto temp = div100(sns_sht3x.get_temp_cent());
auto humd = div100(sns_sht3x.get_humid_per_dmil);

Serial << crlf << "SHT3X:"
  << format(" T=%c%d.%02d", temp.neg ? '-' : ' ', temp.quo, temp.rem)
  << format(" T=%c%d.%02d", humd.neg ? '-' : ' ', humd.quo, humd.rem);
```

Here [`div100()`](../api-reference/funcs/utility/div100.md) is used to decompose 100x values into integer and decimal parts.



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

Reads the humidity. `get_humid()` returns a value in %, and `get_humid_per_dmil()` returns an integer value of 100 times %.

On error, values between `-32760` and `-32768` are returned.



## Common methods

### setup()

```cpp
void setup(uint32_t arg1 = 0UL) 
```

Allocates and initializes the memory area for the sensor.

The 8 bits from LSB of `arg1` can store the I2C address. If not specified, leave it as 0.

```cpp
#include <SNS_SHT3X>
SNS_SHT3X sns_sht3x;
bool b_found_sht3x = false;

void setup() {
 	sns_sht3x.setup();
	if (!sns_sht3x.probe()) {
		delayMicroseconds(100); // just in case, wait for devices to listen furthre I2C comm.
		sns_sht3x.setup(0x45); // alternative ID
		if (sns_sht3x.probe()) b_found_sht3x = true;
	} else {
		b_found_sht3x = true;
	}
}
```

In the above example, we first try to initialize with the default I2C ID, and if there is no response, we try to initialize with an address of `0x45`.



### begin(), end()

```cpp
void begin()
void end()
```

Starts acquiring a sensor. It takes a few ms to read the value of the sensor and must wait until `available()` is `true`.

It does not support `end()`.



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

In the case of a sensor with wait processing, `arg1` is given `E_EVENT_TICK_TIMER` or `E_EVENT_START_UP` to indicate the elapse of time. After calling this method, if the required time has elapsed, `available()` will be set to `true` and the sensor value can be read.



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



### sns\_stat()

```cpp
uint32_t sns_stat()
```

Various information of the sensor device is stored.

* Stored values are undefined for this device.



### sns\_opt()

```cpp
uint32_t& sns_opt()
```

The value passed in `setup(uint32_t arg1)` is stored.

* The lower 8 bits contain the address of the specified I2C device.
