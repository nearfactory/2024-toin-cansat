---
title: "&lt;MONOSTICK&gt;"
author: "Mono Wireless Inc."
description: "&lt;MONOSTICK&gt;"
---
# \<MONOSTICK>

MONOSTICK用のボードビヘイビアです。内蔵ウォッチドッグタイマーの制御とLED点灯用の手続きが含まれます。



## 定数

以下の定義が利用可能になります。

```cpp
const uint8_t PIN_LED = mwx::PIN_DIGITAL::DIO16;  // LED

const uint8_t PIN_WDT = mwx::PIN_DIGITAL::DIO9;     // WDT (shall tick < 1sec)
const uint8_t PIN_WDT_EN = mwx::PIN_DIGITAL::DIO11; // WDT (LO as WDT enabled)

const uint8_t PIN_LED_YELLOW = mwx::PIN_DIGITAL::DO1; // YELLOW LED
```

`MONOSTICK::PIN_LED`のようにアクセスできます。



## ハードの初期化

```cpp
pinMode(PIN_LED, OUTPUT_INIT_HIGH);
pinMode(PIN_WDT, OUTPUT_INIT_LOW);
pinMode(PIN_WDT_EN, OUTPUT_INIT_LOW);
pinMode(PIN_LED_YELLOW, OUTPUT);
```

上記のコードのように各ピンが初期化されます。



### ウォッチドッグタイマー

起動時、スリープ起床時、起動後一定時間経過後に外部のウォッチドッグタイマーを再セットします。

{% hint style="warning" %}
ウォッチドッグタイマーのタイムアウトは１秒です。

MONOSTICKでは通常スリープするアプリケーションを実行しませんが、その場合は`MONOSTICK::PIN_WDT_EN`をHIGHにしてからスリープします。
{% endhint %}



## メソッド

### set\_led()

```cpp
void set_led_red(uint8_t mode, uint16_t tick)
void set_led_yellow(uint8_t mode, uint16_t tick)


```

LED(赤、黄)の制御を行います。

{% hint style="warning" %}
ボードビヘイビアでの制御を行わない場合は、このメソッドを呼び出さないでください。
{% endhint %}

{% hint style="warning" %}
黄色のLED(`MONOSTICK::PIN_LED_YELLOW`)はSPIMISOピン(半導体のピン名DO1)です。本ボードビヘイビアではPWM制御による点灯用のメソッドや手続きは含まれません。必要に応じて以下の記述を行います。

* `set_led_yellow()`は呼び出さないようにして下さい。
* 始動後にPWM出力の初期化を別途行います。SPIMISOピンはApp\_Twelite標準アプリケーションではPWM3に対応し、Timer3クラスオブジェクトにより制御できます。
* スリープ復帰後にPWM出力の初期化を別途行います。その際、DO1の出力設定を解除します。
* PWM設定前にDO1の出力設定を解除してください。\
  `pinMode(PIN_LED_YELLOW, DISABLE_OUTPUT);`
{% endhint %}

{% hint style="warning" %}
黄色のLED(`MONOSTICK::PIN_LED_YELLOW`)は、スリープ中に点灯させることはできません。
{% endhint %}



`mode`は以下のパラメータを取ります。`tick`は点灯時間\[ms]を指定しますが、詳細は`mode`の解説を参照してください。

| 指定                      | 意味                                                                           |
| ----------------------- | ---------------------------------------------------------------------------- |
| `LED_TIMER::BLINK`      | LEDを点滅させます。`tick`に与える時間\[ms]ごとにON/OFFが切り替わります。スリープ復帰後はカウントをリセットし点灯状態から始まります。 |
| `LED_TIMER::ON_RX`      | パケットの受信時に`tick`に与える時間\[ms]だけ点灯します。                                           |
| `LED_TIMER::ON_TX_COMP` | 送信完了時に`tick`に与える時間\[ms]だけ点灯します。                                              |

{% hint style="info" %}
スリープ復帰後も復帰前の設定が維持されます。
{% endhint %}

