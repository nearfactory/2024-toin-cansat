---
title: "Scratch"
author: "Mono Wireless Inc."
---
# Scratch

Template code.

{% hint style="success" %}
This act includes

* Sending radio packets ('`t`' key)
* Sleep ('`s`' key)
* Input from serial port - [Serial](../api-reference/classes/twe-stream/#read)
* Digital (button) input - [Buttons](../api-reference/predefined\_objs/buttons.md)
{% endhint %}

## setup()

```cpp
void setup() {
	/*** SETUP section */
	tx_busy = false;

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(0xFE); // set Logical ID. (0xFE means a child device with no ID)

	/*** BEGIN section */
	Buttons.begin(pack_bits(PIN_BTN), 5, 10); // check every 10ms, a change is reported by 5 consequent values.

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- Scratch act ---" << mwx::crlf;
}
```

Set `the_twelite` to set the application ID `APP_ID`, the radio channel `CHANNEL` and the receive presence.

It also generates `nwk` and specifies the child machine address `0xFE`. This address means that this is an unnamed child machine that has not been addressed by a child machine.

{% hint style="info" %}
The addresses that can be set are 0x00: parent unit, 0x01\~0xEF: child unit, 0xFE: unspecified child unit address range.

The address to be specified as the transmission destination is 0x00 for the parent machine, 0x01\~0xEF for the specified parent machine address, 0xFE for any child machine address, and 0xFF for any address including the parent machine.
{% endhint %}

It also initializes the Buttons object. This is a chatter-suppressing algorithm by successive references, which determines HI or LOW of the target port (PIN\_BTN only) if the value is the same 5 times in 10ms. [`pack_bits`](../api-reference/funcs/utility/pack\_bits.md)`(N1, N2, ...)` pack\_bits(N1, N2, ...)' does `1UL<<N1 | 1UL << N2 | ...` to make a bitmap.



```cpp
the_twelite.begin(); // start twelite!
```

This is the procedure to start `the_twelite`, it didn't appear in act0..4, but you should call it if you set up `the_twelite` or register various behaviors.



## begin()

```cpp
void begin() {
	Serial << "..begin (run once at boot)" << mwx::crlf;
}
```

Called only once after `setup()` on startup. Only displays a message.



## loop()

### Input detection of buttons (switches).

```cpp
if (Buttons.available()) {
	uint32_t bm, cm;
	Buttons.read(bm, cm);

	if (cm & 0x80000000) {
		// the first capture.
	}

	Serial << int(millis()) << ":BTN" << format("%b") << mwx::crlf;
}
```

The state is determined by continuous reference to [Buttons](../api-reference/predefined\_objs/buttons.md). When the button state changes, it is output serially.



### Input from serial.

```cpp
while(Serial.available()) {
  int c = Serial.read();

	Serial << '[' << char(c) << ']';

  switch(c) {
  case 'p': ... // display millis()
  case 't': ... // transmit radio packet (vTransmit)
        if (!tx_busy) {
					tx_busy = Transmit();
					if (tx_busy) {
						Serial  << int(millis()) << ":tx request success! (" 
										<< int(tx_busy.get_value()) << ')' << mwx::crlf;
 					} else {
						Serial << int(millis()) << ":tx request failed" << mwx::crlf;;
					}
				}
  case 's': ... // sleeping
				Serial << int(millis()) << ":sleeping for " << 5000 << "ms" << mwx::crlf << mwx::flush;
				the_twelite.sleep(5000);
				break;
  }
}
```

If `Serial.available()` is `true`, the input from the serial port is stored. It reads one character from the serial and processes it according to the input character.



#### Enter `t` for wireless transmission

When '`t`' is input, sending is done. In this sample, the tx\_busy flag is used to prevent continuous input.

{% hint style="info" %}
Send requests are stored in a queue up to a certain number of packets, so it is possible to stack requests in the range of the queue (3 packets).

The following is an example of what happens when you don't check `if(!tx_busy)` and type '`tttt`' continuously: the queue fills up at the fourth request and the request fails. The pkt object obtained by `.prepare_tx_packet()` in `Transmit()` will be `false`.

The timing of the transmissions is randomised, so the completion of transmissions is not in the order in which they are requested.

```
--- Scratch act ---
..begin (run once at boot)
[t]11591:Transmit()
11592:tx request success! (1)
[t]11593:Transmit()
11593:tx request success! (2)
[t]11594:Transmit()
11595:tx request success! (3)
[t]11595:Transmit()
TX QUEUE is FULL
11596:tx request failed
11654:tx completed!(id=2, stat=1)
11719:tx completed!(id=3, stat=1)
11745:tx completed!(id=1, stat=1)
```
{% endhint %}



#### Type `s` to sleep.

```cpp
the_twelite.sleep(5000);
```

The system will sleep for 5000ms=5 seconds. After recovery, `wakeup()` is executed.



## wakeup()

```cpp
void wakeup() {
	Serial << int(millis()) << ":wake up!" << mwx::crlf;
}
```

First to be called on sleep wake up. Display of message only.



## Transmit()

```cpp
MWX_APIRET Transmit() {
	Serial << int(millis()) << ":Transmit()" << mwx::crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0xFF)  // Broadcast communications
			<< tx_retry(0x1)    // 1 resend
			<< tx_packet_delay(100,200,20); // Transmission delay between 100-200 ms, retransmission interval 20 ms

		// Specification of the data to be sent (to be determined for each application)
		pack_bytes(pkt.get_payload()
			, make_pair("SCRT", 4) // 4-character identifier
			, uint32_t(millis())   // Timestamp
		);
		
		// Make a request to send
		return pkt.transmit(); 
	} else {
		// Failed at .prepare_tx_packet() (send queue is full)
		Serial << "TX QUEUE is FULL" << mwx::crlf;
	  return MWX_APIRET(false, 0);
	}
}
```

This is the minimum procedure for making a transmission request.

When you leave this function, the request has not yet been executed. You need to wait a while. In this example we have set a delay of 100-200ms for the start of the transmission, so the transmission will not start until 100ms at the earliest.



## on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	Serial 	<< int(millis()) << ":tx completed!"
			<< format("(id=%d, stat=%d)", ev.u8CbId, ev.bStatus) << mwx::crlf;
	tx_busy = false; // clear tx busy flag.
}
```

Called on completion of a transmission; ev contains the transmission ID and completion status.



## on\_rx\_packet()

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	Serial << format("rx from %08x/%d", 
					rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;
}
```

When a packet is received, the sender's address information is displayed.







