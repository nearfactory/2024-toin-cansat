---
title: begin()
author: "Mono Wireless Inc."
---

# begin()

It is called only once before the first call to the `loop()` function; since TWENET is already initialized, there is no need to consider constraints such as `setup()`.

The main usage is to

* Displaying startup messages
* Writing code for testing
* Sleep transition immediately after startup
* Processing that is inconvenient for `setup()` (radio packet processing, timer operation, etc.)

{% hint style="info" %}
This callback function definition is optional.
{% endhint %}

