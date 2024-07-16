# Transmit

The procedure for transmission in a simple network allocates a [tsTxDataApp](../../../twelite-net-api-ref/gou-zao-ti/tstxdataapp.md) structure and stores the information required for transmission in this structure.

1. Allocates the `tsTxDataApp` structure as a local variable and clears the entire structure to zero.
2. Copy the data you wish to send into`.auData` (payload).
3. Store the number of stored bytes of the payload in `.u8Len`.
4. Stores `.u32DstAddr` (destination)
5. Stores `.u32SrcAddr` (the address of the sender)
6. Specify other sending options.
7. call [`ToCoNet_bMacTxReq()`](../../../twelite-net-api-ref/twelite-net-guan-shu/toconet_bmactxreq.md)`.`
8. When the transmission process in TWENET is finished, [`cbToCoNet_TxEvent()`](../../../twelite-net-api-ref/krubakku/cbtoconet_vtxevent.md) is executed.

{% hint style="warning" %}
It is exceptional for a completion event to disappear, but a timeout should be provided to handle exceptions. If the delay or retransmission delay is not set too long in the tsTxDataApp structure, then 100ms is not normally required.
{% endhint %}

### Sample code

```c
static int16 i16TransmitIoData() {
  tsTxDataApp sTx;
  memset(&sTx, 0, sizeof(sTx)); // clear by zero
  uint8 *q = sTx.auData;

  // Compose the payload
  S_OCTET(sAppData.u8AppIdentifier);
  S_BE_WORD(sAppData.sIOData_now.u16Volt);
  ...
	
  // Transmit options
  sTx.u8Len = q - sTx.auData; // packet length
  sTx.u8Cmd = TOCONET_PACKET_CMD_APP_USER_IO_DATA; // Packet type

  // destination
  sTx.u32DstAddr  = TOCONET_MAC_ADDR_BROADCAST; // broadcast address
  sTx.u8Retry     = 0x81; // retry once

  // Specifying frame counts and callback identifiers
  sAppData.u16TxFrame++;
  sTx.u8Seq = (sAppData.u16TxFrame & 0xFF);
  sTx.u8CbId = sTx.u8Seq;

  /* SimpleNet(MAC mode) allows fine-grained specification */
  sTx.bAckReq = FALSE;
  sTx.u32SrcAddr = sToCoNet_AppContext.u16ShortAddress;
  sTx.u16RetryDur = 4; // Resend interval[ms]
  sTx.u16DelayMax = 16; // Create a blur in the timing of the start of transmission(max 16ms)

  // Sending API
  if (ToCoNet_bMacTxReq(&sTx)) {
    // Success (cbToCoNet_vTxEvent() is fired when the process is finished)
    return sTx.u8CbId; // Success on putting a transmit request
  } else {
    // 失敗
    return -1; // Fail
  }
}
```

{% hint style="info" %}
The `S_OCTET()` `S_BE_WORD()` `S_BE_DWORD()` macros are 1,2,4-byte storage macros used when `uint8 *q` is declared as a pointer to the payload storage area.
{% endhint %}

