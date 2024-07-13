---
title: TwePacketTwelite
author: "Mono Wireless Inc."
---

# TwePacketTwelite

The `TwePacketTwelite` class is a standard application [0x81 command of App\_Twelite](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_twelite ), which is an interpretation of the `TwePacketTwelite` class.

```cpp
class TwePacketTwelite : public TwePacket, public DataTwelite { ... };
```

Various information in the packet data is stored in `DataTwelite` after `parse<TwePacketTwelite>()` is executed. 


### DataTwelite structure

```cpp
struct DataTwelite {
		//Serial # of sender
		uint32_t u32addr_src;
		
		// Logical ID of the sender
		uint8_t u8addr_src;

		// Destination logical ID
		uint8_t u8addr_dst;

		// Timestamp at time of transmission
		uint16_t u16timestamp;

		// Flag for low latency transmission
		bool b_lowlatency_tx;

		// Number of repeat relays
		uint16_t u8rpt_cnt;

		// LQI
		uint16_t u8lqi;

		// DI status (true is active Lo,GND)
		bool DI1, DI2, DI3, DI4;
		// DI status bitmap (DI1,2,3,4 in order from LSB)
		uint8_t DI_mask;

		// true if DI active (has been active in the past)
		bool DI1_active, DI2_active, DI3_active, DI4_active;
		// Active bitmap of DI (DI1,2,3,4 in order from LSB)
		uint8_t DI_active_mask;

		// Supply voltage of the module [mV].
		uint16_t u16Volt;

		// AD value [mV]
		uint16_t u16Adc1, u16Adc2, u16Adc3, u16Adc4;
		// Bitmap that is set to 1 if AD is active (AD1,2,3,4 in order from LSB)
		uint8_t Adc_active_mask;
};
```

