---
title: "Settings (STG) - Interactive settings mode"
author: "Mono Wireless Inc."
description: "Setting BEHAVIOR"
---

# Settings (STG) - Interactive settings mode

BEHAVIOR is a behavior for using the configuration CUI with Interactive settings mode. The interface for configuration is done via serial port input/output, enabling interactive configuration using terminal software such as TWELITE STAGE/TeraTerm/screen.

{% hint style="info" %}
This BEHAVIOR uses `UART0` (`Serial`) internally. Input strings not used by the configuration behavior are put into a FIFO queue allocated in the configuration behavior, and `Serial` behaves as if it refers to this queue. Note that the internal behavior is different from the case where no BEHAVIOR is registered.
{% endhint %}

* [\<SET\_STD>](stg\_std.md)
