---
title: TickTimer
author: "Mono Wireless Inc."
description: system timer (mwx::periph_ticktimer)
---

# TickTimer

TickTimer is used for internal control of TWENET and is implicitly executed. The period of the timer is 1ms. Only the `available()` method is defined in `loop()` for the purpose of describing processing every 1ms by the TickTimer event.

{% hint style="warning" %}
Note that it is not always available in 1ms increments.

There are cases in which events are skipped due to a large delay caused by factors such as the contents of the user program description or the system's internal interrupt processing.

```cpp
void loop() {
  if (TickTimer.available()) {
    if ((millis() & 0x3FF) == 0) { // This may not be processed (could be skipped)
      Serial << '*';
    }
  }
}
```
{% endhint %}



## Methods

available()

```cpp
inline bool available()
```

It is set after the TickTimer interrupt occurs and becomes `true` in the `loop()` immediately following it. It is cleared after `loop()` ends.

