---
title: "BRD_APPTWELITE"
author: "Mono Wireless Inc."
description: IO communication (basic function of standard application App_Twelite)
---

# BRD\_APPTWELITE

This is a sample using board support `<BRD_APPTWELITE>`, which assumes the same wiring as required by App_TweLite.

{% hint style="success" %}
This ACT includes:

* Transmission and reception of wireless packets

* Interactive settings mode - [\<STG\_STD>](../settings/stg\_std.md)

* Digital (button) input - [Buttons](../api-reference/predefined\_objs/buttons.md)

* analog input - [Analogue](../api-reference/predefined\_objs/analogue.md)

  {% endhint %}

{% hint style="warning" %}
This sample cannot communicate with App\_TweLite.
{% endhint %}

## functions of ACT

* Read M1 to determine the parent or child unit.
* Reads the value of DI1-DI4; the Buttons class reduces the effect of chattering, so that changes are notified only when the values are the same consecutively. Communication is performed when there is a change.
* Reads the value of AI1-AI4.
* Sends the values of DI1-4, AI1-4, and VCC to a child unit if it is the parent unit, or to the parent unit if it is a child unit, every DI change or every 1 second.
* Set to DO1-4 or PWM1-4 depending on the value of the received packet.

## How to use ACT

### Required TWELITE and wiring example

| Role        | Example                                                      |
| ----------- | ------------------------------------------------------------ |
| Parent Node | <a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>At a minimum, wire M1=GND, DI1:button, DO1:LED. |
| Child Node  | <a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>At a minimum, wire M1=open, DI1:button, DO1:LED. |

![Wiring example (AI1-AI4 can be omitted)](<../.gitbook/assets/image (19).png>)



## Explanation of ACT

### include part

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <BRD_APPTWELITE>
#include <STG_STD>
```

Include `<TWELITE>` in all ACTs. Here is a simple network [`<NWK_SIMPLE>`](../networks/nwk\_simple.md) and board support `<BRD_APPTWELITE>` should be included.

It also includes `<STG_STD>` to add Interactive settings mode.

### declaration part

```cpp
/*** Config part */
// application ID
const uint32_t DEFAULT_APP_ID = 0x1234abcd;
// channel
const uint8_t DEFAULT_CHANNEL = 13;
// option bits
uint32_t OPT_BITS = 0;
// logical id
uint8_t LID = 0;

/*** function prototype */
MWX_APIRET transmit();
void receive();

/*** application defs */
const char APP_FOURCHAR[] = "BAT1";

// sensor values
uint16_t au16AI[5];
uint8_t u8DI_BM;
```

* sample-act common declaration
* Its prototype declarations (send and receive), since the longer process is functionalized
* Variables for holding data in the application

### setup()

```cpp
void setup() {
	/*** SETUP section */	
	// init vars
	for(auto&& x : au16AI) x = 0xFFFF;
	u8DI_BM = 0xFF;

	// load board and settings
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

	// settings: configure items
	set << SETTINGS::appname("BRD_APPTWELITE");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
	set.reload(); // load from EEPROM.
	OPT_BITS = set.u32opt1(); // this value is not used in this example.
	LID = set.u8devid(); // logical ID

	// the twelite main class
	the_twelite
		<< set                      // apply settings (appid, ch, power)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	if (brd.get_M1()) { LID = 0; }

	// Register Network
	nwk << set // apply settings (LID and retry)
			;

	// if M1 pin is set, force parent device (LID=0)
	nwk << NWK_SIMPLE::logical_id(LID); // write logical id again.
	
	/*** BEGIN section */
	// start ADC capture
	Analogue.setup(true, ANALOGUE::KICK_BY_TIMER0); // setup analogue read (check every 16ms)
	Analogue.begin(pack_bits(
						BRD_APPTWELITE::PIN_AI1,
						BRD_APPTWELITE::PIN_AI2,
						BRD_APPTWELITE::PIN_AI3,
						BRD_APPTWELITE::PIN_AI4,
				   		PIN_ANALOGUE::VCC)); // _start continuous adc capture.

	// Timer setup
	Timer0.begin(32, true); // 32hz timer

	// start button check
	Buttons.setup(5); // init button manager with 5 history table.
	Buttons.begin(pack_bits(
						BRD_APPTWELITE::PIN_DI1,
						BRD_APPTWELITE::PIN_DI2,
						BRD_APPTWELITE::PIN_DI3,
						BRD_APPTWELITE::PIN_DI4),
					5, 		// history count
					4);  	// tick delta (change is detected by 5*4=20ms consequtive same values)	


	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial 	<< "--- BRD_APPTWELITE ---" << mwx::crlf;
	Serial	<< format("-- app:x%08x/ch:%d/lid:%d"
					, the_twelite.get_appid()
					, the_twelite.get_channel()
					, nwk.get_config().u8Lid
				)
			<< mwx::crlf;
	Serial 	<< format("-- pw:%d/retry:%d/opt:x%08x"
					, the_twelite.get_tx_power()
					, nwk.get_config().u8RetryDefault
					, OPT_BITS
			)
			<< mwx::crlf;
}
```

The general flow of the program is the initial setup of each section and the start of each section.



#### Registration of various BEHAVIOR objects

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
```

