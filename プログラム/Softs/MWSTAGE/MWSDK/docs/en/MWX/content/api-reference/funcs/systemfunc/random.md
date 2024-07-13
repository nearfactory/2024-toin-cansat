---
title: random()
author: "Mono Wireless Inc."
description: Generates an random number.
---
# random()

Generates an random number.

```cpp
uint32_t random(uint32_t maxval)
uint32_t random(uint32_t minval, uint32_t maxval)
```

The first line returns the value of `0.. (maxval-1)` value is returned. Note that the value of **maxval is not the maximum value**.

The second line returns the value of `minval..maxval-1`.
