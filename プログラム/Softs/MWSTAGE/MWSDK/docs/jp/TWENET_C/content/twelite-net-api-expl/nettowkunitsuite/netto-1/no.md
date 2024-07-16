# 親機の実装

親機の基本的な実装を見ます。

## モジュール定義, include

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

アプリケーションIDやチャネルは、ここで決めておきます。また初期化処理等は ` vProcessEvCore()` 関数に記述します。

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

 親機では `E_EVENT_TOCONET_NWK_START` メッセージを処理します。

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

受信方法は単純ネットと大きく変わりません。[`tsRxDataApp` 構造体](../../../twelite-net-api-ref/netto-api/structure/tsrxdataapp-netto.md)には違いがあります。

```c
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	uint8 *p = pRx->auData;

	// パケットの表示
	if (pRx->u8Cmd == TOCONET_PACKET_CMD_APP_DATA) {
		// 基本情報
		uint8 u8lqi_1st = pRx->u8Lqi; // LQI
		uint32 u32addr_1st = pRx->u32SrcAddr; // 送信元

		// データの解釈
		uint8 u8b = G_OCTET();
		...
	}
}
```

## vProcessEvCore()

システム始動時 [`E_EVENT_START_UP`](../../../twelite-net-api-ref/yzaibento/ibento.md) に、[`tsToCoNet_NwkLyTr_Config`](../../../twelite-net-api-ref/netto-api/layertree-netto/tstoconet_nwklytr_context.md) 構造体の設定、[`ToCoNet_NwkLyTr_psConfig()`](../../../twelite-net-api-ref/netto-api/layertree-netto/toconet_nwklytr_psconfig.md) 関数の実行、 [`ToCoNet_Nwk_bInit()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_binit.md) 関数によるネットワークの初期化、[`ToCoNet_Nwk_bStart()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bstart.md) 関数によるネットワークの開始を行います。 

ネットワーク開始後に、`E_EVENT_TOCONET_NWK_START` イベントを受け取ります。 

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
			// START イベントを持って RUNNING 状態に遷移
			// * cbToCoNet_vNwkEvent() より伝達
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

