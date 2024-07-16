---
title: "mwx::bigendian"
author: "Mono Wireless Inc."
description: twe::stream にビッグエンディアン順で数値型のデータを出力する
---

# mwx::bigendian

`mwx::stream` の `<<` 演算子に対して数値型をビッグエンディアンのバイト列で出力するヘルパークラスです。

```cpp
Serial << mwx::bigendian(0x1234abcdUL);

// output binary -> 0x12 0x34 0xab 0xcd
```



### コンストラクタ

```cpp
template <typename T>
bigendian::bigendian(T v)
```



| パラメータ | 解説                             |
| ----- | ------------------------------ |
| `v`   | `uint16_t` または `uint32_t` の型の値 |





