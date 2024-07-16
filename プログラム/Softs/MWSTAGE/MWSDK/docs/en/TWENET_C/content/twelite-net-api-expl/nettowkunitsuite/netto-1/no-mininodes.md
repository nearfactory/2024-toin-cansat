# Implementing a child device (MININODES)

We look at the basic implementation of a child device.

{% hint style="info" %}
This page introduces the transmitter-only child unit with the `LAYERTREE_MINININODES` module.
{% endhint %}

## Module definition, include

```c
#define ToCoNet_USE_MOD_NWK_LAYERTREE_MININODES

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## cbAppColdStart()

The application ID and channel are determined here. The initialization process is described in the vProcessEvCore() function.

```c
void cbAppColdStart(bool_t bAfterAhiInit) {
	if (!bAfterAhiInit) { // before AHI init, very first of code.
		// Register modules
		ToCoNet_REG_MOD_ALL();
	} else {
		// TWELITE NET configuration
		/// Application ID
		sToCoNet_AppContext.u32AppId = 0x12345678;
		sToCoNet_AppContext.u8Channel = 18;
		
		/// Receiver circuit is OFF
		sToCoNet_AppContext.bRxOnIdle = FALSE;
		
		/// Keep CCA to a minimum
		sToCoNet_AppContext.u8CCA_Level = 1;
		sToCoNet_AppContext.u8CCA_Retry = 0;

		/// Postpone MAC initialisation at boot.
		///   Initialised just before transmission, or not if not required.
		sToCoNet_AppContext.u8MacInitPending = TRUE;

		// Register user PRSEV.
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);
	}
}
```

## cbAppWarmStart()

Hardware initialization and so on are performed in the same way as for simple nets, but the processing specific to relay nets is performed by `vProcessEvCore()`.

## cbToCoNet_vNwkEvent()

 LayerTree MININODES does not need to handle `E_EVENT_TOCONET_NWK_START` `E_EVENT_TOCONET_NWK_DISCONNECT` events, since procedures such as determining the upper node are omitted.

## cbToCoNet_vRxEvent()

The method of reception is much the same as for simple nets, with the exception of the `tsRxDataApp` structure.

{% hint style="info" %}
Relay packets are not processed by the [`cbToCoNet_vRxEvent()`](../../../twelite-net-api-ref/krubakku/cbtoconet_vrxevent.md) callback. Packets that arrive directly at the relay (e.g. packets from a transmit-only child) are processed.
{% endhint %}

```c
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	uint8 *p = pRx->auData;

	// Show a packet
	if (pRx->u8Cmd == TOCONET_PACKET_CMD_APP_DATA) {
		// Basic information
		uint8 u8lqi_1st = pRx->u8Lqi; // LQI
		uint32 u32addr_1st = pRx->u32SrcAddr; // Src address

		// Interpreting the data
		uint8 u8b = G_OCTET();
		...
	}
}
```

## vProcessEvCore()

On `E_EVENT_START_UP` when the system starts, sets the `tsToCoNet_NwkLyTr_Config` structure, executes the `ToCoNet_NwkLyTr_psConfig_MiniNodes()` function, initializes the network with the `ToCoNet_Nwk_bInit()` function and starts the network with the `ToCoNet_Nwk_bStart()` function. 

Execute [`ToCoNet_Nwk_bPause()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bpause.md) before sleep and [`ToCoNet_Nwk_bResumte()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bresume.md) when waking up from sleep.

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;

static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	switch (pEv->eState) {
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
				// RESUME
				ToCoNet_Nwk_bResume(pContextNwk);
			} else {
				// Processing at start-up
				memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
				sNwkLayerTreeConfig.u8Role = 
						TOCONET_NWK_ROLE_ENDDEVICE;
				// Network initialisation
				pContextNwk = 
					ToCoNet_NwkLyTr_psConfig_MiniNodes(&sNwkLayerTreeConfig);
	
				if (sAppData.pContextNwk) {
					ToCoNet_Nwk_bInit(pContextNwk);
					ToCoNet_Nwk_bStart(pContextNwk);
				}
			}
		}
		// Transition to RUNNING state
		ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
	break;
	
	case E_STATE_RUNNING:
		if (E_EVENT_NEW_STATE) {
			// Sensor data acquisition etc.
		} else {
			// 
			tsTxDataApp sTx;
			memset(&sTx, 0, sizeof(sTx)); // Always clear 0 before you use it!
			uint8 *q =  sTx.auData;
		
			sTx.u32SrcAddr = ToCoNet_u32GetSerial();
		
			if (IS_APPCONF_OPT_TO_ROUTER()) {
				// Received once by the router in the application and redelivered from the router to the parent unit
				sTx.u32DstAddr = TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE;
			} else {
				//The router does not receive the data in the application, but simply relays it
				sTx.u32DstAddr = TOCONET_NWK_ADDR_PARENT;
			}
		
			// Preparing the payload
			S_OCTET('T');
			S_OCTET(sAppData.sFlash.sData.u8id);
			S_BE_WORD(sAppData.u16frame_count);		

			// Sensor-specific data
			memcpy(q,pu8Data,u8Len);
			q += u8Len;
		
			sTx.u8Cmd = 0; // Takes a value of 0..7. Use this when you want to separate packet types.
			sTx.u8Len = q - sTx.auData; // Packet size
			sTx.u8CbId = sAppData.u16frame_count & 0xFF; // Number to be notified by TxEvent, not to the destination
			sTx.u8Seq = sAppData.u16frame_count & 0xFF; // Sequence number (to be notified to destination)
			sTx.u8Retry = sAppData.u8Retry;

			if (ToCoNet_Nwk_bTx(pNwk, &sTx)) {
				ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
			} else {
				// ...Failed (set status for sleep process)
				ToCoNet_Event_SetState(pEv, E_STATE_SLEEP);
			}
		}
		break;
		
	case E_STATE_APP_WAIT_TX:
		if (...) { // Wait Tx Completion
			ToCoNet_Event_SetState(pEv, E_STATE_SLEEP);
		}
		break;
	
	case E_STATE_APP_SLEEP:
		if (eEvent == E_EVENT_NEW_STATE) {
			// Pause processing of the relay net.
			ToCoNet_Nwk_bPause(sAppData.pContextNwk);

			// Enters cyclic sleep
			//  - 5 seconds after the first time, 5 seconds after the next time, based on the return to sleep
			vSleep(5000, TRUE, FALSE);
		}
		break;

	default:
		break;
	}
}

```

