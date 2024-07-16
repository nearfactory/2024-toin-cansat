---
title: delayMicroseconds()
author: "Mono Wireless Inc."
description: Wait for time by polling (specified in μsec).
---
# delayMicroseconds()

{% hint style="success" %}
It is not included in MWSDK2020\_05. Supported packages will be MWSDK\_2020\_07\_UNOFFICIAL or later.
{% endhint %}

Wait for time by polling (specified in μsec).

```cpp
void delayMicroseconds(uint32_t microsec)
```

Wait for a given period of time in `microsec`.

The time is measured by the TickTimer count. When waiting for a long time, the CPU clock is reduced and polling is performed.

{% hint style="warning" %}
In the `setup(), wakeup()` function, TickTimer is not yet running, so it waits for a while in a while loop. In this case, the error with the specified value will be large. This loop counter is adjusted to 32Mhz. If the CPU clock is changed in these functions, the error will be proportional to the clock.

If you specify a short time, such as less than 10 for a parameter, the error may be large.
{% endhint %}
