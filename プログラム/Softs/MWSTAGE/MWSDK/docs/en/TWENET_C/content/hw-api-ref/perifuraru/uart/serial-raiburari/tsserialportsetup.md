# tsSerialPortSetup

Configure the basic settings for the serial port. Specify the UART hardware to be used and the FIFO memory for input and output.

| uint8   | u8SerialPort           | Serial port (E_AHI_UART\_0 or E_AHI_UART\_1 should be specified)                                                                                                                |
| ------- | ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| uint8\* | pu8SerialRxQueueBuffer | Pointer to a FIFO buffer for Rx (UART receive). This buffer shall be statically allocated.                                                                                      |
| uint16  | u16SerialRxQueueSize   | Size of the Rx FIFO buffer                                                                                                                                                      |
| uint8\* | pu8SerialTxQueueBuffer | Pointer to a FIFO buffer for Tx (UART transmission). This buffer shall be statically allocated.                                                                                 |
| uint16  | u16SerialTxQueueSize   | Size of the Tx FIFO buffer                                                                                                                                                      |
| uint32  | u32BaudRate            | Baud rate (`u32BaudRate`): Specify 9600 to 115200. Or set the MSB to `0x80XX00YY`, where `XX` is the CBP and `YY` is the divider setting (see semiconductor peripherals manual) |
| uint8   | u8RX_FIFO_LEVEL        | `E_AHI_UART_FIFO_LEVEL_1` is specified.                                                                                                                                         |

{% hint style="danger" %}
#### about u32BaudRate

The standard baud rates are 9600, 19200, 38400 and 115200 bps. Other values, out of range values, CBP and divider settings are possible, but are not supported.

If a high baud rate (230400,250000,500000,1000000 \[maximum]) is set, processing problems such as dropouts may become more pronounced.
{% endhint %}



### Sample code

```c
tsSerialPortSetup sSerPort;

void vInitSer() {
	/* Create the debug port transmit and receive queues */
	static uint8 au8SerialTxBuffer[256];
	static uint8 au8SerialRxBuffer[256];
​
	/* Initialise the serial port to be used for debug output */
	memset(&sSerPort, 0, sizeof(tsSerialPortSetup));
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
}
```
