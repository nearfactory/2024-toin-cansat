# ToCoNet_Event_vKeepStateOnRamHoldSleep()

## 解説

スリープ復帰時に指定した状態を保持します。

{% hint style="info" %}
ユーザ定義イベント処理関数の第一引数 teEvent \*pEv に対し pEv->bKeepStateOnSetAll = TRUE をセットします。
{% endhint %}

## 引数

| 型          | 名前  | 詳細                 |
| ---------- | --- | ------------------ |
| tsEvent \* | pEv | ユーザイベント定義関数の管理構造体。 |

## 戻り値

なし。

## サンプル

処理を行い、条件が満足しなければ短いスリープを実施し、条件が成立したら特定の処理を行う例です。例えばセンサーのAD完了待ちなどを複数回行うような場合です。

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
		 * 1: 本状態への遷移、E_EVENT_NEW_STATE での処理を行う
		 * 2: cond のチェック、FALSE なら、スリープを実行
		 *         TRUE ならE_STATE_APP_TX_REQUEST　へ遷移。
		 * 3: スリープからの起床 E_EVENT_START_UP での処理を行い、
		 *    2: の処理を実行
		 */
		if (eEvent == E_EVENT_NEW_STATE) {
			// 状態遷移直後に実行される
			cond = ...; // update
		} 
		else if ((eEvent == E_EVENT_START_UP)
			&& (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK)) {
			// ToCoNet_vSleep() の起床時に呼び出される。
			cond = ...; // update 	
		}

		// 条件の確認
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