Register BEHAVIOR objects to determine system behavior. It can be an Interactive settings mode, board support, or a network description of wireless packets.

{% hint style="warning" %}
It will not work unless registered within `setup()`.
{% endhint %}



#### Configure the Interactive settings mode

```cpp
// Initialize Interactive settings mode
auto&& set = the_twelite.settings.use<STG_STD>();

set << SETTINGS::appname("BRD_APPTWELITE");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.
LID = set.u8devid(); // logical ID;
```

Initialize the Interactive settings mode. First, a `set` object is obtained. Then, the following process is performed.

* Set the application name to `"BRD_APPTWELITE"` (used in the menu)
* Rewrite default Application ID and CHANNEL values
* Delete unnecessary items
* reads configuration values saved by `set.reload()`.
* Copy the values of `OPT_BITS` and `LID` into the variables

{% hint style="info" %}
Reflection of the read Interactive settings mode settings is described below.
{% endhint %}



Below is an example screen. + + + and + three times with a pause of 0.2 to 1 second to bring up the Interactive settings mode screen.

```
[CONFIG/BRD_APPTWELITE:0/SID=8XXYYYYY]
a: (0x1234ABCD) Application ID [HEX:32bit]
i: (        13) Device ID [1-100,etc]
c: (        13) Channel [11-26]
x: (      0x03) RF Power/Retry [HEX:8bit]
o: (0x00000000) Option Bits [HEX:32bit]

 [ESC]:Back [!]:Reset System [M]:Extr Menu    
```

{% hint style="info" %}
Option Bits are not used in this sampler, although they are displayed in the menu.
{% endhint %}



#### the\_twelite

This object behaves as the core of TWENET.

```cpp
auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
```

Register a board (this ACT registers `<BRD_APPTWELITE>`). Specify the name of the board you want to register with `<>` after `use` as follows.

The return value obtained by universal reference (`auto&&`) is a board object of reference type. This object contains board-specific operations and definitions. The following example uses the board object to check the status of the M1 pin: if the M1 pin is LO, set LID = 0, i.e., the Parent Node address.

```cpp
	if (brd.get_M1()) { LID = 0; }
```



Initial configuration is required to make the the\_twelite work. Application ID and wireless CHANNEL settings are mandatory.

```cpp
	// the twelite main class
	the_twelite
		<< set
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)
```

Use `<<` to apply the setting to `the_twelite`.

