---
title: "Slp_Wk_and_Tx"
author: "Mono Wireless Inc."
---
# Slp\_Wk\_and\_Tx

Slp\_Wk\_and\_Tx is a template source code for an application which, after a regular wake-up, does something (e.g. acquires sensor data) and sends the result as a wireless packet.

In the form of `setup()` and `loop()`, conditional branches which are difficult to read in `loop()` tend to occur. In this Act, we use [SM\_SIMPLE state machine](../api-reference/classes/smsimple-suttomashin.md) in `loop()` and simple state transition by switch syntax to improve the visibility of the code.

{% hint style="success" %}
This act includes

* The control structure of a typical intermittent operation (sleep -> wake -> measure -> radio transmission -> sleep)
* Generation and transmission procedures for outgoing packets and waiting for completion
{% endhint %}



## ACT features.

* After starting up, the system goes through an initialization process and then goes to sleep.
  1. `setup()`Initialise
  2. `begin()` Run sleep
* After waking up from sleep, the state variables are initialized and the actions are performed in the following order
  1. `wakeup()` wakes up from sleep, performs each initialization
  2. `loop()/INIT->WORK_JOB state`: does some processing (in this Act, the counter is updated every TickCount for 1ms and moves to TX state after a count determined by a random number)
  3. `loop()/TX state`: Make a request to send
  4. `loop()/WAIT_TX state`: Waiting for transmission completion
  5. `loop()/EXIT_NORMAL state`: Run sleep (back to 1.)
* `loop()/EXIT_FATAL state` :Resetting the module if an error occurs

## Description of the ACT

### Declarations

#### Includes

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <SM_SIMPLE>

#include "Common.h"
```

To send packets, `<NWK_SIMPLE>` is included. Also, basic definitions such as application ID are in `"Common.h"`.



#### State definition.

In order to describe the sequential processing in `loop()`, this sample uses the concept of a state machine (state transition). It uses `<SM_SIMPLE>`, which summarizes the processing of very simple state transitions.

An enum `STATE` is defined in `Common.h` for the following states.

```cpp
enum class STATE {
    INIT = 0,    // INIT STATE
    WORK_JOB,    // do some job (e.g sensor capture)
    TX,          // reuest transmit
    WAIT_TX,     // wait its completion
    EXIT_NORMAL, // normal exiting.
    EXIT_FATAL   // has a fatal error (will do system reset)
};
```

Declares an [SM\_SIMPLE state machine](../api-reference/classes/smsimple-suttomashin.md) (state transition) using the `STATE` state enumeration.

```cpp
SM_SIMPLE<STATE> step;
```

The `step` declared here contains functions for managing state, timeouts and waiting for processing.



#### Sensor data.

In this sample we do not process the sensor data, but we prepare dummy data for explanation.

```cpp
struct {
	uint16_t dummy_work_ct_now;
	uint16_t dummy_work_ct_max;  // counter for dummy work job. 
} sensor;
```



### setup()

```cpp
void setup() {
	/*** SETUP section */
	step.setup(); // init state machine
	
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle(false);  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(DEVICE_ID); // set Logical ID. 

	/*** BEGIN section */
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- Sleep an Tx Act ---" << crlf;
}
```

Initializes variables and class objects.

* Initialization of the `step` state machine
* Initialization of `the_twelite` class object
* Register and initialize the network `<NWK_SIMPLE>` (register `DEVICE_ID`).



This is followed by the initiation of class objects and hardware.

```cpp
the_twelite.begin(); // start twelite!
```

This is the procedure to start `the_twelite`, it didn't appear in act0..4, but you should call it if you set up `the_twelite` or register various behaviors.



### begin()

```cpp
void begin() {
	Serial << "..begin (run once at boot)" << crlf;
	SleepNow();
}
```

Called only once, immediately after `setup()`. The `SleepNow()` function is called to perform the first sleep procedure.



### wakeup()

```cpp
void wakeup() {
　memset(&sensor, 0, sizeof(sensor));
	Serial << crlf << int(millis()) << ":wake up!" << crlf;
}
```

Called immediately after waking up. Here, it initializes the sensor data area and outputs a message on waking.



### loop()

```cpp
void loop() {
	do {
		switch(step.state()) {
		case STATE::INIT:
			sensor.dummy_work_ct_now = 0;
			sensor.dummy_work_ct_max = random(10,1000);
			step.next(STATE::WORK_JOB);
		break;

		...
		}
	} while (step.b_more_loop());
}

```

The above code is a simplified version of the actual code.

This control structure uses the [SM\_SIMPLE state machine](../api-reference/classes/smsimple-suttomashin.md). It is a loop with _`do..while()`_ syntax. Inside the loop is a _`switch case` _ clause, which splits the process according to the state obtained by `.state()`. State transitions call `.next()` to rewrite internal variables in the state machine to the new state values.

`step.b_more_loop()` is set to true if there is a state transition by `.next()`. The purpose of this is to execute the code of the next state (_case clause_) without escaping `loop()` when a state transition occurs.

Below is a description of each state.

#### STATE::INIT

```cpp
sensor.dummy_work_ct_now = 0;
sensor.dummy_work_ct_max = random(10,1000);

