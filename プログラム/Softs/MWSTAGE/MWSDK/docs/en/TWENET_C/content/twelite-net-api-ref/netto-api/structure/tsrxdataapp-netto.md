# tsRxDataApp (中継ネット)

[中継ネット](../../../twelite-net-api-expl/nettowkunitsuite/netto-1/)でパケットを受信した時に cbToCoNet_vRxEvent() に渡される構造体。

{% hint style="warning" %}
[単純ネット](../../../twelite-net-api-expl/nettowkunitsuite/netto/)と共通の構造体です。中継ネット、単純ネットどちらかでしか使用されないメンバーもあります。
{% endhint %}

| 定義名                                                                     | 解説                                                                                                                                                                                                                   |
| ----------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint8 u8Cmd`                                                           | パケット種別。送信元で任意に指定できます。データ用、コマンド用といった区分けに使います。値域：0-7                                                                                                                                                                   |
| `uint8 u8Len`                                                           | ペイロード長。後述の `au8Data[]` に格納されるデータ長。                                                                                                                                                                                   |
| `uint8 u8Seq`                                                           | シーケンス番号。送信元で任意に設定できます。重複パケットの判定などに使用します。値域: 0-255                                                                                                                                                                    |
| `uint32 u32SrcAddr`<br>`uint32 u32DstAddr` | 送信元、送信先アドレス。拡張アドレスが格納されます。                                                                                                                                                                                           |
| `uint8 auData[]`                                                        | ペイロード。送信元で任意に設定できます。                                                                                                                                                                                                 |
| `uint8 u8Lqi`                                                           | 受信品質 LQI 値。中継されてきた場合は、最後の中継機のパケットの受信強度になります。値域：0..255 （受信時の電界強度に対応し 0 が最弱、255 が最強）                                                                                                                                   |
| `uint8 u8Tick`                                                          | 受信時のミリ秒カウンタ                                                                                                                                                                                                          |
| `uint8 u8Hops`                                                          | 中継ホップ数（直接受信した場合は 0）                                                                                                                                                                                                  |
| `uint8 u8RouteXOR`                                                      | 中継経路の中継機のアドレスの XOR 値で中継経路の推定に使用します。<br>直接受信した場合は 0 となり、`0x81234567` と `0x82345678` の中継機を経由した場合、`0x81 0x23 0x45 0x67 0x82 0x34 0x56 0x78` の XOR 値  `0x18` が格納されます。 |
| `uint8 u8Lqi1st`                                                        | 最初の中継機が受信したときの LQI 値が格納されます。                                                                                                                                                                                         |
| `uint8 bSecurePkt`                                                      | パケットが暗号化されていた場合 `TRUE` になります。この時点で既に `auData[]` は平文に復号されています。このメンバーが `FALSE` で受信されたパケットの処理をしなければ、平文パケットを無視し、より安全な暗号化パケットのみを処理することになります。                                                                              |

## Can I become who I want to be?

That's a tough question but thankfully, our team is on it. Please bear with us while we're investigating.

## Have you had a chance to answer the previous question?

Yes, after a few months we finally found the answer. Sadly, Mike is on vacations right now so I'm afraid we are not able to provide the answer at this point.

