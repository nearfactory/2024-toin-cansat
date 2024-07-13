# vTimerConfig()

## Explanation

Initialize the Timer.

## Argument

| Type                                       | Name | Remark                        |
| ------------------------------------------ | ---- | ----------------------------- |
| ``[`tsTimerContext`](tstimercontext.md)`*` | psTC | Structure for timer settings. |

## Returns

None.

## Sample code

```c
tsTimerContext sTimerApp; // global or static allocation

// set 64ticks/sec
memset(&sTimerApp, 0, sizeof(tsTimerContext));
sTimerApp.u8Device = E_AHI_DEVICE_TIMER0;
sTimerApp.u16Hz = 64;
sTimerApp.u8PreScale = 4; // 15625ct@2^4
```

