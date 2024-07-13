# tsTxDataApp

無線送信を行うとき、送信したいデータ、宛先、送信時の諸設定を構造体に格納し、送信API にポインタとして渡す。

### メンバー一覧（単純ネット）

| 定義名                                            | 解説                                                                                                                                                                                                                                          |
| ---------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint32 u32SrcAddr`                            | 送信元アドレス。`0xFFFF` 未満ならショートアドレス。`0x8000000` 以上は拡張アドレス。通常は [`sToCoNet_AppContext.u16ShortAddress`](stoconet_appcontext.md) または  [`ToCoNet_u32GetSerial()`](../twelite-net-guan-shu/toconet_u32getserial.md)   より自アドレスを指定する。                    |
| `uint32 u32DstAddr`                            | 宛先アドレス。0xFFFF 以下ならショートアドレス。`0x8000000` 以上は拡張アドレス。  `TOCONET_MAC_ADDR_BROADCAST(0xFFFF)`は ブロードキャスト送信を示す。                                                                                                                                     |
| `uint8 u8Cmd`                                  | パケット種別。アプリケーションで自由に設定できる。<br> 値域: `0..7`                                                                                                                                                                                  |
| `uint8 u8CbId`                                 | コールバックID。送信要求時に設定した値が   [`cbToCoNet_TxEvent()`](../krubakku/cbtoconet_vtxevent.md)  により渡されます。この値によって送信したパケットと送信完了イベントを対応付けます。本値は送信パケットには含まれません。値域：0-255                                                                                     |
| `uint8 au8Data[]`                              | パケットのペイロード（[最大パケットサイズ](../../twelite-net-api-expl/paketto/pakettono.md)）                                                                                                                                                                |
| bool_t bAckReq                                 | `TRUE` ならACK送信要求を設定します。同報通信時には `FALSE` を設定します。                                                                                                                                                                                              |
| `uint8 u8Retry`                                | ToCoNet で実施する再送回数。MAC層での再送に失敗した場合、ToCoNet で再度パケット送信を試みます。MSB を設定すると、成功失敗に関わらず指定回数の再送を行います。同報通信では殆どの場合送信が成功するため、複数回送信が必要な場合はこのビットを立てます。例えば0x83 を指定すると、都合４回送信が行われます。値域は 0x0～0xF, 0x80～0x8F。                                                   |
| `uint16 u16DelayMin, u16DelayMax, u16RetryDur` | 送信開始まで `u16DelayMin` \[ms] 遅延し、最大 `u16DelayMax` \[ms] 待ちます。最大値は乱数により決定するため、送信にタイミングに意図的なブレを作るため使用します。送信開始後は `u16RetryDur`\[ms] 間隔で再送が行われます（この間隔は固定）。この処理はシステムのタイマーにより制御されるため、タイマー刻みの精度（標準は4ms）で実行されます。値域には制限はありませんが、一般的には、長くとも 1000ms 程度です。 |
| `uint16 bSecurePkt`                            | 平文で送信する場合は `ALSE`, 暗号化する場合は `TRUE` を指定します。事前に [`ToCoNet_bRegisterAesKey()`](../twelite-net-guan-shu/toconet_bregisteraeskey.md) により暗号化鍵を登録しておきます。※最大利用バイト数が２バイト減少します。（[最大パケットサイズ](../../twelite-net-api-expl/paketto/pakettono.md)）     |
