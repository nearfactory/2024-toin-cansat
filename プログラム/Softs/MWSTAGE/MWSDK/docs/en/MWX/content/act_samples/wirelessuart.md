---
title: "WirelessUART"
author: "Mono Wireless Inc."
---
# WirelessUART

WirelessUART performs serial communication.



## ACT Features.

* Communicate between two UART-connected TWELITEs in ASCII format.



## how to use act

### Required TWELITE

Two of the following devices serially connected to a PC.

* [MONOSTICK BLUE or RED](https://mono-wireless.com/jp/products/MoNoStick/index.html)
* [TWELITE DIP](https://mono-wireless.com/jp/) connected to UART with [TWELITE R](https://mono-wireless.com/jp/products/TWE-LITE-R/index.html) products/TWE-Lite-DIP/index.html) etc.

{% hint style="info" %}
Packets addressed to the Parent Node can also be received by [`Parent_MONOSTICK`](parent\_monostick.md).
{% endhint %}



## Explanation of ACT

### setup()

```cpp
void setup() {
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

	/*** INTERACTIE MODE */
	// settings: configure items
	set << SETTINGS::appname("WirelessUART");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set << SETTINGS::lid_default(DEFAULT_LID); // set default lid
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
	set.reload(); // load from EEPROM.
	
	/*** SETUP section */
	// the twelite main class
	the_twelite
		<< set                      // from iteractive mode (APPID/CH/POWER)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	nwk	<< set;						// from interactive mode (LID/REPEAT)

	/*** BEGIN section */
	SerialParser.begin(PARSER::ASCII, 128); // Initialize the serial parser
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- WirelessUart (id=" << int(nwk.get_config().u8Lid) << ") ---" << mwx::crlf;
}
```

Interactive settings mode is initialized. This sample provides two or more devices that have **different logical device IDs (LIDs)** from each other.



```cpp
SerialParser.begin(PARSER::ASCII, 128); 
```

Initialize [serial parser](../api-reference/predefined\_objs/serialparser.md).



### loop()

```cpp
while(Serial.available())  {
	if (SerialParser.parse(Serial.read())) {
		Serial << ".." << SerialParser;
		const uint8_t* b = SerialParser.get_buf().begin();
		uint8_t addr = *b; ++b; // the first byte is destination address.
		transmit(addr, b, SerialParser.get_buf().end());
	}
}
```

When data input from the serial is received, one byte is input to the serial parser. When the ASCII format is accepted to the end, `SerialParser.parse()` returns `true`.

The `SerialParser` can access the internal buffer with `smplbuf`. In the example above, the first byte of the buffer is taken as the destination address, and the second byte to the end is passed to the `transmit()` function.



### on\_rx\_packet()

When a packet is received, a buffer `smplbuf_u8<128> buf` containing the payload followed by the source as the first byte is created and output serially from the serial parser `serparser_attach pout` for output.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	// check the packet header.
	const uint8_t* p = rx.get_payload().begin();
	if (rx.get_length() > 4 && !strncmp((const char*)p, (const char*)FOURCHARS, 4)) {
		Serial << format("..rx from %08x/%d", rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;

		smplbuf_u8<128> buf;
		mwx::pack_bytes(buf			
				, uint8_t(rx.get_addr_src_lid())            // src addr (LID)
				, make_pair(p+4, rx.get_payload().end()) );	// data body

		serparser_attach pout;
		pout.begin(PARSER::ASCII, buf.begin(), buf.size(), buf.size());
		Serial << pout;
	}
}
```





## command for testing

{% hint style="warning" %}
Test data must be entered into the terminal using the **paste function**. This is because there is a timeout for input.

Note: To paste in TWE Programmer or TeraTerm, use Alt+V.
{% endhint %}

{% hint style="info" %}
CR LF is required at the end of the input.

At first try a series ending in X where CR LF can be omitted. If no terminating string is entered, the series will be ignored.
{% endhint %}



### example

```
:FE00112233X

:FE001122339C

```

Send `00112233` to any Child Node.



### example

```
:03AABBCC00112233X

:03AABBCC0011223366

```

Send `AABBCC00112233` to Child Node #3.



### example

```
:FF00112233X

:00112233X

```

Sent to any Parent Node or Child Node (`0xFF`) and to the Parent Node (`0x00`).

