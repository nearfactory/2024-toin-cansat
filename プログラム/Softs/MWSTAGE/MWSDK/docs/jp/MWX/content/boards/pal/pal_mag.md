---
title: "&lt;PAL_MAG&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_MAG&gt;"
---
# \<PAL\_MAG>

[開閉センサーパル OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) のボードビヘイビアです。

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_MAG>();
}
```



開閉センサーパルのセンサーは磁気センサーで、２本の信号線の割り込みの入力のみです。

```cpp
const uint8_t PAL_MAG::PIN_SNS_NORTH = 16;
const uint8_t PAL_MAG::PIN_SNS_OUT1 = 16;
const uint8_t PAL_MAG::PIN_SNS_SOUTH = 17;
const uint8_t PAL_MAG::PIN_SNS_OUT2 = 17;
```

PAL\_MAG::PIN\_SNS\_NORTHはセンサーがN極を検出したとき、PAL\_MAG::PIN\_SNS\_SOUTHはセンサーがN極を検出したときに割り込みが入ります。



スリープ前に以下の設定をしておきます。

```cpp
pinMode(PAL_MAG::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
pinMode(PAL_MAG::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);
```



起床時に起床要因のIOを確認します。

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
```