* `set` reflects some of the settings (Application ID, radio channel, etc.) read from Interactive settings mode. The items to be reflected are described in [\<STG\_STD>](../settings/stg\_std.md#no-1).
* `TWENET::rx_when_idle()` Specification to open the receive circuit.

{% hint style="info" %}

The `<<, >>` operator is originally a bit shift operator, but it is used differently from its meaning.
Within the MWX library, the above settings and serial port input/output are used in the library, following the C++ standard library's use of input/output.

```cpp
// The following statements are not available in the MWX library
#include <iostream>
std::cout << "hello world" << std::endl;
```
{% endhint %}



Next, register the network.

```cpp
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
nwk << set;
nwk << NWK_SIMPLE::logical_id(LID);
```

The first line is written in the same way as for board registration, where `<>` is `<NWK_SIMPLE>`.

The second and third lines are settings for `<NWK_SIMPLE>`. The first one reflects the value of the settings in Interactive settings mode. The items to be reflected are LID and the number of retransmissions. In this application, LID is set again in the third line because LID=0 may be set depending on the state of the M1 pin.



#### Analogue

Class object that handles ADCs (analog-to-digital converters).

```cpp
Analogue.setup(true, ANALOGUE::KICK_BY_TIMER0);
```

The Initialization is performed by `Analogue.setup()`. The parameter `true` specifies to wait in place until the ADC circuit is stable; the second parameter specifies to start the ADC synchronously with Timer0.

```cpp
	Analogue.begin(pack_bits(
						BRD_APPTWELITE::PIN_AI1,
						BRD_APPTWELITE::PIN_AI2,
						BRD_APPTWELITE::PIN_AI3,
						BRD_APPTWELITE::PIN_AI4,
				   	PIN_ANALOGUE::VCC));
```

To start the ADC, call `Analogue.begin()`. The parameter is a bitmap corresponding to the pin of the ADC target.

The `pack_bits()` function is used to specify a bitmap. It is a function with variable number of arguments, each argument specifies a bit position to be set to 1. For example, `pack_bits(1,3,5)` returns the binary value `101010`. This function has the `constexpr` specification, so if the parameters are constants only, they are expanded to constants.

Parameters and `BRD_APPTWELITE::` specified as `PIN_AI1..4` are defined, corresponding to AI1..AI4 used in App\_Twelite. AI1=ADC1, AI2=DIO0, AI3=ADC2, AI4=DIO2 and so on are assigned. PIN_ANALOGUE. `PIN_ANALOGUE::` defines the list of pins available for ADC.

{% hint style="info" %}
Except for the first time, the ADC is started in the interrupt handler.
{% endhint %}



#### Buttons

Detects changes in DIO (digital input) values; Buttons only detect a change in value after the same value has been detected a certain number of times in order to reduce the effects of chattering (sliding) on mechanical buttons.

```cpp
Buttons.setup(5);
```

Initialization is done with `Buttons.setup()`. The parameter 5 is the number of detections required to determine the value, but it is the maximum value that can be set. Internally, the internal memory is allocated based on this number.

```cpp
Buttons.begin(pack_bits(
						BRD_APPTWELITE::PIN_DI1,
						BRD_APPTWELITE::PIN_DI2,
						BRD_APPTWELITE::PIN_DI3,
						BRD_APPTWELITE::PIN_DI4),
					5, 		// history count
					4);  	// tick delta
```

Start is done with `Buttons.begin()` The first parameter is the DIO to be detected. The second parameter is the number of detections required to determine the state. Since `4` is specified, the HIGH and LOW states are determined when the same value is detected five times in a row every 4ms.

{% hint style="info" %}
Detection of the DIO state in Buttons is done by an event handler. Event handlers are called in the application loop after an interrupt occurs, so there is a delay compared to interrupt handlers.
{% endhint %}



#### Timer0

```cpp
Timer0.begin(32, true); // 32hz timer
```

Since App\_Twelite uses a timer origin to control the application, the same timer interrupt/event should be run in this ACT. Of course, you can also use the system's TickTimer, which runs every 1 ms.

The first parameter in the above example specifies a timer frequency of 32 Hz; setting the second parameter to `true' enables software interrupts.

After calling `Timer0.begin()`, the timer starts running.



#### Start of the operation of the\_twelite

```cpp
the_twelite.begin(); // start twelite!
```

Execute `the_twelite.begin()` at the end of the `setup()` function.



#### Serial

Serial objects can be used without initialization or initiation procedures. 

```cpp
	Serial 	<< "--- BRD_APPTWELITE ---" << mwx::crlf;
	Serial	<< format("-- app:x%08x/ch:%d/lid:%d"
					, the_twelite.get_appid()
					, the_twelite.get_channel()
					, nwk.get_config().u8Lid
				)
			<< mwx::crlf;
	Serial 	<< format("-- pw:%d/retry:%d/opt:x%08x"
					, the_twelite.get_tx_power()
					, nwk.get_config().u8RetryDefault
					, OPT_BITS
			)
			<< mwx::crlf;
```

This sample displays some system configuration values as a startup message. The `Serial` object is given a string of type _const char\*_, a string of type _int_ (no other integer types), a `format()` that behaves almost identically to _printf_, and a `crlf` that outputs newline characters to the _<<_ operator. 

{% hint style="info" %}
The namespace `mwx::` may be omitted in the sample. In the above, it is described as `mwx::crlf`, but may be described as `crlf`. mwx::namespace is designed to be partially optional.
{% endhint %}



### ループ loop()

Loop functions are called as callback functions from the main loop of the TWENET library. The basic description here is to wait for the object to be used to become available and then process it. This section describes the use of some of the objects used in ACT.

{% hint style="warning" %}
The main loop of the TWENET library processes incoming packets and interrupt information stored in the FIFO queue in advance as events, after which `loop()` is called. After exiting `loop()`, the CPU enters DOZE mode and waits until a new interrupt occurs with low current consumption.

Therefore, code that assumes the CPU is always running will not work well.
{% endhint %}

```cpp
/*** loop procedure (called every event) */
void loop() {
	if (Buttons.available()) {
		uint32_t bp, bc;
		Buttons.read(bp, bc);

		u8DI_BM = uint8_t(collect_bits(bp, 
							BRD_APPTWELITE::PIN_DI4,   // bit3
							BRD_APPTWELITE::PIN_DI3,   // bit2
							BRD_APPTWELITE::PIN_DI2,   // bit1
							BRD_APPTWELITE::PIN_DI1)); // bit0

		transmit();
	}

	if (Analogue.available()) {
		au16AI[0] = Analogue.read(PIN_ANALOGUE::VCC);
		au16AI[1] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI1);
		au16AI[2] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI2);
		au16AI[3] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI3);
		au16AI[4] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI4);
	}

	if (Timer0.available()) {
		static uint8_t u16ct;
		u16ct++;

		if (u8DI_BM != 0xFF && au16AI[0] != 0xFFFF) { // finished the first capture
			if ((u16ct % 32) == 0) { // every 32ticks of Timer0
				transmit();
			}
		}
	}
}
```



#### Buttons

It becomes available at the timing when a change in DIO (digital IO) input is detected, and is read by `Buttons.read()`.

```cpp
	if (Buttons.available()) {
		uint32_t bp, bc;
		Buttons.read(bp, bc);
```

The first parameter is a bitmap of the HIGH/LOW of the current DIO, ordered from bit0 to DIO0,1,2,... in order from bit0. For example, for DIO12, HIGH / LOW can be determined by evaluating `bp & (1UL << 12)`. The bit that is set to 1 is HIGH.

{% hint style="info" %}
When the IO state is determined for the first time, MSB (bit31) is set to 1. The first time the IO status is determined is also performed when the device returns from sleep mode.
{% endhint %}

Next, the values are extracted from the bitmap and stored in `u8DI_BM`. Here, the `collect_bits()` function provided by the MWX library is used.

```cpp
u8DI_BM = uint8_t(collect_bits(bp, 
		BRD_APPTWELITE::PIN_DI4,   // bit3
		BRD_APPTWELITE::PIN_DI3,   // bit2
		BRD_APPTWELITE::PIN_DI2,   // bit1
		BRD_APPTWELITE::PIN_DI1)); // bit0

/* collect_bits performs the following operations
u8DI_BM = 0;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI1)) u8DI_BM |= 1;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI2)) u8DI_BM |= 2;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI3)) u8DI_BM |= 4;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI4)) u8DI_BM |= 8;
*/
```

`collect_bits()` takes an integer argument of bit positions similar to `pack_bits()` above. It is a function with a variable number of arguments and arranges the parameters as many as necessary. In the above process, bit0 is stored in u8DI\_BM as the value of DI1, bit1 as the value of DI2, bit2 as the value of DI3, and bit3 as the value of DI4.


In App\_Twelite, since wireless transmission is performed when there is a change from DI1 to DI4, the transmission process is performed starting from `Buttons.available()`. The details of the `transmit()` process are described below.

```cpp
transmit();
```



#### Analogue

It becomes available at `loop()` immediately after the analog-to-digital conversion of the ADC is completed. Until the next ADC starts, data can be read back as if it was acquired immediately before.

```cpp
if (Analogue.available()) {
	au16AI[0] = Analogue.read(PIN_ANALOGUE::VCC);
	au16AI[1] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI1);
	au16AI[2] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI2);
	au16AI[3] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI3);
	au16AI[4] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI4);
}
```

Use the `Analogue.read()` or `Analogue.read_raw()` method to read ADC values. `read()` is the value converted to mV, `read_raw()` is the ADC value of 0..1023. The parameter is the pin number of ADC, which is defined in `PIN_ANALOGUE::` or `BRD_APPTWELITE::`.

{% hint style="warning" %}
ADC values that are executed cyclically may read more recent values prior to the AVAILABLE notification, depending on the timing.

Since this ACT processes at a relatively slow cycle of 32 Hz, it is not a problem if the processing is done immediately after the AVAILABLE decision, but if the conversion cycle is short, or if you are doing a relatively long process in `loop()`, be careful.

In `Analogue`, you can specify a callback function that will be called from within the interrupt handler after the conversion is complete. For example, this callback function will store the value in the FIFO queue, and the application loop will perform asynchronous processing such as sequential reading of the queue value.
{% endhint %}



#### Timer0

The `Timer0` runs at 32 Hz. It becomes available at `loop()` immediately after a timer interrupt occurs. In other words, it processes 32 times per second. Here, the transmission is processed when it reaches exactly 1 second.

```cpp
if (Timer0.available()) {
	static uint8_t u16ct;
	u16ct++;

	if (u8DI_BM != 0xFF && au16AI[0] != 0xFFFF) { // finished the first capture
		if ((u16ct % 32) == 0) { // every 32ticks of Timer0
			transmit();
		}
	}
}
```

AppTwelite performs a periodic transmission about every second. When `Timer0` becomes available, it increments `u16ct`. Based on this counter value, `transmit()` is called to send a radio packet when the count is completed 32 times.

The value judgment of `u8DI_BM` and `au16AI[]` is whether or not it is just after initialization. If the values of DI1..DI4 and AI1..AI4 are not yet stored, nothing is done.



### transmit()

This function requests TWENET to send a radio packet. At the end of this function, the wireless packet is not yet processed. The actual transmission will be completed in a few ms or later, depending on the transmission parameters. This section describes typical transmission request methods.

```cpp
MWX_APIRET transmit() {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
	  auto&& set = the_twelite.settings.use<STG_STD>();
		if (!set.is_screen_opened()) {
			Serial << "..DI=" << format("%04b ", u8DI_BM);
			Serial << format("ADC=%04d/%04d/%04d/%04d ", au16AI[1], au16AI[2], au16AI[3], au16AI[4]);
			Serial << "Vcc=" << format("%04d ", au16AI[0]);
			Serial << " --> transmit" << mwx::crlf;
		}

		// set tx packet behavior
		pkt << tx_addr(u8devid == 0 ? 0xFE : 0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x1 send two times in total)
			<< tx_packet_delay(0,50,10); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(APP_FOURCHAR, 4) // string should be paired with length explicitly.
			, uint8_t(u8DI_BM)
		);

		for (auto&& x : au16AI) {
			pack_bytes(pkt.get_payload(), uint16_t(x)); // adc values
		}
		
		// do transmit 
		return pkt.transmit();
	}
	return MWX_APIRET(false, 0);
}
```

####

#### function prototype

```
MWX_APIRET transmit()
```

`MWX_APIRET` is a class that handles return values with data members of type `uint32_t`, where MSB (bit31) is success/failure and the rest are used as return values.


#### Obtaining network and packet objects

```cpp
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

