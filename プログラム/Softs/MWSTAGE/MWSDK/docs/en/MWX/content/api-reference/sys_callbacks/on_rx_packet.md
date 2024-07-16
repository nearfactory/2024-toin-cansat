---
title: "on_rx_packet()"
author: "Mono Wireless Inc."
description: "on_rx_packet()"
---
# on_rx_packet()

Receives incoming packets.

```cpp
void on_rx_packet(mwx::packet_rx& pkt, bool_t &b_handled) 
```

When a wireless packet is received, this function is called from within the MWX library with the data stored in `pkt` as [packet_rx](../classes/packet_rx.md). If this function is not defined in the application, a weak function that does nothing is linked.

Setting `b_handled` to *true* in this function tells the MWX library that the incoming packet has been processed in the application. If set to `processed', it suppresses unnecessary processing. (Do not process `the_twelite.receiver`)


{% hint style="warning" %}
When using [BEHAVIOR](../behavior/README.md), use the callback function in BEHAVIOR.
{% endhint %}

{% hint style="warning" %}
The `the_twelite.receiver` is not recommended.

The receiver was previously processed by `the_twelite.receiver` with the intention of describing it in `loop()`. However, `on_rx_packet()` was added because it is a delayed processing by a queue, which in principle causes overflow, and also because it tends to be complicated to describe.
{% endhint %}