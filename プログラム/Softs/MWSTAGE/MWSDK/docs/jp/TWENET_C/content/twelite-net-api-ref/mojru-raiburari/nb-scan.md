# NB SCAN

NB SCAN (Neighbour Scan) は、近隣のノードを探索する目的で使用します。

探索を行うノードは、応答要求の同報通信を行います。これに周囲のノードが応え、応答のあったノードの一覧を報告します。探索されたノードは [LQI](../../twelite-net-api-expl/yong-yu.md#lqi) の順にソートされます。

{% hint style="warning" %}
[単純ネット](../../twelite-net-api-expl/nettowkunitsuite/netto/)のみ利用可能。

比較的通信が安定し、通信量も少ない条件では探索の成功率が高くなりますが、条件が悪化すると全く探索できなくなる場合もあります。

条件の悪い場所では、存在を知らせるべきノードが定期的に同報通信によるビーコンパケットを送信し、探索側がこのビーコンを受信しながら近隣ノードの存在を確認する実装を行います。
{% endhint %}

## 定義

探索する側は`ToCoNet_USE_MOD_NBSCAN`を、探索される側は`ToCoNet_USE_MOD_NBSCAN_SLAVE`を定義しておく。

```c
// define modules
#define ToCoNet_USE_MOD_NBSCAN
#define ToCoNet_USE_MOD_NBSCAN_SLAVE

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## 関数

### ToCoNet_EnergyScan_bStart()

近隣探索を開始します。

```
uint8 ToCoNet_NbScan_bStart (uint32 u32ChMask, uint16 u16Dur_ms)
```

#### 引数

| 型      | 名前        | 詳細                                                               |
| ------ | --------- | ---------------------------------------------------------------- |
| uint32 | u32ChMask | 探索するチャネルマスク(ch16のみなら 1UL<<16 を指定、ch16,17なら 1UL<<16\|1UL<<17 を指定) |
| uint16 | u16Dur_ms | 各チャネルの探索時間。おおむね 50ms 以上を推奨値とします。                                 |

#### 戻り値

| 型      | 詳細                                      |
| ------ | --------------------------------------- |
| boot_t | TRUE なら要求は受け付けられ、FALSE なら要求は受け付けられなかった。 |



### ToCoNet_NbScan_bStartToFindAddr()

特定アドレスのノードを見つける目的で、近隣探索を開始します。

```
uint8 ToCoNet_NbScan_bStartToFindAddr (uint32 u32ChMask, uint32 u32Addr)
```

{% hint style="info" %}
同じチャネルのノードが存在するかは、[MAC Ack 付き](../../twelite-net-api-expl/yong-yu.md#mac-ackackackki)のパケット送信を試みれば確認できますが、ノードのチャネルの設定が不明である場合に利用します。
{% endhint %}

#### 引数

| 型        | 名前          | 詳細                                                                                   |
| -------- | ----------- | ------------------------------------------------------------------------------------ |
| `uint32` | `u32ChMask` | 探索するチャネルマスク(ch16のみなら `1UL<<16` を指定、ch16,17なら `1UL<<16\|1UL<<17` を指定)                 |
| `uint32` | `u32Addr`   | 探索するノードの[ロングアドレス](../../twelite-net-api-expl/paketto/adoresuno.md#ronguadoresu)。 |

#### 戻り値

| 型        | 詳細                                          |
| -------- | ------------------------------------------- |
| `boot_t` | `TRUE` なら要求は受け付けられ、`FALSE` なら要求は受け付けられなかった。 |



## イベント

結果は [cbToCoNet_vNwkEvent](../krubakku/cbtoconet_vnwkevent.md)() より [E_EVENT_TOCONET_NWK_SCAN_COMPLETE](../yzaibento/ibento.md) イベントにて報告されます。イベント引数には tsToCoNet_NbScan_Result 構造体へのアドレスが指定されます。

```c
case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
    _C {
        uint8 *pu8Result = (uint8*)u32arg;
    }
```

pu8Result の値

| \[0]     | 計測されたチャネル数 \[N]。     |
| -------- | -------------------- |
| \[1]     | 計測された一番若い番号のチャネルレベル。 |
| \[2]     | 次に若いチャネルレベル。         |
| ... \[N] | Nまで続く。               |

レベルは 0..255 の値を取り、0 が最弱、255が最強となる。値が高いほどノイズの多いチャネルとなります。

## 構造体

### tsToCoNet_NbScan_Result

近隣探索の結果を格納する構造体。

| `uint8`                    | `u8found`                     | 見つかった数。                                                                            |
| -------------------------- | ----------------------------- | ---------------------------------------------------------------------------------- |
| `uint8`                    | `u8scanMode`                  | 探索方法。                                                                              |
| `uint8`                    | `u8IdxLqiSort`                | [LQI](../../twelite-net-api-expl/yong-yu.md#lqi) に順に並べたときの`sScanResult`のインデックス |
| `tsToCoNet_NbScan_Entitiy` | `sScanResult[NBSCAN_MAXLIST]` | 探索されたノード情報                                                                         |

#### u8scanModeのビットマップ

| ビットマスク                              | 解説                                       |
| ----------------------------------- | ---------------------------------------- |
| `TOCONET_NBSCAN_NORMAL_MASK`        | `ToCoNet_EnergyScan_bStart()`による探索       |
| `TOCONET_NBSCAN_QUICK_EXTADDR_MASK` | `ToCoNet_NbScan_bStartToFindAddr()`による探索 |

### tsToCoNet_NbScan_Entitiy

近隣探索で発見された各ノードの情報を格納した構造体。

| 型        | 名前        | 解説                                 |
| -------- | --------- | ---------------------------------- |
| `uint32` | `u32addr` | ロングアドレス。                           |
| `uint16` | `u16addr` | ショートアドレス。                          |
| `uint8`  | `u8ch`    | 発見されたノードの設定されたチャネル。                |
| `uint8`  | `u8lqi`   | 発見されたノードのLQI。                      |
| `uint8`  | `bFound`  | 探索に成功した場合は `TRUE`、失敗した場合は `FALSE`。 |

## サンプル

```c
// define modules
#define ToCoNet_USE_MOD_NBSCAN
#define ToCoNet_USE_MOD_NBSCAN_SLAVE

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	int i;
	switch(eEvent) {
	case E_EVENT_TOCONET_NWK_START:
		break;

	case E_EVENT_TOCONET_NWK_SCAN_COMPLETE:
		_C {
            tsToCoNet_NbScan_Result *pNbsc = (tsToCoNet_NbScan_Result *)u32arg;

            if (pNbsc->u8scanMode & TOCONET_NBSCAN_NORMAL_MASK) { // 通常探索による結果
                // 全チャネルスキャン結果
                for(i = 0; i < pNbsc->u8found; i++) { // 見つかった数
                    // LQI 順に取り出す
                    tsToCoNet_NbScan_Entitiy *pEnt = 
                        &pNbsc->sScanResult[pNbsc->u8IdxLqiSort[i]];
                    if (pEnt->bFound) {
                        // エントリが見つかった
                        //   pEnt->u8ch(チャネル), 
                        //   pEnt->u32addr(ロングアドレス), 
                        //   pEnt->u16addr(ショートアドレス),
                        //   pEnt->u8lqi(LQI値)
                    }
                }
            } else if (pNbsc->u8scanMode & TOCONET_NBSCAN_QUICK_EXTADDR_MASK) { // アドレス指定探索の結果
                if(pNbSc->u8found) {
                    tsToCoNet_NbScan_Entitiy *pEnt = &pNbsc->sScanResult[0];
                    // 見つかった場合は一つだけなので最初の要素を取り出す。
                    //   pEnt->u8ch,
                    //   pEnt->u8lqi(LQI値)
                } else {
                    //　見つからなかった
                }
            }
        }
		break;
```
