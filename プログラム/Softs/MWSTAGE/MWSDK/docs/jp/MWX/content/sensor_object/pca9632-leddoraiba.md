---
title: "PCA9632 - LEDドライバ"
author: "Mono Wireless Inc."
description: "PCA9632 - LEDドライバ"
---

# PCA9632 - LEDドライバ

通知パル (NOTICE PAL) に使用されている LED ドライバです。

{% hint style="info" %}
通知PAL上での制御は[ボード(BRD) PAL\_NOTICE](../boards/pal/pal\_notice.md)より行います。
{% endhint %}



## 処理の流れ

1. `Wire.begin()`: バスの初期化
2. `.setup()`: クラスオブジェクトの初期化
3. `.reset()`: ドライバの初期化
4. 各種手続き



## PCA9632について

4chのLEDドライバです。

* 各chは消灯・全点灯・PWM点灯・点滅の４つの状態を指定できる
* 各chで独立して照度制御(PWM)できる
* 点滅指定したchはすべて同じ点滅パターンとなる
* 点滅時はPWMによる各ch個別の照度制御ができる



## 動作に必要な手続き

### Wireバス

`setup()`メソッド呼び出し前に`Wire.begin()`によりWireが動作状態にしておきます。



### スリープ復帰時の手続き

スリープ直前もWireバスが動作状態にしておきます（スリープ復帰後自動でWireを回復します）。



### コード例

```cpp
#include <TWELITE>
#include <SNS_PCA9632>

SNS_PCA9632 pca;
```

`#include <SNS_PCA9632>`と`SNS_PCA9632`クラスオブジェクトの宣言が必要です。



#### 初期化&リセット

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

上記の例ではLED1,3をPWM制御により点灯します。

{% hint style="warning" %}
点灯時はドライバが消費する電流値に気を付けてください。
{% endhint %}



## メソッド

### コンストラクタ, setup()

```cpp
SnsPCA9632(uint8_t i2c_addr = DEFAULT_I2C_ADDRESS)
void setup(uint8_t i2c_addr = DEFAULT_I2C_ADDRESS)
```

コンストラクタでは`i2c_addr`を指定します。

グローバル宣言でクラスオブジェクトを定義した場合、コンストラクタが呼びだされませんので、`setup()`を呼び出すようにしてください。



### reset()

```cpp
bool reset()
```

デバイスを初期化します。\
レジスタアドレス 0x0 から順に` {0x81, 0x35, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x0B, 0x00}` を書き込みます。



### set\_mode2()

```cpp
bool set_mode2(uint8_t u8var = 0x35)
```

MODE2レジスタに値を書き込みます。



### set\_power\_mode()

```cpp
bool set_power_mode(bool b_pow_on)
```

`b_pow_on`を`true`にすると通常運用、`false`にするとスリープします。



### set\_blink\_cycle() set\_blink\_cycle\_ms()

```cpp
bool set_blink_cycle(uint8_t u8var)
bool set_blink_cycle_ms(uint16_t u16ms)
```

点滅(グループPWM)周期を決めます。

`u8var`を指定すると周期は`(u8var+1)/24`\[秒]となります。

`u16ms`は周期を\[ms]で指定します。



### set\_blink\_duty()

```cpp
bool set_blink_duty(uint8_t u8duty);
```

点滅(グループPWM)のデューティ比を決めます。点灯期間は`u8duty/256`となり、0は消灯相当、255は全灯相当になります。



### &#x20;set\_led\_duty()

```cpp
bool set_led_duty(uint8_t port, uint8_t duty)
```

明るさ（PMW制御のデューティ比）を指定します。

`port`は対象のLED(`SNS_PCA9632::LED1..4`)を指定します。

`duty`は0..255を指定し、比率`duty/256`で点灯します。



### set\_led\_duty\_all()

```cpp
bool set_led_duty_all(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4)
```

全てのLEDに対して明るさ（PMW制御のデューティ比）を指定します。

`p1,p2,p3,p4`はLED1..4のデューティで0..255を指定します。比率`duty/256`で点灯します。



### set\_led\_status()

```cpp
bool set_led_status(uint8_t u8led1, uint8_t u8led2, uint8_t u8led3, uint8_t u8led4)
```

全てのLEDの点灯状態を変更します。

`u8led1..4`は順にLED1..4の状態を指定します。

指定できる状態は、以下の通りです。

|                          | 内容            |
| ------------------------ | ------------- |
| `SNS_PCA9632::LED_OFF`   | 消灯            |
| `SNS_PCA9632::LED_ON`    | 全灯            |
| `SNS_PCA9632::LED_PWM`   | 照度制御(PWM)     |
| `SNS_PCA9632::LED_BLINK` | 点滅制御(グループPWM) |
| `SNS_PCA9632::LED_NOP`   | 状態を変更しない      |



### probe()

```cpp
bool probe()
```

I2Cバス上にデバイスが存在すれば`true`を返します。



### show\_registers()

```cpp
void show_registers()
```

レジスタ(0x0-0x8)の値を表示します。

