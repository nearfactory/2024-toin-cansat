# SERIAL_i16RxChar()

## 解説

FIFOキューから１文字取り出す。

## 引数

| 型     | 名前           | 詳細                                              |
| ----- | ------------ | ----------------------------------------------- |
| uint8 | u8SerialPort | シリアルポート (E_AHI_UART\_0 または E_AHI_UART\_1 を指定する） |

## 戻り値

| 型     | 詳細                                          |
| ----- | ------------------------------------------- |
| int16 | 0x00-0xFF なら正しく１バイトを取り出せたことを示し、それ以外はエラーとなる。 |

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

