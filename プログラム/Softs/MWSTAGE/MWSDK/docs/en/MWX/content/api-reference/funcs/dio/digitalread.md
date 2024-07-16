---
title: TITLE
author: "Mono Wireless Inc."
description: Reads the value of the port of the input configuration.
---
# digitalRead()

Reads the value of the port of the input configuration.

```cpp
static inline E_PIN_STATE digitalRead(uint8_t u8pin)
```

Get the input value of a pin that has been previously set as an input, either `LOW` or `HIGH`.

{% hint style="info" %}
No conversion operator from type `E_PIN_STATE` to type `int` is defined, so direct assignment to a numeric type is not possible.
{% endhint %}
