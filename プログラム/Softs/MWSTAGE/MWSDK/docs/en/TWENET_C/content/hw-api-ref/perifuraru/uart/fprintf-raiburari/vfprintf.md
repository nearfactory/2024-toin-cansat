# vfPrintf()

## 解説

Outputs to the destination (UART) indicated by the [`tsFILE`](tsfile.md) structure using the printf format.

## Arguments

| Type                  | None     | Remark             |
| --------------------- | -------- | ------------------ |
| [tsFILE](tsfile.md)\* | psStream | Destination        |
| const char \*         | pcFormat | Format string      |
| ...                   |          | variable arguments |

#### Supported formats

| s | string                                   |
| - | ---------------------------------------- |
| d | integer (upto 32bits)                    |
| u | unsigned integer(upto 32bits)            |
| x | Hexadecimal numbers. a-f are lower case. |
| X | Hexadecimal numbers, A-F in upper case.  |
| b | 0/1 binary                               |

####

## Returns

None

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
