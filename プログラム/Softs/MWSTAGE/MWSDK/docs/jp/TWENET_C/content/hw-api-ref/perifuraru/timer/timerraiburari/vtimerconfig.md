# vTimerConfig()

## 解説

Timer の初期化を行う。

## 引数

| 型                                     | 名前   | 詳細          |
| ------------------------------------- | ---- | ----------- |
| [tsTimerContext](tstimercontext.md)\* | psTC | タイマー設定の構造体。 |

## 戻り値

なし

## サンプル

```c
tsTimerContext sTimerApp; // global or static allocation

// set 64ticks/sec
memset(&sTimerApp, 0, sizeof(tsTimerContext));
sTimerApp.u8Device = E_AHI_DEVICE_TIMER0;
sTimerApp.u16Hz = 64;
sTimerApp.u8PreScale = 4; // 15625ct@2^4
```

