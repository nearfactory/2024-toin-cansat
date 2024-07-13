---
title: "&lt;CUE&gt;"
author: "Mono Wireless Inc."
description: "&lt;CUE&gt;"
---
# \<CUE>

[TWELITE CUE](https://mono-wireless.com/cue)上のペリフェラルに対応するボードビヘイビアです。

ボード上の加速度センサー、磁気センサー、LEDを取り扱えるようになっています。

* 加速度センサー
* 磁気センサー
* LED

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<CUE>();
}
```



## 加速度センサー

[MC3630センサー](../sensor\_object/mc3630.md)のメンバーオブジェクト (sns\_MC3630) が定義されています。



## 磁気センサー

開閉センサーパルのセンサーは磁気センサーで、２本の信号線の割り込みの入力のみです。

```cpp
const uint8_t CUE::PIN_SNS_NORTH = 16;
const uint8_t CUE::PIN_SNS_OUT1 = 16;
const uint8_t CUE::PIN_SNS_SOUTH = 8;
const uint8_t CUE::PIN_SNS_OUT2 = 8;
```

PAL\_MAG::PIN\_SNS\_NORTHはセンサーがN極を検出したとき、PAL\_MAG::PIN\_SNS\_SOUTHはセンサーがN極を検出したときに割り込みが入ります。



スリープ前に以下の設定をしておきます。

```cpp
pinMode(CUE::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
pinMode(CUE::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);
```



起床時に起床要因のIOを確認します。

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(CUE::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(CUE::PIN_SNS_SOUTH);
```



## LED

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



## ウォッチドッグタイマー

起動時、スリープ起床時、起動後一定時間経過後に外部のウォッチドッグタイマーを再セットします。

{% hint style="warning" %}
ウォッチドッグタイマーをタイムアウトしないためにTWELITEを60秒以内の設定（キャリブレーション済み内部CRタイマー使用時）で起床してください。
{% endhint %}

