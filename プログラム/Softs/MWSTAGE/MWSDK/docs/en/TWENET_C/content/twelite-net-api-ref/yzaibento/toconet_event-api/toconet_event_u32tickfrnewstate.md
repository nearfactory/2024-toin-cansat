# ToCoNet_Event_u32TickFrNewState()

## Explanation

Obtain the time elapsed since the transition to the current state of the user-defined event function.

{% hint style="info" %}
Used for timeouts, e.g. waiting for transmission completion.
{% endhint %}

{% hint style="warning" %}
The value in the initial state `E_STATE_IDLE` is undefined.
{% endhint %}

## Argument

| Type       | Name | Remark                                                    |
| ---------- | ---- | --------------------------------------------------------- |
| tsEvent \* | pEv  | Management structure for user event definition functions. |

## Returns

| Type   | Remark                                 |
| ------ | -------------------------------------- |
| uint32 | the elapsed time since the transition. |

## Sample code

The following example shows how to process a send (transition to `E_STATE_APP_TX_REQUEST`) when a certain condition is met in `E_STATE_IDLE`. If the transmission is successful, `cond_if_tx_complete` is set to `1` or `0xff`, but if this value is not set before 100ms has elapsed, the transmission times out.

```c
uint8 cond_if_tx_complete;

void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if(bStatus & 0x01) {
		cond_if_tx_complete = 0x1; // TX success
	} else {
		cond_if_tx_complete = 0xFF; // TX FAIL
	}
}

void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg) {
    static bool_t cond;
	
	switch (pEv->eState) {
	case E_STATE_IDLE:
		...
		if(...) {
			// changer state for wireless transmission
			ToCoNet_Event_SetState(pEv, E_STATE_APP_TX_REQUEST);
		}
		break;
		
	case E_STATE_APP_TX_REQUEST:
		if (eEvent == E_EVENT_NEW_STATE) {
			// place a transmission request.
			tsTxDataApp sTx;
			...
			cond_if_tx_complete = FALSE; // set completion flag as deferred.
			if (!ToCoNet_bMacTxReq(&sTx)) {
				// fails
				ToCoNet_Event_SetState(pEv, E_STATE_APP_ERROR);
			}
		}
		
		// Confirmation of transmission completion conditions
		if (cond_if_tx_complete == 0x1) {
			// Tx success!
			ToCoNet_Event_SetState(pEv, E_STATE_IDLE);
		} else if (cond_if_tx_complete == 0xFF) {
			// Tx fails
 			ToCoNet_Event_SetState(pEv, E_STATE_APP_ERROR);
 		}
		
		// Tx Timeout
		if (ToCoNet_Event_u32TickFrNewState(pEv) > 100) {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_ERROR);
		}
		break;
		
	case E_STATE_APP_ERROR:
		... // error handling
		break;
}
```

