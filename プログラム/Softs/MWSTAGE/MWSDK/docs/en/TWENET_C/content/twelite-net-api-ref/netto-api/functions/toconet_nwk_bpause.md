# ToCoNet_Nwk_bPause()

## Explanation

Pause the RelayNet.

{% hint style="info" %}
Called before sleep is executed.
{% endhint %}

## Argument

| Type                                                                | Name        | Remark                                                                  |
| ------------------------------------------------------------------- | ----------- | ----------------------------------------------------------------------- |
| [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md)`*` | pContextNwk | A structure that manages the internal information of the relay network. |

## Returns

| Type   | Remark                        |
| ------ | ----------------------------- |
| bool_t | `TRUE`: Success `FALSE`: Fail |

## Sample code

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;
...
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
				// RESUME
				ToCoNet_Nwk_bResume(pContextNwk);
			} else {
				// Processing at start-up
				memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
				sNwkLayerTreeConfig.u8Role = 
						TOCONET_NWK_ROLE_ENDDEVICE;
				// Init the network
				pContextNwk = 
					ToCoNet_NwkLyTr_psConfig_MiniNodes(&sNwkLayerTreeConfig);
	
				if (sAppData.pContextNwk) {
					ToCoNet_Nwk_bInit(pContextNwk);
					ToCoNet_Nwk_bStart(pContextNwk);
				}
			}
...
	case E_STATE_APP_SLEEP:
		if (eEvent == E_EVENT_NEW_STATE) {
			// Pause processing of the relay net.
			ToCoNet_Nwk_bPause(sAppData.pContextNwk);
​
			// Enters cyclic sleep
	//  - 5 seconds after the first time, 
	//    5 seconds after the next time, based on the return to sleep
			vSleep(5000, TRUE, FALSE);
		}
```

