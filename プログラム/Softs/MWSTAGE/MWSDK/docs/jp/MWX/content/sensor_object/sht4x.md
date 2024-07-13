---
title: "SHT4x - 温湿度センサー"
author: "Mono Wireless Inc."
description: "SHT4x - 温湿度センサー"
---

# SHT4x - 温湿度センサー

I2Cバスを利用した温湿度センサーです。

{% hint style="warning" %}
ボードビヘイビア `<ARIA>` を読み込んだ時のみ使用可能です。`begin()`以外の共通メソッドの手続きはボードビヘイビア中で実行されています。
{% endhint %}



## 処理の流れ

1. `Wire.begin()`: バスの初期化
2. `.begin()`: センサーの動作開始
3. 時間待ち数ms
4. `.available()`が`true`になる
5. `.get_temp(), .get_humid()`: 値の読み出し



## 動作に必要な手続き

### Wireバス

`begin()`メソッド呼び出し前に`Wire.begin()`によりWireが動作状態にしておきます。



### スリープ復帰時の手続き

スリープ直前もWireバスが動作状態にしておきます（スリープ復帰後自動でWireを回復します）。



## メソッド

### get\_temp(), get\_temp\_cent()

```cpp
double get_temp()
int16_t get_temp_cent()
```

温度を読み出す。`get_temp()`は℃で、`get_temp_cent()`は℃の100倍の値を整数値で返します。

エラー時は-32760～-32768の値が返ります。



### get\_humid(), get\_humid\_per\_dmil()

```cpp
double get_humid()
int16_t get_humid_per_dmil()
```

湿度を読み出す。`get_humid()`は%で、`get_humid_per_dmil()`は%の100倍の値を整数値で返します。

エラー時は-32760～-32768の値が返ります。



## 共通メソッド

### setup()

```cpp
void setup() 
```

センサー用のメモリ領域の確保や初期化を行います。



### begin(), end()

```cpp
void begin()
void end()
```

センサーの取得を開始します。センサーの値を読み出すまで約5ms待ち時間が必要です。

`end()`には対応しません。



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

待ち時間処理のセンサーの場合は`arg1`に`E_EVENT_TICK_TIMER`または`E_EVENT_START_UP`を与え時間の経過を知らせます。このメソッド呼出し後に、必要な時間が経過していればavailableになり、センサー値の読み出しが可能になります。



### available()

```cpp
bool available()
```

センサーが読み出し条件を満足したときに`true`を返します。



### probe()

```cpp
bool probe()
```

センサーが接続されているときに`true`を返します。

