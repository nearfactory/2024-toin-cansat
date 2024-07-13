---
title: "&lt;PAL_AMB&gt;"
author: "Mono Wireless Inc."
description: "&lt;PAL_AMB&gt;"
---
# \<PAL\_AMB>

[環境センサーパル AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) のボードビヘイビアです。

[共通定義](./)に加えボード上のセンサーを取り扱えるようになっています。

* 温湿度センサー SHTC3
* 照度センサー LTR308ALS

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_AMB>();
}
```



## メンバーオブジェクト

### sns\_SHTC3

[SHTC3センサー](../../sensor\_object/shtc3.md)のオブジェクトです。

### sns\_LTR308ALS

[LTR308ALSセンサー](../../sensor\_object/ltr-308als.md)のオブジェクトです。









