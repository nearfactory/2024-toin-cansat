# SERIAL_vInit()

## Explanation

Initialise the UART (serial port).

This function initializes one UART port and makes it accessible by the SERIAL library.

{% hint style="info" %}
It is necessary to statically allocate a memory area for the FIFO buffer used internally by the SERIAL library during initialization.
{% endhint %}

{% hint style="info" %}
At the same time, we initialize the [`tsFILE`](../fprintf-raiburari/tsfile.md) structure for output [`vfPrintf()`](../fprintf-raiburari/vfprintf.md), [`vPutChar()`](../fprintf-raiburari/vputchar.md).
{% endhint %}

## Argument

| Type                                        | Name    | Remark                              |
| ------------------------------------------- | ------- | ----------------------------------- |
| [tsSerialPortSetup](tsserialportsetup.md)\* | psSetup | Structure for configuring the UART. |

## Returns

None

## Sample code

refer to [`tsSerialPortSetup`](tsserialportsetup.md).
