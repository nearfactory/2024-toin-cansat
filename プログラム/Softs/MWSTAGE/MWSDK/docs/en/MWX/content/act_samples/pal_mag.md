---
title: "PAL_MAG"
author: "Mono Wireless Inc."
---

# PAL\_MAG

[OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) is used to acquire sensor values.


{% hint style="success" %}
This ACT includes

* Sending and receiving wireless packets
* Configuring settings via Interactive settings mode - [\<STG\_STD>](../settings/stg\_std.md)
* State transition control by state machine - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MAG>](../boards/pal/pal\_mag.md) or [\<CUE>](../boards/cue.md) board manipulation via board behavior
{% endhint %}



## ACT functions

* Uses the OPEN-CLOSE SENSE PAL to interrupt and wake up when the magnetic sensor is detected and transmit wirelessly.
* Uses the sleep function to operate on coin cell batteries.



## How to use ACT

### Required TWELITE

| Role | Example                                                                                                                                                                                               |
| -- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Parent Node | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUE or RED</a> Act <a href="parent_monostick.md">Parent\_MONOSTICK</a> in action.                                   |
| Child Node | [BLUE PAL or RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) + [OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html)  |



## Explanation of ACT

### Include

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <PAL_MAG>
```

Board BEHAVIOR for open/close sensor pal [`<PAL_MAG>`](../boards/pal/pal\_mag.md) is included.



### setup()

```cpp
void setup() {
	/*** SETUP section */
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_MAG>();
	// now it can read DIP sw status.
	u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
	if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE

	// LED setup (use periph_led_timer, which will re-start on wakeup() automatically)
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)

	// the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID); // set Logical ID. (0xFE means a child device with no ID)

	/*** BEGIN section */
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- PAL_MAG:" << FOURCHARS << " ---" << mwx::crlf;
}
```



First, register the board behavior `<PAL_MAG>`. When the board behavior is initialized, the sensors and DIOs are initialized. The reason for doing this first is that it is common to check the status of the board's DIP SW, etc., and then configure the network settings and so on.

```cpp
auto&& brd = the_twelite.board.use<PAL_MAG>();

u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE
```

Here, 3 bits of the 4-bit DIP SW on the board are read and set as the ID of the Child Node; if 0, the Child Node without ID (`0xFE`) is assumed.

Set the LED settings. Here the ON/OFF blinking is set to blink every 10ms (in an application that sleeps and has a short wake-up time, this is almost equivalent to setting the LED to turn on during wake-up).

```cpp
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



### begin()

The `begin()` function exits the `setup()` function (after which TWENET is initialized) and is called just before the first `loop()`.

```cpp
void begin() {
	sleepNow(); // the first time is just sleeping.
}
```

Call `sleepNow()` after `setup()` ends to perform the first sleep.



### sleepNow()

```cpp
void sleepNow() {
	uint32_t u32ct = 60000;
	
	pinMode(PAL_MAG::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
	pinMode(PAL_MAG::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);

	the_twelite.sleep(u32ct);
}
```

Before going to sleep, set the interrupt for the DIO pin of the magnetic sensor. Use `pinMode()`, the second parameter is `PIN_MODE::WAKE_FALLING`. This is a setting to wake up when the pin state changes from HIGH to LOW.

In line 7, `the_twelite.sleep()` executes sleep. The parameter 60000 is the wake-up setting required to reset the watchdog on the TWELITE PAL board. If not reset, a hard reset will be performed after 60 seconds.

###

### wakeup()

When the program wakes up from sleep, `wakeup()` is called. After that, `loop()` is called each time. Before `wakeup()`, each peripheral such as UART and devices on the board are woken up (e.g. resetting the watchdog timer). For example, it restarts the LED lighting control.

```cpp
void wakeup() {
	if (the_twelite.is_wokeup_by_wktimer()) {
		sleepNow();
	}
}
```

Here, in the case of wake-up from the wake-up timer (`the_twelite.is_wokeup_by_wktimer()`), sleep is performed again. This is a wake-up only for the purpose of resetting the watchdog timer described above.

In the case of wakeup upon detection of the magnetic sensor, the loop() process will continue.



### loop()

Here, the DIO of the detected magnetic sensor is checked, packets are sent, and sleep is performed again after packet transmission is complete.

```cpp
void loop() {
	if (!b_transmit) {
		if (auto&& pkt = 
      the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet())

			uint8_t b_north = 
			  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
			uint8_t b_south = 
			  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
	
			Serial << "..sensor north=" << int(b_north) 
			       << " south=" << int(b_south) << mwx::crlf;
	
			// set tx packet behavior
			pkt << tx_addr(0x00)
				<< tx_retry(0x1)
				<< tx_packet_delay(0, 0, 2);
	
			// prepare packet payload
			pack_bytes(pkt.get_payload()
				, make_pair(FOURCHARS, 4) 
				, b_north
				, b_south
			);
	
			// do transmit
			MWX_APIRET ret = pkt.transmit();
	
			if (ret) {
				u8txid = ret.get_value() & 0xFF;
				b_transmit = true;
			}
			else {
				// fail to request
				sleepNow();
			}
		} else {
		  sleepNow();
		}
	} else { 
		if (the_twelite.tx_status.is_complete(u8txid)) {		
			b_transmit = 0;
			sleepNow();
		}
	}
}
```



The `b_transmit` variable controls the behavior within `loop()`. After a successful transmission request, this value is set to 1, and the program waits for the packet transmission to complete.

```cpp
	if (!b_transmit) {
```



Check the detection DIO pins of the magnetic sensor. There are two types of detection pins: N-pole detection and S-pole detection. If you simply want to know that a magnet is approaching, you need to know that one of the pins has been detected.

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
```

Use `the_twelite.is_wokeup_by_dio()` to check the pin of the wakeup factor. The parameter is the pin number. The reason why the return value is stored in uint8\_t is to store it in the payload of the packet.



After setting communication conditions and storing data in the payload, transmission is performed.

```cpp
// do transmit
MWX_APIRET ret = pkt.transmit();
```



Then, if `b_transmit` is `true` during `loop()`, a completion check is performed, and if completed, `sleepNow()` puts the program to sleep.

```cpp
if (the_twelite.tx_status.is_complete(u8txid)) {		
	b_transmit = 0;
	sleepNow();
}
```

The completion of transmission is confirmed by `the_twelite.tx_status.is_complete(u8txid)`.  The `u8txid` is the ID value returned on transmission.
