# ToCoNet_vDebugInit()

## Explanation

Initialisation of the TWELITE NET debug message output.

The specified [UART](../../hw-api-ref/perifuraru/uart/) output is used in common with the application's output, and debug messages are output during the application's output.

{% hint style="warning" %}
Set `TOCONET_DEBUG` to `1` in the [Makefile](../../twelite-sdk-howto/birudonitsuite/makefile-nitsuite.md) and run the build.
{% endhint %}

{% hint style="danger" %}
The debug messages are intended for internal TWELITE NET development and the format of the output will not be disclosed. We may ask you to provide us with the output in order for us to analyse the problem.
{% endhint %}

## Argument

| Type                                                                              | Name   | Remark                                                       |
| --------------------------------------------------------------------------------- | ------ | ------------------------------------------------------------ |
| [`tsFILE`](../../hw-api-ref/perifuraru/uart/fprintf-raiburari/tsfile.md)`*` | `pSer` | Pointer to a structure defining the UART output destination. |

## Returns

None

## Sample code

```c
#include "serial.h"
#include "fprintf.h"

tsFILE sSerStream; // use for fprintf
tsSerialPortSetup sSerPort;

void initser() {
  //... setup serial port
  sSerPort.u8SerialPort = E_AHI_UART_0;
  //...
  SERIAL_vInitEx(&sSerPort, pUartOpt);
  
  // setup fprintf
  sSerStream.bPutChar = SERIAL_bTxChar; // lib function
  sSerStream.u8Device = E_AHI_UART_0; // UART0
  
  // setup TWELITE NET debug message.
  ToCoNet_vDebugInit(&sSerStream);
  ToCoNet_vDebugLevel(0); // start with no debug message.
}
```

