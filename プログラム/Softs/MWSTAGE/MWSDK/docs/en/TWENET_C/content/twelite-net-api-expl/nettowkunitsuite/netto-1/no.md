# Implementation of the parent  device

This section looks at the basic implementation of the parent device.

## Module definition, include

```c
#define ToCoNet_USE_MOD_NWK_LAYERTREE
#define ToCoNet_USE_MOD_NBSCAN
#define ToCoNet_USE_MOD_NBSCAN_SLAVE
#define ToCoNet_USE_MOD_NWK_MESSAGE_POOL
#define ToCoNet_USE_MOD_DUPCHK

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## cbAppColdStart()

The application ID and channel are determined here. The initialization process is described in the `vProcessEvCore()` function.

```c
void cbAppColdStart(bool_t bAfterAhiInit) {
	if (!bAfterAhiInit) { // before AHI init, very first of code.
		// Register modules
		ToCoNet_REG_MOD_ALL();
	} else {
		// TWELITE NET configuration
		sToCoNet_AppContext.u32AppId = 0x12345678;
		sToCoNet_AppContext.u8Channel = 18;
		sToCoNet_AppContext.bRxOnIdle = TRUE;

		// Register user PRSEV.
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);
	}
}
```

## cbToCoNet_vNwkEvent()

The parent machine processes the `E_EVENT_TOCONET_NWK_START` message.

```c
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	switch (eEvent) {
	case E_EVENT_TOCONET_NWK_START:
		// send this event to the local event machine.
		ToCoNet_Event_Process(eEvent, u32arg, vProcessEvCore);
		break;
	default:
		break;
	}
}
```

## cbToCoNet_vRxEvent()

The receiving method is much the same as for simple nets. There is a difference in the [`tsRxDataApp`](../../../twelite-net-api-ref/netto-api/structure/tsrxdataapp-netto.md) structure.

```c
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	uint8 *p = pRx->auData;

	// View packets
	if (pRx->u8Cmd == TOCONET_PACKET_CMD_APP_DATA) {
		// Basic information
		uint8 u8lqi_1st = pRx->u8Lqi; // LQI
		uint32 u32addr_1st = pRx->u32SrcAddr; // Sender

		// Interpreting the data
		uint8 u8b = G_OCTET();
		...
	}
}
```

## vProcessEvCore()

On `E_EVENT_START_UP` when the system starts, sets the `tsToCoNet_NwkLyTr_Config` structure, executes the `ToCoNet_NwkLyTr_psConfig()` function, initializes the network with the `ToCoNet_Nwk_bInit()` function and starts the network with the `ToCoNet_Nwk_bStart()` function. 

The `E_EVENT_TOCONET_NWK_START` event is received after the network starts.

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;

static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	switch (pEv->eState) {
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			V_PRINTF(LB"[E_STATE_IDLE]");

			// Configure the Network
			sNwkLayerTreeConfig.u8Layer = 0;
			sNwkLayerTreeConfig.u8Role 
				= TOCONET_NWK_ROLE_PARENT;

			pContextNwk =
				ToCoNet_NwkLyTr_psConfig(&sNwkLayerTreeConfig);
			if (pContextNwk) {
				ToCoNet_Nwk_bInit(pContextNwk);
				ToCoNet_Nwk_bStart(pContextNwk);
			}

		} else if (eEvent == E_EVENT_TOCONET_NWK_START) {
			// Transition to the RUNNING state with a START event
			// * Transmission from cbToCoNet_vNwkEvent()
			ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
		} else {
			;
		}
		break;

	case E_STATE_RUNNING:
		if (eEvent == E_EVENT_NEW_STATE) {
			V_PRINTF(LB"[E_STATE_RUNNING]");
		} else {
			; // DO SOMETHING ELSE!
		}
		break;

	default:
		break;
	}
}

```