step.next(STATE::WORK_JOB);
```

Initialises the sensor values of the dummies. One is determined randomly by an add counter and one by a counter stop value.



#### STATE::WORK\_JOB

```cpp
if (TickTimer.available()) {
	Serial << '.';
	sensor.dummy_work_ct_now++;
	if (sensor.dummy_work_ct_now >= sensor.dummy_work_ct_max) {
		Serial << crlf;
		step.next(STATE::TX);
	}
}
```

In the WORK\_JOB state, we work on a timer every 1ms; every Tick timer becomes `TickTimer.available()`; every Tick timer adds a counter and when it reaches `dummy_work_ct_max`, we move to the next state `STATE::TX` .



#### STATE::TX

```cpp
if (Transmit()) {
	Serial << int(millis()) << ":tx request success!" << crlf;
	step.set_timeout(100);
	step.clear_flag();
	step.next(STATE::WAIT_TX);
} else {
	// normall it should not be here.
	Serial << int(millis()) << "!FATAL: tx request failed." << crlf;
	step.next(STATE::EXIT_FATAL);
}
```

Call the `Transmit()` function to request packet transmission. If the request succeeds, the function transits to `STATE::WAIT_TXEVENT` and waits for the completion of transmission. Here, we use the timeout and flag functions of the SM\_SIMPLE state machine to wait for completion (it is simple to determine the value of a variable by changing its value in the waiting loop).

We don't usually expect a single request to fail, but if it does, it will go to the `STATE::EXIT_FATAL` exception handling state.

{% hint style="warning" %}
You should not sleep at this point, as the packet has not yet been sent. In most cases, you should **wait for transmission to complete** before continuing.
{% endhint %}

{% hint style="info" %}
The `Transmit()` function returns a `MWX_APIRET` object, which holds a bool type success or failure and a value of up to 31 bits. It can be evaluated as a `bool` type, so an `if` statement will return `true` if the send request succeeds, or `false` if it fails.
{% endhint %}



#### STATE::WAIT\_TX

```cpp
if (step.is_flag_ready()) {
	Serial << int(millis()) << ":tx completed!" << crlf;
	step.next(STATE::EXIT_NORMAL);
} else if (step.is_timeout()) {
	Serial << int(millis()) << "!FATAL: tx timeout." << crlf;
	step.next(STATE::EXIT_FATAL);
}
```

Waiting for completion of transmission is judged by setting the flag of the state machine function with `on_tx_comp()` described later. The timeout is judged by the elapsed time since `.set_timeout()` was done by calling `.is_timeout()`.

Normally you will get a completion notice whether the transmission succeeds or fails, but it will time out and go to `STATE::EXIT_FATAL` for exception handling.



#### STATE::EXIT\_NORMAL

```
SleepNow();
```

Call `SleepNow()` to start the sleep process.



#### STATE::EXIT\_FATAL

```cpp
Serial << crlf << "!FATAL: RESET THE SYSTEM.";
delay(1000); // wait a while.
the_twelite.reset_system();
```

As a critical error, a system reset is performed.



### SleepNow()

```cpp
void SleepNow() {
	uint16_t u16dur = SLEEP_DUR;
	u16dur = random(SLEEP_DUR - SLEEP_DUR_TERMOR, SLEEP_DUR + SLEEP_DUR_TERMOR);

	Serial << int(millis()) << ":sleeping for " << int(u16dur) << "ms" << crlf;
	Serial.flush();

	step.on_sleep(); // reset status of statemachine to INIT state.

	the_twelite.sleep(u16dur, false);
}
```

Periodic sleep is performed. The sleep time is set to a constant time blur using the `random()` function. This is because when the transmission cycles of several devices are synchronized, the failure rate may increase significantly.

Before sleeping, the state of the SM\_SIMPLE state machine is set by calling `.on_sleep()`.



### Transmit()

```cpp
MWX_APIRET vTransmit() {
	Serial << int(millis()) << ":vTransmit()" << crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x3 send four times in total)
			<< tx_packet_delay(0,0,2); // send packet w/ delay (send first packet with randomized delay from 0 to 0ms, repeat every 2ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(FOURCC, 4) // string should be paired with length explicitly.
			, uint32_t(millis()) // put timestamp here.
			, uint16_t(sensor.dummy_work_ct_now) // put dummy sensor information.
		);
		
		// do transmit 
		//return nwksmpl.transmit(pkt);
		return pkt.transmit(); 
	}

	return MWX_APIRET(false, 0);
}
```

This function requests to send a wireless packet to the parent device with ID=`0x00`. The data to be stored is a four-character identifier (`FOURCC`) commonly used in the Act sample, plus the system time \[ms] and a dummy sensor value (`sensor.dummy_work_ct_now`).

The first step is to get an object that contains the transmitted packets. This object can then be manipulated to set the data and conditions for transmission.

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

In the mwx library, the `if` statement is used to get an object and the `bool` decision of the object is `true`. Here, a board object is retrieved by `the_twelite.network.use<NWK_SIMPLE>()`, and a packet object is retrieved by `.prepare_tx_packet()` of the board object. Failure to retrieve the packet object is not normally expected, but when it does occur, it is when the transmit queue is full and the transmit request cannot be accepted. Since this example is for a single transmission only, the error is limited to an unexpected serious problem.

```cpp
pkt << tx_addr(0x00) // Destination
		<< tx_retry(0x1) // Number of resends
		<< tx_packet_delay(0,0,2); // Transmission delay
