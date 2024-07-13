# ToCoNet_Nwk_bTx()

## 解説

中継ネットでの無線パケットの送信要求を行う。

{% hint style="warning" %}
配送に関する詳細は [`tsTxDataApp`](../structure/tstxdataapp-netto.md) 構造体の解説を参照してください。
{% endhint %}

### 宛先について

配送先は２種類選択できます。

{% hint style="info" %}
`TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE` を指定することで、近隣の親機、中継器に直接伝達されますが、この時の子機と親機、中継器間の LQI を集計することで、どの親機、中継器が子機に近い位置にいるかどうかを判定できます。

_LQIと距離の精度は高くなく、１m以内といったごく近い位置にいるか、それ以外かを弁別できる程度です。_
{% endhint %}

| 宛先アドレス                             | 解説                                                                  |
| ---------------------------------- | ------------------------------------------------------------------- |
| `TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE` | 電波到達可能な親機または中継器（複数あればその全て）に直接伝達する。                                  |
| `TOCONET_NWK_ADDR_PARENT`          | 親機宛に配送する。（中継器がパケットを受信しても `ToCoNet_vRxEvent()` によりパケットの受信を知ることは出来ない) |

## 引数

| 型                                                                   | 名前          | 詳細                  |
| ------------------------------------------------------------------- | ----------- | ------------------- |
| [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md)`*` | pContextNwk | 中継ネットの内部情報を管理する構造体。 |
| [`tsTxDataApp`](../structure/tstxdataapp-netto.md)`*`               | sTx         | 送信データを格納した構造体。      |

## 戻り値

| 型      | 詳細                                        |
| ------ | ----------------------------------------- |
| bool_t | `TRUE`: 要求が受け付けられた、`FALSE`: 要求が受け付けられなかった |

## サンプル

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;
...
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
​
		//	センサ固有のデータ
		memcpy(q,pu8Data,u8Len);
		q += u8Len;
	
		sTx.u8Cmd = 0; // 0..7 の値を取る。パケットの種別を分けたい時に使用する
		sTx.u8Len = q - sTx.auData; // パケットのサイズ
		sTx.u8CbId = sAppData.u16frame_count & 0xFF; // TxEvent で通知される番号、送信先には通知されない
		sTx.u8Seq = sAppData.u16frame_count & 0xFF; // シーケンス番号(送信先に通知される)
		sTx.u8Retry = sAppData.u8Retry;
​
		if (ToCoNet_Nwk_bTx(pNwk, &sTx)) {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		} else {
			// ...失敗した（スリープ処理)
			ToCoNet_Event_SetState(pEv, E_STATE_SLEEP);
		}
```

