# BTM library (consecutive reading) DIO input

The BTM library provides a method to determine the state of a button by consecutive reading (determine the change conditional on the value being consecutive).

## Initialisation

In the following example, a change is determined when the value is the same for five consecutive times in a judgment every 8ms. The judgment is called from an external interrupt handler (such as TimerTimer).

```c
#include "btnMgr.h"

#define u32PortInputMask ((1UL << 11) | (1UL << 12) | (1UL << 13))
static tsBTM_Config sBTM_Config; //!< Button input (state is determined by consecutive reading) management structure
static PR_BTM_HANDLER pr_BTM_handler; //!< Event handler for button input (called at the start of TickTimer)

...
    // initialization
    memset(&sBTM_Config, 0, sizeof(tsBTM_Config));
    sBTM_Config.bmPortMask = u32PortInputMask;
    sBTM_Config.u16Tick_ms = 8; // Decision cycle
    sBTM_Config.u8MaxHistory = 5; // Five times the same value.
    sBTM_Config.u8DeviceTimer = 0xFF; // Use TickTimer for this purpose.
    pr_BTM_handler = prBTM_InitExternal(&sBTM_Config);
    
    // Start BTM processing
    vBTM_Enable();
```

## DIO's consecutive reading judgment

The following example calls `pr_BTM_handler` in the interrupt handler of TickTimer. It reads the IO status and performs the consecutive judgment. The method to obtain the state when it changes is described later.

Also, the timer cycle is given as an argument of `pr_BTM_handler`, and each call is calculated internally as if the time had elapsed by this value.

```c
PUBLIC uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;

	switch (u32DeviceId) {
		case E_AHI_DEVICE_TICK_TIMER:
		// Driving the Button Handler
		if (pr_BTM_handler) {
			// Call the handler.
			(*pr_BTM_handler)(4); // Pass the period [ms] of the TickTimer. 
		}
		break;
	}
```

## Detection of DIO changes

Execute the `bBTM_GetState()` function, and if it is `TRUE`, it detects that there is a change due to consecutive reading.

{% hint style="info" %}
After calling `bBTM_GetState()`, the internal state is changed to read. Successive calls will return FALSE (no change) the second time.
{% endhint %}



The following example shows how to check the change of DIO in the event processing part of the TickTimer event (`E_AHI_DEVICE_TICK_TIMER`).

```c
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	case E_AHI_DEVICE_TICK_TIMER: //Relatively frequent processing
		// Determine the button.
		_C {
			uint32 bmPorts, bmChanged, i;
			if (bBTM_GetState(&bmPorts, &bmChanged)) {
				// bmPorts shows the pin value.
				// bmChanged shows the changing pin
				
				if (bmChanged & (1UL << 11)) {
					if (bmPorts & (1UL << 11)) {
	// When there is a change in DIO11 and it is judged to be Lo.
	// For example, make a wireless transmission request here.
					}
				}
			}
		}
		break;
		
```
