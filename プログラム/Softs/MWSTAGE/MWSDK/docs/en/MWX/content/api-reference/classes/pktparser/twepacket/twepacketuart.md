---
title: TwePacketUART
author: "Mono Wireless Inc."
---

# TwePacketUART

The `TwePacketAppUart` class is the format in which the extended format of the App\_UART is received by the parent and repeater application App\_Wings.

```cpp
class TwePacketAppUART : public TwePacket, public DataAppUART
```

Various information in the packet data is stored in `DataAppUART` after `parse<TwePacketUART>()` execution.



{% hint style="info" %}
The simple format cannot be interpreted. `parse<TwePacketUART>()` returns `E_PKT::PKT_ERROR`. To check the contents, refer to the original byte sequence directly.
{% endhint %}



### DataAppUART structure

```cpp
struct DataAppUART {
		/**
		 * source address (Serial ID)
		 */
		uint32_t u32addr_src;

		/**
		 * source address (Serial ID)
		 */
		uint32_t u32addr_dst;

		/**
		 * source address (logical ID)
		 */
		uint8_t u8addr_src;

		/**
		 * destination address (logical ID)
		 */
		uint8_t u8addr_dst;

		/**
		 * LQI value
		 */
		uint8_t u8lqi;

		/**
		 * Response ID
		 */
		uint8_t u8response_id;

		/**
		 * Payload length
		 */
		uint16_t u16paylen;

		/**
		 * payload
		 */
#if MWX_PARSER_PKT_APPUART_FIXED_BUF == 0
		mwx::smplbuf_u8_attach payload;
#else
		mwx::smplbuf_u8<MWX_PARSER_PKT_APPUART_FIXED_BUF> payload;
#endif
	};
```

The `payload` is the data part, but the method of data storage changes depending on the macro definition.

If `MWX_PARSER_PKT_APPUART_FIXED_BUF` is compiled with the value `0`, `payload` refers directly to the byte sequence for packet analysis. If the value of the original byte sequence is changed, the data in `payload` will be destroyed.

If you define the value of `MWX_PARSER_PKT_APPUART_FIXED_BUF` to be greater than `0`, the `payload` will allocate a buffer of that value (bytes). However, if the data of the serial message exceeds the buffer size, `parse<TwePacketAppUART>()` fails and returns `E_PKT::PKT_ERROR`.
