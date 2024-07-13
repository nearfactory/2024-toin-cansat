---
title: pinMode()
author: "Mono Wireless Inc."
description: Sets the DIO (general-purpose digital IO) pin.
---

# pinMode()

Sets the DIO (general-purpose digital IO) pin.

```cpp
void pinMode(uint8_t u8pin, E_PIN_MODE mode)
```

This function allows you to change the state of DIO0..19 and the pins DO0,1. The setting contents are described in the enumeration value of `E_PIN_MODE`, [description of DIO](README.md) and [Description of DO](README.md).

{% hint style="danger" %}
DO0,1 are special pins, which in principle are used for other purposes, but can also be configured as outputs. However, these pins have hardware restrictions, so care must be taken when using them.

Both pins must be guaranteed to be at a HIGH level when power is applied. If the circuit is configured to take unstable voltages, the module will not start up.
{% endhint %}
