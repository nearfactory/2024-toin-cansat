# tsSerialPortSetup

シリアルポートの基本設定を行います。利用する UART ハードウェアと入出力用の FIFO メモリの指定を行います。

| uint8   | u8SerialPort           | シリアルポート (E_AHI_UART\_0 または E_AHI_UART\_1 を指定する）                                                            |
| ------- | ---------------------- | ---------------------------------------------------------------------------------------------------------- |
| uint8\* | pu8SerialRxQueueBuffer | Rx (UART受信) 用のFIFOバッファーへのポインタを指定する。このバッファーは静的に確保する。                                                        |
| uint16  | u16SerialRxQueueSize   | Rx FIFOバッファーのサイズ                                                                                           |
| uint8\* | pu8SerialTxQueueBuffer | Tx (UART送信) 用のFIFOバッファーへのポインタを指定する。このバッファーは静的に確保する。                                                        |
| uint16  | u16SerialTxQueueSize   | Tx FIFOバッファーのサイズ                                                                                           |
| uint32  | u32BaudRate            | ボーレート (u32BaudRate) : 9600〜115200を指定します。または MSB を設定し 0x80XX00YY なら、XXがCBP、YYがディバイザの設定となる(半導体ペリフェラルマニュアル参照) |
| uint8   | u8RX_FIFO_LEVEL        | E_AHI_UART_FIFO_LEVEL\_1 を指定する。                                                                            |

{% hint style="danger" %}
#### u32BaudRateについて

 標準的なボーレートは 9600,19200,38400,115200bps です。これ以外の値、また範囲外の値、またCBP,ディバイザによる設定も可能ですが、サポート対応外とします。

高速なボーレート (230400,250000,500000,1000000\[最大値]) を設定した場合、取りこぼし等処理の問題が顕著になる場合があります。
{% endhint %}



### サンプルコード

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
