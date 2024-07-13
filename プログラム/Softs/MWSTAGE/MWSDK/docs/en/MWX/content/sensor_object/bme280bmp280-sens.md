---
title: "BMx280 - Environmental Sensors"
author: "Mono Wireless Inc."
description: "BMx280 - Environmental Sensors"
---
# BMx280 - Environmental Sensors

Barometric pressure, temperature, and humidity (BME280 only) sensor using I2C bus.

{% hint style="info" %}
This sensor is not used in the TWELITE PAL series. Please refer to the following for usage examples.
[https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30](https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30)
{% endhint %}



## Process flow

1 `Wire.begin()`: Initialize bus
2 `.setup()`: Initialize the sensor
3.`.begin()`: Start operation of the sensor 4.
4.wait()`: wait for a few ms
5.`.available()` becomes `true
6. `.get_press(), .get_temp(), .get_humid()`: read values



## Procedures required for operation

### Wire bus

Before calling the `setup()` method, the Wire must be put into operation by `Wire.begin()`.



### Procedures when returning from sleep mode

Keep the Wire bus in the operating state just before sleep (the Wire is automatically restored after sleep).



### Code Example

```cpp
#include <TWELITE>
#include <SNS_BME280>

SNS_BME280 sns_bme280; // Declaration of Objects
```

`#include <SNS_SHT3X>` and declaration of `SNS_SHT3X` class object is required.



#### initialization

```cpp
void setup() {
    Wire.begin();
    sns_bme280.setup();
}
```



#### Start acquiring sensor values

```cpp
void loop() {

  if(eState == E_STATE::INIT) {
    sns_bme280.begin();
    eState = E_STATE::CAPTURE;
  }

}
```

Call `.begin()` to start acquiring sensor values. It takes several ms to complete.

In the above `loop()`, the process is designed to branch according to the state variable `eState`. ([Reference](../act\_samples/slp\_wk\_and\_tx.md))



#### センサー値の取得待ち

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_bme280.available()) {
      // Sensor value readout available
    }
  }

}
```

Whether the sensor value is ready or not can be determined by `.available()`.



#### Readout of sensor values

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_bme280.available()) {
      Serial << crlf << "BMx280:"
            << " P=" << int(sns_bme280.get_press()) << "hP";
            << " T=" << sns_bme280.get_temp() << 'C'
						<< " H=" << sns_bme280.get_humid() << '%';
    }
  }

}
```

As soon as the sensor value is ready, the value can be read out.



`.get_temp(), get_humid()` includes floating point operations; you can also get 100x integer values.

```cpp
auto temp = div100(sns_bme280.get_temp_cent());
auto humd = div100(sns_bme280.get_humid_per_dmil);

Serial << crlf << "BMx280:"
  << " P=" << int(sns_bme280.get_press()) << "hP";
  << format(" T=%c%d.%02d", temp.neg ? '-' : ' ', temp.quo, temp.rem)
  << format(" T=%c%d.%02d", humd.neg ? '-' : ' ', humd.quo, humd.rem);
```

Here [`div100()`](../api-reference/funcs/utility/div100.md) is used to decompose 100x values into integer and decimal parts.



## Methods

### get\_press()

```cpp
int16_t get_press()
```

Reads the atmospheric pressure. The unit is the hectopascal (_hectopascal_), usually around 1000.



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

Reads the humidity. `get_humid()` returns an integer value in % and `get_humid_per_dmil()` returns 100 times %.

On error, values between -32760 and -32768 are returned.



## Common methods

### setup()

```cpp
void setup(uint32_t arg1 = 0UL) 
```

Allocates and initializes the memory area for the sensor.

The 8 bits from LSB of `arg1` can store the I2C address. If not specified, leave it as 0.

```cpp
#include <SNS_BME280>
SNS_BME280 sns_bme280;
bool b_found_bme280 = false;

void setup() {
  ...
  sns_bme280.setup();
	if (!sns_bme280.probe()) {
			delayMicroseconds(100); // device needs small time for further I2C comm.
			sns_bme280.setup(0x77); // alternative ID
			if (sns_bme280.probe()) b_found_bme280 = true;
	} else {
			b_found_bme280 = true;
	}
	...
```

The above code first tries to see if the device responds with the default I2C ID, and if not, tries with `0x77`.



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

Various information on the sensor device is stored.

* The lower 8 bits store the chip model of BME280/BMP280. If it is `0x60`, it is BME280, and if it is `0x58`, it is BMP280.



### sns\_opt()

```cpp
uint32_t& sns_opt()
```

The value passed in `setup(uint32_t arg1)` is stored.

* The lower 8 bits contain the address of the specified I2C device.





