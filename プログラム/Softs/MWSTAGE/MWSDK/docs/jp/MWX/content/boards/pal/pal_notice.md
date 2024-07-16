---
title: "&lt;PAL_NOTICE&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_NOTICE&gt;"
---
# &lt;PAL\_NOTICE&gt;

[通知パル NOTICE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/notice-pal.html) のボードビヘイビアです。

[共通定義](./)に加えボード上のセンサーを取り扱えるようになっています。

* LEDドライバ PCA9632
* 加速度センサー MC3630

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_NOTICE>();
}
```



## メンバーオブジェクト

### sns\_PCA9632

[PCA9632デバイス](../../sensor_object/pca9632-leddoraiba.md)のオブジェクトです。ボード定義ではWireの初期化、デバイスの初期化を実施します。原則として後述のPCA9632操作メソッドを用いて制御します。

{% hint style="warning" %}
CR2032コイン電池から大きな電流を継続的に取り出すことは出来ません。本ボード定義上では通常時の最大照度設定を約50%のPWMデューティ比としていますが、この設定でも過負荷による影響が顕出する場合があります。動作要件に応じて発光するLEDの組み合わせ、点灯照度、点滅を調整してください。
{% endhint %}

{% hint style="info" %}
コイン電池を利用する場合、周期的に一瞬光るといった動作を行えば大きな電流による影響を緩和できます。

例えば１秒おきに20ms程度光るような設定を行えば、視覚的に十分な明るさを維持でき、上記影響を緩和できるだけでなく、電池の消耗も小さくできます（連続で5mA程度流す照度設定を、1秒に20ms点灯する場合はLED消費の平均電流は0.1mA）。
{% endhint %}



### sns\_MC3630

[MC3630センサー](../../sensor_object/mc3630.md)のオブジェクトです。ボード定義ではSPIの初期化、MC3630デバイスの初期化、MC3630の割り込み処理などを行っています。諸処理は[`sns_MC3630`](../../sensor_object/mc3630.md)定義の手続きを用います。



## PCA9632-定義

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



### 点灯状態

| 定義 | 意味 |
| :--- | :--- |
| `PAL_NOTICE::LED_OFF` | 消灯 |
| `PAL_NOTICE::LED_ON` | 点灯（PWM照度制御\) |
| `PAL_NOTICE::LED_BLINK` | 点滅 |
| `PAL_NOTICE::LED_NOP` | 変更しない |



### LED識別子

| 定義 | 意味 |
| :--- | :--- |
| `PAL_NOTICE::LED_R` | LED赤 |
| `PAL_NOTICE::LED_G` | LED緑 |
| `PAL_NOTICE::LED_B` | LED青 |
| `PAL_NOTICE::LED_W` | LED白 |



### レジスタ設定定義

| 定義 | 意味 |
| :--- | :--- |
| `PAL_NOTICE::LED_REG_MAX_PWM` | 標準照度のPMWレジスタ設定値（全灯の1/2を標準とする） |
| `PAL_NOTICE::LED_REG_BOOST_PWM` | ブースト時のPWMレジスタ設定値 |



## PCA9632-操作メソッド

### マスタースイッチ

```cpp
void set_led_master_sw_on() { digitalWrite(PIN_SNS_EN, LOW); }
void set_led_master_sw_off() { digitalWrite(PIN_SNS_EN, HIGH); }
```

NOTICE PAL では、PCA9632の出力後段にFETスイッチを設けています。このスイッチをONにしない限りLEDは点灯しません。



### 点灯状態変更

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

個別のLEDを消灯、点灯、点滅に設定します。



```cpp
void set_leds(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
void set_leds_off()
```

set\_leds\(\)は全てのLEDの点灯状態を変更します。パラメータは[点灯状態](pal_notice.md#dian-deng-zhuang-tai)`PAL_NOTICE::LED_OFF` `PAL_NOTICE::LED_ON` `PAL_NOTICE::LED_BLINK` `PAL_NOTICE::LED_NOP` のいずれかを指定します。 



### 点灯照度制御

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

PWMのデューティ比（LEDの点灯明るさ）を指定します。

`set_led_brightness_?_reg()`と`set_leds_brightness_reg()`はレジスタ値を直接指定します。0..255を指定し点灯は比`duty/256`となります。

`set_led_brightness_?1000()`と`set_leds_brightness1000()`は、デューティ比を0..1000で指定します。0は消灯相当で値が大きくなるほど明るく（点灯区間が長くなる）なります。`boost`を`false`にすると1000を指定したときのレジスタ値が127となります。`true`では255となります。

{% hint style="warning" %}
NOTICE PALがコイン電池により運用されるため、規定最大値を約50%のデューティ比としています。
{% endhint %}

{% hint style="warning" %}
最終的な設定はレジスタ値に割り当てられるため0..1000段階の指定であっても、実際の制御値はレジスタ値の精度になります。
{% endhint %}



### 点滅制御

```cpp
void set_blink_cycle_ms(uint16_t x)
void set_blink_duty1000(uint16_t x)
```

`PAL_NOTICE::LED_BLINK` を指定したLEDは、指定した周期・点灯期間比で点滅します。

* 個別に点滅パターンを指定することは出来ません。
* 上記の点灯照度設定で指定したPWMデューティ比で点灯するため、明るさの制御も可能です。



`set_blink_cycle_ms()`は`x`で指定した期間\[ms\]を周期として点滅します。

`set_blink_duty1000()`は`x`を0..1000で指定し`周期*x/1000`を点灯期間として点滅します。

{% hint style="warning" %}
最終的な設定はレジスタ値に割り当てられるため0..1000段階の指定であっても、実際の制御値はレジスタ値の精度になります。
{% endhint %}



### テスト点灯

```cpp
void test_led()
```

ごく短い間４つのLEDを点灯します。点灯後はマスタースイッチがON（`set_led_master_sw_on()`）になります。

