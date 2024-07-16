# User defined event handling functions

A user-defined event handling function is a function defined by the user for the purpose of performing event handling in the form of a callback from TWENET. Although we call them functions, they behave as state transition machines.

```c
void cbAppColdStart(bool_t bInit) {
    if (bInit == FALSE) {
 		...
    } else {
    	...
    	ToCoNet_Event_Register_State_Machine(vProcessEvCore);
    }
}

void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg) {
    // check boot seq
	if (eEvent == E_EVENT_START_UP) {
		if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
			// woke from NORMAL SLEEP (RAMHOLD)
		}
	    if (u32evarg & EVARG_START_UP_WAKEUP_MASK) {
			// woke from DEEP SLEEP
	    } else {
			// COLD boot
	    }
	}
	
	switch (pEv->eState) {
	case E_STATE_IDLE:
		; // some task
		ToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
		break;
	case E_STATE_APP_NORMAL:
		; // some task (e.g. send s Tx packet)
		if (cond) {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		}
		break;
	case E_STATE_APP_WAIT_TX:
		; // some task (e.g. wait until Tx finishes)
		if (cond) {
		    ToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
		}
		break;
	}
}
```



### Register

Up to two user-defined event processing functions can be registered with [`ToCoNet_Event_Register_State_Machine()`](toconet_event-api/toconet_event_register_state_machine.md).

{% hint style="info" %}
User-defined event handling functions must be registered to receive [`E_EVENT_START_UP`](ibento.md) events.
{% endhint %}



### The notified event

The following three events are notified. Other events are passed on as function callback calls by the [`ToCoNet_Event_Process()`](toconet_event-api/toconet_event_process.md#toconet_event_process) function.

* [E_EVENT_START_UP](ibento.md) : on start-up
* [E_EVENT_TICK_TIMER](ibento.md) : every tick timer fired (4ms default)
* [E_EVENT_TICK_SECOND](ibento.md) : every 1sec

The following code is an example of communicating the completion of a transmission to the user-defined event processing function `vProcessEvCore()`.

```c
cbToCoNet_vTxEvent(uint8 u8CbId, uint8 u8Stat) {
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, vProcessEvCore);
}
```

{% hint style="warning" %}
Calling `vProcessEvCore()` without using `ToCoNet_Event_Process()` should be avoided. The state transitions shown in [the flow diagram](../../twelite-net-api-expl/twenet-fur/yzaibentofur.md) will not take place and there will be inconsistencies in the information contained in the management structure of the user-defined event processing function.

It must not be called from an interrupt handler.
{% endhint %}



### About state transitions

The state machine of user-defined event handling functions starts from `E_STATE_IDLE (0)`.

When a user-defined event processing function is called, a state transition is declared by calling the `ToCoNet_Event_SetState()` function. After declaring the state transition and exiting the user defined event handling function, the function is called again with the `E_EVENT_NEW_STATE` event as a parameter. This process is continuous as long as the state transition continues ([flow diagram](../../twelite-net-api-expl/twenet-fur/yzaibentofur.md)).

{% hint style="info" %}
You can keep the previous state by calling [`ToCoNet_Event_vKeepStateOnRamHoldSleep()`](toconet_event-api/toconet_event_vkeepstateonramholdsleep.md) before sleep (hold RAM), and receive `E_EVENT_START_UP` event after sleep is restored.
{% endhint %}

