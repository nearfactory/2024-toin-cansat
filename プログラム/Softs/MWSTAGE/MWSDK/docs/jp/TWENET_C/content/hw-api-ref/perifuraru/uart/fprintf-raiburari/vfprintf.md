# vfPrintf()

## 解説

[tsFILE](tsfile.md) 構造体の示す出力先（UART）に printf 書式にて出力する。

## 引数

| 型                     | 名前       | 詳細   |
| --------------------- | -------- | ---- |
| [tsFILE](tsfile.md)\* | psStream | 出力先  |
| const char \*         | pcFormat | 出力書式 |
| ...                   |          | 可変引数 |

#### 対応する書式

| s | 文字列             |
| - | --------------- |
| d | 整数(32bitまで)     |
| u | 符号なし整数(32bitまで) |
| x | 16進数。a-f は小文字。  |
| X | 16進数。A-Fは大文字。   |
| b | bit列            |

####

## 戻り値

なし。

## サンプル

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
