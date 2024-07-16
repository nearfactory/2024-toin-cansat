---
title: "Parent_MONOSTICK"
author: "Mono Wireless Inc."
description: Parent application (for MONOSTICK)
---

# Parent\_MONOSTICK

This act uses MONOSTICK as a parent device. It outputs the data payload of packets from the child machine to the serial port. It can display packets in many samples of sample acts.

{% hint style="success" %}
This act includes

* Receiving wireless packets
* Data interpretation of received packets
* Interactive mode settings - [\<STG\_STD>](../settings/stg\_std.md)
* Conversion of byte strings to ASCII format - [serparser](../api-reference/classes/ser\_parser.md)
{% endhint %}

## Act Features

* Receives packets from the child of the sample act and outputs them to the serial port.

## How to use Act

### TWELITE and wiring required

| Role          | Items                                                                                     |
| ------------- | ----------------------------------------------------------------------------------------- |
| Parent Device | [MONOSTICK BLUE or RED](https://mono-wireless.com/jp/products/MoNoStick/)                 |
| Child Device  | Sample Act Child Setup (e.g. `Slp_Wk_and_Tx`, `PAL_AMB`, `PAL_MAG`, `PAL_MOT???, etc...`) |

Please check the following default settings at first.

* Application ID: `0x1234abcd`
* Channel: `13`



## Act Explanation

### Declaration part

#### includes

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <MONOSTICK>
#include <NWK_SIMPLE>
#include <STG_STD>
```

Include board behavior for [`<MONOSTICK>`](../boards/monostick.md). This board support includes LED control and watchdog support.

* \<NWK\_SIMPLE> Loads the definition of a simple relay net
* \<STG\_STD> Loads the interactive mode definition



#### Other

```cpp
// application ID
const uint32_t DEFAULT_APP_ID = 0x1234abcd;
// channel
const uint8_t DEFAULT_CHANNEL = 13;
// option bits
uint32_t OPT_BITS = 0;

/*** function prototype */
bool analyze_payload(packet_rx& rx);
```

Declaration of default values, function prototypes, etc.



### setup()

```cpp
auto&& brd = the_twelite.board.use<MONOSTICK>();
auto&& set = the_twelite.settings.use<STG_STD>();
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
```

In `setup()`, first load the `<MONOSTICK>` board behavior, the `<STG_STD>` interactive mode behavior, and the `<NWK_SIMPLE>` behavior using `use<>`. This procedure is always done in `setup()`.



```cpp
set << SETTINGS::appname("PARENT"); // Title in the settings screen
set << SETTINGS::appid_default(DEFAULT_APP_ID); // Application ID Default
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // channel default
set << SETTINGS::lid_default(0x00); //LID Default
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
set.reload(); // Read settings from non-volatile memory
OPT_BITS = set.u32opt1(); // Example of reading (option bits)
```

The interactive mode is then set up and the settings are read out. The [`<STG_STD>`](../settings/stg\_std.md) interactive mode provides standard items, but allows for some customization for each act you create.

* `appname`→ Act name that appears in the title line of the configuration screen
* `appid_default`→ Default Application ID
* `ch_default`→ Default channel
* `lid_default`→ Default value of device ID (LID)
* .`hide_items()`→ Item Hide Settings

Always call `.reload()` before reading the configuration values. Each set value has its own method for reading, such as `.u32opt1()`.



```cpp
the_twelite
	<< set                    // Reflects interactive mode settings
	<< TWENET::rx_when_idle() // Set RF to receive when idle.
	;

// Register Network
nwk << set;		            // Reflects interactive mode settings
nwk << NWK_SIMPLE::logical_id(0x00) // Only the LID is reconfigured.
	;
```

Some settings can be directly reflected using [`<STG_STD>`](../settings/stg\_std.md#no-1) objects. In addition, if you want to rewrite a specific value due to a DIP switch setting, for example, you can rewrite the value separately after it is reflected. In the above example, the application ID, channel, radio output, etc. are set in [`the_twelite`](../api-reference/predefined\_objs/the\_twelite.md#yan-suan-zi-she-ding) object, the LID and the retransmission count are set in the [`nwk`](../networks/nwk\_simple.md#yan-suan-zi-she-ding)object, and then the LID is set to 0 again.



```cpp
brd.set_led_red(LED_TIMER::ON_RX, 200); // RED (on receiving)
brd.set_led_yellow(LED_TIMER::BLINK, 500); // YELLOW (blinking)
```

Procedures for controlling LED lighting are available in the `<MONOSTICK>` board behaviour.

The first line sets the red LED to switch on for 200 ms after receiving a radio packet. The first parameter `LED_TIMER::ON_RX` means when a radio packet is received; the second specifies the lighting time in ms.

The second line specifies the blinking of the LEDs: the first parameter `LED_TIMER::BLINK` specifies the blinking, the second parameter is the blinking on/off switching time: every 500ms the LEDs are switched on and off (i.e. Repeat blinking with a 1 s cycle).



```cpp
the_twelite.begin(); // start twelite!
```

Procedure for starting `the_twelite`, which did not appear in act0..4, but must be called if you have configured `the_twelite` or registered various behaviours.



### loop()

There is no processing during `loop()` in this sample.

```cpp
void loop() {
}
```



### on\_rx\_packet()

Callback function called when a packet is received. In this example, some output is produced for the received packet data.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {  
	Serial << ".. coming packet (" << int(millis()&0xffff) << ')' << mwx::crlf;

  ...
  
	// packet analyze
	analyze_payload(rx);
}
```



#### analyze\_payload()

The `analyze_payload()` called at the end of the function contains code to interpret some sample act packets. Refer to the code in correspondence with the packet generation part in the sample act.

```cpp
bool b_handled = false;

uint8_t fourchars[4]{};
auto&& np = expand_bytes(
	    rx.get_payload().begin(), rx.get_payload().end()
		, fourchars
    );
    
if (np == nullptr) return;

// display fourchars at first
Serial
	<< fourchars 
	<< format("(ID=%d/LQ=%d)", rx.get_addr_src_lid(), rx.get_lqi())
	<< "-> ";
	
```

This function first reads the four-character identification data into the `fourchars[4]` array.

Reading is done using the [`expand_bytes()`](../api-reference/funcs/utility/expand\_bytes.md) function.The first and second parameters of this function follow the C++ standard library's practice of giving the first pointer `.begin()` and the next `.end()` of the payload section of the incoming packet. The following parameters are variable arguments, giving the data variables to be read. The return value is _nullptr_ in case of an error, otherwise the next interpretation pointer. If interpreted to the end, `.end()` is returned. The parameter here is `uint8_t fourchars[4]`.



{% hint style="info" %}
This description only supports the `uint8_t[N]` type, where the array length `N` is specified; when using the `uint8*`, `char*`, `char[]` types, etc., the specification must be made using `make_pair(char*,int)`.

```cpp
char fourchars[5]{}; // Allocate 5 bytes including the terminating character `\0`.
auto&& np = expand_bytes(
	    rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((char *)fourchars, 4)
	);
```
{% endhint %}



Processing is then carried out for the 4-byte header. Here, the packets of the sample act Slp\_Wk\_and\_Tx are interpreted and the contents are displayed.

```cpp
// Slp_Wk_and_Tx
if (!b_handled && !strncmp(fourchars, "TXSP", 4)) {
	b_handled = true;
	uint32_t tick_ms;
	uint16_t u16work_ct;

	np = expand_bytes(np, rx.get_payload().end()
		, tick_ms
		, u16work_ct
	);

	if (np != nullptr) {
		Serial << format("Tick=%d WkCt=%d", tick_ms, u16work_ct);
	} else {
		Serial << ".. error ..";
	}
}
```

Set `b_handled` to _true_ so that the other interpreters' decisions are skipped.

"TXSP" packets contain the values of a system timer count of type `uint32_t` and a dummy counter of type `uint16_t`. Each variable is declared and read using the `expand_bytes()` function. The difference from the above is that the first parameter is `np` as the first pointer to read. The `tick_ms` and `u16work_ct` are given as parameters and the value stored in the payload is read as a big-endian format byte sequence.

If the readout is successful, the contents are output and the process is complete.



#### Define and output your own ASCII format

It is structured by ASCII format in a user-defined order.

```cpp
smplbuf_u8<128> buf;
mwx::pack_bytes(buf
	, uint8_t(rx.get_addr_src_lid()		// Logical ID of the sender
	, uint8_t(0xCC)											   // 0xCC
	, uint8_t(rx.get_psRxDataApp()->u8Seq)	// Sequence number of the packet
	, uint32_t(rx.get_addr_src_long())	// Serial number of the sender.
	, uint32_t(rx.get_addr_dst())		// destination address
	, uint8_t(rx.get_lqi())			// LQI:Link Quality Indicator
	, uint16_t(rx.get_length())		// Number of following bytes
	, rx.get_payload() 			// data payload
);

serparser_attach pout;
pout.begin(PARSER::ASCII, buf.begin(), buf.size(), buf.size());

Serial << "FMT PACKET -> ";
pout >> Serial;
Serial << mwx::flush;
```

The first line declares a buffer as a local object to store the data sequence before conversion to ASCII format.

The second line uses `pack_bytes()` to store the data sequence into the buf mentioned earlier. See comments in the source code for the data structure. The `pack_bytes()` parameter can also be a container of the form `smplbuf_u8 (smplbuf<uint8_t, ALLOC>)`.

{% hint style="info" %}
The sequence number of the packet is automatically set by `<NWK_SIMPLE>` and is assigned in the order of the transmitted packets. This value is used for packet duplication detection.

The LQI (Link Quality Indicator) is a value corresponding to the signal strength at the time of reception; the higher the value, the stronger the field strength. However, there is no strict relationship defined between this value and the physical quantity, and even if the LQI is higher relative to the noise in the environment, more noise will also reduce the success rate of communication.
{% endhint %}

Lines 13, 14 and 17 are the declaration, configuration and output of the serial parser.



#### Dump output including NWK\_SIMPLE header.

The first output (which is prevented from being executed by `if(0)`) displays all data including the control data of `<NWK_SIMPLE>`. There are 11 bytes of control data. Normally, the control information is not directly referenced.

```cpp
serparser_attach pout;
pout.begin(PARSER::ASCII, rx.get_psRxDataApp()->auData, 
    rx.get_psRxDataApp()->u8Len, rx.get_psRxDataApp()->u8Len);

Serial << "RAW PACKET -> ";
pout >> Serial;
Serial << mwx::flush;

// Reference: packet structure of the control unit.
// uint8_t  : 0x01 fixws
// uint8_t  : LID of sender
// uint32_t : Long address (Serial Number) of sender
// uint32_t : Destination address
// uint8_t  : Number of repeating
```

The first line declares the serial parser for output as a local object. It does not have an internal buffer, but diverts an external buffer and uses the output function of the parser to output the byte sequence in the buffer in [ASCII format](../api-reference/classes/ser\_parser.md#asuk)

The second line sets the buffer for the serial parser. It specifies an already existing data array, i.e. the payload part of the incoming packet. `serparser_attach pout` declares the serial parser using an already existing buffer. The first parameter of `pout.begin()` specifies the corresponding format of the parser as `PARSER::ASCII`, i.e. ASCII format; the second specifies the first address of the buffer; the third specifies the The length of valid data in the buffer and the fourth specifies the maximum length of the buffer. The fourth parameter has the same value as the third, as it is for output and is not used for format interpretation.

Output to the serial port in line 6 using the `>>` operator.

The `Serial << mwx::flush` in line 7 is a specification to wait until the output of data that has not been output here is finished. (`Serial.flush()` is the same process.)
