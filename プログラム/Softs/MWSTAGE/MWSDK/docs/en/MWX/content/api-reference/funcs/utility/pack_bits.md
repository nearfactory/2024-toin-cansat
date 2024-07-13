---
title: pack_bits()
author: "Mono Wireless Inc."
description: Sets 1 at the specified bit position.
---
# pack\_bits()

Sets 1 at the specified bit position.

```cpp
constexpr uint32_t pack_bits(...)
```

Parameters can be specified as a variable number of arguments, each parameter specifying a 0..31 integer that specifies a bit position. For example, specifying `pack_bits(1,3,6)` returns `((1UL<<1)|(1UL<<3)|(1UL<<6))`.

{% hint style="info" %}
`constexpr` will expand constants at compile time if computation by constants is possible.
{% endhint %}



## Background

There are situations where values are referenced and set in various bitmaps, such as the status of IO ports (DI, DO), to simplify the description.
