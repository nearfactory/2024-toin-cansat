# NB SCAN

NB SCAN (Neighbour Scan) is used to search for neighbouring nodes.

The node performing the search sends a broadcast request for a response. Surrounding nodes respond to this and report a list of nodes that have responded. The nodes are sorted by LQI.

{% hint style="warning" %}
Only available on [SimpleNet](../../twelite-net-api-expl/nettowkunitsuite/netto/).

Under relatively stable and low traffic conditions, the success rate of the search will be high, but if conditions deteriorate, the search may not be possible at all.

In poor conditions, the node that should announce its presence periodically sends beacon packets using broadcast communication, and the searcher receives these beacons and implements a check for the presence of neighbouring nodes.
{% endhint %}

## Definitions

The searcher should define `ToCoNet_USE_MOD_NBSCAN` and the searched should define `ToCoNet_USE_MOD_NBSCAN_SLAVE`.

```c
// define modules
#define ToCoNet_USE_MOD_NBSCAN
#define ToCoNet_USE_MOD_NBSCAN_SLAVE

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"
```

## functions

### ToCoNet_EnergyScan_bStart()

Start the neighbourhood search.

```
uint8 ToCoNet_NbScan_bStart (uint32 u32ChMask, uint16 u16Dur_ms)
```

#### Arguments

| Type   | Name      | Remark                                                                           |
| ------ | --------- | -------------------------------------------------------------------------------- |
| uint32 | u32ChMask | Channel mask to search (`1UL<<16` for ch16 only, `1UL<<16\|1UL<<17` for ch16,17) |
| uint16 | u16Dur_ms | The search time for each channel. Roughly 50ms or more is recommended.           |

#### Returns

| Type   | Remark                                                                       |
| ------ | ---------------------------------------------------------------------------- |
| boot_t | If `TRUE` the request was accepted, if `FALSE` the request was not accepted. |



### ToCoNet_NbScan_bStartToFindAddr()

A neighbourhood search is started with the aim of finding a node at a specific address.

```
uint8 ToCoNet_NbScan_bStartToFindAddr (uint32 u32ChMask, uint32 u32Addr)
```

{% hint style="info" %}
The existence of a node on the same channel can be checked by attempting to send a packet with MAC Ack, but this is used when the node's channel configuration is unknown.
{% endhint %}

#### Arguments

| Type     | Name        | Remark                                                                           |
| -------- | ----------- | -------------------------------------------------------------------------------- |
| `uint32` | `u32ChMask` | Channel mask to search (`1UL<<16` for ch16 only, `1UL<<16\|1UL<<17` for ch16,17) |
| `uint32` | `u32Addr`   | The long address of the node to be searched.                                     |

#### Returns

| Type     | Remark                                                                       |
| -------- | ---------------------------------------------------------------------------- |
| `boot_t` | If `TRUE` the request was accepted, if `FALSE` the request was not accepted. |



## Events

The result is reported by [`cbToCoNet_vNwkEvent()`](../krubakku/cbtoconet_vnwkevent.md) in the `E_EVENT_TOCONET_NWK_SCAN_COMPLETE` event. The event argument is the address to the `tsToCoNet_NbScan_Result` structure.

```c
case E_EVENT_TOCONET_ENERGY_SCAN_COMPLETE:
    _C {
        uint8 *pu8Result = (uint8*)u32arg;
    }
```

the value of `pu8Result`

| \[0]     | Number of channels measured \[N]                        |
| -------- | ------------------------------------------------------- |
| \[1]     | Measured level of the channel with the youngest number. |
| \[2]     | Next youngest channel                                   |
| ... \[N] | Continues up to N.                                      |

The level takes the value 0..255, with 0 being the weakest and 255 the strongest. The higher the value, the noisier the channel.

## Structures

### tsToCoNet_NbScan_Result

A structure to store the results of the neighbourhood search.

| `uint8`                    | `u8found`                     | Count of found nodes                           |
| -------------------------- | ----------------------------- | ---------------------------------------------- |
| `uint8`                    | `u8scanMode`                  | Scan method.                                   |
| `uint8`                    | `u8IdxLqiSort`                | Index of the `sScanResult` when ordered by LQI |
| `tsToCoNet_NbScan_Entitiy` | `sScanResult[NBSCAN_MAXLIST]` | Found node information                         |

#### bitmap information of u8scanMode

| Bitmask                             | Remark                                     |
| ----------------------------------- | ------------------------------------------ |
| `TOCONET_NBSCAN_NORMAL_MASK`        | by the `ToCoNet_EnergyScan_bStart()`       |
| `TOCONET_NBSCAN_QUICK_EXTADDR_MASK` | by the `ToCoNet_NbScan_bStartToFindAddr()` |

### tsToCoNet_NbScan_Entitiy

A structure containing information about each node found in the neighbourhood search.

| Type     | Name      | Remark                                                |
| -------- | --------- | ----------------------------------------------------- |
| `uint32` | `u32addr` | Long address of the node.                             |
| `uint16` | `u16addr` | Short address of the node.                            |
| `uint8`  | `u8ch`    | The channel of the node that was discovered.          |
| `uint8`  | `u8lqi`   | LQI of the discovered node.                           |
| `uint8`  | `bFound`  | If the search succeeds, `TRUE`, if it fails, `FALSE`. |

## Sample code

```c
// define modules
#define ToCoNet_USE_MOD_NBSCAN
#define ToCoNet_USE_MOD_NBSCAN_SLAVE

// includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	int i;
	switch(eEvent) {
	case E_EVENT_TOCONET_NWK_START:
		break;

	case E_EVENT_TOCONET_NWK_SCAN_COMPLETE:
		_C {
            tsToCoNet_NbScan_Result *pNbsc = (tsToCoNet_NbScan_Result *)u32arg;

            if (pNbsc->u8scanMode & TOCONET_NBSCAN_NORMAL_MASK) { // Results from normal search
                // All channel scan results
                for(i = 0; i < pNbsc->u8found; i++) { // Number found
                    // Extract in LQI order
                    tsToCoNet_NbScan_Entitiy *pEnt = 
                        &pNbsc->sScanResult[pNbsc->u8IdxLqiSort[i]];
                    if (pEnt->bFound) {
                        // Entry found
                        //   pEnt->u8ch(channel), 
                        //   pEnt->u32addr(long addr), 
                        //   pEnt->u16addr(short addr),
                        //   pEnt->u8lqi(LQI)
                    }
                }
            } else if (pNbsc->u8scanMode & TOCONET_NBSCAN_QUICK_EXTADDR_MASK) { // アドレス指定探索の結果
                if(pNbSc->u8found) {
                    tsToCoNet_NbScan_Entitiy *pEnt = &pNbsc->sScanResult[0];
                    // If found, there is only one, so the first element is taken out.
                    //   pEnt->u8ch,
                    //   pEnt->u8lqi(LQI)
                } else {
                    // not found
                }
            }
        }
		break;
```
