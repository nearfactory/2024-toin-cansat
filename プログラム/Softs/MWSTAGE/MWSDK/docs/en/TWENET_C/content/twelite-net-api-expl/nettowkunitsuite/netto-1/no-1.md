# Implementation of repeaters

We look at the basic implementation of a repeater.

{% hint style="info" %}
The relay provides a function of relaying in response to network requirements, but can also perform other tasks such as transmitting sensor information. As a rule, they are always powered.
{% endhint %}

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

The relay processes the `E_EVENT_TOCONET_NWK_START E_EVENT_TOCONET_NWK_DISCONNECT` message.

```c
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	switch (eEvent) {
	case E_EVENT_TOCONET_NWK_START:
		A_PRINTF( LB"[E_EVENT_TOCONET_NWK_START]");
		break;

	case E_EVENT_TOCONET_NWK_DISCONNECT:
		A_PRINTF( LB"[E_EVENT_TOCONET_NWK_DISCONNECT]");
		break;
	}
}
```

## cbToCoNet_vRxEvent()

The method of reception is much the same as for simple nets, with the exception of the [`tsRxDataApp`](../../../twelite-net-api-ref/netto-api/structure/tsrxdataapp-netto.md) structure.

{% hint style="info" %}
Relay packets are not processed by the `cbToCoNet_vRxEvent()` callback. Packets that arrive directly at the relay (e.g. packets from a transmit-only child) are processed. 
{% endhint %}

```c
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	uint8 *p = pRx->auData;

	// Show packets
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

On system start-up `E_EVENT_START_UP` of the data, set the `tsToCoNet_NwkLyTr_Config` structure, execute the `ToCoNet_NwkLyTr_psConfig()` function, initialize the network with the `ToCoNet_Nwk_bInit()` function and start the network with the `ToCoNet_Nwk_bStart()` function. 

The setting must specify the repeater (`TOCONET_NWK_ROLE_ROUTER`), the number of layers, and the NB beacon (`TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON`).

We get `E_EVENT_TOCONET_NWK_START` event after network start. It also receives `E_EVENT_TOCONET_NWK_DISCONNECT` if the upper node is lost.

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;

static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	switch (pEv->eState) {
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
			
			// Determination of the number of layers
			sNwkLayerTreeConfig.u8Layer = 4;

			// Use of NB beacon-based networks
			sNwkLayerTreeConfig.u8StartOpt =
				TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON;

			// Starts as a repeater
			sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ROUTER;
			pContextNwk = ToCoNet_NwkLyTr_psConfig(&sNwkLayerTreeConfig);
			if (pContextNwk) {
				ToCoNet_Nwk_bInit(pContextNwk);
				ToCoNet_Nwk_bStart(pContextNwk);
			} else {
				// fatal error
			}
		}
		break;

	default:
		break;
	}
}

```

