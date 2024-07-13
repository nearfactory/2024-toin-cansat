# ToCoNet_vSleep()

## Explanation

Sleep with [WakeTimer](../../hw-api-ref/perifuraru/waketimer.md).

#### About wake-up interruptions

It is also possible to set an interrupt source other than WakeTimer. It is set beforehand before calling this function.

#### E_EVENT_TOCONET_ON_SLEEP event

Just before sleep, `E_EVENT_TOCONET_ON_SLEEP` is passed to each [user-defined event processing function.](../yzaibento/)

{% hint style="warning" %}
To prevent `ToCoNet_vSleep()` from being called recursively, the following code example handles `E_EVENT_NEW_STATE`.
{% endhint %}

```c
void vMyProcessEv(...) {
    ...
    case E_STATE_SLEEPING:　
        if (eEvent == E_EVENT_NEW_STATE) { 
            ToCoNet_vSleep(…);
        }
        break;
    ...
}
```

#### About sleep time

The WakeTimer works with the accuracy of a 32 kHz oscillator, so the accuracy of the oscillator is very important for the wake-up accuracy. The built-in oscillator must be calibrated before use (unless otherwise specified in `sToCoNet_AppContext`, in which case it will be executed in the TWENET library on cold start).

{% hint style="warning" %}
Extremely short sleep intervals cannot be set. The lower limit is approximately 30ms.

The wake-up time depends on the accuracy of the WakeTimer, but even in the best case there is an error of about ±0.1%.
{% endhint %}

## Arguments

| Type     | Name          | Remark                                                                                                                                                                                                                                                                                                                                                                                                            |
| -------- | ------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint8`  | `u8Device`    | Wake-up timer to be used.                                                                                                                                                                                                                                                                                                                                                                                         |
| `uint32` | `u32Periodms` | Specifies the sleep period in ms.                                                                                                                                                                                                                                                                                                                                                                                 |
| `bool_t` | `bPeriodic`   | Specifies whether periodic sleep should be performed. If cycle sleep is specified, the time elapsed since the last wake-up time is subtracted from the next wake-up time. If the next cycle has already passed, the sleep is performed for the specified period from the time of the call to this function, otherwise the sleep is performed for the specified period from the time of the call to this function. |
| `bool_t` | `bRamOff`     | If `FALSE`, sleep is performed with RAM retained. If `TRUE`, sleep is performed without retaining RAM.                                                                                                                                                                                                                                                                                                            |

## Returns

None

## Sample code

```c
// set UART Rx port as interrupt source
vAHI_DioSetDirection(u32DioPortWakeUp, 0); // set as input

// set DIO wakeup source
(void)u32AHI_DioInterruptStatus(); // clear interrupt register
vAHI_DioWakeEnable(u32DioPortWakeUp, 0); // enable ports
vAHI_DioWakeEdge(u32DioPortWakeUp, 0); // set edge (rising)

// 10sec
ToCoNet_vSleep(E_AHI_WAKE_TIMER_0, 10000, FALSE, FALSE); 
```
