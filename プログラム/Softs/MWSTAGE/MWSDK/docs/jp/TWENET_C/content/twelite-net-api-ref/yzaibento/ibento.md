# イベント

## 組み込みイベント

| 定義                  | 解説                                                                                                                                                                                     |
| ------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| E_EVENT_START_UP    | システム起動（またはスリープ復帰）時に発生します。イベント引数は EVARG_START_UP_WAKEUP_MASK または(EVARG_START_UP_WAKEUP_MASK \| EVARG_START_UP_WAKEUP\_ RAMHOLD_MASK) が与えられ、前者はDEEPスリープからの復帰、後者は通常のRAM保持スリープからの復帰を意味します。 |
| E_EVENT_NEW_STATE   | 状態遷移したときに最初に呼ばれるイベントです。※ システム始動時には呼び出されません（START_UPイベントが発生する）。イベント引数は未定義。                                                                                                              |
| E_EVENT_TICK_TIMER  | システムタイマー(4ms)毎に呼び出されます。イベント引数は未定義。※ システム内のモジュール処理が終わった後に呼ばれます。呼び出しはばらつきが発生するため、タイミングを優先したい処理は cbToCoNet_u8HwInt() 中の割り込みハンドラ中の E_AHI_DEVICE_TICK_TIMER に記述します。                         |
| E_EVENT_TICK_SECOND | １秒ごとに呼び出されます。イベント引数は未定義。                                                                                                                                                               |
| E_ORDER_INITIALIZE  | 初期化を促すメッセージとして利用する。                                                                                                                                                                    |
| E_ORDER_KICK        | 開始を促すメッセージとして利用する。                                                                                                                                                                     |

## ユーザ定義イベント

ユーザ定義のイベント名を用意する場合は、ToCoNet_EVENT_APP_BASE以降の値を割り振るようにしてください。

```c
typedef enum
{
	E_EVENT_APP_BASE = ToCoNet_EVENT_APP_BASE,
    E_EVENT_TICK_A,
    E_EVENT_SCAN_FINISH,
    E_EVENT_SLAVE_CONF_FAIL,
    E_EVENT_SLEEP_REQUEST
} teEventApp;
```

{% hint style="warning" %}
enum を 16bit や 8bit に小さくするコンパイルオプションは適用しないでください。
{% endhint %}
