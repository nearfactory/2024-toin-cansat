# vTimerStart()

## 解説

Timer を開始する。

本関数は、既に開始済みのTimerについても呼び出し可能です。Duty 比の変更を行う場合などに利用します。

## 引数

| 型                                      | 名前   | 詳細          |
| -------------------------------------- | ---- | ----------- |
| ​[tsTimerContext](tstimercontext.md)\* | psTC | タイマー設定の構造体。 |

## 戻り値

なし

## サンプル

```c
// initialize and start
vTimerConfig(&sTimerApp); // initialize
vTimerStart(&sTimerApp); // start

// change duty
sTimerPWM.u16Duty = 256; // set new duty ratio
vTimerStart(&sTimerPWM); // just start again to change duty
```

