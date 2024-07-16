# ToCoNet_Event_u32TickFrNewState()

## 解説

ユーザ定義イベント関数の現在の状態に遷移してからの経過時間を得る。

{% hint style="info" %}
送信完了待ちなどのタイムアウトに利用します。
{% endhint %}

{% hint style="warning" %}
初期状態 E_STATE_IDLE での値は不定です。
{% endhint %}

## 引数

| 型          | 名前  | 詳細                 |
| ---------- | --- | ------------------ |
| tsEvent \* | pEv | ユーザイベント定義関数の管理構造体。 |

## 戻り値

| 型      | 詳細             |
| ------ | -------------- |
| uint32 | 状態遷移してからの経過時間。 |

## サンプル

以下の例では、E_STATE_IDLE で特定の条件が成立した時に、送信処理を行います（E_STATE_APP_TX_REQUESTへ遷移）。送信が成功した場合 cond_if_tx_complete が 1 または 0xff に設定しますが、この値が 100ms 経過するまでに設定されなければタイムアウトとします。

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
			// 無線送信する
			ToCoNet_Event_SetState(pEv, E_STATE_APP_TX_REQUEST);
		}
		break;
		
	case E_STATE_APP_TX_REQUEST:
		if (eEvent == E_EVENT_NEW_STATE) {
			// 送信処理
			tsTxDataApp sTx;
			...
			cond_if_tx_complete = FALSE; // 完了フラグ
			if (!ToCoNet_bMacTxReq(&sTx)) {
				// 送信失敗
				ToCoNet_Event_SetState(pEv, E_STATE_APP_ERROR);
			}
		}
		
		// 送信完了条件の確認
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
		... // エラー処理
		break;
}
```

