---
title: "BRD_ARIA"
author: "Mono Wireless Inc."
description: TWELITE ARIA - TWELITE ARIA is used to acquire sensor values.
---
# BRD\_ARIA

TWELITE ARIA - TWELITE ARIA is used to acquire sensor values.

{% hint style="success" %}
This ACT includes

* Transmission of wireless packets
* Interactive settings mode settings - [\<STG\_STD>](../settings/stg\_std.md)
* State transition control by state machine - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<ARIA>](../boards/aria.md)Board operation with board behavior
{% endhint %}



## ACT Features

* Acquire sensor values using TWELITE ARIA - Twilight Aria.
* Use the sleep function to operate with coin cell batteries.

## how to use act

### Required TWELITE

| Role | Example                                                                                                                                                            |
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Parent Node | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUE or RED</a><br>Act <a href="parent_monostick.md">Parent\_MONOSTICK</a> to work. |
| Child Node | [TWELITE ARIA BLUE or RED](https://mono-wireless.com/jp/products/twelite-aria/index.html)                                                                    |


## Explanation of ACT

### Include

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>// network support
#include <ARIA> // TWELITE ARIA
#include <STG_STD> // Interactive settings mode
#include <SM_SIMPLE> // Simple State Machine
```

Include board BEHAVIOR of TWELITE ARIA[`<ARIA>`](../boards/aria.md).

### setup()

```cpp
void setup(){
	/*** SETUP section */
	/// init vars or objects
	step.setup(); // initialize state machine
	
	/// load board and settings objects
	auto&& brd = the_twelite.board.use<ARIA>(); // load board support
	auto&& set = the_twelite.settings.use<STG_STD>(); // load save/load settings(interactive mode) support
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>(); // load network support

	/// configure settings
	// configure settings
	set << SETTINGS::appname("ARIA");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

	// if SET=LOW is detected, start with intaractive mode.
	if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
		set << SETTINGS::open_at_start();
		step.next(STATE::INTERACTIVE);
		return;
	}

	// load values
	set.reload(); // load from EEPROM.
	OPT_BITS = set.u32opt1(); // this value is not used in this example.
	
	LID = set.u8devid(); // set logical ID

	/// configure system basics
	the_twelite << set; // apply settings (from interactive mode)

	/// configure network
	nwk << set; // apply settings (from interactive mode)
	nwk << NWK_SIMPLE::logical_id(LID); // set LID again (LID can also be configured by DIP-SW.)	

	/// configure hardware
	// LED setup (use periph_led_timer, which will re-start on wakeup() automatically)
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
	
	// let the TWELITE begin!
	the_twelite.begin();

	/*** INIT message */
	Serial << "--- ARIA:" << FOURCHARS << " ---" << mwx::crlf;
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


The first step is to initialize variables, etc. Here we are initializing the state machine step.

First we register the board support [`<ARIA>`](../boards/aria.md) is registered first. When initializing the board support, sensors and DIOs are initialized.

```cpp
auto&& brd = the_twelite.board.use<ARIA>();
```

The next step is to initialize and read out the Interactive settings mode.

```cpp
// configure settings
set << SETTINGS::appname("ARIA");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

// if SET=LOW is detected, start with intaractive mode.
if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
	set << SETTINGS::open_at_start();
	step.next(STATE::INTERACTIVE);
	return;
}

// load values
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.

LID = set.u8devid(); // set logical ID
```

Here we retrieve the SET object, reflect the application name, reflect the default Application ID and communication channel, and delete unnecessary items in the settings menu.

Next, the SET pin status is read out. Since this sample performs intermittent operation by sleep, Interactive settings mode transition by +++ input is not possible. Instead, the sample transitions to Interactive settings mode with the SET pin = LO state at startup. In this case, `SETTINGS::open_at_start()` is specified, which means that the interactive mode screen will be displayed as soon as `setup()` is finished.

Finally, `.reload()` is executed to read the set values from EEPROM. The configuration values are copied to each variable.



Since this ACT exclusively transmits wireless packets, the TWENET configuration does not include a specification (`TWENET::rx_when_idle()`) to open the receive circuit during operation.

```
the_twelite << set; // apply settings (from interactive mode)
```


Next, configure the LED settings. (In an application that sleeps and wakes for a short period of time, this is almost the same as setting the LED to turn on during wake-up.)

```cpp
brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



### loop()

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<ARIA>();

	do {
		switch (step.state()) {
		 // behavior of each state
		case STATE::INIT:
		...
		break;
		...
		}
	while(step.b_more_loop());
}	
```

The `loop()` is controlled by the [SMSMSIMPLE_SIMPLE state machine](../api-reference/classes/smsimple-suttomashin.md)`step` for control. This is to concisely represent the sequence of events from sleep recovery, sensor value acquisition, wireless packet transmission, waiting for transmission to complete, and sleep. In the combat of the loop, a `brd` object is acquired.



#### case STATE::INTERACTIVE:

It is not convenient to have the main loop running during Interactive settings mode, so it is fixed in this state.



#### case STATE::INIT:

```cpp
brd.sns_SHT4x.begin();

step.next(STATE::SENSOR);
```

Start sensor data acquisition.


#### case STATE::SENSOR:

```cpp
//  wait until sensor capture finish
if (!brd.sns_SHT4x.available()) {
	brd.sns_SHT4x.process_ev(E_EVENT_TICK_TIMER);
}else{ // now sensor data is ready.
	sensor.i16temp = brd.sns_SHT4x.get_temp_cent();
	sensor.i16humid = brd.sns_SHT4x.get_humid_per_dmil();

	// read magnet sensor
	sensor.b_north = digitalRead(ARIA::PIN_SNS_NORTH);
	sensor.b_south = digitalRead(ARIA::PIN_SNS_SOUTH);

	Serial << "..finish sensor capture." << mwx::crlf
		<< "  MAGnet   : north=" << int(sensor.b_north) << mwx::crlf
		<< "             south=" << int(sensor.b_south) << mwx::crlf
		<< "  SHT4x    : temp=" << div100(sensor.i16temp) << 'C' << mwx::crlf
		<< "             humd=" << div100(sensor.i16humid) << '%' << mwx::crlf
		;
	Serial.flush();

	step.next(STATE::TX);
}
```

The sensors on the board are accessible as `.sns_SHT4x` and operate on this object. Wait for the completion of the sensor. If the sensor has not yet been acquired (`.available()` is `false`), a time elapsed event (`.process_ev(E_EVENT_TICK_TIMER)`) is sent to the sensor.

When the sensor is available, the sensor value is acquired and a transition is made to STATE\_TX.

The temperature and humidity sensor can be acquired as follows

* `.get_temp_cent()` : `int16_t` : 1°C as 100 (2560 for 25.6°C)
* `.get_temp()` : `float` : Float value (25.6 for 25.6 °C)
* `.get_humid_dmil()` : `int16_t` : humidity with 1% as 100 (5680 for 56.8%)
* `.get_temp()` : `float` : Float value (56.8 for 56.8%)



#### case STATE::TX:

The transmission procedure is the same as in the other ACT samples. Here, the settings are set to minimize one retransmission and retransmission delay.

```cpp
pkt << tx_addr(0x00)  // Parent Node 0x00
	<< tx_retry(0x1)    // 1 retry
	<< tx_packet_delay(0, 0, 2); // Delay is minimal
```

The identifier `FOURCHARS` and the sensor data are stored in the payload part of the packet. Of the values obtained, the temperature value is `int16_t`, but is cast to `uint16_t` because the data structure of the outgoing packet is to be stored unsigned.

```cpp
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(FOURCHARS, 4)  // just to see packet identification, you can design in any.
	, uint8_t(sensor.b_north)
	, uint8_t(sensor.b_south)
	, uint16_t(sensor.i16temp)
	, uint16_t(sensor.i16humid)
);
```

Requests transmission. If the send request succeeds, prepare the send completion city. Specify `.clear_flag()` to wait for the completion event and `set_timeout(100)` for timeout in case of emergency. The unit of 100 in the parameter is milliseconds [ms].

```cpp
// do transmit
MWX_APIRET ret = pkt.transmit();

if (ret) {
	step.clear_flag(); // waiting for flag is set.
	step.set_timeout(100); // set timeout
	step.next(STATE::TX_WAIT_COMP);
}
```



#### case STATE::TX\_WAIT\_COMP:

This section determines timeouts and transmission completion events.

```cpp
if (step.is_timeout()) { // maybe fatal error.
	the_twelite.reset_system();
}
if (step.is_flag_ready()) { // when tx is performed
	Serial << "..transmit complete." << mwx::crlf;
	Serial.flush();
	step.next(STATE::GO_SLEEP);
}
```



#### STATE::GO\_SLEEP:

Processes `sleepNow()`.



### on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}
```

This is a system event called when transmission is complete. Here, `.set_flag()` is used to indicate completion.



### sleepNow()

This section contains a collection of procedures for going to sleep.

```cpp
void sleepNow() {
	step.on_sleep(false); // reset state machine.

	// randomize sleep duration.
	uint32_t u32ct = 1750 + random(0,500);

	// set an interrupt for MAGnet sensor.
	pinMode(ARIA::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
	pinMode(ARIA::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);

	// output message
	Serial << "..sleeping " << int(u32ct) << "ms." << mwx::crlf;
	Serial.flush(); // wait until all message printed.
	
	// do sleep.
	the_twelite.sleep(u32ct);
}
```

Initialize the state of the state machine by `.on_sleep(false)` before sleep. The parameter `false' starts from `STATE::INIT(=0)` after returning from sleep.

Here, the time until wake-up is set between 1750 ms and 2250 ms by a random number. This avoids continuous collisions with packets from other devices transmitting at similar periods.

{% hint style="info" %}
If the cycles are exactly the same, packets from each other will collide and communication will be difficult. Usually, the timer cycles shift with each other over time, so that communication is restored after a short period of time, and then collisions occur again after another period of time.
{% endhint %}

In lines 8 and 9, before going to sleep, the DIO pins of the magnetic sensor are set to interrupt. The second parameter is `PIN_MODE::WAKE_FALLING`. This is a setting to wake up when the pin state changes from HIGH to LOW.

Lines 11 and 12, this example waits for output from the serial port to go to sleep. Normally, we want to minimize energy consumption, so the output from the serial port before sleep is minimal (or none).

Line 12, to enter sleep, call `the_twelite.sleep()`. In this call, the pre-sleep procedures of the hardware on the board are performed. For example, LEDs are turned off.

The sleep time is specified in ms as a parameter.

{% hint style="danger" %}
TWELITE ARIA must always wake up once within 60 seconds to reset the watchdog timer. The sleep time must be specified not to exceed `60000`.
{% endhint %}



### wakeup()

When the program wakes up from sleep, `wakeup()` is called. After that, `loop()` is called each time. Before `wakeup()`, each peripheral such as UART and devices on the board are woken up. For example, it restarts the LED lighting control.

```cpp
void wakeup() {
    Serial	<< mwx::crlf
        	<< "--- ARIA:" << FOURCHARS << " wake up ";

    if (the_twelite.is_wokeup_by_wktimer()) {
        Serial << "(WakeTimer) ---";
    } else 
    if (the_twelite.is_wokeup_by_dio(ARIA::PIN_SNS_NORTH)) {
        Serial << "(MAGnet INT [N]) ---";
    } else 
    if (the_twelite.is_wokeup_by_dio(ARIA::PIN_SNS_SOUTH)) {
        Serial << "(MAGnet INT [S]) ---";
    } else {
        Serial << "(unknown source) ---";
    }

	Serial  << mwx::crlf
			<< "..start sensor capture again."
			<< mwx::crlf;
}
```



