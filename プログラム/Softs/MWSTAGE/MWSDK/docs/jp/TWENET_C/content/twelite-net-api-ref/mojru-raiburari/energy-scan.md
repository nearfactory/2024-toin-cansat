# ENERGY SCAN

{% hint style="danger" %}
現SDK (2018-05, TWELITE NET 1.2.0以降)では、未サポートです。
{% endhint %}

[モジュール](../../twelite-net-api-expl/mojru.md)定義 ToCoNet_REG_MOD_ENERGYSCAN により有効化されます。指定したチャネルレベル（ノイズ・信号を含めた値）を計測します。

チャネルレベルが低いチャネルは通信が少なくまたノイズも小さいことが示唆されますが、その瞬間のチャネルレベルがチャネルの空きを保証するわけではありません。長時間に渡る複数回の評価を行わない限り判定は困難です。

{% hint style="warning" %}
単純ネットのみ利用可能です。
{% endhint %}

{% hint style="danger" %}
本機能は使用するタイミングなどによりハングアップを招く場合があります。無線パケット通信を行わないタイミングでの利用を推奨します。
{% endhint %}

## 定義

```c
// define modules
#define ToCoNet_USE_MOD_ENERGYSCAN

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## 関数

### ToCoNet_EnergyScan_bStart()

エナジースキャンを開始します。完了後 E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE イベントが発生します。

```
bool_t ToCoNet_EnergyScan_bStart(uint32 u32ChMask, uint8 u8Scale)
```

#### 引数

| 型        | 名前           | 詳細                                                                                       |
| -------- | ------------ | ---------------------------------------------------------------------------------------- |
| `uint32` | `u32ChMask`  | 測定するチャネルマスク(ch16のみなら `1UL<<16` を指定、ch16,17なら `1UL<<16\|1UL<<17` を指定)                     |
| `uint8`  | `u8PreScale` | 測定時間。およそ`(2^u8PreScale)*16`\[ms]。測定チャネルの数分必要です。測定時間は１秒程度で内部タイム・アウトするため、これを越えないようにしてください。 |

#### 戻り値

| 型        | 詳細                                          |
| -------- | ------------------------------------------- |
| `boot_t` | `TRUE` なら要求は受け付けられ、`FALSE` なら要求は受け付けられなかった。 |

## イベント

```c
case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
    _C {
        uint8 *pu8Result = (uint8*)u32arg;
    }
```

pu8Result の値

| バイト位置    | 解説                  |
| -------- | ------------------- |
| \[0]     | 計測されたチャネル数\[N]。     |
| \[1]     | 計測された一番若い番号のチャネルレベル |
| \[2]     | 次に若いチャネルレベル         |
| ... \[N] | Nまでデータが続く           |

レベルは 0..255 の値を取り、0 が最弱、255が最強となる。値が高いほどノイズの多いチャネルとなります。

## サンプル

```c
// define module
#define ToCoNet_USE_MOD_ENERGYSCAN

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

void inSomeFunc() {
	// start energy scan (scan 1ch only)
	ToCoNet_EnergyScan_bStart(1 << sAppData.u8channel, 2); 
	sAppData.bTxBusy = TRUE; // set internal busy flag.
}

void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	int i;
	switch(eEvent) {
	case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
		_C {
			uint8 *pu8Result = (uint8*)u32arg;
			sAppData.u8ChEnergy = pu8Result[1];
			
			//save history...
			int i, max = 0;
			static uint8 u8Hist[8] = { 0 };
			static uint8 u8Idx = 0;
			u8Hist[(u8Idx++) & 0x7] = pu8Result[1];
			for (i = 0; i < 8; i++) {
				if (u8Hist[i] > max) max = u8Hist[i];
			}
			sAppData.u8ChEnergyMax = max;
			sAppData.bTxBusy = FALSE;
		}
		break;
	default:
		break;
	}
}
```
