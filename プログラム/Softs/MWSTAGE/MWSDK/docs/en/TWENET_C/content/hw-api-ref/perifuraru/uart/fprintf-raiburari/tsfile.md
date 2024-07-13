# tsFILE

A structure defining the output destination specified by [`vfPrintf()`](vfprintf.md) [`vPutChar()`](vputchar.md).

### Member

| `uint8`                                             | u8Device | Serial port (E_AHI_UART\_0 or E_AHI_UART\_1 should be specified)                           |
| --------------------------------------------------- | -------- | ------------------------------------------------------------------------------------------ |
| `bool_t (*bPutChar) (uint8 u8Device, uint8 u8Char)` | bPutChar | Pointer to the function for output, `SERIAL_bTxChar()` is provided for the SERIAL library. |

{% hint style="info" %}
`SERIAL_bTxChar()` puts the bytes passed as `u8Char` into a FIFO queue in the SERIAL library.

By preparing your own output functions, you can also use it to output strings to non-UART devices.
{% endhint %}

### Sample code

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



The following is an example of using this as an output code for a character LCD.

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
