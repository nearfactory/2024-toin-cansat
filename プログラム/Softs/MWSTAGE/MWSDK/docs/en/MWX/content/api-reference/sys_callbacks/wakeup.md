---
title: wakeup()
author: "Mono Wireless Inc."
description: the function called when waking up from sleep.
---
# wakeup()

Called before `loop()` when waking up from sleep, and includes procedures for initialization after returning from sleep and for branching processing depending on the state of return.

{% hint style="info" %}
If there is no processing in `loop()` but only reading of sensors, etc., sleep can be performed again in this function.
{% endhint %}

{% hint style="info" %}
This callback function definition is optional.
{% endhint %}
