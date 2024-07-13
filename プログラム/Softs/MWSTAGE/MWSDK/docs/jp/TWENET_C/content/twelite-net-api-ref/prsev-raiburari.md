# PRSEV ライブラリ

複数の違った振る舞いをするアプリケーションをひとつのバイナリに格納する方法と、イベント状態ごとに関数定義する方法を提供します。

{% hint style="info" %}
[App_Tag](https://mono-wireless.com/jp/products/TWE-APPS/App_Tag) のエンドデバイスでは、複数のセンサーの取り扱いについて一つのアプリケーションにまとめて、設定により動作を切り替えています。

各 vProcessEv\_???.c ファイルには、個別の振る舞いをするアプリケーションの定義や手続きが含まれます。
{% endhint %}

## コールバック関数の定義

[コールバック関数](krubakku/)は静的に定義するので、複数のアプリケーション動作をひとつにまとめるには都合が良くありません。ここでは、ソースファイル中に static 定義します。

```c
/**
 * ハードウェア割り込み
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
 * ハードウェアイベント（遅延実行）
 * @param u32DeviceId
 * @param u32ItemBitmap
 */
static void cbAppToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
}

/**
 * メイン処理
 */
static void cbAppToCoNet_vMain() {
	/* handle serial input */
	vHandleSerialInput();
}

/**
 * ネットワークイベント
 * @param eEvent
 * @param u32arg
 */
static void cbAppToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
}

/**
 * RXイベント
 * @param pRx
 */
static void cbAppToCoNet_vRxEvent(tsRxDataApp *pRx) {
}

/**
 * TXイベント
 * @param u8CbId
 * @param bStatus
 */
static void cbAppToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	// 送信完了
	ToCoNet_Event_Process(E_ORDER_KICK, 0, vProcessEvCore);
}

```

最後に一覧のtsCbHandler構造体を用意します。

```c
/**
 * アプリケーションハンドラー定義
 *   使わないものは NULL にする。
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

## 状態の定義

利用するイベントの数だけ定義を行います。

```c
/**
 * イベント処理関数リスト
 */
static const tsToCoNet_Event_StateHandler asStateFuncTbl[] = {
	PRSEV_HANDLER_TBL_DEF(E_STATE_IDLE),
	PRSEV_HANDLER_TBL_DEF(E_STATE_RUNNING),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_WAIT_TX),
	PRSEV_HANDLER_TBL_DEF(E_STATE_APP_SLEEP),
	PRSEV_HANDLER_TBL_TRM
};

```

実体は以下のように定義します。

```c
PRSEV_HANDLER_TBL_DEF(E_STATE_IDLE, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// 開始時の状態
	if (eEvent == E_EVENT_START_UP) {
		// 電源投入またはスリープ復帰時のイベント処理
		...
		ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
	}
}

PRSEV_HANDLER_TBL_DEF(E_STATE_RUNNING, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// 実行中（センサーの取得、送信要求など）
	bOk = bTransmitToParent( ... ); // 送信処理を行う
	if ( bOk ) {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
	} else {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP);
	}
}

PRSEV_HANDLER_TBL_DEF(E_STATE_APP_WAIT_TX, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// 送信完了待ち
	if (..) {
		ToCoNet_Event_SetState(pEv, E_STATE_APP_SLEEP); // スリープ状態へ遷移
	}
}

PRSEV_HANDLER_DEF(E_STATE_APP_SLEEP, tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	// スリープ手続き(以下にコード例)
	if (eEvent == E_EVENT_NEW_STATE) {
		// Sleep は必ず E_EVENT_NEW_STATE 内など１回のみ呼び出される場所で呼び出す。
		V_PRINTF(LB"Sleeping...");
		V_FLUSH();

		// Mininode の場合、特別な処理は無いのだが、ポーズ処理を行う
		ToCoNet_Nwk_bPause(sAppData.pContextNwk);

		// 周期スリープに入る
		//  - 初回は５秒あけて、次回以降はスリープ復帰を基点に５秒
		vSleep(sAppData.sFlash.sData.u32Slp, sAppData.u16frame_count == 1 ? FALSE : TRUE, FALSE);
	}
}
```

## イベント処理関数

イベント処理関数を以下のように定義しておきます。先程の状態の定義で用意した asStateFuncTbl に従って、イベント処理が行われます。

```c
/**
 * イベント処理関数
 * @param pEv
 * @param eEvent
 * @param u32evarg
 */
static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg) {
	ToCoNet_Event_StateExec(asStateFuncTbl, pEv, eEvent, u32evarg);
}

```

##  初期化処理

以下のようにグローバル変数として用意した psCbHandler,  pvProcessEv1 に値を格納する初期化関数を用意しておきます。

```c
/**
 * アプリケーション初期化
 */
void vInitAppStandard() {
	psCbHandler = &sCbHandler;
	pvProcessEv1 = vProcessEvCore;
}
```

## アプリケーションの実行

[コールバック関数](krubakku/)を格納する[メインファイル](../twelite-net-api-expl/ssukdono.md#meinfuiru)について説明します。メインファイルでは、上記で定義した個々の処理用の関数を呼び出す処理を行います。

ユーザ定義イベント処理関数とTWELITE NET の[コールバック関数](krubakku/)は以下のように定義しておきます。関数ポインタで指定されるアプリケーション独自のコールバック関数を呼び出し、アプリケーション独自のユーザ定義イベント処理を実行します。

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
 * メイン処理
 */
void cbToCoNet_vMain(void) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vMain) {
		(*psCbHandler->pf_cbToCoNet_vMain)();
	}
}

/**
 * 受信処理
 */
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vRxEvent) {
		(*psCbHandler->pf_cbToCoNet_vRxEvent)(pRx);
	}
}

/**
 * 送信完了イベント
 */
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vTxEvent) {
		(*psCbHandler->pf_cbToCoNet_vTxEvent)(u8CbId, bStatus);
	}
}

/**
 * ネットワークイベント
 * @param eEvent
 * @param u32arg
 */
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vNwkEvent) {
		(*psCbHandler->pf_cbToCoNet_vNwkEvent)(eEvent, u32arg);
	}
}

/**
 * ハードウェアイベント処理（割り込み遅延実行）
 */
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	if (psCbHandler && psCbHandler->pf_cbToCoNet_vHwEvent) {
		(*psCbHandler->pf_cbToCoNet_vHwEvent)(u32DeviceId, u32ItemBitmap);
	}
}

/**
 * ハードウェア割り込みハンドラ
 */
uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	bool_t bRet = FALSE;
	if (psCbHandler && psCbHandler->pf_cbToCoNet_u8HwInt) {
		bRet = (*psCbHandler->pf_cbToCoNet_u8HwInt)(u32DeviceId, u32ItemBitmap);
	}
	return bRet;
}
```
