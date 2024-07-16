---
title: "&lt;PAL_AMB&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_AMB&gt;"
---
# \<PAL\_AMB>

Board BEHAVIOR for [Environmental Sensor Pal AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html).

[Common Definition](./) plus the sensors on the board can be handled.

* Temperature/humidity sensor SHTC3
* Illuminance sensor LTR308ALS

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_AMB>();
}
```



## member object

### sns\_SHTC3

[SHTC3 sensor](../../sensor\_object/shtc3.md) object.

### sns\_LTR308ALS

[LTR308ALS sensor](../../sensor\_object/shtc3.md) object.

