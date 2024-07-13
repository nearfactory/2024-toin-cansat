# SERIAL_i16RxChar()

## Explanation

Retrieve one character from the FIFO queue.

## Argument

| Type  | Name         | Remark                                                           |
| ----- | ------------ | ---------------------------------------------------------------- |
| uint8 | u8SerialPort | Serial port (E_AHI_UART\_0 or E_AHI_UART\_1 should be specified) |

## Returns

| Typr  | Remark                                                                                              |
| ----- | --------------------------------------------------------------------------------------------------- |
| int16 | A value of 0x00-0xFF indicates that a byte has been correctly retrieved, otherwise an error occurs. |

## Sample code

```c
void cbToCoNet_vMain(void) {
	while (!SERIAL_bRxQueueEmpty(sSerPort.u8SerialPort)) {
		int16 i16Char;
​		i16Char = SERIAL_i16RxChar(sSerPort.u8SerialPort);
​		vfPrintf(&sSerStream, "\n\r# [%c] --> ", i16Char);
	    SERIAL_vFlush(sSerStream.u8Device);
		...
	}
}
```

