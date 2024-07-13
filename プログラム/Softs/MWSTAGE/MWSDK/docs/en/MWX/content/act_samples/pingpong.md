---
title: "PingPong"
author: "Mono Wireless Inc."
---
# PingPong

Send a PING wireless packet from one of the two serially connected TWELITEs and receive a PONG wireless packet back from the other.

{% hint style="success" %}
This ACT includes.

* Sending a prompt response from the receipt of a wireless packet
* Transmission with direct address of the peer
* Input from serial port - [Serial](../api-reference/classes/twe-stream/README.md)
* Digital (button) input - [Buttons](../api-reference/predefined\_objs/buttons.md)
* Analogue input - [Analogue](../api-reference/predefined\_objs/analogue.md)
{% endhint %}





## how to use act

### Required TWELITE

Two of any of the following.

* [MONOSTICK BLUE or RED](https://mono-wireless.com/jp/products/MoNoStick/index.html)
* [TWELITE DIP](https://mono-wireless.com/jp/) connected to UART with [TWELITE R](https://mono-wireless.com/jp/products/TWE-LITE-R/index.html) products/TWE-Lite-DIP/index.html), etc.



## Explanation of ACT

### Include

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
```

Include `<TWELITE>` in all ACTs. Here, the simple network [`<NWK_SIMPLE>`](../networks/nwk\_simple.md) should be included.

### Declaration section

```cpp
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

// DIO pins
const uint8_t PIN_BTN = 12;

/*** function prototype */
void vTransmit(const char* msg, uint32_t addr);

/*** application defs */
// packet message
const int MSG_LEN = 4;
const char MSG_PING[] = "PING";
const char MSG_PONG[] = "PONG";
```

* Sample act common declarations
* Prototype declarations for longer processes (sending and receiving), since they are made into functions
* Variables for holding data in the application

### セットアップ setup()

```cpp
void setup() {
	/*** SETUP section */
	Buttons.setup(5); // init button manager with 5 history table.
	Analogue.setup(true, 50); // setup analogue read (check every 50ms)

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	/*** BEGIN section */
	Buttons.begin(pack_bits(PIN_BTN), 5, 10); // check every 10ms, a change is reported by 5 consequent values.
	Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC)); // _start continuous adc capture.

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
}
```

The general flow of the program is the initial setup of each section and the start of each section.


#### the\_twelite

This object is the core class object for manipulating TWENET.

```cpp
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)
```

`the_twelite` に設定を反映するには `<<` を用います。

Use `<<` to reflect the setting in `the_twelite`.

* `TWENET::appid(APP_ID)` to specify the Application ID.
* `TWENET::channel(CHANNEL)` to specify the channel.
* `TWENET::rx_when_idle()` Specifies that the receive circuit is open.

{% hint style="info" %}
The `<<, >>` operator is originally a bit shift operator, but it is used differently from its meaning.

```cpp
// The following statements are not available in the MWX library
#include <iostream>
std::cout << "hello world" << std::endl;
```
{% endhint %}



Next, register the network.

```cpp
auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
nwksmpl << NWK_SIMPLE::logical_id(0xFE);
        << NWK_SIMPLE::repeat_max(3);
```

The first line is written in the same way as the board registration, specifying `<>` as `<NWK_SIMPLE>`.

The second line specifies `<NWK_SIMPLE>`, specifying `0xFE` (`WK_SIMPLE` is a **Child Node with an unset ID**).

The third line specifies the maximum number of relays. This explanation does not touch on relaying, but packets are relayed when operating with multiple units.



```cpp
the_twelite.begin(); // start twelite!
```

Execute `the_twelite.begin()` at the end of the `setup()` function.



#### Analogue

Class object that handles ADCs (analog-to-digital converters).

```cpp
Analogue.setup(true);
```

Initialization `Analogue.setup()`. The parameter `true` specifies to wait in place until the ADC circuit is stable.



```cpp
Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC), 50); 
```

To start the ADC, call `Analogue.begin()`. The parameter is a bitmap corresponding to the pin to be ADC'd.

The `pack_bits()` function is used to specify the bitmap. It is a function with variable number of arguments, each argument specifies a bit position to be set to 1. For example, `pack_bits(1,3,5)` returns the binary value `101010`. This function has the `constexpr` specification, so if the parameters are constants only, they are expanded to constants.

The parameters are specified as `PIN_ANALOGUE::A1` (ADC0) and `PIN_ANALOGUE::VCC` (module supply voltage).

The second parameter is specified as `50`, and the ADC operation is started by default with TickTimer, which is set to

{% hint style="info" %}
Except for the first time, the ADC is started in an interrupt handler.
{% endhint %}



#### Buttons

Detects changes in DIO (digital input) values; Buttons only detect a change in value after the same value has been detected a certain number of times in order to reduce the effects of mechanical button chattering.

```cpp
Buttons.setup(5);
```

Initialization is done with `Buttons.setup()`. The parameter 5 is the number of detections required to determine the value, but it is the maximum value that can be set. Internally, the internal memory is allocated based on this number.

```cpp
Buttons.begin(pack_bits(PIN_BTN),
					5, 		// history count
					10);  	// tick delta
