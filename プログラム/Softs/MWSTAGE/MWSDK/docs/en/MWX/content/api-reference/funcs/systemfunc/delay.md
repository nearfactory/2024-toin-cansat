---
title: delay()
author: "Mono Wireless Inc."
description: Waiting for time by polling.
---
# delay()

Waiting for time by polling.

```cpp
void delay(uint32_t ms)
```

The program waits for a given period of time in `ms`.

The time is measured by the TickTimer count. When waiting for a long period of time, the CPU clock is decreased and polling is performed.

{% hint style="info" %}
Every 5ms after calling `delay()`, TWELITE microcontroller performs internal watchdog processing.

For example, if you execute `while(1) delay(1);`, the watchdog processing is not performed because 5ms does not elapse inside `delay()`, and the reset is executed after a certain time.
{% endhint %}

{% hint style="warning" %}
In the `setup(), wakeup()` function, the TickTimer is not yet running, so it waits for a time by a while loop. In this case, the error with the specified value will be large. This loop counter is adjusted to 32Mhz. If the CPU clock is changed in these functions, the error will be proportional to the clock.

If you specify a short time, such as 1 or 2 as a parameter, the error may be large.
{% endhint %}

