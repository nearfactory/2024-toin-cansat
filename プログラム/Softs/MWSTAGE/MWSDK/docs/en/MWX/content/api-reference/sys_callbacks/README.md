---
title: Call back functions
author: "Mono Wireless Inc."
description: Call back functions describing an application.
---

# Callback functions

This is a callback function that describes the application. Callback means called by the system (library). The user defines several callback functions to describe the behavior of the system.

The following callback functions are mandatory definitions.

* `setup()`
* `loop()`

If no other functions are defined, an empty function that does nothing is linked instead.



### Normal callback call sequence

```
init_coldboot()
  ↓ (TWENET internal processing: Initialization 1)
setup()
  ↓（TWENET internal processing: Initialization 2)
begin() --- First time only
  ↓
loop() <--+
  ↓       |Event processing, behavior processing
CPU DOZE -+
```

{% hint style="warning" %}
Please refer to the source code `mwx_appcore.cpp` if you want to see the exact behavior.
{% endhint %}



### Order of callback invocation on return to sleep

```
the_twelite.sleep()
  ↓ sleeping...
 
 
init_warmboot()
  ↓ (TWENET internal processing: Initialization 3)
wakeup()
  ↓（TWENET internal processing: Initialization 4)
loop() <--+
  ↓       |Event processing, behavior processing
CPU DOZE -+
```

{% hint style="warning" %}
Please refer to the source code `mwx_appcore.cpp` if you want to see the exact behavior.
{% endhint %}

