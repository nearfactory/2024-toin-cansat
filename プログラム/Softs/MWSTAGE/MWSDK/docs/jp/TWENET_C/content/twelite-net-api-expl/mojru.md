# モジュール

TWELITE NET のライブラリの機能取捨選択をコンパイル時に行います。いくつかの機能は依存関係があります。

| モジュール定義                                                                                     | 解説                                                                                                                                                    |
| ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| ToCoNet_REG_MOD_ENERGYSCAN                                                                  | チャネルの入力レベルを計測します。入力時には送受信が不可能になります。<br>関数： ToCoNet_EnergyScan_bStart()イベント：E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE                                |
| ToCoNet_USE_MOD_NBSCAN<br>ToCoNet_USE_MOD_NBSCAN_SLAVE                               | 近隣のモジュールを探索します。関数：ToCoNet_NbScan_bStart(), ToCoNet_NbScan_bStartToFindRole(), ToCoNet_NbScan_bStartToFindAddr()イベント：E_EVENT_TOCONET_NWK_SCAN_COMPLETE |
| ToCoNet_USE_MOD_RAND_XOR_SHIFT <br>                                                  | 乱数生成アルゴリズムを登録します。登録しない場合は、内蔵のハードウェア乱数が使用されます。ただし、連続的に 32bit の乱数系列が必要な場合はソフトウェア乱数を使用します。外部の32kHz水晶振動子や発振器を接続する場合は、XOR_SHIFT の定義が必要です。                  |
| ToCoNet_USE_MOD_NWK_LAYERTREE                                                               | レイヤーツリー型ネットワーク層を利用します。依存モジュール： ToCoNet_USE_MOD_NBSCAN, ToCoNet_USE_MOD_NBSCAN_SLAVE, ToCoNet_USE_MOD_DUPCHK                                           |
| ToCoNet_USE_MOD_NWK\_ LAYERTREE_MININODES                                                   | 同報送信専用のレイヤーツリー型ネットワークにデータ送信可能な省電力「ミニノード」。依存モジュール:  ToCoNet_USE_MOD_DUCHK                                                                              |
| ToCoNet_USE_MOD_DUPCHK                                                                      | パケットの重複チェッカ。最大４０ノードまでの重複パケットを管理する。レイヤーツリー型「ミニノード」を使用する時には必須定義モジュール。                                                                                   |
| ToCoNet_REG_MOD_NWK \_MESSAGE_POOL                                                          | メッセージプール機能を利用します。依存モジュール：ToCoNet_USE_MOD_NWK_LAYERTREE                                                                                                |
| ToCoNet_USE_MOD_CHANNEL_MGR                                                                 | チャネルアジリティを利用します。u32ChMask に指定したチャネルを利用して、複数チャネル駆動の通信を行います。                                                                                            |
| ToCoNet_USE_MOD_TXRXQUEUE_SMALL　ToCoNet_USE_MOD_TXRXQUEUE_MID　ToCoNet_USE_MOD_TXRXQUEUE_BIG | 送信キューのサイズを決定します。SMALLは送信用で 3ヶ、MIDは6ヶ、BIGは20ヶのキューを確保します。パケット分割を行うような一度に多くのパケットを連続的に送信する場合はBIGを指定します。１ヶあたり約128バイトのメモリを消費し、未定義時は MID となります。              |

