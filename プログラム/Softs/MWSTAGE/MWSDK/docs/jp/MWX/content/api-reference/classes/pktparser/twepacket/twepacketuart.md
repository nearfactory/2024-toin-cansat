---
title: "TwePacketUART"
author: "Mono Wireless Inc."
description: "TwePacketUART"
---
# TwePacketUART

`TwePacketAppUart`クラスは、App\_UARTの拡張書式を親機・中継器アプリApp\_Wingsで受信したときの形式です。

```cpp
class TwePacketAppUART : public TwePacket, public DataAppUART
```

パケットデータ内の諸情報は`parse<TwePacketUART>()`実行後に`DataAppUART`に格納されます。



{% hint style="info" %}
簡易形式は解釈できません。`parse<TwePacketUART>()`では`E_PKT::PKT_ERROR`を戻します。内容を確認するには元のバイト列を直接参照してください。
{% endhint %}



### DataAppUART構造体

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

`payload`はデータ部分ですが、マクロ定義によってデータ格納の方法が変わります。

`MWX_PARSER_PKT_APPUART_FIXED_BUF`の値が`0`としてコンパイルした場合は、`payload`はパケット解析を行うバイト列を直接参照します。元のバイト列の値が変更されると`payload`中のデータは破壊されます。

`MWX_PARSER_PKT_APPUART_FIXED_BUF`の値を`0`より大きい値として定義した場合は、`payload`にはその値（バイト数）のバッファが確保されます。ただしシリアル電文のデータがバッファサイズを超えた場合は`parse<TwePacketAppUART>()`は失敗し`E_PKT::PKT_ERROR`を戻します。
