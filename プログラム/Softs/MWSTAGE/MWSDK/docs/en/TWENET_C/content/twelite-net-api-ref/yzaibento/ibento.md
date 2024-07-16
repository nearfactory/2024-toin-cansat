# Events

## Built-on events

| Name                | Remark                                                                                                                                                                                                                                                                                                                                    |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| E_EVENT_START_UP    | This event occurs when the system is woken up (or wakes up from sleep). The event argument is given as `EVARG_START_UP_WAKEUP_MASK` or `(EVARG_START_UP_WAKEUP_MASK \| EVARG_START_UP_WAKEUP_ RAMHOLD_MASK)`, the former means return from DEEP sleep, the latter means return from normal RAM hold sleep.                                |
| E_EVENT_NEW_STATE   | This is the first event called when a state transition occurs. It is not called when the system starts (START_UP event occurs). The event argument is undefined.                                                                                                                                                                          |
| E_EVENT_TICK_TIMER  | Called every system timer (4ms, as default). The event argument is undefined. It is called after the module processing in the system is finished. Because of the variability of the calls, processes which require priority in timing should be described in `E_AHI_DEVICE_TICK_TIMER` in the interrupt handler in `cbToCoNet_u8HwInt()`. |
| E_EVENT_TICK_SECOND | Called every second. The event argument is undefined.                                                                                                                                                                                                                                                                                     |
| E_ORDER_INITIALIZE  | Use this as a message to encourage initialization.                                                                                                                                                                                                                                                                                        |
| E_ORDER_KICK        | Use this as a message to encourage people to start.                                                                                                                                                                                                                                                                                       |

## User-defined events

If you prepare user-defined event names, assign values after `ToCoNet_EVENT_APP_BASE`.

```c
typedef enum
{
	E_EVENT_APP_BASE = ToCoNet_EVENT_APP_BASE,
    E_EVENT_TICK_A,
    E_EVENT_SCAN_FINISH,
    E_EVENT_SLAVE_CONF_FAIL,
    E_EVENT_SLEEP_REQUEST
} teEventApp;
```

{% hint style="warning" %}
Do not apply the compile option to reduce the enum to 16 or 8 bits.
{% endhint %}
