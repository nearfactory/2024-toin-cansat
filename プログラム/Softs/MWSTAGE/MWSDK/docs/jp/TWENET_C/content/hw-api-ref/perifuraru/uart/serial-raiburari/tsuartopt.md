# tsUartOpt

[SERIAL_vInitEx](serial_vinitex.md)() によるオプションの指定を行います。この構造体は静的にメモリ確保する必要はありません。

{% hint style="danger" %}
これらオプションについては、原則としてサポート対象外です。利用にあたっては出力信号についてはオシロスコープなどで確認してください。標準的にサポートする通信設定は以下です。

* ハードフローなし
* パリティなし
* ストップビット 1bit
* データ長 8bit
{% endhint %}

{% hint style="warning" %}
構造体は、必ず 0 クリアしてから利用します。
{% endhint %}

| bool_t | bHwFlowEnabled | TRUE:ハードウェアフロー有効 FALSE:無効                                             |
| ------ | -------------- | --------------------------------------------------------------------- |
| bool_t | bParityEnabled | TRUE: パリティ有効 FALSE:無効                                                 |
| uint8  | u8ParityType   | E_AHI_UART_EVEN_PARITY:偶数 E_AHI_UART_ODD_PARITY:奇数 (パリティ無効時は値の設定不要) |
| uint8  | u8StopBit      | E_AHI_UART\_1\_STOP_BIT: 1bit, E_AHI_UART\_2\_STOP_BIT: 1.5/2bit      |
| uint8  | u8WordLen      | 7: 7bit, 8: 8bit                                                      |

### サンプルコード

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