```

The start is done with `Buttons.begin()` The first parameter is the DIO to be detected. The second parameter is the number of detections required to determine the state. The third parameter is the detection interval. Since `10` is specified, the HIGH and LOW states are determined when the same value is detected five times in a row every 10 ms.

{% hint style="info" %}
The detection of the DIO state in Buttons is done by an event handler. Event handlers are called in the application loop after an interrupt has occurred, so there is a delay compared to interrupt handlers.
{% endhint %}



#### Serial

Serial objects can be used without initialization or initiation procedures.

```cpp
Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
```

Outputs a string to the serial port. `mwx::crlf` is a newline character.



###  loop()

Loop function are called as callback functions from the main loop of the TWENET library. The basic description here is to wait for the object to be used to become available and then process it. This section describes the use of some objects used in ACT.

{% hint style="warning" %}
The main loop of the TWENET library processes incoming packets and interrupt information stored in the FIFO queue in advance as events, after which `loop()` is called. After exiting `loop()`, the CPU enters DOZE mode and waits until a new interrupt occurs with low current consumption.

Therefore, code that assumes the CPU is always running will not work well.
{% endhint %}

```cpp
void loop() {
	  // read from serial
		while(Serial.available())  {
				int c = Serial.read();
				Serial << mwx::crlf << char(c) << ':';
				switch(c) {
				    case 't':
				    	  vTransmit(MSG_PING, 0xFF);
				        break;
				    default:
							  break;
				}
		}


	// Button press
	if (Buttons.available()) {
		uint32_t btn_state, change_mask;
		Buttons.read(btn_state, change_mask);

		// Serial << fmt("<BTN %b:%b>", btn_state, change_mask);
		if (!(change_mask & 0x80000000) && (btn_state && (1UL << PIN_BTN))) {
			// PIN_BTN pressed
			vTransmit(MSG_PING, 0xFF);
		}
	}
}
```



#### Serial

```cpp
		while(Serial.available())  {
				int c = Serial.read();
				Serial << mwx::crlf << char(c) << ':';
				switch(c) {
				    case 't':
				    	  vTransmit(MSG_PING, 0xFF);
				        break;
				    default:
							  break;
				}
		}
```

While `Serial.available()` is `true`, there is input from the serial port. The data is stored in the internal FIFO queue, so there is some leeway, but it should be read out promptly. To read data, call `Serial.read()`.

Here, the `vTransmit()` function is called to send a PING packet in response to a `'t'` key input.



#### Buttons

It becomes available at the timing when a change in DIO (digital IO) input is detected, and is read by `Buttons.read()`.

```cpp
	if (Buttons.available()) {
		uint32_t btn_state, change_mask;
		Buttons.read(btn_state, change_mask);
```

The first parameter is a bitmap of the HIGH/LOW of the current DIO, ordered from bit0 to DIO0,1,2,... . and so on, starting from bit 0. For example, for DIO12, HIGH / LOW can be determined by evaluating `btn_state & (1UL << 12)`. If the bit is set to 1, it is HIGH.

{% hint style="info" %}
When the IO state is determined for the first time, MSB (bit31) is set to 1. The initial determination process is also performed when the device returns from sleep.
{% endhint %}

```cpp
// Serial << fmt("<BTN %b:%b>", btn_state, change_mask);
if (!(change_mask & 0x80000000) && (btn_state && (1UL << PIN_BTN))) {
	// PIN_BTN pressed
	vTransmit(MSG_PING, 0xFF);
```

The `vTransmit()` is called at the timing when the button is **released** except for the initial confirmation. To make the timing of the press `(! (btn_state && (1UL << PIN_BTN)))` to invert the condition logically.



### transmit()

This function requests TWENET to send a wireless packet. At the end of this function, the wireless packet is not yet processed. The actual transmission will be completed in a few ms or later, depending on the transmission parameters. This section describes typical transmission request methods.

```cpp
void vTransmit(const char* msg, uint32_t addr) {
	Serial << "vTransmit()" << mwx::crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
			, uint16_t(analogRead(PIN_ANALOGUE::A1)) // possible numerical values types are uint8_t, uint16_t, uint32_t. (do not put other types)
			, uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)) // A1 and VCC values (note: alalog read is valid after the first (Analogue.available() == true).)
			, uint32_t(millis()) // put timestamp here.
		);
	
		// do transmit 
		pkt.transmit();
	}
}
```

#### Getting Network and Packet Objects

```cpp
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

