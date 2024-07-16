# vTimerStop()

## Explanation

Stop the Timer.

## Argument

| Type                                        | Name | Remark                        |
| ------------------------------------------- | ---- | ----------------------------- |
| `​`[`tsTimerContext`](tstimercontext.md)`*` | psTC | Structure for timer settings. |

## Returns

None.

## Sample code

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

