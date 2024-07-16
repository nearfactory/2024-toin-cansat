---
title: "PCA9632 - LED Driver"
author: "Mono Wireless Inc."
description: "PCA9632 - LED Driver"
---
# PCA9632 - LED Driver

LED driver used for NOTICE PAL.

{% hint style="info" %}
The control on the NOTICE PAL is done via [board(BRD) PAL\_NOTICE](../boards/pal/pal\_notice.md).
{% endhint %}



## Process flow

1.`Wire.begin()`: Initialize bus
2 `.setup()`: Initialize the class object
`.reset()`: Initialization of the driver 4.
4. various procedures



## About PCA9632

This is a 4-channel LED driver.

* Each channel can specify 4 states: off, all lights on, PWM on, and blinking.
* Each channel can control illuminance (PWM) independently.
* All chs designated as blinking have the same blinking pattern.
* When blinking, illuminance can be controlled for each channel individually by PWM



## Procedures required for operation

### Wire bus

Before calling the `setup()` method, Wire must be put into operation state by `Wire.begin()`.



### Procedures when returning from sleep mode

Keep the Wire bus in the operating state just before sleep (the Wire is automatically restored after sleep recovery).



### Code Example

```cpp
#include <TWELITE>
#include <SNS_PCA9632>

SNS_PCA9632 pca;
```

`#include <SNS_PCA9632>` and declaration of `SNS_PCA9632` class object is required.



#### Initialization & Reset

```cpp
void setup() {
    Wire.begin();
    pca.setup();
    pca.reset();
}
```



#### 点灯

```cpp
...
 ​  pca.set_led_duty_all(
      127,
      127,
      127,
      127
   );
   
   pca.set_led_status(
      SNS_PCA9632::LED_PWM,
      SNS_PCA9632::LED_NOP,
      SNS_PCA9632::LED_PWM,
      SNS_PCA9632::LED_NOP);
```

In the above example, LEDs 1 and 3 are turned on by PWM control.

{% hint style="warning" %}
Be careful of the current value consumed by the driver when lighting up.
{% endhint %}



## Methods.

### Constructor, setup()

```cpp
SnsPCA9632(uint8_t i2c_addr = DEFAULT_I2C_ADDRESS)
void setup(uint8_t i2c_addr = DEFAULT_I2C_ADDRESS)
```

Specify `i2c_addr` in the constructor.

If you define a class object in a global declaration, the constructor will not be called, so call `setup()`.



### reset()

```cpp
bool reset()
```

Initializes the device. \0x0}
Writes `{0x81, 0x35, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0B, 0x00}` starting with register address 0x0.



### set\_mode2()

```cpp
bool set_mode2(uint8_t u8var = 0x35)
```

Write a value to the MODE2 register.



### set\_power\_mode()

```cpp
bool set_power_mode(bool b_pow_on)
```

Setting `b_pow_on` to `true` will cause normal operation, setting it to `false` will cause sleep.



### set\_blink\_cycle() set\_blink\_cycle\_ms()

```cpp
bool set_blink_cycle(uint8_t u8var)
bool set_blink_cycle_ms(uint16_t u16ms)
```

Determines the blinking (group PWM) period.

If `u8var` is specified, the period is `(u8var+1)/24`[s].

The `u16ms` specifies the period in œms.



### set\_blink\_duty()

```cpp
bool set_blink_duty(uint8_t u8duty);
```

Determines the duty ratio of blinking (group PWM). The lighting period is `u8duty/256`, where 0 corresponds to lights off and 255 corresponds to all lights on.



### &#x20;set\_led\_duty()

```cpp
bool set_led_duty(uint8_t port, uint8_t duty)
```

brightness (duty ratio of PMW control).

`port` specifies the target LED (`SNS_PCA9632::LED1..4`).

... `duty` specifies 0..255, and the LEDs are turned on with the ratio `duty/256`.



### set\_led\_duty\_all()

```cpp
bool set_led_duty_all(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4)
```

Specify the brightness (duty ratio of PMW control) for all LEDs.

`p1,p2,p3,p4` specifies 0...255 in duty for LED1...4. The LEDs turn on at a ratio `duty/256`.



### set\_led\_status()

```cpp
bool set_led_status(uint8_t u8led1, uint8_t u8led2, uint8_t u8led3, uint8_t u8led4)
```

Changes the lighting state of all LEDs.

`u8led1..4` specifies the state of LED1..4 in order.

The states that can be specified are as follows.

|    | Content |
| ------------------------ | ------------- |
| `SNS_PCA9632::LED_OFF` | OFF |
| `SNS_PCA9632::LED_ON` | All lights on
| `SNS_PCA9632::LED_PWM` | Illumination control(PWM)
| `SNS_PCA9632::LED_BLINK` | Blink control (group PWM)
| `SNS_PCA9632::LED_NOP` | Do not change state |



### probe()

```cpp
bool probe()
```

Returns `true` if a device exists on the I2C bus.



### show\_registers()

```cpp
void show_registers()
```

Displays the value of register (0x0-0x8).

