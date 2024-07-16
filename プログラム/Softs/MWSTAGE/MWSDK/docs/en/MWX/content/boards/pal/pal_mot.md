---
title: "&lt;PAL_MOT&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_MOT&gt;"
---
# \<PAL\_MOT>

Board BEHAVIOR for [MOTION SENSE PAL MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html).

In addition to [common definitions](./), it can handle sensors on the board.

* Accelerometer MC3630

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_MOT>();
}
```



## Member Objects

### sns\_MC3630

[MC3630 Sensor](../../sensor\_object/mc3630.md) object.

###
