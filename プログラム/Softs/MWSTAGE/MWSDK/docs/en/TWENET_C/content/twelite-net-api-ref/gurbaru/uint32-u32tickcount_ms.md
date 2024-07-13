# uint32 u32TickCount_ms

This is the ms timestamp since system startup.

| Remark                                                                                                                                                                                                                                 |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| The timestamp is updated in the [TickTimer](../../hw-api-ref/perifuraru/ticktimer.md) cycle (in the hardware interrupt handler). For example, if the TickTimer period \[ms] is 4 ms, the timestamp will also be counted up by 4. |
| If the maximum count value is exceeded, the value returns to 0.                                                                                                                                                                        |
| By calculating {past timestamp} - {current timestamp} you can calculate the time elapsed since the past (not exceeding the maximum count).                                                                                             |
| The starting value on return from sleep is indefinite. Please do not quote a value saved before sleep and do not process it after sleep.                                                                                               |
