---
title: TwePacketIO
author: "Mono Wireless Inc."
---

# TwePacketIO

The `TwePacketAppIO` class interprets the standard app [serial message (0x81) of App\_IO](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_io) The `TwePacketAppIO` class is the one that interprets the

```cpp
class TwePacketAppIO : public TwePacket, public DataAppIO { ... };
```

Various information in the packet data is stored in `DataTwelite` after `parse<TwePacketIO>()` execution.



### DataAppIO structure

```cpp
struct DataAppIO {
		// Serial # of sender
		uint32_t u32addr_src;
		
		// Logical ID of the sender
		uint8_t u8addr_src;

		// Destination logical ID
		uint8_t u8addr_dst;

		// Timestamp at the time of transmission
		uint16_t u16timestamp;

		// Flag for low latency transmission
		bool b_lowlatency_tx;

		// Number of repeat relays
		uint16_t u8rpt_cnt;

		// LQI value
		uint16_t u8lqi;

		// DI status bitmap (DI1,2,3,4,...) in order from LSB
		uint8_t DI_mask;

		// Active (1 if used) bitmap of DI (DI1,2,3,4,...) in order from LSB
		uint8_t DI_active_mask;
		
		// Bitmap of whether DI is interrupt-derived or not (DI1,2,3,4,...) in order from LSB
		uint16_t DI_int_mask;
};
```
