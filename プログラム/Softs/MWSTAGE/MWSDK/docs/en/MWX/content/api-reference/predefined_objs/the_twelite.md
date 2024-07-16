---
description: The core class object of TWENET (mwx::twenet)
---

# the\_twelite

The `the_twelite` object summarizes the procedures for using TWENET and includes procedures for operating the wireless microcontroller, such as basic wireless settings, sleep, and other procedures.



## summary.

`the_twelite` is set up in the `setup()` function, which also calls `the_twelite.begin()` to start the process. It is not possible to set up `the_twelite` except in `setup()`.

```cpp
void setup() {
  ...
 	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();  
  ...
  the_twelite.begin();
}
```

In the above example, the application ID is set, the communication channel is set, and the receiving circuit is set.



Various procedures are included.

```cpp
// Get a serial number
uint32_t u32hwser = the_twelite.get_hw_serial();

// Set channel to 11
the_twelite.change_channel(11);

// Sleep for 1 second.
the_twelite.sleep(1000);

// Perform a reset
the_twelite.reset_system();
```



It is also possible to register classes that handle wireless networks, classes that summarize board support, and classes that perform event-driven processing described by the user. By registering these classes, dedicated functions can be easily used. These classes are referred to as "[behaviors](the\_twelite.md#biheibia)" in this explanation.

```cpp
void setup() {
	/*** SETUP section */
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_AMB>();
	
	...
	
	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID);

```

In the above example, two types of behaviors are registered: environmental sensor pal behavior `<PAL_AMB>` and simple relay network `<NWK_SIMPLE>`. By registering these, hardware such as sensors on the environmental sensor pal can be easily handled. In addition, the complicated handling of wireless packets can be implicitly provided with functions such as relay processing and automatic discarding of duplicate packet arrivals.



## Methods

{% hint style="warning" %}
The MWX library defines other methods in addition to those introduced here.

Act descriptions include those that are not directly related to the act description, those that are set up but do not function effectively, and those that are used internally.
{% endhint %}



### `<<operator` (setting)

The `<<` operator is used to initialize the object `the_twelite`.

The following configuration class objects are used as input, and default values are applied if no configuration is made.

#### TWENET::appid(uint32\_t id)

Set the parameter `id` to the specified application ID. This is a required specification.

Reading the settings is done with `uint32_t the_twelite.get_appid()`.

#### TWENET::channel(uint8\_t ch)

Set the specified channel number (`11`..`26`) in parameter `ch`.

Reading the settings is done with `uint8_t` `the_twelite.get_channel()`.

#### TWENET::tx\_power(uint8\_t pw)

Parameter `pw` is set to (`0`. `3`) for the radio output. The default is (3: no output attenuation).

Reading the settings is done with `uint8_t the_twelite.get_tx_power()`.

#### TWENET::rx\_when\_idle(uint8\_t bEnable)

If the parameter `bEnable` is `1`, the receiver circuit is always activated and can receive radio packets from others. The default is `0`, which is dedicated to transmission only.

Reading the settings is done with `uint8_t the_twelite.get_rx_when_idle()`.

#### TWENET::chmgr(uint8\_t ch1 = 18, uint8\_t ch2 = 0, uint8\_t ch3 = 0)

Enables the channel manager. If multiple channels are specified, sending/receiving is performed on multiple channels; if 0 is specified for ch2 and ch3, the specification is disabled.

#### STG\_STD

Reflects the setting [values in interactive mode](../../settings/stg\_std.md#no-1).

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
set.reload();       // Load settings
the_twelite << set; // Reflects interactive mode settings
```

The items that will be reflected are as follows

* **app\_id**
* **channel**
* **tx\_power**
* Number of retransmissions when MAC ack is used



{% hint style="warning" %}
There are other settings in the MWX library code that are irrelevant to the library's functionality at this time or that may cause conflicts if set.
{% endhint %}



### begin()

```cpp
void begin()
```

It is executed after preconfiguration (see `<<` operator) and registration of the behavior. It is usually placed at the end of the `setup()` function.

* `the_twelite` finished setting up
* Behavior initialization

{% hint style="warning" %}
TWENET initialization is also performed after the `setup()` function exits. Since many processes are to be executed after `setup()` exits, do not do anything other than initialization here.
{% endhint %}

#### 例

```cpp
void setup() {
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_AMB>();
	
	// settings
 	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();  
	
	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID);
	
	// somo others
	
	// begin the TWENET!
	the_twelite.begin();
}
```



### change\_channel()

```cpp
inline bool change_channel(uint8_t u8Channel)
```

Changes channel settings. On failure, the channel is not changed and `false` is returned.



### get\_channel\_phys()

```cpp
uint8_t get_channel_phys()
```

Obtains the currently set channel number (11..26) from the MAC layer API.



### get\_hw\_serial()

```cpp
inline uint32_t get_hw_serial()
```

Get the module serial number.



### sleep()

```cpp
inline void sleep(
        uint32_t u32Periodms, 
        bool_t bPeriodic = true, 
        bool_t bRamOff = false, 
        uint8_t u8Device = TWENET::SLEEP_WAKETIMER_PRIMARY)
