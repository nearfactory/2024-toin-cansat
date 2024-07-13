# ToCoNet_bMacTxReq()

## Explanation

Requests a wireless transmission in a simple network. After registering the request in the transmission queue, this function exits. The actual transmission process is executed later.

{% hint style="warning" %}
Even if you are using a [relay net](../../twelite-net-api-expl/nettowkunitsuite/netto-1/), you can send and receive messages from the [simple net](../../twelite-net-api-expl/nettowkunitsuite/netto/) side by side. If you use this function when the relay net is in operation, unexpected behavior may occur (for example, the relay net may have a lot of communication in the background, and you may throw a transmission request to interrupt the communication).
{% endhint %}

## Argument

| Type                                                                | Name   | Remark                                                                                                              |
| ------------------------------------------------------------------- | ------ | ------------------------------------------------------------------------------------------------------------------- |
| [`tsTxDataApp`](../netto-api/structure/tstoconet_nwk_context.md)`*` | `psTx` | This structure contains the transmission information. It may be allocated as a local variable in the calling scope. |

{% hint style="danger" %}
The memory area of the `tsTxDataApp` structure should always be cleared to zero before storing the value.
{% endhint %}

## Returns

| Type     | Remark                                                                       |
| -------- | ---------------------------------------------------------------------------- |
| `boot_t` | If `TRUE` the request was accepted, if `FALSE` the request was not accepted. |

## Sample code

```c
static int16 i16TransmitIoData() {
  tsTxDataApp sTx;
  memset(&sTx, 0, sizeof(sTx)); // MUST ZERO CLEARED
  uint8 *q = sTx.auData; // fpr use of S_OCTET(), etc.

  // Construct its payload
  S_OCTET(sAppData.u8AppIdentifier);
  S_BE_WORD(sAppData.sIOData_now.u16Volt);
  ...
	
  // Transmit configuration
  sTx.u8Len = q - sTx.auData; // payload length
  sTx.u8Cmd = TOCONET_PACKET_CMD_APP_USER_IO_DATA; // packet type

  // Transmit options
  sTx.u32DstAddr  = TOCONET_MAC_ADDR_BROADCAST; // broadcast
  sTx.u8Retry     = 0x81; // one re-transmit

  // frame sequence number and callback id
  sAppData.u16TxFrame++;
  sTx.u8Seq = (sAppData.u16TxFrame & 0xFF);
  sTx.u8CbId = sTx.u8Seq;

  /* more detailed control options for simple net. */
  sTx.bAckReq = FALSE;
  sTx.u32SrcAddr = sToCoNet_AppContext.u16ShortAddress;
  sTx.u16RetryDur = 4; // Re-transmission interval[ms]
  sTx.u16DelayMax = 16; // Blurring of the transmission start timing (max. 16 ms)

  // transmit request to TWELITE NET.
  //   note: actual transmit occurrs afterwards.
  if (ToCoNet_bMacTxReq(&sTx)) {
    // Success (cbToCoNet_vTxEvent() is fired when the process is finished)
    return sTx.u8CbId;
  } else {
    // failed to place this request
    return -1;
  }
}

```

