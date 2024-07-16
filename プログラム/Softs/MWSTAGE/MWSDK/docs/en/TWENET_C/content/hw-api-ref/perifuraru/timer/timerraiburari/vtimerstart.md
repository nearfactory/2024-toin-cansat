# vTimerStart()

## Explanation

Start the Timer.

This function can be called for timers that have already started, for example, to change the duty ratio.

## Argument

| Type                                        | Name | Remark                        |
| ------------------------------------------- | ---- | ----------------------------- |
| `​`[`tsTimerContext`](tstimercontext.md)`*` | psTC | Structure for timer settings. |

## Returns

None

## Sample code

```c
// initialize and start
vTimerConfig(&sTimerApp); // initialize
vTimerStart(&sTimerApp); // start

// change duty
sTimerPWM.u16Duty = 256; // set new duty ratio
vTimerStart(&sTimerPWM); // just start again to change duty
```