Get a network object with `the_twelite.network.use<NWK_SIMPLE>()`. Use that object to get a `pkt` object with `.prepare_tx_packet()`.

Here it is declared in a conditional expression in an if statement. The declared `pkt` object is valid until the end of the _if_ clause. pkt object gives a response of type _bool_, which here is `true` if the TWENET send request queue is free and the send request is accepted, or `false` if there is no free queue.


#### Display suppression during Interactive settings mode screen display

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
if (!set.is_screen_opened()) {
    //Not during Interactive settings mode screen!
}
```

Suppresses screen output when Interactive settings mode screen is displayed.


#### Packet transmission settings

```cpp
pkt << tx_addr(u8devid == 0 ? 0xFE : 0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
		<< tx_retry(0x1) // set retry (0x3 send four times in total)
		<< tx_packet_delay(0,50,10); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)
```

Packets are configured using the `<<` operator as in the initialization settings of `the_twelite`.

* Specify the destination address in the `tx_addr()` parameter. If it is `0x00`, it means that you are the Child Node and broadcast to the Parent Node, and if it is `0xFE`, it means that you are the Parent Node and broadcast to any Child Node.
* The `tx_retry()` parameter specifies the number of retransmissions. The `1` in the example sends one retry, i.e., a total of two packets. Sending only one wireless packet will fail a few percent of the time even under good conditions.
* `tx_packet_delay()` Sets the transmission delay. The first parameter is the minimum wait time to start sending, the second is the maximum wait time, in this case approximately from 0 to 50 ms after issuing a request to send. The third is the retransmission interval. The third is the retransmission interval, meaning that a retransmission is performed every 10 ms after the first packet is sent.



#### Data payload of the packet

Payload means a loaded item, but in wireless packets it is often used to mean "the main body of data to be sent". In addition to the main body of data, the data in a wireless packet also contains some auxiliary information, such as address information.

Be aware of the data order of the data payload for correct transmission and reception. In this example, the data order is as follows. Construct the data payload according to this data order.

```cpp
# Index of first byte: Data type : Number of bytes : Contents

