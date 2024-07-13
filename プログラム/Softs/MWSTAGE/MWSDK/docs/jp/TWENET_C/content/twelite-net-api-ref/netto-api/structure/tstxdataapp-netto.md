# tsTxDataApp (中継ネット)

[中継ネット](../../../twelite-net-api-expl/nettowkunitsuite/netto-1/)で使用する送信用の構造体。

{% hint style="warning" %}
構造体は必ず 0 で初期化してから使用します。ローカル変数での定義でも構いません。

この構造体は単純ネットと共用して使用されます。単純ネットまたは中継ネットどちらかでしか使わないメンバーも存在します。未使用のメンバーは 0 で初期化されている前提を置きます。
{% endhint %}

| 定義名                                             | 解説                                                                                                                                                                                                                                            |
| ----------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint32 u32DstAddr`                             | 宛先アドレス。以下の定義済みアドレスは特殊な意味合いを持つ。  子機からは親機に送信する際は`TOCONET_NWK_ADDR_PARENT`  を指定するが、電波到達範囲内の中継器に直接送信する`TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE` を指定する。親機からは対象の子機のロングアドレスまたは全ノードに伝達する  `TOCONET_NWK_ADDR_BROADCAST` を指定する。                                |
| `uint8 u8Cmd`                                   | パケット種別。アプリケーションで自由に設定できる。値域は 0..7。                                                                                                                                                                                                            |
| `uint8 u8CbId`                                  | コールバックID。送信要求時に設定した値が [`cbToCoNet_TxEvent()`](../../krubakku/cbtoconet_vtxevent.md) により渡されます。この値によって送信したパケットと送信完了イベントを対応付けます。本値は送信パケットには含まれません。値域：0-255                                                                                       |
| `uint8 au8Data[]`                               | パケットのペイロード([パケットの最大長](../../../twelite-net-api-expl/paketto/pakettono.md))。                                                                                                                                                               |
| `uint8 u8Retry`                                 | LAYERTREE_MININODES で使用します。再送回数は `0x80` (再送ない) `0x81`(再送1回) ... `0x8F`(再送16回) と指定します。                                                                                                                                                         |
| `uint16 u16DelayMin,  u16DelayMax, u16RetryDur` | LayerTree ネットワーク層では使用されない。ただし LAYERTREE_MININODES で使用します。パケットの送信遅延を設定します。内容は [tsTxDataApp](../../gou-zao-ti/tstxdataapp.md) を参照。                                                                                                              |
| `uint16 bSecurePkt`                             | 平文で送信する場合は `FALSE`, 暗号化する場合は `TRUE` を指定します。事前に [`ToCoNet_bRegisterAesKey()`](../../twelite-net-guan-shu/toconet_bregisteraeskey.md) により暗号化鍵を登録しておきます。※最大利用バイト数が２バイト減少します([パケットの最大長](../../../twelite-net-api-expl/paketto/pakettono.md))。 |
