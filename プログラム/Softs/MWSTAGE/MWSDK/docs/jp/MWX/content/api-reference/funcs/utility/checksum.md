---
title: "CRC8, XOR, LRC"
author: "Mono Wireless Inc."
description: "CRC8, XOR, LRC"
---

# CRC8, XOR, LRC

チェックサムの計算で良く用いられる値です。

```cpp
uint8_t CRC8_u8Calc(uint8_t *pu8Data, uint8_t size, uint8_t init=0)
uint8_t CRC8_u8CalcU32(uint32_t u32c, uint8_t init=0)
uint8_t CRC8_u8CalcU16(uint16_t u16c, uint8_t init=0)
uint8_t XOR_u8Calc(uint8_t *pu8Data, uint8_t size)
uint8_t LRC_u8Calc(uint8_t* pu8Data, uint8_t size)
```

CRC8, XOR, LRC([アスキー形式](../../classes/ser\_parser.md#asuk)で使用)の計算を行います。

`CRC8_u8CalcU16(), CRC8_u8CalcU32()`は`u16c, u32c`をビッグエンディアン並びとして、CRC8を計算します。

{% hint style="info" %}
CRC8は、計算式や初期値などによって種類がありますが、本ライブラリでは多項式を`X^8+X^5+X^4+1`(Polynomial Valueを0x31)をとしたものを使用しています。これはCRC8-CCITT や CRC8-Maximと呼ばれることがあります。
{% endhint %}

{% hint style="info" %}
XORは各要素の排他的論理和 XOR をとったものです。
{% endhint %}

{% hint style="info" %}
LRCは各要素の値の合計を計算し、下位８ビットの２の補数を取ります。結果、チェックサムを含め全要素を足し算すると０になります。
{% endhint %}



## 背景

無線パケットのデータ列、アスキー形式のチェックサム(LRC)、各種センサーのデータチェック用に利用されるため、ライブラリ手続きとして追加した。
