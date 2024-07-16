# BTMライブラリ(連照式 DIO 入力 )

機械式のボタンでは、チャタリングの発生により押し下げが正しく検知できない場合があります。BTMライブラリでは連照によりボタンの状態確定を行う方法を提供します。

## 初期化

以下の例では、8msごとの判定で、５連続で同一値となった場合に変化を確定します。判定は、外部の割り込みハンドラ([TimerTimer](../hw-api-ref/perifuraru/ticktimer.md) など)から呼び出します。

```c
#include "btnMgr.h"

#define u32PortInputMask ((1UL << 11) | (1UL << 12) | (1UL << 13))
static tsBTM_Config sBTM_Config; //!< ボタン入力（連照により状態確定する）管理構造体
static PR_BTM_HANDLER pr_BTM_handler; //!< ボタン入力用のイベントハンドラ (TickTimer 起点で呼び出す)

...
    // 初期化
    memset(&sBTM_Config, 0, sizeof(tsBTM_Config));
    sBTM_Config.bmPortMask = u32PortInputMask;
    sBTM_Config.u16Tick_ms = 8; // 判定周期
    sBTM_Config.u8MaxHistory = 5; // ５回同じ値で確定
    sBTM_Config.u8DeviceTimer = 0xFF; // TickTimer を流用する。
    pr_BTM_handler = prBTM_InitExternal(&sBTM_Config);
    
    // BTM 処理開始
    vBTM_Enable();
```

## DIOの連照判定

以下の例では[TickTimer](../hw-api-ref/perifuraru/ticktimer.md)の割り込みハンドラ中で`pr_BTM_handler` を呼び出します。IO 状態を読み出して、連照判定を実施します。状態が変化したときの取得方法は後述します。

また`pr_BTM_handler` の引数としてタイマー周期を与えており、呼び出しごとにこの値分だけ時間が経過したとして内部計算されます。

```c
PUBLIC uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;

	switch (u32DeviceId) {
		case E_AHI_DEVICE_TICK_TIMER:
		// ボタンハンドラの駆動
		if (pr_BTM_handler) {
			// ハンドラを稼働させる
			(*pr_BTM_handler)(4); // TickTimer の周期[ms]を入れる。 
		}
		break;
	}
```

## DIOの変化の検出

`bBTM_GetState()` 関数を実行し `TRUE` のときに、連照による変化があったと検出します。

{% hint style="info" %}
`bBTM_GetState()` 呼び出し後、内部状態が読み出し済みに変化します。連続して呼び出しても２回目は `FALSE` （変化なし）と返します。
{% endhint %}



以下の例では [TickTimer](../hw-api-ref/perifuraru/ticktimer.md)のイベント(`E_AHI_DEVICE_TICK_TIMER` )のイベント処理部で [DIO](../hw-api-ref/perifuraru/dio.md) の変化を確認しています。

```c
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	case E_AHI_DEVICE_TICK_TIMER: //比較的頻繁な処理
		// ボタンの判定を行う。
		_C {
			uint32 bmPorts, bmChanged, i;
			if (bBTM_GetState(&bmPorts, &bmChanged)) {
				// bmPorts がポートの値
				// bmChanged が変化があったポート
				
				if (bmChanged & (1UL << 11)) {
					if (bmPorts & (1UL << 11)) {
						// DIO11 に変化があり Lo と判定された場合
						// 例えば、ここで無線送信要求を行う。
					}
				}
			}
		}
		break;
		
```
