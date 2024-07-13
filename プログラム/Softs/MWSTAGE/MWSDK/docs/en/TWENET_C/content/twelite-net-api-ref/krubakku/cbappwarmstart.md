# cbAppWarmStart()

## 解説

`cbAppWarmStart(bool_t bStart)` is called after RAM retention sleep is restored with `bStart == FALSE` and then `bStart == TRUE`. The `bStart == TRUE` clause is used to initialize the application.

{% hint style="info" %}
When waking from sleep, where RAM is not retained, [`cbAppColdStart()`](cbappcoldstart.md) is called.
{% endhint %}

Some hardware states, such as RAM and [DIO](../../hw-api-ref/perifuraru/dio.md), are retained, so there is no need to reinitialise them.

## Argument

| Type     | Name     | Remark                                                                                            |
| -------- | -------- | ------------------------------------------------------------------------------------------------- |
| `bool_t` | `bStart` | It is called twice on return from sleep, the first time as `FALSE` and the second time as `TRUE`. |

#### bStart

`FALSE`: It is just after the microcontroller wakes up, and no initialization of peripherals etc. has been done. Here you can check the cause of the recovery from sleep (e.g. whether it is due to a timer or a DIO interrupt).

`TRUE`: Called after the initialization of the TWELITE NET library and hardware API, but before the initialization of the library. The following procedure is described here.

* Hardware initialisation (many peripherals are reinitialised; DIO settings are retained during sleep)
* Starts MAC layer by [`ToCoNet_vMacStart()`](../twelite-net-guan-shu/toconet_vmacstart.md) 

## Returns

None

## Sample code

```c
uint32 u32DioPortWakeUp; // DIO interrupt mask bits 
static bool_t bWakeupByButton; // TRUE: woke up by DIO interrupt

void cbAppWarmStart(bool_t bStart)
{
	if (!bStart) {
		// before AHI init, very first of code.
		//  to check interrupt source, etc.
		bWakeupByButton = FALSE;

		if(u8AHI_WakeTimerFiredStatus()) {
			// wake up timer
		} else
		if(u32AHI_DioWakeStatus() & u32DioPortWakeUp) {
			// woke up from DIO events
			bWakeupByButton = TRUE;
		} else {
			bWakeupByButton = FALSE;
		}
	} else {
		// Initialize hardware
		vInitHardware(TRUE);

		// MAC start
		ToCoNet_vMacStart();
	}
}
```

