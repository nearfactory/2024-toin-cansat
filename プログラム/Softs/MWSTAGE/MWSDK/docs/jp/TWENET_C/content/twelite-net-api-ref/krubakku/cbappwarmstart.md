# cbAppWarmStart()

## 解説

`cbAppWarmStart(bool_t bStart)` は、`bStart == FALSE` で RAM 保持スリープ復帰後に呼び出され、その後 `bStart == TRUE` で呼び出されます。`bStart == TRUE` の節でアプリケーションの初期化処理を行います。

{% hint style="info" %}
RAMが保持されないスリープからの復帰時は、[`cbAppColdStart()`](cbappcoldstart.md) が呼び出されます。
{% endhint %}

RAMや[DIO](../../hw-api-ref/perifuraru/dio.md)等の一部のハードウェア状態は保持されるため、これらの再初期化は不要です。

## 引数

| 型        | 名前       | 詳細                                        |
| -------- | -------- | ----------------------------------------- |
| `bool_t` | `bStart` | スリープ復帰時に２回呼び出され、最初が`FALSE`、2回目が`TRUE`となる。 |

#### bStart

`FALSE`：マイコンの起動直後で、ペリフェラル等の初期化も行われていない。ここではスリープからの復帰要因（タイマーによるものなのかDIO割り込みによるものかなど）を確認できます。

`TRUE`：TWELITE NET ライブラリやハードウェアAPIの初期化が行わた後、ライブラリの初期化前で呼び出される。ここでは以下の手続きを記述します。

* ハードウェアの初期化（多くのペリフェラルは再初期化します。DIOの設定はスリープ中も保持されます）
* [`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) によるMAC 層の開始

## 戻り値

なし

## サンプル

```c
uint32 u32DioPortWakeUp; // DIO interrupt mask bits 
static bool_t bWakeupByButton; // TRUE: woke up by DIO interrupt

void cbAppWarmStart(bool_t bStart)
{
	if (!bStart) {
		// before AHI init, very first of code.
		//  to check interrupt source, etc.
		bWakeupByButton = FALSE;

		if(u8AHI_WakeTimerFiredStatus()) {
			// wake up timer
		} else
		if(u32AHI_DioWakeStatus() & u32DioPortWakeUp) {
			// woke up from DIO events
			bWakeupByButton = TRUE;
		} else {
			bWakeupByButton = FALSE;
		}
	} else {
		// Initialize hardware
		vInitHardware(TRUE);

		// MAC start
		ToCoNet_vMacStart();
	}
}
```

