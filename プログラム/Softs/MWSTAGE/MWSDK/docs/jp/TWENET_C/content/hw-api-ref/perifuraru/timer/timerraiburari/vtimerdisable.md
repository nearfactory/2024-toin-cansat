# vTimerDisable()

Timer を破棄する。

## 引数 <a href="yin-shu" id="yin-shu"></a>

| 型                                      | 名前   | 詳細          |
| -------------------------------------- | ---- | ----------- |
| ​[tsTimerContext](tstimercontext.md)\* | psTC | タイマー設定の構造体。 |

## 戻り値 <a href="ri" id="ri"></a>

なし

## サンプル <a href="sanpuru" id="sanpuru"></a>

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

​\
