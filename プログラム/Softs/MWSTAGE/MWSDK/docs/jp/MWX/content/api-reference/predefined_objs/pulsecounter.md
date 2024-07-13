---
title: "PulseCounter"
author: "Mono Wireless Inc."
description: "パルスカウンタ (mwx::periph_pulse_counter)"
---

# PulseCounter

パルスカウンタは、マイコンのCPUが稼働していない時もパルスを読み取り計数する回路です。パルスカウンターは２系統あります。PC0は`PulseCounter0`, PC1は`PulseCounter1`に割り当てられます。

また**`PulseCounter`**は**`PulseCounter1`**の別名です。



## メソッド

### begin()

```cpp
void begin(uint16_t refct = 0, 
           E_PIN_INT_MODE edge = PIN_INT_MODE::FALLING,
           uint8_t debounce = 0)
```

オブジェクトを初期化し、計数を開始します。１番目のパラメータ`refct`は割り込みやavailable判定の基準となるカウント数です。この数を超えたときにアプリケーションに報告されます。また`refct`には0を指定することもできます。この場合は、スリープ起床要因にはなりません。

２番目のパラメータ`edge`は割り込みが立ち会がり(`PIN_INT_MODE::RISING`)か立下り(`PIN_INT_MODE::FALLING`)を指定します。

３番目の`debounce`は、0,1,2,3の値をとります。1,2,3の設定はノイズの影響を小さくするため値の変化の検出に連続した同じ値を要する設定です。

|  設定 | 連続サンプル数 | 最大検出周波数 |
| :-: | :-----: | :-----: |
| `0` |    -    |  100Khz |
| `1` |    2    |  3.7Khz |
| `2` |    4    |  2.2Khz |
| `3` |    8    |  1.2Khz |



### end()

```cpp
void end()
```

検出を中止します。



### available()

```cpp
inline bool available()
```

指定カウント数(`begin()`の`refct`)が０の場合は、カウントが１以上で`true`を返します。

指定カウント数(`begin()`の`refct`)が1以上の場合は、検出回数が指定カウント数を超えた場合に`true`となります。



### read()

```cpp
uint16_t read()
```

カウント値を読み出します。読み出し後にカウント値を０にリセットします。



