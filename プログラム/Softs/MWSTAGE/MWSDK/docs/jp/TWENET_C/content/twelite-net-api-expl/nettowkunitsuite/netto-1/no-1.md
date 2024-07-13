# 中継器の実装

中継機の基本的な実装を見ます。

{% hint style="info" %}
中継器は、ネットワークの要求に応えて中継する機能を提供しますが、中継以外にもセンサー情報の送信といった処理も可能です。原則として常時給電で動作させます。
{% endhint %}

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

 中継機では `E_EVENT_TOCONET_NWK_START`  `E_EVENT_TOCONET_NWK_DISCONNECT`  メッセージを処理します。

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

受信方法は単純ネットと大きく変わりません。[`tsRxDataApp` 構造体](../../../twelite-net-api-ref/netto-api/structure/tsrxdataapp-netto.md)には違いがあります。

{% hint style="info" %}
中継パケットは `cbToCoNet_vRxEvent()`  コールバックでは処理されません。中継器に直接届いたパケット(送信専用の子機からパケットなど)を処理します。 
{% endhint %}

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

設定では中継器の指定(`TOCONET_NWK_ROLE_ROUTER`)、レイヤー 数、NBビーコンの指定(`TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON`)が必須です。

ネットワーク開始後に、`E_EVENT_TOCONET_NWK_START` イベントを受け取ります。また、上位ノードをロストした場合 `E_EVENT_TOCONET_NWK_DISCONNECT` を受け取ります。 

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;

static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	switch (pEv->eState) {
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
			
			// layer 数の決定
			sNwkLayerTreeConfig.u8Layer = 4;

			// NBビーコン方式のネットワークを使用する
			sNwkLayerTreeConfig.u8StartOpt =
				TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON;

			// 中継器として始動
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

