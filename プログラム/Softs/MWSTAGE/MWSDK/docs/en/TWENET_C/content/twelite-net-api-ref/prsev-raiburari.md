# PRSEV library

It provides a way to store several differently behaving applications in a single binary, and a way to define a function for each event state.

{% hint style="info" %}
The **App_Tag** end device combines the handling of several sensors into a single application and switches the behaviour depending on the configuration.

Each `vProcessEv_? .c` file contains the definition of the application and the procedures for its individual behaviour.
{% endhint %}

## Defining a callback function

[Callback functions](krubakku/) are defined statically, which is not a good way to combine multiple application behaviours into one. Here, they are defined static in the source file.

```c
/**
 * Hardware interrupt
 * @param u32DeviceId
 * @param u32ItemBitmap
 * @return
 */
static uint8 cbAppToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;
	switch (u32DeviceId) {
	default:
		break;
	}
	return u8handled;
}

/**
 * Hardware events (delayed execution)
 * @param u32DeviceId
 * @param u32ItemBitmap
 */
static void cbAppToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
}

/**
 * Main processing
 */
static void cbAppToCoNet_vMain() {
	/* handle serial input */
	vHandleSerialInput();
}

/**
 * Network events
 * @param eEvent
 * @param u32arg
 */
static void cbAppToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
}

/**
 * RX events
 * @param pRx
 */
static void cbAppToCoNet_vRxEvent(tsRxDataApp *pRx) {
}

/**
 * TX events
 * @param u8CbId
 * @param bStatus
 */
static void cbAppToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	// tx complete
	ToCoNet_Event_Process(E_ORDER_KICK, 0, vProcessEvCore);
}

```

Finally, we have the `tsCbHandler` structure in the list.

```c
/**
 * Application Handler Definition
 *   If it is not used, set it to `NULL`.
 */
static tsCbHandler sCbHandler = {
	NULL, // cbAppToCoNet_u8HwInt,
	cbAppToCoNet_vHwEvent,
	NULL, // cbAppToCoNet_vMain,
	NULL, // cbAppToCoNet_vNwkEvent,
	NULL, // cbAppToCoNet_vRxEvent,
	cbAppToCoNet_vTxEvent
};

```

## State definitions

You can define as many events as you wish to use.

```c
/**
 * Event processing function list
 */
static const tsToCoNet_Event_StateHandler asStateFuncTbl[] = {
	PRSEV_HANDLER_TBL_DEF(E_STATE_IDLE),
	PRSEV_HANDLER_TBL_DEF(E_STATE_RUNNING),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_WAIT_TX),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_SLEEP),
	PRSEV_HANDLER_TBL_TRM
};

```

An entity is defined as follows:

```c
PRSEV_HANDLER_TBL_DEF(E_STATE_IDLE, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// State at the start
	if (eEvent == E_EVENT_START_UP) {
		// Event handling on power-up or sleep-wake
		...
		ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
	}
}

PRSEV_HANDLER_TBL_DEF(E_STATE_RUNNING, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// Running (acquisition of sensors data, transmission requests, etc.)
	bOk = bTransmitToParent( ... ); // Sending process.
	if ( bOk ) {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
	} else {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
	}
}

PRSEV_HANDLER_TBL_DEF(E_STATE_APP_WAIT_TX, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// Waiting for transmission completion
	if (..) {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_APP_SLEEP, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// Sleep procedure (example code below)
	if (eEvent == E_EVENT_NEW_STATE) {
		// Sleep API is always called in a place where it is called only once, such as in `E_EVENT_NEW_STATE`.
		V_PRINTF(LB"Sleeping...");
		V_FLUSH();

		// In the case of Mininode, there is no special processing, but the pause processing
		ToCoNet_Nwk_bPause(sAppData.pContextNwk);

		// Enters cyclic sleep
		//  - 5 seconds after the first time, 5 seconds after the next time, based on the return to sleep
		vSleep(sAppData.sFlash.sData.u32Slp, sAppData.u16frame_count == 1 ? FALSE : TRUE, FALSE);
	}
}
```

## Event handling functions

The event handling function is defined as follows. The event processing is done according to `asStateFuncTbl` prepared in the previous state definition.

```c
/**
 * Event handling function
 * @param pEv
 * @param eEvent
 * @param u32evarg
 */
static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	ToCoNet_Event_StateExec(asStateFuncTbl, pEv, eEvent, u32evarg);
}

```

##  Initialisation process

We will prepare an initialization function to store values in `psCbHandler` and `pvProcessEv1`, which are prepared as global variables as follows.

```c
/**
 * Init the app.
 */
void vInitAppStandard() {
	psCbHandler = &sCbHandler;
	pvProcessEv1 = vProcessEvCore;
}
```

## Running the application

This section describes the main file which contains the callback functions. The main file handles the calling of the functions for the individual processes defined above.

The user-defined event processing function and the TWELITE NET callback function should be defined as follows Call the application's own callback function, specified by the function pointer, to execute the application's own user-defined event processing.

```c
void cbAppColdStart(bool_t bInit) {
    if (bInit == FALSE) {
 		...
    } else {
        switch(u8AppType) {
        case 0x00: vInitAppStandard(); break;// AppStandard で起動
        ...
        }
    	...
    	ToCoNet_Event_Register_State_Machine(pvProcessEv1);
    }
}

/**
 * main procedure
 */
void cbToCoNet_vMain(void) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vMain) {
		(*psCbHandler->pf_cbToCoNet_vMain)();
	}
}

/**
 * rx events
 */
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vRxEvent) {
		(*psCbHandler->pf_cbToCoNet_vRxEvent)(pRx);
	}
}

/**
 * tx completion event
 */
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vTxEvent) {
		(*psCbHandler->pf_cbToCoNet_vTxEvent)(u8CbId, bStatus);
	}
}

/**
 * network event
 * @param eEvent
 * @param u32arg
 */
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vNwkEvent) {
		(*psCbHandler->pf_cbToCoNet_vNwkEvent)(eEvent, u32arg);
	}
}

/**
 * Hardware event processing (interrupt delayed execution)
 */
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vHwEvent) {
		(*psCbHandler->pf_cbToCoNet_vHwEvent)(u32DeviceId, u32ItemBitmap);
	}
}

/**
 * Hardware interrupt handler
 */
uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	bool_t bRet = FALSE;
	if (psCbHandler && psCbHandler->pf_cbToCoNet_u8HwInt) {
		bRet = (*psCbHandler->pf_cbToCoNet_u8HwInt)(u32DeviceId, u32ItemBitmap);
	}
	return bRet;
}
```
