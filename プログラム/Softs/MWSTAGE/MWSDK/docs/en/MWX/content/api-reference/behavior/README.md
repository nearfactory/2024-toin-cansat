---
title: "BEHAVIOR"
author: "Mono Wireless Inc."
---

# BEHAVIOR

BEHAVIOR can be defined by defining the class in the specified way, so that [`the_twelite`](../predefined\_objs/the\_twelite.md) class object. The registered BEHAVIOR will be embedded in TWENET, allowing the user code to describe the application's behavior. It is possible to define callback functions for interrupts and events from TWENET, which is not possible in a loop description. Although it requires more definitions than a loop description, it is suitable for describing more complex applications.

{% hint style="success" %}
See sample BEHAVIOR [PAL\_AMB-behavior](pal\_amb-behavior.md).
{% endhint %}

## Class Definition (.hpp)

BEHAVIOR definition requires a class definition as shown below.

```cpp
class MY_APP_CLASS: MWX_APPDEFS_CRTP(MY_APP_CLASS)
{
public:
    static const uint8_t TYPE_ID = 0x01;

    // load common definition for handlers
    #define __MWX_APP_CLASS_NAME MY_APP_CLASS
    #include "_mwx_cbs_hpphead.hpp"
    #undef __MWX_APP_CLASS_NAME

public:
    // constructor
    MY_APP_CLASS() {}

    void _setup() {}    
    void _begin() {}

public:
    // TWENET callback handler (mandate)
    void loop() {}
    void on_sleep(uint32_t & val) {}
    void warmboot(uint32_t & val) {}
    void wakeup(uint32_t & val) {}

    void on_create(uint32_t& val) { _setup();  }
    void on_begin(uint32_t& val) { _begin(); }
    void on_message(uint32_t& val) { }

public:
    void network_event(mwx::packet_ev_nwk& pEvNwk) {}
    void receive(mwx::packet_rx& rx) {}
    void transmit_complete(mwx::packet_ev_tx& evTx) {}
};
```

The above example defines a BEHAVIOR class with the name MY\_APP\_CLASS. MY\_APP\_CLASS must be described in several places.


```cpp
class MY_APP_CLASS: MWX_APPDEFS_CRTP(MY_APP_CLASS)
```

Define the class name and the base (parent) class. `MWX_APPDEFS_CRTP()` is a macro.



```cpp
    #define __MWX_APP_CLASS_NAME MY_APP_CLASS
    #include "_mwx_cbs_hpphead.hpp"
    #undef __MWX_APP_CLASS_NAME
```

Here, the necessary definitions are imported by `#include`.



```cpp
MY_APP_CLASS() {}
```

Constructor definition.



## methods

### loop()

This is the main loop and has the same role as `loop()` in the global definition.



### on\_create()

`on_create()` is called at object creation time (`use<>()` method). The `val` is a parameter for future extension.



### on\_begin()

on_begin()` is called after `setup()` ends. `val` is a parameter for future extension.



### on\_sleep()

Called before sleep. `val` is a parameter for future extension.



### warmboot()

Called in the initial stage when returning from sleep. The `val` is a parameter for future expansion.

At this point, the peripherals have not yet been initialized. The sleep wake-up factor can be checked.


### wakeup()

Called when waking up from sleep. The `val` is a parameter for future extension.

{% hint style="info" %}
Sleep can also be called here.
{% endhint %}



### receive()

```cpp
void receive(mwx::packet_rx& rx)
```

When a packet is received, it is called with the received packet information as `rx`.


### transmit\_complete()

```cpp
void transmit_complete(mwx::packet_ev_tx& evTx)
```

The transmission information is called as `evTx` when packet transmission is completed. The `evTx.u8CbId` is the ID at the time of transmission and `evTx.bStatus` is the flag indicating success (`1`) or failure (`0`) of the transmission.





## Define handlers (.cpp)

BEHAVIOR handlers (interrupt, event, and state definitions) are defined in a cpp file. The file cannot be split and all handler definitions must be in one file.

{% hint style="warning" %}
Even in the case of BEHAVIORs that do not define handlers, be sure to create the following cpp file.
{% endhint %}



The required definitions of the MWX library (`#include "_mwx_cbs_cpphead.hpp"`) must be included at the beginning and end of the cpp file.


