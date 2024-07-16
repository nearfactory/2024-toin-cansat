---
title: setup()
author: "Mono Wireless Inc."
description: the setup function to initialize an application.
---
# setup()

Called at the beginning of code execution to write initialization code.

{% hint style="warning" %}
TWENET initialization is also executed after the `setup()` function exits. Do not do anything other than initialization here, since most of the processing is done after TWENET exits.

Items to be noted are listed below.

* Sleep `the_twenet.sleep()` cannot be executed. If you want to sleep immediately after initialization, write the first sleep process in the `begin()` function.
* The `delay()` function is replaced by the processing described below. In this case, the parameter `ms` does not specify milliseconds. \frz

\* Alternative processing for `delay()`.

```cpp
static inline void delay(uint32_t ms) {
		volatile uint32_t ct = ms * 4096;
		while (ct > 0) {
			--ct;
		}
}
```
{% endhint %}

