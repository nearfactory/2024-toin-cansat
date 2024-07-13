# ToCoNet_bMacTxReq()

## 解説

単純ネットにおける無線送信要求を行います。送信キューに要求を登録したのち、本関数は終了します。実際の送信処理は後で実行されます。

{% hint style="warning" %}
[中継ネット](../../twelite-net-api-expl/nettowkunitsuite/netto-1/)を利用している場合でも、[単純ネット](../../twelite-net-api-expl/nettowkunitsuite/netto/)の送受信は並存可能です。中継ネット稼働時に本関数による送信要求を行なった場合、想定しない振る舞い等が発生することも考えられますので（例えば、中継ネットではバックグラウンドで多数の通信をしている場合があり、その通信に割り込む形で送信要求を投げる）、十分な検証の上使用してください。
{% endhint %}

## 引数

| 型                                                                   | 名前     | 詳細                                            |
| ------------------------------------------------------------------- | ------ | --------------------------------------------- |
| [`tsTxDataApp`](../netto-api/structure/tstoconet_nwk_context.md)`*` | `psTx` | 送信情報を格納した構造体です。呼び出しスコープ内のローカル変数として確保しても構いません。 |

{% hint style="danger" %}
tsTxDataApp 構造体のメモリ領域は、必ずゼロクリアしてから値を格納します。
{% endhint %}

## 戻り値

| 型        | 詳細                                          |
| -------- | ------------------------------------------- |
| `boot_t` | `TRUE` なら要求は受け付けられ、`FALSE` なら要求は受け付けられなかった。 |

## サンプル

```c
static int16 i16TransmitIoData() {
  tsTxDataApp sTx;
  memset(&sTx, 0, sizeof(sTx)); // 必ずセロクリアする
  uint8 *q = sTx.auData; // S_OCTET() などのマクロ用

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

