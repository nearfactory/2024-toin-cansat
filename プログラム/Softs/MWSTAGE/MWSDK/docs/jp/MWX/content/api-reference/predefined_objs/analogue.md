---
title: "Analogue"
author: "Mono Wireless Inc."
description: "ADC (mwx::periph_analogue.hpp)"
---

# Analogue

Analogueは、ADCの実行と値の取得を行います。一度に複数のチャネルを連続取得でき、またこれをタイマーなどの周期に合わせて逐次実行可能です。

## 定数

### ピンの定義

| 定数                                                                                               | 種別                | 標準アプリでのピン名 |
| ------------------------------------------------------------------------------------------------ | ----------------- | :--------: |
| `uint8_t PIN_ANALOGUE::A1 = 0`                                                                   | ADC1ピン            |     AI1    |
| `uint8_t PIN_ANALOGUE::A2 = 1`                                                                   | ADC2ピン            |     AI3    |
| `uint8_t PIN_ANALOGUE::A3 = 2``uint8_t PIN_ANALOGUE::D0 = 2` | ADC3ピン (DIO0) \*1 |     AI2    |
| `uint8_t PIN_ANALOGUE::A4 = 3``uint8_t PIN_ANALOGUE::D1 = 3` | ADC4ピン (DIO1) \*1 |     AI4    |
| `uint8_t PIN_ANALOGUE::VCC = 4`                                                                  | Vcc 電源電圧          |            |

{% hint style="warning" %}
標準アプリ(App\_Twelite)では、半導体データシート中のピン名ADC2/ADC3が、TWELITE DIPの並びにあわせてAI3/AI2 となっています。ご注意ください。
{% endhint %}

{% hint style="danger" %}
\*1 ディジタル、アナログ共用のADC2/ADC3ピンは利用手続きと利用制限があります。

ADC開始前に利用するピンをプルアップ無しとします。これを実行しないと常にプルアップ電圧をADCで観察することになります。

```
pinMode(PIN_DIGITAL::DIO0, PIN_MODE::INPUT); 
pinMode(PIN_DIGITAL::DIO1, PIN_MODE::INPUT);
```

****

**通常の回路構成では、スリープ時には電流リークが発生します。** ソフトウェアの記述のみで回避することは出来ません。

スリープ時の電流リーク回避には、アナログ回路部分のGNDをFETスイッチなどで切り離し、スリープ中はフローティング状態にします。またスリープ前には入力かつプルアップ状態にピンを設定します。
{% endhint %}



## メソッド

### setup()

```cpp
void setup(
        bool bWaitInit = false,
        uint8_t kick_ev = E_AHI_DEVICE_TICK_TIMER,
        void (*fp_on_finish)() = nullptr) 
```

ADCの初期化を行います。setup()では、半導体内部のレギュレータの始動、周期実行するためのタイマーデバイスの指定、指定チャネルすべてのADCが終了したときに呼び出されるコールバック関数の指定します。

| パラメータ          | 解説                                                                                                                                                                                         |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `bWaitInit`    | `true`を指定すると、半導体内部のレギュレータの初期化を待つ。                                                                                                                                                          |
| `kick_ev`      | 周期実行に指定するタイマーデバイスを指定する。指定可能なデバイスは、以下の５種類で、初回以外は割り込みハンドラ内でADが開始される。`E_AHI_DEVICE_TICK_TIMER (TickTimer)``E_AHI_DEVICE_TIMER0 .. 4 (Timer0 .. 4)` |
| `fp_on_finish` | 指定されたポートすべてのADCが終了後に、割り込みハンドラ内から呼び出されるコールバック関数。ADC計測値をFIFOキューなどに別途格納したい場合に利用する。                                                                                                            |



### begin()

```cpp
void begin(uint8_t bmPorts, uint8_t capt_tick = 1)
```

１番目のパラメータにはADCを行いたいポートを指定します。ポートの指定はピンの定義で述べたポート番号に対応するビットをセットしたビットマップになります。例えば `PIN_ANALOGUE::A2`と`PIN_ANALOGUE::VCC`の２つのピンの値を得たい場合は `(1 <<PIN_ANALOGUE::A1 | 1<<PIN_ANALOGUE::VCC )`を指定します。[`pack_bits`](../funcs/utility/pack\_bits.md)を用い`pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::VCC)`のように記述することもできます。

`begin()`の呼び出し後、速やかに最初のADC処理が開始され、その終了割り込から次のピンの処理を開始します。すべての処理が終われば(指定されている場合）コールバック関数が呼び出されます。次のタイマー割り込みが発生まで待ってから新たなADC処理を開始します。

２番目のパラメータは、ACを開始するまでのタイマー割り込みの回数を指定します。例えば`TickTimer`は1msごとに呼び出されますが、パラメータに`16`を指定すれば 16msごとの処理になります。



```cpp
void begin()
```

デフォルトのADCピン(`PIN_ANALOGUE::A1`,`PIN_ANALOGUE::A2`)を指定してADC処理を開始します。`end()`では中断したADC処理を再開します。



### end()

```cpp
void end()
```

ADC処理を終了し、半導体内部のレギュレータを停止します。



### available()

```cpp
inline bool available()
```

ADCの値が取得後に`true`になります。本関数により確認した後は次のADC完了までは`false`です。



### read(), read\_raw()

```cpp
inline int16_t read(uint8_t s)
inline int16_t read_raw(uint8_t s)
```

ADC値を読み出します。パラメータには読み出したいADCピンを指定します。`read()`はmVに変換した読み値、`read_raw()`はADCの値(0..1023)を戻します。

{% hint style="info" %}
Vccは`read()`で読み出すことを推奨します。`read_raw()`の値からmVに変換するには、特別な変換式を適用する必要があるためです。
{% endhint %}

{% hint style="warning" %}
ADC完了(available)後、次のADC処理が実行するタイミング付近まで遅れて値を読み出すと、次のADC値が戻される場合があります。ADCの処理は割り込みハンドラで実施されているため`loop()`の処理中であっても値が更新されるためです。
{% endhint %}



## ADC割り込みハンドラ

ADCの割り込みハンドラは`setup()`の呼び出し時に`periph_analogue::ADC_handler()`に設定されます。

半導体のペリフェラルライブラリで別途ハンドラを指定すると正常に動作しなくなります。



## スリープ時の振る舞い

ADCが`begin()`により周期実行状態であれば、スリープ復帰後もADC処理を再開します。