```cpp
#include <TWELITE>
#include "myAppClass.hpp" // BEHAVIOR definition file

/*****************************************************************/
// MUST DEFINE CLASS NAME HERE
#define __MWX_APP_CLASS_NAME MY_APP_CLASS
#include "_mwx_cbs_cpphead.hpp" // Definition at the beginning
/*****************************************************************/
```

At the beginning of the file, include the .hpp file of the BEHAVIOR definition as shown above. Specify the class name of the behavior in `__MWX_APP_CLASS_NAME`. In the above, it is `MY_APP_CLASS`.



```cpp
/*****************************************************************/
// common procedure (DO NOT REMOVE)
#include "_mwx_cbs_cpptail.cpp"
// MUST UNDEF CLASS NAME HERE
#undef __MWX_APP_CLASS_NAME
/*****************************************************************/
```

At the end of the file, include the necessary definitions (`#include "_mwx_cbs_cpptail.cpp"`).



The handler definition is written as shown in the following example. Types of definitions are described later. The definition of the handler to be used is described by using the macro for definition. Do not write handlers that are not used.

The `MWX_????? _INT()` is the definition of an interrupt handler, and `MWX_? _EVENT()` is the definition of an event handler, and `MWX_STATE()` is the state definition of a state machine.

```cpp
// TickTimer interrupt
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled) {
	// blink LED
	digitalWrite(PAL_AMB::PIN_LED, 
	      ((millis() >> 9) & 1) ? PIN_STATE::HIGH : PIN_STATE::LOW);
}

// PAL_AMB::PIN_BIN(12) event
MWX_DIO_EVENT(PAL_AMB::PIN_BTN, uint32_t arg) {
	Serial << "Button Pressed" << mwx::crlf;
	
	static uint32_t u32tick_last;
	uint32_t tick = millis();

	if (tick - u32tick_last > 100) {
		PEV_Process(E_ORDER_KICK, 0UL);
	}

	u32tick_last = tick;
}

// Operation definition of state STATE_0
MWX_STATE(E_MWX::STATE_0, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_START_UP) {
		Serial << "[STATE_0:START_UP]" << mwx::crlf;	
	} else
	if (ev == E_ORDER_KICK) {
		PEV_SetState(E_MWX::STATE_1);
	}
}

// Operation definition for state STATE_1
MWX_STATE(E_MWX::STATE_1, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_1]" << mwx::crlf;	
	} else
	if (ev == E_ORDER_KICK) {
		PEV_SetState(E_MWX::STATE_2);
	} else
	if (ev == E_EVENT_TICK_SECOND) {
		Serial << "<1>";
	}	
}
```



## Interrupt and Event Handlers

Interrupt handlers are executed when a microcontroller interrupt occurs, interrupting the code currently being executed. For this reason, it is desirable to write as short a process as possible, and great care must also be taken with regard to manipulation of variables and the like.

The interrupt handler has a parameter `uint8_t& handled`, and setting this value to `true` will prevent subsequent event calls from being made.

If the interrupt handler exits with `handled` set to `false`, the event handler will be called in the application loop (normal code). The event handler has no `handled` parameter. Since the event handler is normal code, it can perform relatively large processing. However, the event handler also incurs overhead, so it may not be able to handle the processing that is called at each frequent interrupt. In addition, since events are processed by the system's internal FIFO queue, events may be lost if they cannot be processed within a certain period of time.

The following is an explanation of macros for defining handler functions.

### DIO

```cpp
MWX_DIO_INT(N, uint32_t arg, uint8_t& handled)
MWX_DIO_EVENT(N, arg)
```

DIO (digital IO) interrupt event. `N` specifies the target DIO number. The `arg` is a definition for future extension.

To generate an interrupt, use [`pinMode()`](../funcs/dio/pinmode.md), [`attachDioInt()`](../funcs/dio/attachintdio.md).



### TICKTIMER

```cpp
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled)
MWX_TICKTIMER_EVENT(uint32_t arg)
```

TickTimer interrupt and event. The `arg` is a definition for future extension.

{% hint style="danger" %}
The `handled` flag of the TickTimer must not be set to `true`, otherwise TWENET will not work.
{% endhint %}



