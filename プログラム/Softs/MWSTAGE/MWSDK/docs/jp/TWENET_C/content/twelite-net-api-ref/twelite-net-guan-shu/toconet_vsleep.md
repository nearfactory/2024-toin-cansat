# ToCoNet_vSleep()

## 解説

[WakeTimer](../../hw-api-ref/perifuraru/waketimer.md) によるスリープを行います。

#### 起床割り込みについて

WakeTimer 以外の割り込み源を設定することもできます。本関数呼び出し前にあらかじめ設定しておきます。

#### E_EVENT_TOCONET_ON_SLEEP イベント

スリープ直前に、各[ユーザ定義イベント処理](../yzaibento/)関数に `E_EVENT_TOCONET_ON_SLEEP` が伝達されます。

{% hint style="warning" %}
`ToCoNet_vSleep()` が再帰的に呼び出されないように、以下のコード例では  `E_EVENT_NEW_STATE` の処理としています。
{% endhint %}

```c
void vMyProcessEv(...) {
    ...
    case E_STATE_SLEEPING:　
        if (eEvent == E_EVENT_NEW_STATE) { 
            ToCoNet_vSleep(…);
        }
        break;
    ...
}
```

#### スリープ時間について

WakeTimer は 32kHz のオシレータの精度により動作する都合、起床精度についてはオシレータの精度が大きく関わります。内蔵のオシレータは必ずキャリブレーションを実施してから利用するようにしてください（特に[`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md)での指定がなければコールドスタート時に TWENET ライブラリ内で実行されます）。

{% hint style="warning" %}
極端に短いスリープ間隔は設定できません。概ね30ms程度を下限とします。

起床時間については WakeTimer の精度次第ですが、もっとも良い場合でも±1ms程度の誤差は発生します。
{% endhint %}

## 引数

| 型        | 名前            | 詳細                                                                                                                                                |
| -------- | ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint8`  | `u8Device`    | 使用するウェイクアップタイマー。                                                                                                                                  |
| `uint32` | `u32Periodms` | スリープ期間を ms で指定する。                                                                                                                                 |
| `bool_t` | `bPeriodic`   | 周期スリープを行うかどうかを指定します。周期スリープを指定した場合、前回の起床時間からの経過時間を差し引いて次の起床を行います。すでに次の周期をすぎていた場合は本関数呼び出し時点から指定期間のスリープとし、周期スリープを指定しない場合、本関数呼び出し時点から指定期間のスリープを行います。 |
| `bool_t` | `bRamOff`     | `FALSE `なら RAM を保持した状態でスリープを行います。`TRUE` なら、RAMを保持しないスリープを行います。                                                                                    |

## 戻り値

なし。

## サンプル

```c
// set UART Rx port as interrupt source
vAHI_DioSetDirection(u32DioPortWakeUp, 0); // set as input

// set DIO wakeup source
(void)u32AHI_DioInterruptStatus(); // clear interrupt register
vAHI_DioWakeEnable(u32DioPortWakeUp, 0); // enable ports
vAHI_DioWakeEdge(u32DioPortWakeUp, 0); // set edge (rising)

// 10sec
ToCoNet_vSleep(E_AHI_WAKE_TIMER_0, 10000, FALSE, FALSE); 
```
