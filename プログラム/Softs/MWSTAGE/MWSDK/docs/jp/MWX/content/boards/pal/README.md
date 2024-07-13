---
title: "PAL"
author: "Mono Wireless Inc."
description: "PAL 共通定義"
---

# PAL

TWELITE PALのハードには共通部分があり、ボードビヘイビアも共通ハードについては、共通のインタフェースを定義しています。



## 定数

以下の定義が利用可能になります。

```cpp
static const uint8_t PIN_BTN = 12; // button (as SET)
static const uint8_t PIN_LED = 5;  // LED
static const uint8_t PIN_WDT = 13; // WDT (shall tick every 60sec)

static const uint8_t PIN_D1 = 1; // DIP SW1
static const uint8_t PIN_D2 = 2; // DIP SW2
static const uint8_t PIN_D3 = 3; // DIP SW3
static const uint8_t PIN_D4 = 4; // DIP SW4

static const uint8_t PIN_SNS_EN = 16;
static const uint8_t PIN_SNS_INT = 17;
```

`PAL_AMB::PIN_BTN`のようにアクセスできます。



## ハードの初期化

```cpp
pinMode(PIN_BTN, INPUT_PULLUP);
pinMode(PIN_LED, OUTPUT_INIT_HIGH);
pinMode(PIN_WDT, OUTPUT_INIT_HIGH);

pinMode(PIN_D1, INPUT_PULLUP);
pinMode(PIN_D2, INPUT_PULLUP);
pinMode(PIN_D3, INPUT_PULLUP);
pinMode(PIN_D4, INPUT_PULLUP);
```

上記のコードのように各ピンが初期化されます。



### ウォッチドッグタイマー

起動時、スリープ起床時、起動後一定時間経過後に外部のウォッチドッグタイマーを再セットします。

{% hint style="warning" %}
ウォッチドッグタイマーをタイムアウトしないためにTWELITEを60秒以内の設定（キャリブレーション済み内部CRタイマー使用時）で起床してください。
{% endhint %}



## メソッド

### set\_led()

```cpp
void set_led(uint8_t mode, uint16_t tick)
```

LED(D1)の制御を行います。

{% hint style="warning" %}
ボードビヘイビアでの制御を行わない場合は、このメソッドを呼び出さないでください。
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



### led\_one\_shot()

```cpp
void led_one_shot(uint16_t tick)
```

指定期間だけLEDを点灯します。`set_led()`の機能と同時には使えません。



### get\_D1() .. D4(), get\_DIPSW\_BM()

```cpp
inline uint8_t get_D1()
inline uint8_t get_D2()
inline uint8_t get_D3()
inline uint8_t get_D4()
inline uint8_t get_DIPSW_BM()
```

`get_D1() .. get_D4()`はDIP SWがHIGH(スイッチが上)の時`0`、LOW(スイッチが下)のとき`1`を返します。

`get_DIPSW_BM()`はDIP SWの設定値を`0..15`で返します。`SW1==LOW` を`1`, `SW2 == LOW`を`2`, `SW3 == LOW`を`4`, `SW4 == LOW`を`8`とした和を返します。

{% hint style="warning" %}
D1..D4の`HIGH(1),LOW(0)`の値とは反対になります。

DIP SWの`LOW(0)`側がセットされた、つまり、`1`の値を持つと意味付けしているためです。
{% endhint %}

{% hint style="warning" %}
この値はシステム起動時に確認されて以降は、スイッチを操作しても更新されません。
{% endhint %}

