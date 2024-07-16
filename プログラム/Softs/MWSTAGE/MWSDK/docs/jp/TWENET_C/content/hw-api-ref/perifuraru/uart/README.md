# UART

TWELITE 無線マイコンには 2ch の UART ハードウェアが利用できます。この UART ポートは各々16バイトのハードウェアFIFOバッファーがあります。

TWELITE NET での処理については [SERIAL ライブラリ](broken-reference)を用いるのが標準的な方法です。SERIAL ライブラリ内でUART の割り込み処理を行い、マイコン内に割り当てたFIFOメモリを用いた入出力手続きを API としてを提供します。

### 初期化

[tsSerialPortSetup](serial-raiburari/tsserialportsetup.md) 構造体に初期化パラメータを格納して [SERIAL_vInit](serial-raiburari/serial_vinit.md)() 関数を呼び出します。ここでシリアルデータの入力用と出力用に使うFIFOバッファー用のメモリ領域を静的に確保しておく必要があります。

また出力用に [tsFILE 構造体](fprintf-raiburari/tsfile.md)を用意しておきます。

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

### 出力

出力には [vfPrintf](fprintf-raiburari/vfprintf.md)() または [vPutChar](fprintf-raiburari/vputchar.md)() を用います。パラメータに tsFILE 構造体へのポインタを指定します。

{% hint style="warning" %}
システム起動時またはスリープ復帰時の初期化直後の出力処理が不安定になる場合があります。動作に問題がある場合は、時間をおいて出力するようにしてください。
{% endhint %}



以下は無線パケット受信時に、パケットの情報を出力する例です。

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



全ての出力をしてからスリープに入る時など、出力完了を待ちたい場合は [SERIAL_vFlush](serial-raiburari/serial_vflush.md)() 関数を呼び出します。この関数は出力のキューが空になるまでブロックします。

{% hint style="warning" %}
出力完了待ちの処理で動作が不安定になる場合があります。この場合は、この待ち処理を削除して動作確認します。

待ち処理を省略し出力が途切れる場合は、出力用のFIFOメモリを余分に確保します。
{% endhint %}

### 入力

入力があると、ライブラリ内で初期化時に登録したFIFOメモリにデータが投入されます。このデータを [SERIAL_bRxQueueEmpty](serial-raiburari/serial_brxqueueempty.md)() と [SEREAL_i16RxChar](serial-raiburari/serial_i16rxchar.md)() により読み出します。

この処理は [cbToCoNet_vMain](../../../twelite-net-api-ref/krubakku/cbtoconet_vmain.md)() 処理中に記述します。

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
