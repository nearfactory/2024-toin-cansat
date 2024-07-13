# tsRxDataApp

無線パケットの受信時に[`cbToCoNet_vRxEvent()`](../krubakku/cbtoconet_vrxevent.md)に渡される構造体。

| 定義名                                    | 解説                                                                                                                                       |
| -------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| `uint8 u8Cmd`                          | パケット種別。送信元で任意に指定します。データ用、コマンド用といった区分けに使います。値域：`0-7`                                                                                      |
| `uint8 u8Len`                          | ペイロード長。後述の `au8Data[]` に格納されるデータ長。（[最大パケットサイズ](../../twelite-net-api-expl/paketto/pakettono.md)）                                     |
| `uint8 u8Seq`                          | シーケンス番号。送信元で任意に設定します。重複パケットの判定などに使用します。値域: `0-255`                                                                                       |
| `uint32 u32SrcAddr  uint32 u32DstAddr` | 送信元、送信先アドレス。`0-0xFFFF` の範囲はショートアドレスモード、`0x8000000` 以上は拡張アドレスによる送受信を示します。`0xFFFF` はブロードキャスト（同報）通信になります。                                   |
| `uint8 auData[]`                       | ペイロード。ペイロード長分のデータが有効（[最大パケットサイズ](https://sdk.twelite.info/\~/edit/drafts/-LAzW5puLXpmlZOxrWPi/twenet_api_overview/packet/packet_maxlen)） |
| `uint8 u8Lqi`                          | 受信品質 LQI 値。値域：0..255 （受信時の電界強度に対応し 0 が最弱、255 が最強）                                                                                        |
| `uint8 bSecurePkt`                     | パケットが暗号化されていた場合 TRUE になります。この時点で既に auData\[] は平文に復号されています。                                                                               |
| `uint32 u32Tick`                       | 受信時のタイムスタンプ([`u32TickCount_ms`](../gurbaru/uint32-u32tickcount_ms.md))                                                                   |