00: uint8_t[4] : 4 : four-character identifier
04: uint8_t    : 1 : Bitmap of DI1..4
06: uint16_t   : 2 : Voltage value of Vcc
08: uint16_t   : 2 : ADC value of AD1 (0..1023)
10: uint16_t   : 2 : ADC value of AD2 (0..1023)
12: uint16_t   : 2 : ADC value of AD3 (0..1023)
14: uint16_t   : 2 : ADC value of AD4 (0..1023)
```

{% hint style="info" %}
The data payload can contain 90 bytes (actually several more bytes).

Every byte of an IEEE802.15.4 wireless packet is precious. There is a limit to the amount of data that can be sent in a single packet. If a packet is split, the cost of the split packet is high, since it is necessary to take into account transmission failures. Also, sending one extra byte consumes energy equivalent to approximately 16 µs x the current used during transmission, which is especially significant for battery-powered applications.

The above example is a compromise to some extent for the sake of explanation. To save money, the 00: identifier should be a simple 1 byte, and the Vcc voltage value may be rounded to 8 bits. Also, each AI1..AI4 value is 10 bits, using 6 bytes for a total of 40 bits = 5 bytes.
{% endhint %}



Let's actually construct the data structure of the above data payload. The data payload can be referenced as a container of type `simplbuf<uint8_t>` by `pkt.get_payload()`. In this container, we build the data based on the above specification.

```cpp
auto&& payl = pkt.get_payload();
payl.reserve(16); // Resize to 16 bytes
payl[00] = APP_FOURCHAR[0];
payl[01] = APP_FOURCHAR[1];
...
payl[08] = (au16AI[0] & 0xFF00) >> 8; //Vcc
payl[09] = (au16AI[0] & 0xFF);
...
payl[14] = (au16AI[4] & 0xFF00) >> 8; // AI4
payl[15] = (au16AI[4] & 0xFF);
```

It can be written as above, but the MWX library provides an auxiliary function `pack_bytes()` for data payload construction.

```cpp
// prepare packet payload
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(APP_FOURCHAR, 4) // string should be paired with length explicitly.
	, uint8_t(u8DI_BM)
);

