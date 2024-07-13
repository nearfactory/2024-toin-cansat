---
title: "LTR-308ALS - 照度センサー"
author: "Mono Wireless Inc."
description: "LTR-308ALS - 照度センサー"
---

# LTR-308ALS - 照度センサー

I2Cバスを利用した照度センサーです。

{% hint style="warning" %}
ボードビヘイビア `<PAL_AMB>` を読み込んだ時のみ使用可能です。`begin()`以外の共通メソッドの手続きはボードビヘイビア中で実行されています。
{% endhint %}



## 処理の流れ

1. `Wire.begin()`: バスの初期化
2. `.begin()`: センサーの動作開始
3. 時間待ち50ms
4. `.available()`が`true`になる
5. `.get_luminance()`: 値の読み出し



## 動作に必要な手続き

### Wireバス

`.begin()`メソッド呼び出し前に`Wire.begin()`によりWireが動作状態にしておきます。



### スリープ復帰時の手続き

スリープ直前もWireバスが動作状態にしておきます（スリープ復帰後自動でWireを回復します）。



## メソッド

### get\_luminance()

```cpp
uint32_t get_luminance()
```

照度\[lx]を整数値で返します。

エラーの時は`-1`が返ります。



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

センサーの取得を開始します。センサーの値を読み出すまで約50ms待ち時間が必要です。

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
