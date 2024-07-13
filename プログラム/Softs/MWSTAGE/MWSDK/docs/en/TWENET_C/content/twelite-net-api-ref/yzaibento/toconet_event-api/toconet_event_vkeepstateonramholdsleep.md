# ToCoNet_Event_vKeepStateOnRamHoldSleep()

## Explanation

Keep the current state after waking up from sleep.

{% hint style="info" %}
This function sets the first argument `teEvent *pEv` of a user-defined event handling function to `pEv->bKeepStateOnSetAll = TRUE`.
{% endhint %}

## Argument

| Type       | Name | Remark                                                    |
| ---------- | ---- | --------------------------------------------------------- |
| tsEvent \* | pEv  | Management structure for user event definition functions. |

## Returns

None.

## Sample code

This is an example of a process that performs a short sleep if the conditions are not met, and then performs a specific process if the conditions are met. This is the case, for example, when waiting for a sensor to complete its AD several times.

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
    static bool_t cond;
	
	switch (pEv->eState) {
	case E_STATE_IDLE:
		; // some task
		ToCoNet_Event_SetState(pEv, E_STATE_APP_RUNNING);
		break;
	case E_STATE_APP_RUNNING:
		/* 
		 * 1: Transition to this state and processing in E_EVENT_NEW_STATE
		 * 2: Check for cond, if FALSE, sleep
		 *         If TRUE, transition to E_STATE_APP_TX_REQUEST.
		 * 3: Waking up from sleep E_EVENT_START_UP is performed and
		 *    perform the process of 2:.
		 */
		if (eEvent == E_EVENT_NEW_STATE) {
			// Executed immediately after a state transition
			cond = ...; // update
		} 
		else if ((eEvent == E_EVENT_START_UP)
			&& (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK)) {
			// Called when ToCoNet_vSleep() wakes up.
			cond = ...; // update 	
		}

		// Check the conditions
		if (!cond) {
	 		ToCoNet_Event_vKeepStateOnRamHoldSleep();
			ToCoNet_vSleep(E_AHI_WAKE_TIMER_1, 
				50, FALSE, FALSE);
		} else {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_TX_REQUEST);
		}
		break;
	case E_STATE_APP_TX_REQUEST:
		... // transmi a packet
		break;
}
```

