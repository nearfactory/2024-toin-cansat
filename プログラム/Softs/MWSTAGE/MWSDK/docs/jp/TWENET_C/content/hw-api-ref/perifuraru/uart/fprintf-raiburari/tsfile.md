# tsFILE

[vfPrintf](vfprintf.md)() [vPutChar](vputchar.md)() で指定する出力先を定義した構造体。

### メンバー

| uint8                                              | u8Device | シリアルポート (E_AHI_UART\_0 または E_AHI_UART\_1 を指定する）                 |
| -------------------------------------------------- | -------- | --------------------------------------------------------------- |
| bool_t (\*bPutChar) (uint8 u8Device, uint8 u8Char) | bPutChar | 出力用の関数ポインタ。SERIAL ライブラリ用には SERIAL_bTxChar() が準備されているので、これを指定する。 |

{% hint style="info" %}
SERIAL_bTxChar() は、u8Char として渡されたバイトを SERIAL ライブラリ内の FIFO キューに投入します。

独自に出力関数を準備することで、UART 以外への文字列の出力にも利用できます。
{% endhint %}

### サンプルコード

```c
#include "serial.h"
#include "fprintf.h"

tsFILE sSerStream;
tsSerialPortSetup sSerPort;

void vSerialInit(uint32 u32Baud, tsUartOpt *pUartOpt) {
	// initialize sSerPort
	...
	SERIAL_vInit(&sSerPort); 

	// for vfPrintf()
	sSerStream.bPutChar = SERIAL_bTxChar;
	sSerStream.u8Device = E_AHI_UART_0;
}

void vSerOut() {
    vfPrintf(&sSerStream, "HELLO!");
}

```



以下は、キャラクタ LCD の出力コードとして利用した一例です。

```c
#include "serial.h"
#include "fprintf.h"

tsFILE sLcdStream;

// handle LCD display
PUBLIC bool_t LCD_bTxChar(uint8 u8Device, uint8 u8Data) {
	int i;

	switch (u8Data) {
	case '\n':
	...
}

void vInitHardware() {
    /* Initisalise the LCD */
    vLcdReset(3, 0);
    
    /* register for vfPrintf() */
    sLcdStream.bPutChar = LCD_bTxChar;
    sLcdStream.u8Device = 0xFF;
}

void vSomeOutput() {
    vfPrintf(&sLcdStream, "Hello World!\n");
}
```
