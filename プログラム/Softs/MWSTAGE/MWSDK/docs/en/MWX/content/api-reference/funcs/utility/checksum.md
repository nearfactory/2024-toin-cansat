---
title: CRC8, XOR, LRC
author: "Mono Wireless Inc."
description: functions for calculating checksums.
---

# CRC8, XOR, LRC

This value is often used in checksum calculations.

```cpp
uint8_t CRC8_u8Calc(uint8_t *pu8Data, uint8_t size, uint8_t init=0)
uint8_t CRC8_u8CalcU32(uint32_t u32c, uint8_t init=0)
uint8_t CRC8_u8CalcU16(uint16_t u16c, uint8_t init=0)
uint8_t XOR_u8Calc(uint8_t *pu8Data, uint8_t size)
uint8_t LRC_u8Calc(uint8_t* pu8Data, uint8_t size)
```

CRC8, XOR, LRC([ASCII format](../../classes/ser\_parser.md#asuk)) calculations.

`CRC8_u8CalcU16(), CRC8_u8CalcU32()` computes CRC8 using `u16c, u32c` as big-endian sequence.

{% hint style="info" %}
There are different types of CRC8 depending on the calculation formula, initial value, etc. This library uses a polynomial formula of `X^8+X^5+X^4+1` (Polynomial Value is 0x31). This is sometimes called CRC8-CCITT or CRC8-Maxim.
{% endhint %}

{% hint style="info" %}
XOR is the exclusive OR XOR of each element.
{% endhint %}

{% hint style="info" %}
LRC calculates the sum of the values of each element and takes the two's complement of the lower 8 bits. The result is zero when all elements, including the checksum, are added together.
{% endhint %}


## Background

Added as a library procedure because it is used to check data strings in wireless packets, checksums (LRC) in ASCII format, and various sensors.
