---
title: digitalWrite()
author: "Mono Wireless Inc."
description: Change the setting of the digital output pins.
---
# digitalWrite()

Change the setting of the digital output pins.

```cpp
static inline void digitalWrite(uint8_t u8pin, E_PIN_STATE ulVal)
```

The first parameter specifies the pin number to be set, and the second parameter specifies either `HIGH` or `LOW`.

{% hint style="info" %}
The input is of type `E_PIN_STATE`. The conversion operator from `E_PIN_STATE` to `int` type is not defined, so direct numeric input is not allowed.
{% endhint %}