Get a network object with `the_twelite.network.use<NWK_SIMPLE>()`. Use that object to get a `pkt` object by `.prepare_tx_packet()`.

Here it is declared in the conditional expression of the if statement. The declared `pkt` object is valid until the end of the if clause. pkt object gives a response of type bool, which here is `true` if there is a free space in TWENET's send request queue and the send request is accepted, or `false` if there is no space.



#### Settings for sending packets

```cpp
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)
```

Packets are configured using the `<<` operator as in `the_twelite` initialization setup.

* Specify the destination address in the `tx_addr()` parameter. If it is `0x00`, it means that you are the Child Node and broadcast to the Parent Node, and if it is `0xFE`, it means that you are the Parent Node and broadcast to any Child Node.
* The `tx_retry()` parameter specifies the number of retransmissions. In the example `3` means that the number of retransmissions is 3, i.e., the packet is sent 4 times in total. Sending only one wireless packet may fail a few percent of the time even under good conditions.
* `tx_packet_delay()` Sets the transmission delay. The first parameter is the minimum wait time to start sending and the second is the maximum wait time. The third is the retransmission interval. The third is the retransmission interval, meaning that a retransmission is performed every 20 ms after the first packet is sent.



#### Data Payload in a Packet

Payload means a loaded item, but in wireless packets it is often used to mean "the main body of data to be sent". In addition to the main body of data, the data in a wireless packet also contains some auxiliary information, such as address information.

For correct transmission and reception, please be aware of the data order of the data payload. In this example, the data order is as follows. Construct the data payload according to this data order.

```
# Index of first byte: Data type : Number of bytes : Contents

00: uint8_t[4] : 4 : four-character identifier
08: uint16_t   : 2 : ADC value of AI1 (0..1023)
06: uint16_t   : 2 : Voltage value of Vcc (2000..3600)
10: uint32_t   : 4 : millis() system time
```

{% hint style="info" %}
The data payload can contain 90 bytes (actually a few more bytes).

Every byte in an IEEE802.15.4 wireless packet is precious. There is a limit to the amount of data that can be sent in a single packet. If a packet is split, the cost of the split packet is high because it must take into account transmission failures. Also, sending one extra byte consumes energy equivalent to approximately 16 µs x current during transmission, which can be significant, especially for battery-powered applications.
{endhint %}



Let's actually build the data structure of the above data payload. The data payload can be referenced as a container of type `simplbuf<uint8_t>` via `pkt.get_payload()`. In this container, we build the data based on the above specification.

It can be written as above, but the MWX library provides an auxiliary function `pack_bytes()` for data payload construction.

```cpp
// prepare packet payload
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
	, uint16_t(analogRead(PIN_ANALOGUE::A1)) // possible numerical values types are uint8_t, uint16_t, uint32_t. (do not put other types)
	, uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)) // A1 and VCC values (note: alalog read is valid after the first (Analogue.available() == true).)
	, uint32_t(millis()) // put timestamp here.
);
```

The first parameter of `pack_bytes` specifies the container. In this case, it is `pkt.get_payload()`.

The parameters after that are variable arguments, specifying as many values of the corresponding type in `pack_bytes` as needed. The `pack_bytes` internally calls the `.push_back()` method to append the specified value at the end.

The third line, `make_pair()`, is a standard library function to generate `std::pair`. This is specified to avoid confusion of string types (specifically, whether or not to include null characters when storing payloads). The first parameter of `make_pair()` is the string type (`char*`, `uint8_t*`, `uint8_t[]`, etc.) The second parameter is the number of bytes to store in the payload.

Lines 4, 5, and 6 store values of numeric types (`uint8_t`, `uint16_t`, `uint32_t`). Numeric types such as signed, or even the same numeric type such as `char` are cast to the three types listed on the left and submitted.

`analogRead()` and `analogRead_mv()` get the result of ADC. The former is the ADC value (0..1023) and the latter is the voltage\[mv]\(0..2470). The supply voltage of the module is read internally from the value of the voltage divider resistor, so we use `adalogRead_mv()` to perform that conversion.



