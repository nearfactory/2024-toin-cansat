# Timer

16bit カウンタを用いるハードウェアタイマー(TIMER0 .. 4) を利用可能です。

このタイマーは、PWM 出力などのペリフェラルとして利用することもできますし、ソフトウェア処理を行うハードウェアタイマーとして利用することも可能です。

## ポートの割り当て

| タイマー番号 | 主        | 副     | 副2    | 解説           |
| ------ | -------- | ----- | ----- | ------------ |
| TIMER0 | 8, 9, 10 | 2,3,4 |       | 汎用タイマー機能利用可能 |
| TIMER1 | 11       | 5     |       | PWM 出力専用     |
| TIMER2 | 12       | 6     | DO0\* | PWM 出力専用     |
| TIMER3 | 13       | 7     | DO1\* | PWM 出力専用     |
| TIMER4 | 17       | 8     |       | PWM 出力専用     |

{% hint style="info" %}
ペリフェラル API vAHI_TimerSetLocation() により、副割り当てに変更できます。

\* TIMER2と3 は DO0,1 に割り当てを変更できます。ただし、これらのポートについては、電源投入時に Vcc レベルになっていないと TWELITE 無線マイコンが正常起動しないことがあります。
{% endhint %}

{% hint style="warning" %}
TWELITE RED ではさらにPWM出力系統が増えていますが、TWELITE NET での標準的な対応は TIMER0..4 とします。
{% endhint %}



## 初期化と開始、終了

事前にペリフェラルマニュアルを参照の上、Timer で利用しないポートを宣言します。この宣言をしないと、Timer の初期化後、該当ポートは汎用のIOポートとして利用できなくなります。

以下の例では bit0,1,2 をセットします。これは TIMER0 で使用する PWM 用のピンを全て汎用IO向けに解放します。

```c
vAHI_TimerFineGrainDIOControl(0x7);
```

> #### tsTimerContext() のパラメータ

| <br>Bit | Timer Input/Output |
| -------------- | ------------------ |
| 0              | TIMER 0 ゲート/イベント入力 |
| 1              | TIMER 0 キャプチャー入力   |
| 2              | TIMER 0 PWM 出力     |
| 3              | TIMER 1 PWM 出力     |
| 4              | TIMER 2 PWM 出力     |
| 5              | TIMER 3 PWM 出力     |
| 6              | TIMER 4 PWM 出力     |
| 7              | 予約                 |

次に Timer ライブラリ用の初期化構造体 [tsTimerContext](timerraiburari/tstimercontext.md) を静的に確保し、設定を行います。

必要なタイマーの周波数に対して適切な PreScale を選択するようにします。

{% hint style="info" %}
プリスケール(u8PreScale) は、要求する周期（周波数）に対して、多くの場合はより小さい値を選択します。

Timer は 16Mhz のカウンター値をプリスケールしており、１周期のカウント値(Ct)は以下の式で与えられます。

```
Ct = 16,000,000 / 2^p / hz
    p  : プリスケール値
    hz : Timer 周期
```

この Ct 値が 16bit の最大数(65535)を超えないように設定します。例えば64Hzのタイマーが欲しい場合は p=2 で Ct = 62500, p=4 で Ct = 15625 となります。 より正確なDuty比のPWM出力が必要な場合は、少しでも小さな p=2 を選択しますが、単純にソフトウェア用のタイマーが必要な場合は p=4 でも違いはありません。
{% endhint %}

```c
tsTimerContext sTimerApp; // global or static allocation

// set 64ticks/sec
memset(&sTimerApp, 0, sizeof(tsTimerContext));
sTimerApp.u8Device = E_AHI_DEVICE_TIMER0;
sTimerApp.u16Hz = 64;
sTimerApp.u8PreScale = 4; // 15625ct@2^4
```

最後に、Timer を開始します。

```c
vTimerConfig(&sTimerApp); // initialize
vTimerStart(&sTimerApp); // start timer
```

Timer を一時的に停止させるには、vTimerStop() を呼び出します。完全に無効化するには vTimerDisable() を呼び出します。

```c
// just stop the timer
vTimerStop(&sTimerApp);
...
// restart
vTimerStart(&sTimerApp);
...
// now, disable timer completely
vTimerStop(&sTimerApp);
vTimerDisable(&sTimerApp);
```

## タイマー割り込み

ToCoNet_u8HwInt() ToCoNet_vHwEvent() の u8DeviceID パラメータが E_AHI_DEVICE_TIMER0 から E_AHI_DEVICE_TIMER4 をとります。

{% hint style="warning" %}
割り込み周波数が高くなるとハード割り込みやイベントの処理が無視できなくなります。

* 不要なタイマーの割り込みは無効にしておく
  * tsTimerContext の bDisableInt を TRUE に設定する
* 割り込み処理のみで対応し、ハードイベントは発生させない
  * 割り込みハンドラを TRUE で return する
{% endhint %}

```c
// hardware interrupt (return quickly!)
uint8 cbToCoNet_u8HwInt(
		uint32 u32DeviceId, 
		uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;

	switch (u32DeviceId) {
	case E_AHI_DEVICE_TIMER0:
		_C {
			static bool_t b;
			b = !b;
			vPortSet_TrueAsLo(PORT_DO1, b);
			
			u8handled = FALSE; // if TRUE, no HwEvent follows.
		}
		break;
	}
}

// hardware event (in the application loop)
//   where we can make a bigger job, like RF transmit.
void cbToCoNet_vHwEvent(
		uint32 u32DeviceId,
		uint32 u32ItemBitmap) {
		
	switch (u32DeviceId) {
	case E_AHI_DEVICE_TIMER0:
		vPutChar(&sSerStream, 'x'); // put x every tick
		if (...) {
			...
			ToCoNet_bMacTxReq(...); // send a packt
		}
		break;
	}	
	
}
```

## PWM 出力

Timerの出力として PWM 出力を設定することが可能です。

{% hint style="info" %}
Duty比の精度を向上させるため、上述のプリスケール値を十分小さい値にしておくことを推奨します。
{% endhint %}

PWM出力を得るためには以下の設定が必要です。

* [tsTimerContext](timerraiburari/tstimercontext.md) の bPWMOut を TRUE に設定する
* [tsTimerContext](timerraiburari/tstimercontext.md) の u16duty を 0..1024 の値に設定する
* vAHI_TimerFineGrainDIOControl() で出力を無効にしていない

```c
tsTimerContext sTimerPWM; // global or static allocation

// initialize
memset(&sTimerPWM, 0, sizeof(tsTimerContext));
sTimerPWM.u8Device = E_AHI_DEVICE_TIMER1;
sTimerPWM.u16Hz = 1000; // 1Khz
sTimerPWM.u8PreScale = 0;
sTimerPWM.u16duty = 512; // 50%
sTimerPWM.bPWMout = TRUE; // PWM out
sTimerPWM.bDisableInt = TRUE; // no interrupt (for CPU save)
// start
vTimerConfig(&sTimerPWM);
vTimerStart(&sTimerPWM);

...
// change duty
sTimerPWM.u16Duty = 256; // set new duty ratio
vTimerStart(&sTimerPWM); // just start again to change duty
```
