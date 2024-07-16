---
title: "&lt;BRD_APPTWELITE&gt;"
author: "Mono Wireless Inc."
description: "&lt;BRD_APPTWELITE&gt;"
---

# \<BRD\_APPTWELITE>

標準アプリケーションApp\_Tweliteと同じ配線を想定したボードビヘイビアです。定数定義と、M1-M3,BPSピンの読み出し機能があります。

## 定数

以下の定数を定義しています。`BRD_APPTWELITE::PIN_DI1`のようにアクセスできます。

```cpp
static const uint8_t PIN_DI1 = mwx::PIN_DIGITAL::DIO12;
static const uint8_t PIN_DI2 = mwx::PIN_DIGITAL::DIO13;
static const uint8_t PIN_DI3 = mwx::PIN_DIGITAL::DIO11;
static const uint8_t PIN_DI4 = mwx::PIN_DIGITAL::DIO16;

static const uint8_t PIN_DO1 = mwx::PIN_DIGITAL::DIO18;
static const uint8_t PIN_DO2 = mwx::PIN_DIGITAL::DIO19;
static const uint8_t PIN_DO3 = mwx::PIN_DIGITAL::DIO4;
static const uint8_t PIN_DO4 = mwx::PIN_DIGITAL::DIO9;

static const uint8_t PIN_M1 = mwx::PIN_DIGITAL::DIO10;
static const uint8_t PIN_M2 = mwx::PIN_DIGITAL::DIO2;
static const uint8_t PIN_M3 = mwx::PIN_DIGITAL::DIO3;
static const uint8_t PIN_BPS = mwx::PIN_DIGITAL::DIO17;

static const uint8_t PIN_AI1 = mwx::PIN_ANALOGUE::A1;
static const uint8_t PIN_AI2 = mwx::PIN_ANALOGUE::A3;
static const uint8_t PIN_AI3 = mwx::PIN_ANALOGUE::A2;
static const uint8_t PIN_AI4 = mwx::PIN_ANALOGUE::A4;
```



## メソッド

DIP SW (M1 M2 M3 BPS) ピンの値を取得するためのメソッドが用意されています。

```cpp
inline uint8_t get_M1()
inline uint8_t get_M2()
inline uint8_t get_M3()
inline uint8_t get_BPS()
inline uint8_t get_DIPSW_BM()
```

戻り値はHIGH, LOWではなく、`0`がセットされていない(HIGH側)、`1`がスイッチがセットされる(LOW側)という意味です。

`get_DIPSW_BM()`は、bit0から順にM1,M2,M3,BPSピンの値を返します。

{% hint style="warning" %}
この値はシステム起動時に確認されて以降は、スイッチを操作しても更新されません。
{% endhint %}
