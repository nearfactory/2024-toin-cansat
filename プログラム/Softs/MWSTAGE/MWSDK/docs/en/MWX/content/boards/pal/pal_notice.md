---
title: "&lt;PAL_NOTICE&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_NOTICE&gt;"
---
# &lt;PAL\_NOTICE&gt;

Board BEHAVIOR for [NOTICE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/notice-pal.html).

In addition to [common definition](README.md) plus the ability to handle sensors on the board.

* LED driver PCA9632
* Accelerometer MC3630

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_NOTICE>();
}
```



## member object

### sns\_PCA9632

[PCA9632 device](... /... /sensor_object/pca9632-leddoraiba.md) object. The board definition performs Wire initialization and device initialization. In principle, it is controlled using the PCA9632 operation method described below.

{% hint style="warning" %}
It is not possible to continuously draw a large current from a CR2032 coin cell. In this board definition, the maximum illuminance setting in normal operation is set at a PWM duty ratio of approximately 50%, but even at this setting, overload effects may be apparent. Adjust the combination of LEDs, illumination intensity, and blinking according to the operating requirements.
{% endhint %}

{% hint style="info" %}
When using coin cell batteries, periodic, momentary flashing can mitigate the effects of large currents.

For example, a setting that allows the LED to flash for 20 ms every second or so will not only maintain sufficient visual brightness to mitigate the above effects, but will also reduce battery drain. )
{endhint %}



### sns\_MC3630

[MC3630 sensor](../../sensor_object/mc3630.md) object. The board definition includes SPI initialization, MC3630 device initialization, and MC3630 interrupt processing. Various processes are handled by [`sns_MC3630`](../../sensor_object/mc3630.md).



## PCA9632-definition

```cpp
static const uint8_t LED_OFF = SnsPCA9632::LED_OFF;
static const uint8_t LED_ON = SnsPCA9632::LED_PWM;
static const uint8_t LED_BLINK = SnsPCA9632::LED_BLINK;
static const uint8_t LED_NOP = SnsPCA9632::LED_NOP;

static const uint8_t LED_R = SnsPCA9632::LED1;
static const uint8_t LED_G = SnsPCA9632::LED2;
static const uint8_t LED_B = SnsPCA9632::LED3;
static const uint8_t LED_W = SnsPCA9632::LED4;

static const uint8_t LED_REG_MAX_PWM = 127;
static const uint8_t LED_REG_BOOST_PWM = 255;
```



### Lighting Status

| Definition | Meaning
| :--- | :--- |
| `PAL_NOTICE::LED_OFF` | OFF
| `PAL_NOTICE::LED_ON` | On (PWM illuminance control)
| `PAL_NOTICE::LED_BLINK` | Blinking
| `PAL_NOTICE::LED_NOP` | Do not change |



### LED identification sub

| Definition | Meaning |
| :--- | :--- |
| `PAL_NOTICE::LED_R` | LED Red
| `PAL_NOTICE::LED_G` | LED Green |
| `PAL_NOTICE::LED_B` | LED Green
| `PAL_NOTICE::LED_W` | LED White



### Register Setting Definition

| Definition | Meaning |
| :--- | :--- |
| `PAL_NOTICE::LED_REG_MAX_PWM` | PMW register set value for standard illuminance (1/2 of all lights are standard) |
| `PAL_NOTICE::LED_REG_BOOST_PWM` | PWM register set value for boosting |



## PCA9632-operating method

### Master Switch

```cpp
void set_led_master_sw_on() { digitalWrite(PIN_SNS_EN, LOW); }
void set_led_master_sw_off() { digitalWrite(PIN_SNS_EN, HIGH); }
```

NOTICE PAL has a FET switch at the rear of the PCA9632 output. The LED will not light unless this switch is turned ON.



### Change of lighting status

```cpp
void set_led_r_blink()
void set_led_r_on()
void set_led_r_off()
void set_led_g_on()
void set_led_g_blink()
void set_led_g_off()
void set_led_b_on()
void set_led_b_blink()
void set_led_b_off()
void set_led_w_on()
void set_led_w_blink()
void set_led_w_off()
```

Set individual LEDs to off, on, or blinking.



```cpp
void set_leds(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
void set_leds_off()
```

set\_leds\(\) changes the lighting status of all LEDs. Parameters are [lighting state](pal_notice.md#dian-deng-zhuang-tai)`PAL_NOTICE::LED_OFF` `PAL_NOTICE::LED_ON` `PAL_NOTICE::LED_BLINK` `PAL_NOTICE::LED_NOP`. 



### Light illumination control

```cpp
void set_led_brightness_r_reg(uint8_t duty)
void set_led_brightness_g_reg(uint8_t duty)
void set_led_brightness_b_reg(uint8_t duty)
void set_led_brightness_w_reg(uint8_t duty)
void set_leds_brightness_reg(uint8_t r, uint8_t g, uint8_t b, uint8_t w)

void set_led_brightness_r1000(uint16_t duty, bool boost = false)
void set_led_brightness_g1000(uint16_t duty, bool boost = false)
void set_led_brightness_b1000(uint16_t duty, bool boost = false)
void set_led_brightness_w1000(uint16_t duty, bool boost = false)
void set_leds_brightness1000(
    uint16_t r, uint16_t g, uint16_t b, uint16_t w, bool boost = false)
```

Specifies the PWM duty ratio (LED lighting brightness).

`set_led_brightness_? _reg()` and `set_leds_brightness_reg()` specify the register value directly. 0...255 is specified and the lighting will be the ratio `duty/256`.

`set_led_brightness_?1000()` and `set_leds_brightness1000()` specify a duty ratio of 0..1000, where 0 corresponds to no light and the larger the value, the brighter (longer lighting interval). If `boost` is set to `false`, the register value is 127 when 1000 is specified. If `boost` is set to `true`, the register value will be 255.

{% hint style="warning" %}
Since PAL is operated by a coin cell battery, the specified maximum value is set to a duty ratio of approximately 50%.
{% endhint %}

{% hint style="warning" %}
The final setting is assigned to the register value, so even if 0...1000 steps are specified, the actual control value will be the precision of the register value.
{% endhint %}



### Blinking Control

```cpp
void set_blink_cycle_ms(uint16_t x)
void set_blink_duty1000(uint16_t x)
```

LEDs with `PAL_NOTICE::LED_BLINK` will blink with the specified period and lighting duration ratio.

* Blinking patterns cannot be specified individually.
* Brightness can also be controlled since the LEDs are turned on at the PWM duty ratio specified in the lighting illuminance setting above.



`set_blink_cycle_ms()` blinks for a period of time specified by `x`.

set_blink_duty1000()` sets `x` to 0..1000 and blinks for `cycle*x/1000` as the lighting period.

{% hint style="warning" %}
The final setting is assigned to the register value, so even if you specify 0...1000 steps, the actual control value will be the precision of the register value.
{% endhint %}



### Test lighting

```cpp
void test_led()
```

Turns on the 4 LEDs for a very short period of time. After lighting, the master switch is turned on (`set_led_master_sw_on()`).

