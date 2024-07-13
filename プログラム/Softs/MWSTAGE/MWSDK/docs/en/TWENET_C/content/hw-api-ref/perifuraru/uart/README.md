# UART

Two UART hardware channels are available for the TWELITE wireless microcontroller. Each of these UART ports has a hardware FIFO buffer of 16 bytes.

SERIAL library (serial.c and uart.c) is the standard method for processing in TWELITE NET, which handles UART interrupts and provides an API for I/O procedures using FIFO memory allocated in the microcontroller.

### Initialisation

We store the initialization parameters in the [`tsSerialPortSetup`](serial-raiburari/tsserialportsetup.md) structure and call the [`SERIAL_vInit()`](serial-raiburari/serial_vinit.md) function. Here we need to statically allocate a memory area for the FIFO buffer to be used for serial data input and output.

We also provide a [`tsFILE`](fprintf-raiburari/tsfile.md) structure for output.

```c
tsFILE sSerStream;
tsSerialPortSetup sSerPort;

void vSerialInit(uint32 u32Baud) {
	/* Create the debug port transmit and receive queues */
	static uint8 au8SerialTxBuffer[256];
	static uint8 au8SerialRxBuffer[256];

	/* Initialise the serial port to be used for debug output */
	sSerPort.pu8SerialRxQueueBuffer = au8SerialRxBuffer;
	sSerPort.pu8SerialTxQueueBuffer = au8SerialTxBuffer;
	sSerPort.u32BaudRate = u32Baud;
	sSerPort.u16AHI_UART_RTS_LOW = 0xffff;
	sSerPort.u16AHI_UART_RTS_HIGH = 0xffff;
	sSerPort.u16SerialRxQueueSize = sizeof(au8SerialRxBuffer);
	sSerPort.u16SerialTxQueueSize = sizeof(au8SerialTxBuffer);
	sSerPort.u8SerialPort = UART_PORT_SLAVE;
	sSerPort.u8RX_FIFO_LEVEL = E_AHI_UART_FIFO_LEVEL_1;
	SERIAL_vInit(&sSerPort);

	// for vfPrintf()
	sSerStream.bPutChar = SERIAL_bTxChar;
	sSerStream.u8Device = UART_PORT_SLAVE;
}
```

### Output

Use [`vfPrintf()`](fprintf-raiburari/vfprintf.md) or [`vPutChar()`](fprintf-raiburari/vputchar.md) for output. The parameter is a pointer to the tsFILE structure.

{% hint style="warning" %}
The output process may be unstable immediately after initialisation when the system starts or when it returns to sleep. If there is a problem with the operation, please wait some time before outputting.
{% endhint %}



The following is an example of outputting packet information when receiving a wireless packet.

```c
void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	vfPrintf(&sSerStream, LB"[PKT Ad:%04x,Ln:%03d,Seq:%03d,Lq:%03d,Tms:%05d \"",
		pRx->u32SrcAddr,
		pRx->u8Len+4, // Actual payload byte: the network layer uses additional 4 bytes.
		pRx->u8Seq,
		pRx->u8Lqi,
		pRx->u32Tick & 0xFFFF);
	...
```



If you want to wait for the output to complete, such as when all the output is done and then goes to sleep, you can call the [`SERIAL_vFlush()`](serial-raiburari/serial_vflush.md) function. This function will block until the output queue is empty.

{% hint style="warning" %}
Waiting for output to complete may cause instability. In this case, remove this waiting process and check the operation.

If the waiting process is omitted and the output is interrupted, an extra FIFO memory is allocated for the output.
{% endhint %}

### Input

When there is an input, data is put into the FIFO memory registered at the time of initialization in the library. This data is read by `SERIAL_bRxQueueEmpty()` and `SEREAL_i16RxChar()`.

This process is described in the [`cbToCoNet_vMain()`](../../../twelite-net-api-ref/krubakku/cbtoconet_vmain.md) process.。

```c
void cbToCoNet_vMain(void) {
	while (!SERIAL_bRxQueueEmpty(sSerPort.u8SerialPort)) {
		int16 i16Char;

		i16Char = SERIAL_i16RxChar(sSerPort.u8SerialPort);

		vfPrintf(&sSerStream, "\n\r# [%c] --> ", i16Char);
	    SERIAL_vFlush(sSerStream.u8Device);
		...
	}
}
```