```

For the resulting `pkt` object, set the conditions for transmission (destination, retransmission, etc.) using the `<<` operator. [`tx_addr`](../api-reference/classes/packet\_tx.md#tx\_addr) specifies the destination of the packet. The `tx_addr` specifies the destination of the packet, the [`tx_retry`](../api-reference/classes/packet\_tx.md#tx\_retry) specifies the number of retransmissions, and the [`tx_packet_delay`](../api-reference/classes/packet\_tx.md#tx\_packet\_delay) specifies the transmission delay.



```cpp
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(FOURCC, 4) // string should be paired with length explicitly.
	, uint32_t(millis()) // put timestamp here.
	, uint16_t(sensor.dummy_work_ct_now) // put dummy sensor information.
);	
```

The payload of a packet is an array of [`smblbuf<uint8_t>`](../api-reference/classes/smplbuf/) derivatives obtained by `pkt.get_payload()`. You can set the value of this array directly, but here we use [`pack_bytes()`](../api-reference/funcs/utility/pack\_bytes.md) to set the value.

{% hint style="info" %}
The maximum length of the payload is 91 bytes in the above example, see[ NWK\_SIMPLE packet structure and maximum length](../networks/nwk\_simple.md#pakettoto) for more information.
{% endhint %}

This function can be specified by a variable number of arguments. The first parameter is an array object obtained from `.get_payload()`.

* `make_pair(FOURCC,4)` : _make\_pair_ is from the C++ standard library and creates a _std::pair object_. It means to write out 4 bytes from the beginning for string type.(This is done to explicitly specify the number of bytes to be written, as the topic of including or excluding the end of an array of string types is confusing)
* If `uint32_t` type data is specified, 4 bytes of data are written in big-endian order.
* The same applies to data of type `uint16_t`.

{% hint style="info" %}
It is also possible to write data using a pointer of type `uint8_t`.

```cpp
auto&& pay = pkt.get_payload(); // get buffer object.

// the following code will write data directly to internal buffer of `pay' object.
uint8_t *p = pay.begin(); // get the pointer of buffer head.

S_OCTET(p, FOURCC[0]); // store byte at pointer `p' and increment the pointer.
S_OCTET(p, FOURCC[1]);
S_OCTET(p, FOURCC[2]);
S_OCTET(p, FOURCC[3]);

S_DWORD(p, millis()); // store uint32_t data.
S_WORD(p, sensor.dummy_work_ct_now); // store uint16_t data.

pay.redim(p - pay.begin());
```

The array object obtained from `.get_payload()` is an array of size 0 with no data stored in it, but it is expanded by writing data to it (actually, writing data to an internal fixed-length buffer and updating the data size in internal management). The final size is the data size of the payload.

Here we use `.begin()` to get a pointer to `uint8_t*`, write data using this pointer, and set the last written size with `.redim()`.

functions (macros) such as `S_OCTET()`, `S_WORD()`, and `S_DWORD()` are used to write data, for example, `S_OCTET(p, 'H')` is the same as `*p = 'H'; p++;`.

The last `.redim()` is a procedure to change the size of an array **without initializing the buffer**. Calling `.resize()` clears everything to zero.
{% endhint %}



Finally, `.transmit()` is called to request sending. The return value is of the type `MWX_APIRET`. After the request, the actual transmission takes place, which may take several to several tens of milliseconds to complete, depending on the transmission parameters and the size of the transmission. On completion, `on_tx_comp()` is called.

```cpp
return pkt.transmit(); 
```

{% hint style="info" %}
``[`MWX_APIRET`](../api-reference/classes/mwx\_apiret.md) is a wrapped class of `uint32_t` type, which uses MSB as a failure success flag and 31 bits as data. It is a return type of `pkt.transmit()`, and success or failure of the request (cast to `bool` type) and the ID are stored in the data part (`.get_value()`).
{% endhint %}



### on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}
```

This is a system event that is called when the transmission is complete. Here, it is set to complete by `.set_flag()`.
