---
title: "PAL_MOT-single"
author: "Mono Wireless Inc."
---

# PAL\_MOT-single

This ACT acquires several samples of acceleration data after sleep recovery and sends that data.

{% hint style="success" %}
The ACT of the "Act" includes the following

* Sending and receiving wireless packets
* Configuration through Interactive settings mode - [\<STG\_STD>](../settings/stg\_std.md)
* State transition control by state machine - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MOT>](../boards/pal/pal\_mot.md) or [\<CUE>](../boards/cue.md) board operation with board BEHAVIOR
{% endhint %}



## Explanation of ACT

Wake up → start acquiring accelerometer data → wait for accelerometer FIFO interrupt → retrieve accelerometer data → wireless transmission → sleep.

{% hint style="info" %}
The accelerometer stops adding data to the FIFO queue when the FIFO queue is full.
{% endhint %}



### 宣言部

#### インクルード

```cpp
#include <TWELITE> // MWX library basic
#include <NWK_SIMPLE> // network
#include <SM_SIMPLE> // State machine (state transition)
#include <STG_STD> // Interactive settings mode

/*** board selection (choose one) */
#define USE_PAL_MOT
//#define USE_CUE
// board dependend definitions.
#if defined(USE_PAL_MOT)
#define BRDN PAL_MOT
#define BRDC <PAL_MOT>
#elif defined(USE_CUE)
#define BRDN CUE
#define BRDC <CUE>
#endif
// include board support
#include BRDC
```

To support MOT PAL or TWELITE CUE, the include part is a macro. Define either `USE_PAL_MOT` or `USE_CUE`.

If `USE_PAL_MOT` is defined, the board BEHAVIOR [`<PAL_MOT>`](../boards/pal/pal\_MOT.md) is included.



#### state-defined

```cpp
enum class E_STATE : uint8_t {
	INTERACTIVE = 255,
	INIT = 0,
	START_CAPTURE,
	WAIT_CAPTURE,
	REQUEST_TX,
	WAIT_TX,
	EXIT_NORMAL,
	EXIT_FATAL
};
SM_SIMPLE<E_STATE> step;
```

Define states for sequential processing during `loop()` and also [state machine](../api-reference/classes/smsimple-suttomashin.md)`step` is declared.



#### Sensor data storage

```cpp
struct {
	int32_t x_ave, y_ave, z_ave;
	int32_t x_min, y_min, z_min;
	int32_t x_max, y_max, z_max;
	uint16_t n_seq;
	uint8_t n_samples;
} sensor;
```

Data structure for storing sensor data.



### setup()

```cpp
/// load board and settings objects
auto&& brd = the_twelite.board.use BRDC (); // load board support
auto&& set = the_twelite.settings.use<STG_STD>(); // load save/load settings(interactive mode) support
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>(); // load network support
```

Registers board, configuration, and network behavior objects.



#### Interactive settings mode

```cpp
// settings: configure items
set << SETTINGS::appname("MOT");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set << SETTINGS::lid_default(0x1); // set default LID
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

// if SET=LOW is detected, start with intaractive mode.
if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
	set << SETTINGS::open_at_start();
	brd.set_led(LED_TIMER::BLINK, 300); // slower blink
	step.next(STATE::INTERACTIVE);
	return;
}

// load settings
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.
```

Initialize the Interactive settings mode.

First, adjust the configuration items. Here, we set the title name `SETTINGS::appname` to be displayed in menu items, the default value of Application ID `SETTINGS::appid_default`, the default value of CHANNEL `SETTINGS::ch_default`, the default value of logical device ID default `SETTINGS::lid_default`, and `.hide_items()` for hidden items.

This sample transitions to Interactive settings mode when the SET pin is LO at startup. If `digitalRead(brd.PIN_SET)` confirms that the pin is LO, specify `SETTINGS::open_at_start()`. This specification causes the Interactive settings mode screen to appear immediately after exiting `SETUP()`. When the screen is displayed, `begin()` and `loop()` are executed. In this sample, the state `STATE::INTERACTIVE` is set so that no action such as sleep is performed during `loop()` and nothing is done.

Next, the configuration values are read. To read the configuration values, be sure to execute `.reload()`. In this sample, the option bit setting `.u32opt1()` is read.



#### the\_twelite

```cpp
the_twelite << set;
```

`the_twelite` is a class object that manages the basic behavior of the system. This object performs various initializations such as Application ID and CHANNEL within `setup()`.

