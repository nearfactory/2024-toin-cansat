# tsToCoNet_Nwk_Context

中継ネットの基本的なパラメータを定義します。

{% hint style="info" %}
将来の拡張性のためごく基本的な内容を tsToCoNet_Nwk_Context 構造体にまとめ、この構造体を拡張する形で、例えば[`tsToCoNet_NwkLrTr_Context`](../layertree-netto/tstoconet_nwklytr_context.md)構造体を定義します。中継ネットAPIでは、常に `tsToCoNet_Nwk_Context *` にキャストして利用します。
{% endhint %}

| 定義名                                       | 解説                                                                                                                                                                                                                                      |
| ----------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `tsToCoNet_Nwk_\ Context_Base_Info sInfo` | ネットワーク情報の共通構造体。本構造体は主にデバッグ表示目的として参照できる。                                                                                                                                                                                                 |
| `sInfo.u8Role`                            | ネットワークの役割(親機`TOCONET_NWK_ROLE_PARENT`・中継機`TOCONET_NWK_ROLE_ROUTER`・子機`TOCONET_NWK_ROLE_ENDDEVICE`)。                                                                                                                                     |
| `sInfo.u8State`                           | 接続状態を示す。<br>`TOCONET_NWK_STATUS_INACTIVE` (`0x00`) : ネットワークが開始されていない<br>`TOCONET_NWK_STATUS_START_MASK` : ネットワークが開始ビット（未接続の場合も）`TOCONET_NWK_STATUS_READY_MASK` : ネットワークが接続可能を意味するビット |
| `sInfo.u8Layer`                           | 中継機の場合、設定したレイヤ数（接続先のレイヤではなく自身のレイヤ）子機の場合、接続先のレイヤ数                                                                                                                                                                                        |
|  `sInfo.u8NwkTypeId`                      | どの種類のネットワークかを格納する。`TOCONET_NWK_TYPE_LAYER_TREE`はLayerTree ネットワークとなり、本構造体は`tsToCoNet_NwkLyTr_Context *`  にダウンキャストできます。                                                                                                                   |
