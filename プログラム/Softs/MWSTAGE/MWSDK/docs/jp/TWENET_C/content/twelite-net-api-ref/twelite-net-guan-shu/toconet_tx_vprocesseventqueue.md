# ToCoNet_Tx_vProcessEventQueue()

## 解説

無線パケット送信要求を速やかに処理する。

本関数は [`ToCoNet_bMacTxReq()`](toconet_bmactxreq.md) 要求直後に呼び出すことで、送信要求を速やかに処理します。この要求を行わない場合は、システムタイマー (デフォルト 4ms) のタイミングで無線送信が処理されます。

{% hint style="warning" %}
速やかに無線送信されるのは、その時点で処理されている送信要求が存在せず、かつ、該当する送信要求に送信遅延が設定されていない場合です。例えば、送信完了を待って、連続的に送信をするような場合に有効です。
{% endhint %}

## 引数

なし

## 戻り値

なし

## サンプル

```c
/* 送信完了を待って次のパケット要求を行う */
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if (sAppData.bOnTx) { // 送信中のフラグ
		vTransmit();
	}
	return;
}

static void vTransmit() {
	_C {
		// transmit Ack back
		tsTxDataApp tsTx;
		memset(&tsTx, 0, sizeof(tsTxDataApp));

		tsTx.u32SrcAddr = ToCoNet_u32GetSerial(); // 自身のアドレス
		tsTx.u32DstAddr = 0xFFFF; // ブロードキャスト

		tsTx.bAckReq = FALSE;

		tsTx.u8Retry = 0; // 再送なし
		tsTx.u16DelayMin = 0;
		tsTx.u16DelayMax = 0;
		tsTx.u16RetryDur = 0; // 遅延は0で速やかに送信

		// ペイロード生成など
		...
		
		// 送信
		if (ToCoNet_bMacTxReq(&tsTx)) {
			// 送信要求が受け付けられたら速やかに処理する。
			ToCoNet_Tx_vProcessQueue(); 
		}
	}
}
```

