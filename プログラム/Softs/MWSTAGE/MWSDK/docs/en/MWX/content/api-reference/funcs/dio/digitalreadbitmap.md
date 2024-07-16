---
title: digitalReadBitmap()
author: "Mono Wireless Inc."
description: Reads the values of all ports in the input settings at once.
---

# digitalReadBitmap()

{% hint style="success" %}
Included in mwx library 0.1.4 or later
{% endhint %}

Reads the values of all ports in the input settings at once.

```cpp
uint32_t digitalReadBitmap()
```

The values are stored in the order of DIO0 ... DIO19 from the LSB side. DIO19 are stored in this order.

The pins on the HIGH side are set to 1 and the pins on the LOW side are set to 0.
