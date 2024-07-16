# 送信

単純ネットワークでの送信手続きは、[tsTxDataApp](../../../twelite-net-api-ref/gou-zao-ti/tstxdataapp.md) 構造体を確保し、この構造体に送信に必要な情報を格納します。

1. ローカル変数として tsTxDataApp 構造体を確保し、構造体全体をゼロクリアする。
2. 送信したいデータを .auData にコピーする（ペイロード）。
3. ペイロードの格納バイト数を .u8Len に格納する。
4. .u32DstAddr を格納する（宛先）
5. .u32SrcAddr を格納する（送り元のアドレス）
6. その他送信オプションを指定します。
7. [ToCoNet_bMacTxReq](../../../twelite-net-api-ref/twelite-net-guan-shu/toconet_bmactxreq.md)() を実行します。
8. TWENET 内での送信処理が終了すると [cbToCoNet_TxEvent](../../../twelite-net-api-ref/krubakku/cbtoconet_vtxevent.md)() が実行されます。

{% hint style="warning" %}
完了イベントが消滅することは例外的ですが、タイムアウトを設け例外処理するようにしてください。 tsTxDataApp 構造体中で遅延や再送遅延が極端に長く設定されていないなら、通常 100ms も必要としません。
{% endhint %}

### サンプルコード

```c
static int16 i16TransmitIoData() {
  tsTxDataApp sTx;
  memset(&sTx, 0, sizeof(sTx)); // ゼロクリア
  uint8 *q = sTx.auData;

  // ペイロードを構成
  S_OCTET(sAppData.u8AppIdentifier);
  S_BE_WORD(sAppData.sIOData_now.u16Volt);
  ...
	
  // 送信
  sTx.u8Len = q - sTx.auData; // パケット長
  sTx.u8Cmd = TOCONET_PACKET_CMD_APP_USER_IO_DATA; // パケット種別

  // 送信する
  sTx.u32DstAddr  = TOCONET_MAC_ADDR_BROADCAST; // ブロードキャスト
  sTx.u8Retry     = 0x81; // 1回再送

  // フレームカウントとコールバック識別子の指定
  sAppData.u16TxFrame++;
  sTx.u8Seq = (sAppData.u16TxFrame & 0xFF);
  sTx.u8CbId = sTx.u8Seq;

  /* MAC モードでは細かい指定が可能 */
  sTx.bAckReq = FALSE;
  sTx.u32SrcAddr = sToCoNet_AppContext.u16ShortAddress;
  sTx.u16RetryDur = 4; // 再送間隔[ms]
  sTx.u16DelayMax = 16; // 送信開始タイミングにブレを作る(最大16ms)

  // 送信API
  if (ToCoNet_bMacTxReq(&sTx)) {
    // 成功（処理が終わると cbToCoNet_vTxEvent() が発生する
    return sTx.u8CbId; // 成功
  } else {
    // 失敗
    return -1; // 失敗
  }
}
```

{% hint style="info" %}
[S_OCTET](../../../twelite-net-api-ref/twelite-net-makuro/utils.h.md#s_octet-x)() [S_BE_WORD](../../../twelite-net-api-ref/twelite-net-makuro/utils.h.md#s_be_word-x-s_le_word-x)() [S_BE_DWORD](../../../twelite-net-api-ref/twelite-net-makuro/utils.h.md#s_be_word-x-s_le_word-x)() マクロは、uint8 \*q をペイロード格納領域のポインタとして宣言しておいた時に使う 1,2,4 バイト格納マクロです。
{% endhint %}