for (auto&& x : au16AI) {
	pack_bytes(pkt.get_payload(), uint16_t(x)); // adc values
}
```

The first parameter of `pack_bytes` specifies the container. In this case, it is `pkt.get_payload()`.

The parameter after that is a variable number of arguments, `pack_bytes`, specifying the required number of values of the corresponding type. The `pack_bytes` internally calls the `.push_back()` method to append the specified value at the end.

The third line, `make_pair()`, is a standard library function to generate `std::pair`. This is to avoid confusion of string types (specifically, including or excluding null characters when storing payloads). The first parameter of `make_pair()` is the string type (`char*`, `uint8_t*`, `uint8_t[]`, etc.) The second parameter is the number of bytes to store in the payload.

The fourth line is of type `uint8_t` and writes a bitmap of DI1..DI4.

Lines 7-9 write the values of the `au16AI` array in sequence. The values are of type `uint16_t` and are 2 bytes, but are written in big-endian order.

{% hint style="info" %}
The for statement in line 7 is a **range for** statement introduced in C++. This syntax can be used for arrays of known size or container classes that can be accessed by `begin(), end()` iterators. au16AI types can also be determined at compile time, so the type specification is also omitted with `auto&&` (see Universal Reference).

If rewritten as a normal for statement, it would look like this

```cpp
for(int i = 0; i < sizeof(au16AI)/sizeof(uint16_t)); i++) {
  pack_bytes(pkt.get_payload(), au16AI[i]);
}
```
{% endhint %}


This completes the packet preparation. Now all that remains is to make a request for transmission.

```cpp
return pkt.transmit();
```

Packets are sent using the `pkt.transmit()` method of the `pkt` object. It returns a return value of type `MWX_APIRET`, which is not used in this ACT.

{% hint style="info" %}
The return value contains information on the success or failure of the request and the number corresponding to the request. To perform a process that waits until the completion of transmission, use this return value.
{% endhint %}



### on\_rx\_packet()

When a wireless packet is received, `on_rx_packet()` is called as a receive event.

{% hint style="info" %}
The procedure with `the_twelite.receiver` stores incoming packets in an internal queue (up to 2 packets) before processing them, but `on_rx_packet()` is called directly from a callback from the TWENET library, so it is more resistant to overflow. However, `loop()` is called directly from a callback from the TWENET library, and is less likely to cause overflow. However, if the processing is stopped for a long time in a `loop()` statement, it may cause the same kind of overflow.
{% endhint %}

Here, the values of DI1...DI4 and AI1...AI4 communicated by the other party are set to its own DO1...DO4 and PWM1...PWM4.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {	
	auto&& set = the_twelite.settings.use<STG_STD>();

	Serial << format("..receive(%08x/%d) : ", rx.get_addr_src_long(), rx.get_addr_src_lid());

  // expand the packet payload
	char fourchars[5]{};
	auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
    );

	// check header
	if (strncmp(APP_FOURCHAR, fourchars, 4)) { return; }

	// read rest of payload
	uint8_t u8DI_BM_remote = 0xff;
	uint16_t au16AI_remote[5];
	expand_bytes(np, rx.get_payload().end()
		, u8DI_BM_remote
		, au16AI_remote[0]
		, au16AI_remote[1]
		, au16AI_remote[2]
		, au16AI_remote[3]
		, au16AI_remote[4]
	);

	Serial << format("DI:%04b", u8DI_BM_remote & 0x0F);
	for (auto&& x : au16AI_remote) {
		Serial << format("/%04d", x);
	}
	Serial << mwx::crlf;

	// set local DO
	digitalWrite(BRD_APPTWELITE::PIN_DO1, (u8DI_BM_remote & 1) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO2, (u8DI_BM_remote & 2) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO3, (u8DI_BM_remote & 4) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO4, (u8DI_BM_remote & 8) ? HIGH : LOW);

	// set local PWM : duty is set 0..1024, so 1023 is set 1024.
	Timer1.change_duty(au16AI_remote[1] == 1023 ? 1024 : au16AI_remote[1]);
	Timer2.change_duty(au16AI_remote[2] == 1023 ? 1024 : au16AI_remote[2]);
	Timer3.change_duty(au16AI_remote[3] == 1023 ? 1024 : au16AI_remote[3]);
	Timer4.change_duty(au16AI_remote[4] == 1023 ? 1024 : au16AI_remote[4]);
}

```