```

Put the module to sleep.

| parameter     | explanation                                                                                                                                                                 |
| ------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `u32Periodms` | Sleep duration\[ms]                                                                                                                                                         |
| `bPeriodic`   | <p>Recalculate the next wake-up time based on the previous wake-up time.<br>※It may be from the current timing for reasons such as the next wake-up timing is imminent.</p> |
| `bRamoff`     | Set to `true` to sleep without retaining RAM (must be reinitialized from `setup()` instead of `wakeup()` after wakeup)                                                      |
| `u8Device`    | <p>Designation of a wake-up timer to be used for sleep. Specify</p><p><code>TWENET::SLEEP_WAKETIMER_PRIMARY</code>or<code>TWENET::SLEEP_WAKETIMER_SECONDARY</code>.</p>     |



{% hint style="info" %}
Before sleep, the `on_sleep()` method of the built-in object or behavior is called to perform the pre-sleep procedure. After returning from sleep, the opposite procedure is performed by the `on_wakeup()` method.
{% endhint %}



### is\_wokeup\_by\_dio()

```cpp
bool is_wokeup_by_dio(uint8_t port)
```

Returns `true` if the return factor from sleep is the specified digital pin.



### is\_wokeup\_by\_wktimer()

```cpp
bool is_wokeup_by_wktimer()
```

Returns `true` if the wake-up timer is the wake-up factor for returning from sleep.



### reset\_system()

```cpp
inline void reset_system()
```

Resets the system. After reset, the process starts from `setup()`.



### stop\_watchdog()

```cpp
inline void stop_watchdog()
```

Stops the watchdog timer. Stop the timer if you are going to wait for polling for a long time.

{% hint style="info" %}
The watchdog timer is restarted each time in the main loop inside the library. It takes about 4 seconds for the timer to run out and reset.
{% endhint %}



### restart\_watchdog()

```cpp
inline void restart_watchdog()
```

Restart the watchdog timer.



## Behavior.

Three behaviors can be registered with `the_twelite`, and the following class objects are defined to store them.

* `network` :  A behavior that implements a network. Normally, [`<NWK_SIMPLE>`](../../networks/nwk\_simple.md) is registered.
* `network2` : BEHAVIOR that implements networking. Use this behavior if you want another network BEHAVIOR to process packets which were not accepted by the first `network` due to the data structure of the payload or other reasons. ([Reference: Using NWK\_LAYERED and NWK\_SIMPLE together](../../networks/nwk_layered.md))
* `board`: Behaviors for board support. Procedures for using each device on the board are added.
* `app`: Behaviors describing user applications. Behaviors can be written in terms of interrupts, events, and state transitions using state machines. It is also easy to define multiple application descriptions and select an application with completely different behavior at startup.
* `settings`: Behavior for performing configuration (interactive mode). Registers [`<STG_STD>`](../../settings/stg\_std.md).



### use\<B>()

```cpp
// example
auto&& brd = the_twelite.board.use<PAL_AMB>();
```

Register behavior **.** Registration is done within [`setup()`](../functions/setup.md). The return value is a reference to the object corresponding to the registered behavior.

After registration, the object is retrieved in the same way as at the time of registration.

{% hint style="danger" %}
If an incorrect behavior is specified, a panic operation (infinite loop) occurs and program operation stops.
{% endhint %}

{% hint style="info" %}
It is not intended to declare objects of the behavior as global variables. Please use `use<B>()` for each use.

```cpp
void loop() {
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
}
```

However, it is possible to define a pointer to an object in a global variable and write as follows (The MWX library has a policy of minimizing the use of pointer types and using reference types, so the following description is not recommended.)

```cpp
NWK_SIMPLE *pNwk = nullptr;

setup() {
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	pNwk = &nwk;
}

void transmit() {
  if (auto&& pkt = pNwk->prepare_tx_packet()) {
    ...
  }
}
```
{% endhint %}



## class object.

`the_twelite` defines the three class objects `board`, `network`, and `app` mentioned above, but also the following

### tx\_status

Notification of transmission completion status.

{% hint style="info" %}
The description of the event-driven behavior is managed by the `transmit_complete()` callback.
{% endhint %}



#### is\_complete()

```cpp
bool is_complete(uint8_t cbid)
```

Returns `true` when the packet with the specified ID has completed transmission.



#### is\_success()

```cpp
bool is_success(uint8_t cbid)
```

Returns `true` when the packet with the specified ID has completed transmission and has been successfully sent.



### receiver

Retrieve incoming packets.



{% hint style="info" %}
In the event-driven behavior description, it is obtained in the `receive()` callback.
{% endhint %}

{% hint style="warning" %}
The received packet data obtained by the `read()` method is designed to be overwritten when subsequent packets are received and processed. If you read the data immediately after `available()` and do some short processing, this will not be a problem, but as a general rule, read the data, copy the data needed for application use, and finish `loop()` promptly. For example, a long `delay()` during `loop()` will cause incoming packets to be dropped.
{% endhint %}



#### available()

```cpp
bool available()
```

Returns `true` if there is an incoming packet that has not yet been read.



#### read()

```cpp
packet_rx& read()
```

Read packets.

