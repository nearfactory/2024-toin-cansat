# vTimerDisable()

Disable the Timer hardware.

## Argument <a href="yin-shu" id="yin-shu"></a>

| Type                                        | Name | Remark                        |
| ------------------------------------------- | ---- | ----------------------------- |
| `​`[`tsTimerContext`](tstimercontext.md)`*` | psTC | Structure for timer settings. |

## Returns <a href="ri" id="ri"></a>

None.

## Sample code <a href="sanpuru" id="sanpuru"></a>

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
