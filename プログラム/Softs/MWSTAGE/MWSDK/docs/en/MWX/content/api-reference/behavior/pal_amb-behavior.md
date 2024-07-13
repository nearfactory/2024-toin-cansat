---
title: "PAL_AMB-behavior"
author: "Mono Wireless Inc."
---

# PAL\_AMB-behavior

[PAL AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) is used to acquire sensor values.

* [BEHAVIOR] (./) is used to describe the Parent Node Child Node.
* The sensor is described directly using [`Wire`](../predefined\_objs/wire/) instead of using the [board behavior](../../boards/pal/pal\_amb.md) function to obtain values.
* Child Nodes are described by a state machine.

{% hint style="success" %}
See [the explanation of BRD_APPTWELITE](../../act\_samples/brd\_apptwelite.md), [the explanation of PAL_AMB](../../act\_samples/pal\_amb.md), and [the explanation of PAL_AMB-usenap](../../act\_samples/pal\_amb-usenap.md) before the explanation of this ACT. Also see [the description of BEHAVIOR](./).
{% endhint %}

{% hint style="info" %}
This sample shows how to write [BEHAVIOR](./). BEHAVIORS are used to describe more complex applications.
{% endhint %}

## ACT FEATURES.

* Uses the environmental sensor PAL AMBIENT SENSE PAL to acquire sensor values.
* Use the sleep function to operate with coin cell batteries.



## How to use ACT

### Preparation for TWELITE

