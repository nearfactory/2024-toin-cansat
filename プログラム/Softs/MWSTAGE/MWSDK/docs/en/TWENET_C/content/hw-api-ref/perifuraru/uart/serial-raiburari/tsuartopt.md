# tsUartOpt

The options are specified by [`SERIAL_vInitEx()`](serial_vinitex.md). This structure does not need to be statically allocated memory.

{% hint style="danger" %}
In principle, these options are not supported. When using them, please check the output signals with an oscilloscope or similar. The following communication settings are supported as standard.

* No hard flow
* Parity: none
* Stop bit: 1bit
* Data: 8bit
{% endhint %}

{% hint style="warning" %}
The structure must be cleared to 0 before use.
{% endhint %}

| bool_t | bHwFlowEnabled | `TRUE`: Hardware flow enabled `FALSE`: Disabled                                                          |
| ------ | -------------- | -------------------------------------------------------------------------------------------------------- |
| bool_t | bParityEnabled | `TRUE`: Parity enabled `FALSE`: Disabled                                                                 |
| uint8  | u8ParityType   | `E_AHI_UART_EVEN_PARITY`: even `E_AHI_UART_ODD_PARITY`: odd (no need to set value if parity is disabled) |
| uint8  | u8StopBit      | `E_AHI_UART_1_STOP_BIT`: 1bit, `E_AHI_UART_2_STOP_BIT`: 1.5/2bit                                         |
| uint8  | u8WordLen      | `7`: 7bit, `8`: 8bit                                                                                     |

### Sample code

```c
tsUartOpt sUartOpt;
tsSerialPortSetup sSerPort;

void vInitSer() {
	memset(&sUartOpt, 0, sizeof(tsUartOpt));
	sUartOpt.bHwFlowEnabled = FALSE;
	sUartOpt.bParityEnabled = E_AHI_UART_PARITY_ENABLE;
	sUartOpt.u8ParityType = E_AHI_UART_EVEN_PARITY;
	sUartOpt.u8StopBit = E_AHI_UART_2_STOP_BITS;
	sUartOpt.u8WordLen = 7;
	
	/* Create the debug port transmit and receive queues */
	static uint8 au8SerialTxBuffer[256];
	static uint8 au8SerialRxBuffer[256];

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
	
	SERIAL_vInitEx(&sSerPort, &sUartOpt);
}
```