First, the data `rx` of the received packet is passed as a parameter. From `rx` the address information and data payload of the wireless packet is accessed. The parameter `handled` is not normally used.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled)
```

Received packet data is referenced to the source address (32-bit long address and 8-bit logical address) and other information. Output is suppressed when the Interactive settings mode screen is displayed.

```cpp
if (!set.is_screen_opened()) {
   Serial << format("..receive(%08x/%d) : ",
      rx.get_addr_src_long(), rx.get_addr_src_lid());
}
```

{% hint style="info" %}
In `<NWK_SIMPLE>`, two types of addresses are always exchanged: an 8-bit logical ID and a 32-bit long address. When specifying the destination, either the long address or the logical address is specified. When receiving, both addresses are included.
{% endhint %}


The MWX library provides a function `expand_bytes()` as a counterpart to `pack_bytes()` used in `transmit()`.

```cpp
char fourchars[5]{};
auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
	, make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
  );
```

The first line declares an array of type `char` for data storage. The size of the array is 5 bytes because the null character is included at the end for convenience in character output, etc. The trailing `{}` specifies initialization. The trailing `{}` specifies initialization, and although it is sufficient to set the fifth byte to 0, here the entire array is initialized in the default way, that is, to `0`.

The 4-byte string is extracted by `expand_bytes()` in the second line. The reason for not specifying the container type in the parameters is that we need to know the read position to read this continuation. the first parameter specifies the first iterator of the container (`uint8_t*` pointer), which can be obtained by the `.begin()`.  The second parameter is an iterator that points to the next to the end of the container and can be retrieved with the `.end()` method. The second parameter is used to avoid reading beyond the end of the container.

The third variable to read is specified, again by `make_pair`, which is a pair of string array and size.

{% hint style="info" %}
The ACT of `expand_bytes()` omits error checking, for example, if the packet length is incorrect. If you want to make the check strict, judge by the return value of `expand_bytes()`.

The return value of `expand_bytes()` is `uint8_t*`, but returns `nullptr (null pointer)` for accesses beyond the end.
{% endhint %}


If the identifier in the 4-byte string read is different from the identifier specified in this ACT, this packet is not processed.

```cpp
if (strncmp(APP_FOURCHAR, fourchars, 4)) { return; }
```

{% hint style="info" %}
In TWENET, if the Application ID and the physical radio CHANNEL match, any application can receive the packet, even if it is of a different type. For the purpose of avoiding unintended reception of packets created by other applications, it is recommended to check such identifiers and the structure of data payloads to prevent coincidental matches.

The requirements for packet structure on the simple network `<NWK_SIMPLE>` must also be satisfied, so mixed packet reception will not occur unless other applications that do not use the simple network define packets with the same structure (which seems very rare).
{% endhint %}

The next step is to get the data part: store the values of DI1..DI4 and AI1..AI4 in separate variables.

```cpp
	// read rest of payload
	uint8_t u8DI_BM_remote = 0xff;
	uint16_t au16AI_remote[5];
	expand_bytes(np, rx.get_payload().end()
		, u8DI_BM_remote
		, au16AI_remote[0]
		, au16AI_remote[1]
		, au16AI_remote[2]
		, au16AI_remote[3]
		, au16AI_remote[4]
	);