| Role | Example                                                                                                                                                                                            |
| -- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Parent Node | [MONOSTICK BLUE or RED](https://mono-wireless.com/jp/products/MoNoStick/)                                                                                                                     |
| Child Node | [BLUE PAL or RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) + [AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |

{% hint style="warning" %}
When using PAL as the Parent Node, coin cell batteries cannot be used. As a rule of thumb, prepare a power supply environment that can provide a stable current of 50 mA or more.
{% endhint %}



## File Structure

* PAL\_AMB-behavior.hpp : Only `setup()` is defined. read DIP-SW and if D1..D3 is upper position, it works as Parent Node, otherwise it sets ID corresponding to DIP SW as Child Node.
* Parent/myAppBhvParent.hpp : behavior class definition for Parent Node
* Parent/myAppBhvParent.cpp : implementation&#x20;
* Parent/myAppBhvParent-handlers.cpp : implementation of handlers
* Parent/myAppBhvParent.hpp : behavior class definition for Child Nodes
* Parent/myAppBhvParent.cpp : implementation
* Parent/myAppBhvParent-handlers.cpp : implementation of handlers

The Parent Node's BEHAVIOR name is `<MY_APP_PARENT>` and the Child Node is `<MY_APP_CHILD>`.

{% hint style="info" %}
Build files can be added by [Makefile description](...) /... /install\_n\_build/makefile.md).
{% endhint %}



## Initialization setup()

```cpp
// now read DIP sw status can be read.
u8ID = (brd.get_DIPSW_BM() & 0x07);

// Register App Behavior (set differnt Application by DIP SW settings)
if (u8ID == 0) {
	// put settings to the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL)  // set channel (pysical channel)
		<< TWENET::rx_when_idle();   // open RX channel

	the_twelite.app.use<MY_APP_PARENT>();
} else {		
	// put settings to the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)

	the_twelite.app.use<MY_APP_CHILD>();
}
```

If the DIP SW reading is 0, register the behavior `<MY_APP_PARENT>` for the Parent Node, otherwise register the behavior `<MY_APP_CHILD>` for the Child Node.

{% hint style="warning" %}
If the Parent Node is MONOSTICK, the DIP SW for PAL reads 0 and behaves as the Parent Node. However, this behavior is not defined in the MONOSTICK specifications.
{% endhint %}



## Parent Node BEHAVIOR

The Parent Node behaves as a non-sleeping receiver and outputs packet information to the serial port when it receives a packet from a Child Node.



### MY\_APP\_PARENT::receive()

```cpp
void MY_APP_PARENT::receive(mwx::packet_rx& rx) {
	uint8_t msg[4];
	uint32_t lumi;
	uint16_t u16temp, u16humid;

	// expand packet payload (shall match with sent packet data structure, see pack_bytes())
	auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end(), msg);
	
	// if PING packet, respond pong!
	if (!strncmp((const char*)msg, (const char*)FOURCHARS, 4)) {
		// get rest of data
		expand_bytes(np, rx.get_payload().end(), lumi, u16temp, u16humid);

		// print them
		Serial << format("Packet(%x:%d/lq=%d/sq=%d): ",
							rx.get_addr_src_long(), rx.get_addr_src_lid(),
							rx.get_lqi(), rx.get_psRxDataApp()->u8Seq)
			   << "temp=" << double(int16_t(u16temp)/100.0)
			   << "C humid=" << double(int16_t(u16humid)/100.0)
			   << "% lumi=" << int(lumi)
			   << mwx::crlf << mwx::flush;
    }
}
```

When a packet is received for the Parent Node, if the first four characters of the packet can be matched (`FOURCHARS`), the contents of the packet are displayed.



### MY\_APP\_PARENT::MWX\_TICKTIMER\_INT()

```cpp
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled) {
  // blink LED
  digitalWrite(PAL_AMB::PIN_LED, 
    ((millis() >> 9) & 1) ? PIN_STATE::HIGH : PIN_STATE::LOW);
}
```

The Parent Node's interrupt handler blinks the LED.



### MY\_APP\_PARENT::MWX\_DIO\_EVENT(PAL\_AMB::PIN\_BTN)

```cpp
MWX_DIO_EVENT(PAL_AMB::PIN_BTN, uint32_t arg) {
	Serial << "Button Pressed" << mwx::crlf;
	
	static uint32_t u32tick_last;
	uint32_t tick = millis();

	if (tick - u32tick_last > 100) {
		PEV_Process(E_ORDER_KICK, 0UL);
	}

	u32tick_last = tick;
}
```

When the button (5) on the PAL is pressed, the `E_ORDER_KICK` event is issued to the state machine.



### MY\_APP\_PARENT::MWX\_STATE(E\_MWX::STATE\_0 .. 3)

The state machine is described as a reference for state transitions and is not meaningful for the operation of the application. It executes state transitions by the E\_ORDER\_KICK event sent from the button, timeouts, and so on.



## BEHAVIOR of Child Node

The behavior flow of the Child Node is the same as that of the PAL\_AMB-usenap. From the initial sleep, "wake up → start sensor operation → short sleep → wake up → acquire sensor value → wireless transmission → wait for wireless transmission completion → sleep" is repeated.



### MY\_APP\_CHILD::on\_begin()

```cpp
void _begin() {
    // sleep immediately.
    Serial << "..go into first sleep (1000ms)" << mwx::flush;
    the_twelite.sleep(1000);
}
```

The `_begin()` function, called from `on_begin()`, executes the first sleep.

(*It is acceptable to describe this process directly in `on_begin()` without describing it in `_begin()` function.)



### MY\_APP\_CHILD::wakeup()

```cpp
void wakeup(uint32_t & val) {
    Serial << mwx::crlf << "..wakeup" << mwx::crlf;
    // init wire device.
    Wire.begin();
    
    // turn on LED
    digitalWrite(PAL_AMB::PIN_LED, PIN_STATE::LOW);

    // KICK it!
    PEV_Process(E_ORDER_KICK, 0); // pass the event to state machine
}
```

This is a description of the process of waking up from sleep.

The first time `Wire.begin()` is executed here, which is redundant for the second and later times when the device wakes from sleep. This process can be moved to `on_begin()`.



### MY\_APP\_CHILD::transmit\_complete()

```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}
```

Processes `E_ORDER_KICK` messages to the state machine upon completion of transmission.



### MY\_APP\_CHILD::transmit\_complete()

```cpp
static const uint8_t STATE_IDLE = E_MWX::STATE_0;
static const uint8_t STATE_SENSOR = E_MWX::STATE_1;
static const uint8_t STATE_TX = E_MWX::STATE_2;
static const uint8_t STATE_SLEEP = E_MWX::STATE_3;
```

Defines the state name.



### MY\_APP\_CHILD::shtc3\_???()

```cpp
MWX_APIRET MY_APP_CHILD::shtc3_start()
MWX_APIRET MY_APP_CHILD::shtc3_read()
```

This is an example of sensor acquisition implementation for SHTC3. For details on sending commands, etc., refer to the SHTC3 datasheet.



### MY\_APP\_CHILD::ltr308als\_???()

```cpp
MWX_APIRET MY_APP_CHILD::ltr308als_read()
MWX_APIRET MY_APP_CHILD::ltr308als_start()
static MWX_APIRET WireWriteAngGet(uint8_t addr, uint8_t cmd)
```

This is an example of LTR308ALS sensor acquisition implementation. Please refer to the LTR308ALS datasheet for details on sending commands, etc.

`WireWriteAndGet()` sends 1 byte of `cmd` to the device of `addr`, then receives 1 byte and returns the value.



### MY\_APP\_CHILD::STATE\_IDLE (0)

```cpp
MWX_STATE(MY_APP_CHILD::STATE_IDLE, uint32_t ev, uint32_t evarg) {
	if (PEV_is_coldboot(ev,evarg)) {
		Serial << "[STATE_IDLE:START_UP(" << int(evarg) << ")]" << mwx::crlf;
		// then perform the first sleep at on_begin().
	} else
	if (PEV_is_warmboot(ev,evarg)) {
		Serial << "[STATE_IDLE:START_UP(" << int(evarg) << ")]" << mwx::crlf;
		PEV_SetState(STATE_SENSOR);
	}
}
```

State 0 has a special meaning. It is the state immediately after startup or after returning from sleep.

Immediately after startup `PEV_is_coldboot(ev,evarg)` judgment becomes `true` and is called. Since it goes straight to sleep from `on_begin()`, it does not contain any code that transitions the state. **At this point, the major initialization has not yet been completed, so complex processing such as sending wireless packets cannot be performed. **In order to perform the first state transition for such processing, send an event from `on_begin()` and perform the state transition according to that event.

After returning from sleep, there is a first call to `PEV_is_warmboot(ev,evarg)` which will be `true`. A call to `PEV_SetState()` will transition to the `STATE_SENSOR` state.



### MY\_APP\_CHILD::STATE\_SENSOR

```cpp
MWX_STATE(MY_APP_CHILD::STATE_SENSOR, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_SENSOR:NEW] Start Sensor." << mwx::crlf;

		// start sensor capture
		shtc3_start();
		ltr308als_start();

		// take a nap waiting finish of capture.
		Serial << "..nap for 66ms" << mwx::crlf;
		Serial.flush();
		PEV_KeepStateOnWakeup(); // stay this state on waking up.
		the_twelite.sleep(66, false, false, TWENET::SLEEP_WAKETIMER_SECONDARY);
	} else
	if (PEV_is_warmboot(ev,evarg)) {
		// on wakeup, code starts here.
		Serial << "[STATE_SENSOR:START_UP] Wakeup." << mwx::crlf;

		PEV_SetState(STATE_TX);
	}
}
```

When the transition is made from `STATE_IDLE` after returning from sleep, the state handler for `STATE_SENSOR` is called continuously. The event `ev` at this time is `E_EVENT_NEW_STATE`.

Here, the operation of two sensors, SHTC3 and LTR308ALS, is started. After a certain period of time, the sensors will be ready to acquire data. This time wait is done with the sleep setting of `66`ms. Note that `PEV_KeepStateOnWakeup()` is called before sleep. After this call, the state after returning from sleep is not `STATE_IDLE`, but the state it was in when it went to sleep, i.e. `STATE_SENSOR`.

When returning from a short sleep, a call is first made with the `PEV_is_warmboot(ev,evarg)` decision set to `true`. At the time of this call, wireless packets can be sent, etc. Transition to `STATE_TX`.



### MY\_APP\_CHILD::STATE\_TX

```cpp
MWX_STATE(MY_APP_CHILD::STATE_TX, uint32_t ev, uint32_t evarg)
	static int u8txid;

	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_TX:NEW]" << mwx::crlf;
		u8txid = -1;

		auto&& r1 = shtc3_read();
		auto&& r2 = ltr308als_read();

		Serial << "..shtc3 t=" << int(i16Temp) << ", h=" << int(i16Humd) << mwx::crlf;
		Serial << "..ltr308als l=" << int(u32Lumi) << mwx::crlf;

		if (r1 && r2) {
			if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

Here, at the time of the `E_EVENT_NEW_STATE` event, the sensor data reading and wireless packet transmission procedures are started. Please refer to other act sample examples for details of the transmission procedure.

```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}

    // ↓　↓　↓ Send a message

} else if (ev == E_ORDER_KICK && evarg == uint32_t(u8txid)) {
		Serial << "[STATE_TX] SUCCESS TX(" << int(evarg) << ')' << mwx::crlf;
		PEV_SetState(STATE_SLEEP);
}
```

Unlike the ACT description in the loop, the process of waiting for the message by `PEV_Process()` from `transmit_complete()` is used as a confirmation of completion. Sleep is performed upon receipt of the message. Sleep processing is done by transitioning to `STATE_SLEEP`.



```cpp
	if (PEV_u32Elaspsed_ms() > 100) {
		// does not finish TX!
		Serial << "[STATE_TX] FATAL, TX does not finish!" << mwx::crlf << mwx::flush;
		the_twelite.reset_system();
	}
```

Finally, timeout processing is performed. This is in case the completion message of the sent packet has not been returned. `PEV_u32Elaspsed_ms()` returns the elapsed time since the transition to that state in milliseconds [ms]. If the time has elapsed, the above will perform a system reset `the_twelite.reset_system()` (assuming this timeout is too much).



### MY\_APP\_CHILD::STATE\_SLEEP

```cpp
MWX_STATE(MY_APP_CHILD::STATE_SLEEP, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "..sleep for 5000ms" << mwx::crlf;
		pinMode(PAL_AMB::PIN_BTN, PIN_MODE::WAKE_FALLING_PULLUP);
		digitalWrite(PAL_AMB::PIN_LED, PIN_STATE::HIGH);
		Serial.flush();

		the_twelite.sleep(5000); // regular sleep
	}
}
```

Perfom sleep. Describe it in `E_EVENT_NEW_STATE` immediately after the transition from the previous state. Since other events may be called just before sleep, be sure to execute `the_twelite.sleep()` in a decision expression that is executed only once.
