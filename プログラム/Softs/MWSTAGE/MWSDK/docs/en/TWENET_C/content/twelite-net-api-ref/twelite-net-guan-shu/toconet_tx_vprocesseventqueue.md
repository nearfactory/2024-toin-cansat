# ToCoNet_Tx_vProcessEventQueue()

## Explanation

Promptly process radio packet transmission requests.

This function is called immediately after the `ToCoNet_bMacTxReq()` request to process the transmission request as soon as possible. If this request is not made, the radio transmission will be processed at the time of the system timer (default 4ms).

{% hint style="warning" %}
A rapid radio transmission occurs when there are no other transmission requests being processed at the time, and no transmission delay has been set for the relevant transmission request. This is useful, for example, if you want to transmit continuously, waiting for a transmission to complete.
{% endhint %}

## Argument

None

## Returns

None

## Sample code

```c
/* Wait for completion of transmission and then make the next packet request */
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if (sAppData.bOnTx) { // Flag during transmission
		vTransmit();
	}
	return;
}

static void vTransmit() {
	_C {
		// transmit Ack back
		tsTxDataApp tsTx;
		memset(&tsTx, 0, sizeof(tsTxDataApp));

		tsTx.u32SrcAddr = ToCoNet_u32GetSerial(); // its own address
		tsTx.u32DstAddr = 0xFFFF; // broadcast

		tsTx.bAckReq = FALSE;

		tsTx.u8Retry = 0; // NO RE-TRANSMIT
		tsTx.u16DelayMin = 0;
		tsTx.u16DelayMax = 0;
		tsTx.u16RetryDur = 0; // No delay, immediately transmitted

		// Payload construction
		...
		
		// Do transmit
		if (ToCoNet_bMacTxReq(&tsTx)) {
			// The transmission request is processed 
			//as soon as it is accepted.
			ToCoNet_Tx_vProcessQueue(); 
		}
	}
}
```

