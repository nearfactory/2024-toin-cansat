---
title: "TwePacket"
author: "Mono Wireless Inc."
---

# TwePacket

It is a base class of packet type, but the member structure `common` contains address information and other common information.

```cpp
class TwePacket {
	public:
		static const E_PKT _pkt_id = E_PKT::PKT_ERROR;
		
		struct {
			uint32_t tick; // system time at interpretation execution [ms]
			uint32_t src_addr; // source address (serial number)
			uint8_t src_lid; // source address (logical address)
			uint8_t lqi; // LQI
			uint16_t volt; // voltage[mV]
		} common;
};
```

{% hint style="info" %}
This is used when you want to get minimum information such as address information when you want to store mixed types as pktparser type in arrays, etc.
{% endhint %}