```

The first parameter is the return value `np` of the previous `expand_bytes()`. The second parameter is the same.

The third and subsequent parameters are variables of the **matching type** alongside the data payload, in the same order as the **sender's data structure**. Once this process is complete, the specified variable will contain the value read from the payload.

Output to serial port for confirmation. Output is suppressed when the Interactive settings mode screen is displayed.

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
Serial << format("DI:%04b", u8DI_BM_remote & 0x0F);
for (auto&& x : au16AI_remote) {
	Serial << format("/%04d", x);
}
Serial << mwx::crlf;
```

We use `format()` because we need to format output of numbers. helper class that allows the same syntax as `printf()` for the `>>` operator, but limits the number of arguments to four. (`Serial.printfmt()` has no limit on the number of arguments.)

The first line `"DI:%04b"` prints a 4-digit bitmap of DI1..DI4, like `"DI:0010"` The third line `"/%04d"` prints the values of Vcc/AI1..AI4 as integers, like `"/3280/0010/0512/1023/"` The fifth line `mwx::crlf` prints a newline string.

Now that the necessary data has been extracted, all that remains is to change the values of DO1..DO4 and PWM1..PWM4 on the board.

```cpp
// set local DO
digitalWrite(BRD_APPTWELITE::PIN_DO1, (u8DI_BM_remote & 1) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO2, (u8DI_BM_remote & 2) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO3, (u8DI_BM_remote & 4) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO4, (u8DI_BM_remote & 8) ? HIGH : LOW);

// set local PWM : duty is set 0..1024, so 1023 is set 1024.
Timer1.change_duty(au16AI_remote[1] == 1023 ? 1024 : au16AI_remote[1]);
Timer2.change_duty(au16AI_remote[2] == 1023 ? 1024 : au16AI_remote[2]);
Timer3.change_duty(au16AI_remote[3] == 1023 ? 1024 : au16AI_remote[3]);
Timer4.change_duty(au16AI_remote[4] == 1023 ? 1024 : au16AI_remote[4]);
```

`digitalWrite()` changes the value of the digital output, the first parameter is the pin number and the second is `HIGH` (Vcc level) or `LOW` (GND level).

`Timer?.change_duty()` changes the duty ratio of the PWM output. Specify a duty ratio of 0..1024 for the parameter. Note that the maximum value is not 1023 (the maximum value is 1024, a power of 2, due to the high cost of the division performed in the library). Setting the value to `0` will result in an output equivalent to the GND level, and setting it to `1024` will result in an output equivalent to the Vcc level.
