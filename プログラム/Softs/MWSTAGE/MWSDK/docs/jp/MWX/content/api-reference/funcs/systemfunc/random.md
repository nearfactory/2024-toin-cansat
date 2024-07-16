---
title: "random()"
author: "Mono Wireless Inc."
description: "random()"
---

# random()

乱数を生成します。

```cpp
uint32_t random(uint32_t maxval)
uint32_t random(uint32_t minval, uint32_t maxval)
```

１行目は`0..(maxval-1)`の値を戻します。**maxvalの値が最大値ではない**ことに注意してください。

２行目は`minval..maxval-1`の値を戻します。
