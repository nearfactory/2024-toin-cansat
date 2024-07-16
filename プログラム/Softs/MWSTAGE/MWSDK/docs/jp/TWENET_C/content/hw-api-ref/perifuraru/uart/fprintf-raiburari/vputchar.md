# vPutChar()

## 解説

[tsFILE](tsfile.md) 構造体の示す出力先（UART）に１バイト出力する。

## 引数

| 型                     | 名前       | 詳細    |
| --------------------- | -------- | ----- |
| [tsFILE](tsfile.md)\* | psStream | 出力先   |
| uint8                 | u8Char   | 出力バイト |

## 戻り値

なし

## サンプル

```c
#define IS_ASC(c) ((c) >= 0x20 && (c) <= 0x7e)

void cbToCoNet_vRxEvent(tsRxDataApp *pRx) {
	uint8 u8i;
	vfPrintf(&sSerStream, LB"RX(len=%d):[", pRx->u8Len);
	for (i = 0; i < pRx->u8Len; i++) {
		uint8 c = pRx->auData[i];
		vPutChar(&sSerStream, IS_ASC(c) ? c : '.');
	}
}
```

