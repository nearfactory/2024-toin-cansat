# ToCoNet_Nwk_bTx()

## Explanation

Requests the transmission of radio packets on the relay net.

{% hint style="warning" %}
For more information on delivery, see the description of the [`tsTxDataApp`](../structure/tstxdataapp-netto.md) structure.
{% endhint %}

### About the address

You can choose between two delivery addresses.

{% hint style="info" %}
If `TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE` is specified, it is directly transmitted to the neighboring parent or repeater, and the LQI between the child and the parent or repeater can be aggregated to determine which parent or repeater is closer to the child.

Note: The accuracy of the LQI and distance is not high, and it is only possible to distinguish between very close (within 1m) and other positions.
{% endhint %}

| Dst address                        | Remark                                                                                                                                                   |
| ---------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE` | Direct transmission to a reachable parent or repeater (or all of them if there are more than one).                                                       |
| `TOCONET_NWK_ADDR_PARENT`          | The packet is delivered to the parent device. (Even if a repeater receives a packet, it cannot know the reception of the packet by `ToCoNet_vRxEvent()`) |

## Arguments

| Type                                                                | Name        | Remark                                                                  |
| ------------------------------------------------------------------- | ----------- | ----------------------------------------------------------------------- |
| [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md)`*` | pContextNwk | A structure that manages the internal information of the relay network. |
| [`tsTxDataApp`](../structure/tstxdataapp-netto.md)`*`               | sTx         | A structure containing the data to be sent.                             |

## Returns

| Type   | Remark                                                       |
| ------ | ------------------------------------------------------------ |
| bool_t | `TRUE`: The request has been accepted.`FALSE`: not accepted. |

## Sample code

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;
...
	} else {
		// 
		tsTxDataApp sTx;
		memset(&sTx, 0, sizeof(sTx)); // Always clear 0 before you use it!
		uint8 *q =  sTx.auData;
	
		sTx.u32SrcAddr = ToCoNet_u32GetSerial();
	
		if (IS_APPCONF_OPT_TO_ROUTER()) {
			// Received once by the router in the application 
			//and redelivered from the router to the parent unit
			sTx.u32DstAddr = TOCONET_NWK_ADDR_NEIGHBOUR_ABOVE;
		} else {
			// The router does not receive the data in the 
			//application, but simply relays it
			sTx.u32DstAddr = TOCONET_NWK_ADDR_PARENT;
		}
	
		// construct the payload
		S_OCTET('T');
		S_OCTET(sAppData.sFlash.sData.u8id);
		S_BE_WORD(sAppData.u16frame_count);		
​
		// Sensor-specific data
		memcpy(q,pu8Data,u8Len);
		q += u8Len;
	
		sTx.u8Cmd = 0; // Takes a value of 0..7. Use this when you want to separate packet types.
		sTx.u8Len = q - sTx.auData; // Payload size
		sTx.u8CbId = sAppData.u16frame_count & 0xFF; // Number to be notified by TxEvent, not to the destination
		sTx.u8Seq = sAppData.u16frame_count & 0xFF; // Sequence number (to be notified to destination)
		sTx.u8Retry = sAppData.u8Retry;
​
		if (ToCoNet_Nwk_bTx(pNwk, &sTx)) {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		} else {
			// ... Failed (sleep process)
			ToCoNet_Event_SetState(pEv, E_STATE_SLEEP);
		}
```

