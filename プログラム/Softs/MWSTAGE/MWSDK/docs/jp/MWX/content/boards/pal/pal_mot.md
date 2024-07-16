---
title: "&lt;PAL_MOT&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_MOT&gt;"
---

# \<PAL\_MOT>

[動作センサーパル MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) のボードビヘイビアです。

[共通定義](./)に加えボード上のセンサーを取り扱えるようになっています。

* 加速度センサー MC3630

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_MOT>();
}
```



## メンバーオブジェクト

### sns\_MC3630

[MC3630センサー](../../sensor\_object/mc3630.md)のオブジェクトです。

###
