---
title: "&lt;PAL_MAG&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_MAG&gt;"
---

# \<PAL\_MAG>

Board BEHAVIOR for [OPEN-CLOSE SENSE PAL OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html).

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_MAG>();
}
```


The sensor of the open/close sensor pal is a magnetic sensor, which is the only input for the interrupt of the two signal lines.

```cpp
const uint8_t PAL_MAG::PIN_SNS_NORTH = 16;
const uint8_t PAL_MAG::PIN_SNS_OUT1 = 16;
const uint8_t PAL_MAG::PIN_SNS_SOUTH = 17;
const uint8_t PAL_MAG::PIN_SNS_OUT2 = 17;
```

`PAL_MAG::PIN_NSNSNS_NORTH` is interrupted when the sensor detects an N-pole and `PAL_MAG::PIN_NSNSNS_SOUTH` is interrupted when the sensor detects an N-pole.



The following settings should be made before sleep

```cpp
pinMode(PAL_MAG::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
pinMode(PAL_MAG::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);
```



Check the IO of the waking factor upon waking.

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
```

