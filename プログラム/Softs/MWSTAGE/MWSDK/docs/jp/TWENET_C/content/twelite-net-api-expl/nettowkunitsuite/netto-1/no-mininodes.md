# 子機の実装 (MININODES)

子機の基本的な実装を見ます。

{% hint style="info" %}
本ページでは LAYERTREE_MININODES モジュールによる、送信専用子機について紹介します。
{% endhint %}

## モジュール定義, include

```c
#define ToCoNet_USE_MOD_NWK_LAYERTREE_MININODES

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
		/// Application ID
		sToCoNet_AppContext.u32AppId = 0x12345678;
		sToCoNet_AppContext.u8Channel = 18;
		
		/// 受信回路は OFF
		sToCoNet_AppContext.bRxOnIdle = FALSE;
		
		/// CCA は最小限にする
		sToCoNet_AppContext.u8CCA_Level = 1;
		sToCoNet_AppContext.u8CCA_Retry = 0;

		/// 起動時の MAC 初期化を後回しにする。
		///   送信直前で初期化し、不要な場合は初期化しない。
		sToCoNet_AppContext.u8MacInitPending = TRUE;

		// Register user PRSEV.
		ToCoNet_Event_Register_State_Machine(vProcessEvCore);
	}
}
```

## cbAppWarmStart()

単純ネットと同様にハードウェアの初期化等を実施するが、中継ネット特有の処理は `vProcessEvCore()` にて実施する。 

## cbToCoNet_vNwkEvent()

 LayerTree MININODES は、上位ノードの決定といった手続きは省略されるため、 `E_EVENT_TOCONET_NWK_START`  `E_EVENT_TOCONET_NWK_DISCONNECT`  イベントについては処理する必要がありません。

## cbToCoNet_vRxEvent()

受信方法は単純ネットと大きく変わりません。[`tsRxDataApp` 構造体](../../../twelite-net-api-ref/netto-api/structure/tsrxdataapp-netto.md)には違いがあります。

{% hint style="info" %}
中継パケットは [`cbToCoNet_vRxEvent()`](../../../twelite-net-api-ref/krubakku/cbtoconet_vrxevent.md)  コールバックでは処理されません。中継器に直接届いたパケット(送信専用の子機からパケットなど)を処理します。 
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

システム始動時 [`E_EVENT_START_UP`](../../../twelite-net-api-ref/yzaibento/ibento.md) に、[`tsToCoNet_NwkLyTr_Config`](../../../twelite-net-api-ref/netto-api/layertree-netto/tstoconet_nwklytr_context.md) 構造体の設定、[`ToCoNet_NwkLyTr_psConfig_MiniNodes()`](../../../twelite-net-api-ref/netto-api/layertree-netto/toconet_nwklytr_psconfig_mininodes.md) 関数の実行、 [`ToCoNet_Nwk_bInit()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_binit.md) 関数によるネットワークの初期化、[`ToCoNet_Nwk_bStart()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bstart.md) 関数によるネットワークの開始を行います。 

スリープ前には[`ToCoNet_Nwk_bPause()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bpause.md)、スリープからの起床時には [`ToCoNet_Nwk_bResumte()`](../../../twelite-net-api-ref/netto-api/sys_callbacks/toconet_nwk_bresume.md) を実行します。

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
				// 始動時の処理
				memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
				sNwkLayerTreeConfig.u8Role = 
						TOCONET_NWK_ROLE_ENDDEVICE;
				// ネットワークの初期化
				pContextNwk = 
					ToCoNet_NwkLyTr_psConfig_MiniNodes(&sNwkLayerTreeConfig);
	
				if (sAppData.pContextNwk) {
					ToCoNet_Nwk_bInit(pContextNwk);
					ToCoNet_Nwk_bStart(pContextNwk);
				}
			}
		}
		// RUNNING状態へ遷移
		ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
	break;
	
	case E_STATE_RUNNING:
		if (E_EVENT_NEW_STATE) {
			// センサーデータの取得など
		} else {
			// 
			tsTxDataApp sTx;
			memset(&sTx, 0, sizeof(sTx)); // 必ず０クリアしてから使う！
			uint8 *q =  sTx.auData;
		
			sTx.u32SrcAddr = ToCoNet_u32GetSerial();
		
			if (IS_APPCONF_OPT_TO_ROUTER()) {
				// ルータがアプリ中で一度受信して、ルータから親機に再配送
				sTx.u32DstAddr = TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE;
			} else {
				// ルータがアプリ中では受信せず、単純に中継する
				sTx.u32DstAddr = TOCONET_NWK_ADDR_PARENT;
			}
		
			// ペイロードの準備
			S_OCTET('T');
			S_OCTET(sAppData.sFlash.sData.u8id);
			S_BE_WORD(sAppData.u16frame_count);		

			//	センサ固有のデータ
			memcpy(q,pu8Data,u8Len);
			q += u8Len;
		
			sTx.u8Cmd = 0; // 0..7 の値を取る。パケットの種別を分けたい時に使用する
			sTx.u8Len = q - sTx.auData; // パケットのサイズ
			sTx.u8CbId = sAppData.u16frame_count & 0xFF; // TxEvent で通知される番号、送信先には通知されない
			sTx.u8Seq = sAppData.u16frame_count & 0xFF; // シーケンス番号(送信先に通知される)
			sTx.u8Retry = sAppData.u8Retry;

			if (ToCoNet_Nwk_bTx(pNwk, &sTx)) {
				ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
			} else {
				// ...失敗した（スリープ処理)
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
			// 中継ネットのポーズ処理を行う
			ToCoNet_Nwk_bPause(sAppData.pContextNwk);

			// 周期スリープに入る
			//  - 初回は５秒あけて、次回以降はスリープ復帰を基点に５秒
			vSleep(5000, TRUE, FALSE);
		}
		break;

	default:
		break;
	}
}

```