### TIMER

```cpp
MWX_TIMER_INT(N, uint32_t arg, uint8_t& handled)
MWX_TIMER_EVENT(N, uint32_t arg)
```

Timer interrupt event. The`N` specifies the number of the target timer. The `arg` is a definition for future extension.

In order to generate an interrupt, the [Timer object](../predefined\_objs/timers.md) is started with software interrupts enabled.



### Other

{% hint style="warning" %}
Definition of other interrupts and events that are not defined standardly in the MWX library and require an understanding of the AHI Peripherals Manual.
{% endhint %}

Other interrupt events can be received by the following handler functions. These will not be available in the future when dedicated handlers are defined.

```cpp
MWX_MISC_INT(uint32_t arg, uint32_t arg2, handled)
MWX_MISC_EVENT(auint32_t rg, uint32_t arg2)
```

Peripheral (AHI) interrupt handler `u32DeviceId` corresponds to `arg` and `u32ItemBitmap` corresponds to `arg2`.



## State Machine

A state machine (state machine) is a method of describing an application that receives messages and operates by transitioning its state in response to those messages.

The [PAL\_AMB-behavior](pal\_amb-behavior.md) sample describes the flow of the application's operation, including the start of sensor operation, acquisition of sensor values, wireless packet transmission to completion of transmission, and sleep transition. Please refer to it as an actual example.

The events to be received are as follows.

| Event Name | Description                                                         |
| --------------------- | ------------------------------------------------------------ |
| `E_EVENT_START_UP`    | It is called at system startup. Immediately after power-on, it is called with <code>0</code> parameters. <strong>Because it is in the initial stage of execution, <code>PEV_Process()</code> is called once from the begin() method to start the operation when transitioning to the normal processing state. </strong><br><strong></strong><br><strong></strong>It is still called after returning from sleep, but with parameters other than <code>0</code>. <strong>Normal processing can be performed from this state. </strong> |
| `E_EVENT_NEW_STATE`   | It is called in a new state immediately after a state transition. Describes the process that is first executed when a transition is made to a certain state. |
| `E_EVENT_TICK_TIMER`  | Called by TickTimer every 1ms                                |
| `E_EVENT_TICK_SECOND` | It is called every second.                                   |



### PEV\_SetState()

```cpp
void PEV_SetState(uint32_t s)
```

The state is set to `s`.

Exiting the state handler causes a transition to the next state, followed by a state handler being called with the `E_EVENTS_NEW_STATE` event.



### PEV\_u32Elaspsed\_ms()

```cpp
uint32_t PEV_u32Elaspsed_ms()
```

Returns the elapsed time ≪ ms] since the state transition. It is used for purposes such as managing timeouts.



```cpp
MWX_STATE(MY_APP_CHILD::STATE_TX, uint32_t ev, uint32_t evarg) {
  ...
  
	if (PEV_u32Elaspsed_ms() > 100) {
		// does not finish TX!
		Serial << "[STATE_TX] FATAL, TX does not finish!" << mwx::crlf << mwx::flush;
		the_twelite.reset_system();
	}
}
```

In the above example, a system reset is performed after 100 ms.



### PEV\_Process()

```cpp
void PEV_Process(uint32_t ev, uint32_t u32evarg) {
```

Called from outside the state handler. Execute the state handler with the event `ev` parameter `u32evarg`.



```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}
```

The transmission completion event is communicated to the state machine. In other words, call the state handler.

{% hint style="danger" %}
Do not call the state handler directly. It will cause problems such as `E_EVENT_NEW_STATE` not being executed.
{% endhint %}



### PEV\_KeepStateOnWakeup()

```cpp
void PEV_KeepStateOnWakeup()
```

Set just before sleep. After returning from sleep, the previous state is maintained. That is, the state handler is called with `E_EVENT_START_UP` with sleep started.



### PEV\_is\_coldboot()

```cpp
bool PEV_is_coldboot(uint32_t ev, uint32_t u32evarg)
```

Determine if the event is `E_EVENT_START_UP` on wake-up.



### PEV\_is\_warmboot()

```cpp
bool PEV_is_warmboot(uint32_t ev, uint32_t u32evarg)
```

Judges whether the event is `E_EVENT_START_UP` when returning from sleep.