Here [some of the settings for Interactive settings mode](../settings/stg\_std.md#no-1) is reflected.

{% hint style="info" %}
If you wish to change an item reflected in the Interactive settings mode settings to another setting, continue with the setting you wish to override.

```cpp
the_twelite << set;// Interactive settings mode
the_twelite << twenet::channel(19); // set ch overwrite to 19
```
{% endhint %}



#### NWK\_SIMPLE Object

```cpp
nwk << set;
```

Settings are also made for the network behavior object. The logical device ID (LID) and retransmission settings in Interactive settings mode are reflected.



#### Other hardware initialization, etc.

```cpp
brd.set_led(LED_TIMER::BLINK, 100);
```

LED blink settings and other settings.



### begin()

```cpp
void begin() { 
	auto&& set = the_twelite.settings.use<STG_STD>();
	if (!set.is_screen_opened()) {
		// sleep immediately, waiting for the first capture.
		sleepNow();
	}
}
```

Called after `setup()` is finished. Here, the first sleep is performed. However, if the screen in Interactive settings mode is displayed, it does not sleep.



### wakeup()

```cpp
void wakeup() {
	Serial << crlf << "--- PAL_MOT(OneShot):" 
	       << FOURCHARS << " wake up ---" << crlf;
	eState = E_STATE::INIT;
}
```

After waking up, the state variable `eState` is set to the initial state INIT. After this, `loop()` is executed.



### loop()

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();

	do {
		switch(step.state()) {
			case STATE::INTERACTIVE:
			break;
		...
	} while(step.b_more_loop());
}
```

The basic structure of `loop()` is `<SM_STATE>` state machine `state` with _switch ... . case_ clause. The initial state is `STATE::INIT` or `STATE::INTERACTIVE`.



#### STATE::INTERACTIVE

This is the state of the Interactive settings mode screen when it is displayed. Nothing is done. Interactive settings mode is used for Serial input and output in this screen.



#### STATE::INIT

INIT in its initial state.

```cpp
case STATE::INIT:
	brd.sns_MC3630.get_que().clear(); // clear queue in advance (just in case).
	memset(&sensor, 0, sizeof(sensor)); // clear sensor data
	step.next(STATE::START_CAPTURE);
break;
```

In state INIT, initialization (clearing the queue for storing results) and initialization of the data structure for storing results is performed. transition to STATE::START\_CAPTURE. After setting this transition, the _while_ loop is executed again.



#### STATE::CAPTURE

```cpp
case STATE::START_CAPTURE:
	brd.sns_MC3630.begin(
		// 400Hz, +/-4G range, get four samples and will average them.
		SnsMC3630::Settings(
			SnsMC3630::MODE_LP_400HZ, SnsMC3630::RANGE_PLUS_MINUS_4G, N_SAMPLES)); 

	step.set_timeout(100);
	step.next(STATE::WAIT_CAPTURE);
break;
```

In the state START\_CAPTURE, the FIFO acquisition of the MC3630 sensor is started. Here, the FIFO interrupt is set to occur when 4 samples are acquired at 400 Hz.

Set timeout for exception handling and transition to the next state `STATE::WAIT_CAPTURE`.



#### STATE::WAIT\_CAPTURE

```cpp
case STATE::WAIT_CAPTURE:
	if (brd.sns_MC3630.available()) {
		brd.sns_MC3630.end(); // stop now!
```

In the state WAIT\_CAPTURE, it waits for a FIFO interrupt. When an interrupt occurs and data is stored in the result storage queue, `sns_MC3630.available()` becomes `true`. Call `sns_MC3630.end()` to terminate the process.



```cpp
sensor.n_samples = brd.sns_MC3630.get_que().size();
if (sensor.n_samples) sensor.n_seq = brd.sns_MC3630.get_que()[0].get_t();
...
```

Get the number of samples and sample sequence numbers.



```cpp
// get all samples and average them.
for (auto&& v: brd.sns_MC3630.get_que()) {
	sensor.x_ave  += v.x;
	sensor.y_ave  += v.y;
	sensor.z_ave  += v.z;
}

if (sensor.n_samples == N_SAMPLES) {
	// if N_SAMPLES == 2^n, division is much faster.
	sensor.x_ave /= N_SAMPLES;
	sensor.y_ave /= N_SAMPLES;
	sensor.z_ave /= N_SAMPLES;
}
...
```

Reads and averages all sample data.



```cpp
// can also be:
//	int32_t x_max = -999999, x_min = 999999;
//	for (auto&& v: brd.sns_MC3630.get_que()) {
//		if (v.x >= x_max) x_max = v.x;
//		if (v.y <= x_min) x_min = v.x;
//		...
//	}	
auto&& x_minmax = std::minmax_element(
	get_axis_x_iter(brd.sns_MC3630.get_que().begin()),
	get_axis_x_iter(brd.sns_MC3630.get_que().end()));
sensor.x_min = *x_minmax.first;
sensor.x_max = *x_minmax.second;
...
```

Here, the maximum and minimum are obtained for the acquired samples using the iterator corresponding to each axis.

{% hint style="info" %}
The `std::mimmax_element` is introduced as an example of using the algorithm of the C++ Standard Template Library, but you can also find the maximum and minimum in the for loop as shown in the comments.
{% endhint %}



```cpp
if (brd.sns_MC3630.available()) {
  ...
  brd.sns_MC3630.get_que().clear(); // clean up the queue
  step.next(STATE::REQUEST_TX); // next state
} else if (step.is_timeout()) {
  Serial << crlf << "!!!FATAL: SENSOR CAPTURE TIMEOUT.";
  step.next(STATE::EXIT_FATAL);
}
break;
```

Call `.sns_MC3630.get_que().clear()` to clear the data in the queue. If this is not called, subsequent sample acquisitions will not be possible. After that, it transits to the `STATE::REQUEST_TX` state.

. `is_timeout()` checks for timeout. If timeout occurs, it transits to `STATE::EXIT_FATAL` as an error.



#### STATE::REQUEST\_TX

```cpp
case STATE::REQUEST_TX:
	if (TxReq()) {
		step.set_timeout(100);
		step.clear_flag();
		step.next(STATE::WAIT_TX);
	} else {
		Serial << crlf << "!!!FATAL: TX REQUEST FAILS.";
		step.next(STATE::EXIT_FATAL);
	}
break;
```

In state `REQUEST_TX`, the locally defined function `TxReq()` is called to process the obtained sensor data and generate and send a transmission packet. The transmission request may fail due to the status of the transmission queue or other factors. If the transmission request succeeds, TxReq()` returns as true, but no transmission is performed yet. The `on_tx_comp()` callback is called to complete the transmission.

Also, `.clear_flag()` clears the flag to indicate that transmission is complete. At the same time, a timeout is set.



#### E\_SATE::WAIT\_TX

```cpp
case STATE::WAIT_TX:
	if (step.is_flag_ready()) {
		step.next(STATE::EXIT_NORMAL);
	}
	if (step.is_timeout()) {
		Serial << crlf << "!!!FATAL: TX TIMEOUT.";
		step.next(STATE::EXIT_FATAL);
	}
break;
```

In state `STATE::WAIT_TX`, it waits for the completion of wireless packet transmission. The flag is set by the `on_tx_comp()` callback function, and `.is_flag_ready()` becomes _true_ after it is set.



#### E\_SATE::EXIT\_NORMAL, FATAL

```cpp
case STATE::EXIT_NORMAL:
	sleepNow();
break;

case STATE::EXIT_FATAL:
	Serial << flush;
	the_twelite.reset_system();
break;
```

When a series of operations is completed, it transits to the state `STATE::EXIT_NORMAL` and calls the locally defined function `sleepNow()` to execute sleep. When an error is detected, it transits to state `STATE::EXIT_FATAL` and performs a system reset.



### MWX\_APIRET TxReq()

```cpp
MWX_APIRET TxReq() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();
	MWX_APIRET ret = false;

	// prepare tx packet
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {		
		// set tx packet behavior
		pkt << tx_addr(0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x1 send two times in total)
			<< tx_packet_delay(0, 0, 2); // send packet w/ delay
		
		// prepare packet (first)
		pack_bytes(pkt.get_payload() // set payload data objects.
				, make_pair(FOURCHARS, 4)  // just to see packet identification, you can design in any.
				, uint16_t(sensor.n_seq)
				, uint8_t(sensor.n_samples)
				, uint16_t(sensor.x_ave)
				, uint16_t(sensor.y_ave)
				, uint16_t(sensor.z_ave)
				, uint16_t(sensor.x_min)
				, uint16_t(sensor.y_min)
				, uint16_t(sensor.z_min)
				, uint16_t(sensor.x_max)
				, uint16_t(sensor.y_max)
				, uint16_t(sensor.z_max)
			);

		// perform transmit
		ret = pkt.transmit();

		if (ret) {
			Serial << "..txreq(" << int(ret.get_value()) << ')';
		}
	}

	return ret;
}
```

The last step is to generate a packet and request that it be sent. The packet should include the continuation number, the number of samples, the average value of XYZ, the minimum sample value of XYZ, and the maximum sample value of XYZ.



### sleepNow()

```cpp
void sleepNow() {
	Serial << crlf << "..sleeping now.." << crlf;
	Serial.flush();
	step.on_sleep(false); // reset state machine.
	the_twelite.sleep(3000, false); // set longer sleep (PAL must wakeup less than 60sec.)
}
```

Sleep procedure.

* Serial ports should call `Serial.flush()` to output all before sleep.
* The `<SM_SIMPLE>` state machine must do `on_sleep()`.







