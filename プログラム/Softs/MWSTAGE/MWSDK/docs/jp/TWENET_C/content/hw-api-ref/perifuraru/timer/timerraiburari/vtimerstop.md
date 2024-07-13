# vTimerStop()

## 解説

Timer の動作を停止する。

## 引数

| 型                                      | 名前   | 詳細          |
| -------------------------------------- | ---- | ----------- |
| ​[tsTimerContext](tstimercontext.md)\* | psTC | タイマー設定の構造体。 |

## 戻り値

なし。

## サンプル

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

