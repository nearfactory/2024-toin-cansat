# ENERGY SCAN

{% hint style="danger" %}
Not supported by the current SDK (2018-05, TWELITE NET 1.2.0 or later).
{% endhint %}

This is enabled by the [module](../../twelite-net-api-expl/mojru.md) definition `ToCoNet_REG_MOD_ENERGYSCAN`. This module measures the specified channel level (including noise and signal).

A channel with a low channel level implies less communication and less noise, but the channel level at that moment does not guarantee channel availability. It is difficult to determine without multiple evaluations over a long period of time.

{% hint style="warning" %}
Only available on SimpleNet.
{% endhint %}

{% hint style="danger" %}
This function may cause a hang-up depending on the timing of its use. It is recommended to use this function when wireless packet communication is not used.
{% endhint %}

## Definition

```c
// define modules
#define ToCoNet_USE_MOD_ENERGYSCAN

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## functions

### ToCoNet_EnergyScan_bStart()

The energy scan is started. After completion the `E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE` event is raised.

```
bool_t ToCoNet_EnergyScan_bStart(uint32 u32ChMask, uint8 u8Scale)
```

#### Argumtens

| Type     | Name         | Remarks                                                                                                                                                                                  |
| -------- | ------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `uint32` | `u32ChMask`  | Channel mask to be measured (`1UL<<<16` for ch16 only, `1UL<<16\|1UL<<<17` for ch16,17)                                                                                                  |
| `uint8`  | `u8PreScale` | Measurement time. Approximately `(2^u8PreScale)*16`\[ms]. The number of measurement channels is required. The measurement time should not exceed the internal timeout of about 1 second. |

#### Returns

| Type     | Remark                                                                       |
| -------- | ---------------------------------------------------------------------------- |
| `boot_t` | If `TRUE` the request was accepted, if `FALSE` the request was not accepted. |

## Event

```c
case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
    _C {
        uint8 *pu8Result = (uint8*)u32arg;
    }
```

the value of `pu8Result` 

| byte offset | Remark                                                   |
| ----------- | -------------------------------------------------------- |
| \[0]        | Number of channels measured \[N].                        |
| \[1]        | Measured channel level for the youngest numbered channel |
| \[2]        | Next youngest channel number                             |
| ... \[N]    | Data continues up to N                                   |

The level takes the value 0..255, with 0 being the weakest and 255 the strongest. The higher the value, the noisier the channel.

## Sample code

```c
// define module
#define ToCoNet_USE_MOD_ENERGYSCAN

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

void inSomeFunc() {
	// start energy scan (scan 1ch only)
	ToCoNet_EnergyScan_bStart(1 << sAppData.u8channel, 2); 
	sAppData.bTxBusy = TRUE; // set internal busy flag.
}

void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	int i;
	switch(eEvent) {
	case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
		_C {
			uint8 *pu8Result = (uint8*)u32arg;
			sAppData.u8ChEnergy = pu8Result[1];
			
			//save history...
			int i, max = 0;
			static uint8 u8Hist[8] = { 0 };
			static uint8 u8Idx = 0;
			u8Hist[(u8Idx++) & 0x7] = pu8Result[1];
			for (i = 0; i < 8; i++) {
				if (u8Hist[i] > max) max = u8Hist[i];
			}
			sAppData.u8ChEnergyMax = max;
			sAppData.bTxBusy = FALSE;
		}
		break;
	default:
		break;
	}
}
```