This completes the packet preparation. Now all that remains is to make a request for transmission.

```cpp
pkt.transmit();
```

Packets are sent using the `pkt.transmit()` method of the `pkt` object.

{% hint style="info" %}
Although not used in this ACT, the return value contains information about the success or failure of the request and the number corresponding to the request. Use this return value if the process waits until the transmission is complete.
{% endhint %}



### on\_rx\_packet()

This is the process when there is an incoming packet.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
		uint8_t msg[MSG_LEN];
		uint16_t adcval, volt;
		uint32_t timestamp;

		// expand packet payload (shall match with sent packet data structure, see pack_bytes())
		expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
					, msg       // 4bytes of msg
											//   also can be -> std::make_pair(&msg[0], MSG_LEN)
					, adcval    // 2bytes, A1 value [0..1023]
				  , volt      // 2bytes, Module VCC[mV]
					, timestamp // 4bytes of timestamp
        );
		
		// if PING packet, respond pong!
    if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
				// transmit a PONG packet with specifying the address.
        vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
    }

		// display the packet
		Serial << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
                    , rx.get_psRxDataApp()->u32SrcAddr
                    , rx.get_lqi()
                    , rx.get_length()
					, rx.get_psRxDataApp()->u8Seq
                    )
				<< format(" %s AD=%d V=%d TS=%dms>" // note: up to 4 args!
					, msg
					, adcval
					, volt
					, timestamp
					)
               << mwx::crlf
			   << mwx::flush;
	}
```



First, the data of the incoming packet is passed as parameter `rx`. From `rx`, the address information and data payload of the wireless packet is accessed.

```cpp
while (the_twelite.receiver.available()) {
		auto&& rx = the_twelite.receiver.read();
```

In the next line, the received packet data refers to the source address (32-bit long address and 8-bit logical address) and other information.

```cpp
Serial << format("..receive(%08x/%d) : ",
   rx.get_addr_src_long(), rx.get_addr_src_lid());
```

{% hint style="info" %}
In `<NWK_SIMPLE>`, two types of addresses are always exchanged: an 8-bit logical ID and a 32-bit long address. When specifying the destination, either the long address or the logical address is specified. When receiving, both addresses are included.
{% endhint %}



The MWX library provides a function `expand_bytes()` as a counterpart to `pack_bytes()` used in `transmit()`.

```cpp
uint8_t msg[MSG_LEN];
uint16_t adcval, volt;
uint32_t timestamp;

// expand packet payload (shall match with sent packet data structure, see pack_bytes())
expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, msg       // 4bytes of msg
								//   also can be -> std::make_pair(&msg[0], MSG_LEN)
		, adcval    // 2bytes, A1 value [0..1023]
	  , volt      // 2bytes, Module VCC[mV]
		, timestamp // 4bytes of timestamp
    );
```

Lines 1 through 3 specify variables to store data.

The first parameter specifies the first iterator of the container (a `uint8_t*` pointer), which can be retrieved by the `.begin()` method. The second parameter is the next iterator after the end of the container and can be retrieved with the `.end()` method.

The third and subsequent parameters enumerate variables. The payloads are read and stored in the order in which they are listed.

{% hint style="info" %}
This ACT omits error checking, such as if the packet length is wrong. If you want to make the check strict, judge by the return value of `expand_bytes()`.

The return value of `expand_bytes()` is `uint8_t*`, but returns `nullptr (null pointer)` in case of access beyond the end.
{% endhint %}



The process sends a PONG message if the identifier of the 4-byte string read in `msg` is `"PING"`.

```cpp
if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
    vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
}
```



It then displays information on packets that have arrived.

```cpp
		Serial << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
                    , rx.get_psRxDataApp()->u32SrcAddr
                    , rx.get_lqi()
                    , rx.get_length()
										, rx.get_psRxDataApp()->u8Seq
                    )
           << format(" %s AD=%d V=%d TS=%dms>" // note: up to 4 args!
                    , msg
                    , adcval
                    , volt
                    , timestamp
                    )
         << mwx::crlf
			   << mwx::flush;
```

The `format()` is used because numeric formatting output is required. helper class that allows the same syntax as _printf()_ for `>>` operators, but **limits** the number of arguments to a maximum of 8 (for 32-bit parameters). (A compile error will occur if the limit is exceeded. Note that `Serial.printfmt()` has no limit on the number of arguments.)

The `mwx::crlf` specifies a newline character (CR LF), and `mwx::flush` waits for completion of output. (`mxw::flush` may be written as `Serial.flush()`)
