# tsToCoNet_NwkLyTr_Context

[`ToCoNet_NwkLyTr_psConfig()`](toconet_nwklytr_psconfig.md) のパラメータとして LayterTree ネットワークを設定します。本構造体はローカル変数で確保しても構いません。

| 定義名                                       | 解説                                                                                               |
| ----------------------------------------- | ------------------------------------------------------------------------------------------------ |
| `tsToCoNet_Nwk_Context_\ Base_Info sInfo` | ネットワーク情報の共通構造体。                                                                                  |
| `tsToCoNet_NwkLyTr_Config sConf`          | 設定情報です。[`ToCoNet_NwkLyTr_psConfig()`](toconet_nwklytr_psconfig.md) による設定が行われたときに、規定値の展開などが行われます。 |
| `uint32 u32AddrHigherLayer`               | 接続先の上位ノードのアドレス                                                                                   |
| `uint8 u8Ct_Second_To_Rescan`             | 上位を再探索するまでのカウント \[秒]                                                                             |
| `uint8 u8Ct_Second_To_Relocate`           | 上位の存在確認を行うまでのカウント \[秒]                                                                           |
| `uint8 u8Ct_LostParent`                   | 上位の存在確認の失敗数カウント （この値が規定回数まで達すると上位喪失として、再探索が行われる）                                                 |
