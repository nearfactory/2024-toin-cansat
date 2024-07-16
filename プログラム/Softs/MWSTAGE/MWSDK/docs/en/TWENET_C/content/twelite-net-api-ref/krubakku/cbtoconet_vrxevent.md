# cbToCoNet_vRxEvent()

## Explanation

This function is called after receiving a radio packet. The argument [`tsRxDataApp`](../gou-zao-ti/tsrxdataapp.md)` *psRx` is passed, the received data (packet contents, address information, etc.) will be stored. The structure is only valid within the calling scope of this callback function, once you leave the function the value is not guaranteed.

{% hint style="warning" %}
This function should not be delayed if it is received frequently. This may cause delays in releasing the receive queue and may result in missed data.

Incoming packets are stored in a FIFO queue by an internal MAC layer interrupt. When it comes under the control of the application loop, it is fetched from this queue and a callback function is executed.
{% endhint %}

## Argument

| Type                                              | Name   | Remark                                                                                           |
| ------------------------------------------------- | ------ | ------------------------------------------------------------------------------------------------ |
| [`tsRxDataApp`](../gou-zao-ti/tsrxdataapp.md)` *` | `psRx` | The data received is stored in this structure. See the description of the structure for details. |

## Returns

None

## Sample code

```c
// display packet content 
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	int i;
	static uint16 u16seqPrev = 0xFFFF;
	uint8 *p = pRx->auData; // pointer to the payload

	// print control info
	vfPrintf(&sSerStream, LB"[PKT Ad:%04x,Ln:%03d,Seq:%03d,Lq:%03d,Tms:%05d \"",
			pRx->u32SrcAddr, // source address
			pRx->u8Len, // payload size
			pRx->u8Seq, // seq number
			pRx->u8Lqi, // LQI
			pRx->u32Tick & 0xFFFF); // time stamp
	// print first 32 bytes
	for (i = 0; i < pRx->u8Len; i++) {
		if (i < 32) {
			sSerStream.bPutChar(sSerStream.u8Device,
				(pRx->auData[i] >= 0x20 && pRx->auData[i] <= 0x7f) ? pRx->auData[i] : '.');
		} else {
			vfPrintf(&sSerStream, "..");
			break;
		}
	}
	vfPrintf(&sSerStream, "C\"]");
}
```

