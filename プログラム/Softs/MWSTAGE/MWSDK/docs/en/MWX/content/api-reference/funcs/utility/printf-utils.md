---
title: Printf utils
author: "Mono Wireless Inc."
description: Printf utils
---
# Printf utils

The mwx library uses an implementation of printf, which is commonly used in C, for formatting output. Function definitions for several functions are available.

```cpp
int mwx_printf(const char* format, ...)
int mwx_snprintf(char* buffer, size_t count, const char* format, ...)
```

`mwx_printf()` prints a printf output to the Serial object. It is the same process as `Serial.printfmt()`.

mwx_snprintf()` prints a snprintf to a buffer.

{% hint style="info" %}
Array class [smplbuf](... /... /classes/smplbuf/), you can use [mwx::stream to generate data](../../classes/smplbuf/README.md#mwxstreamdedta).
{% endhint %}
