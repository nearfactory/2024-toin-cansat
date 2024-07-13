# vPutChar()

## Explanation

Outputs 1 byte to the output destination (UART) indicated by the [`tsFILE`](tsfile.md) structure.

## Argumtens

| Type                  | Name     | Remark        |
| --------------------- | -------- | ------------- |
| [tsFILE](tsfile.md)\* | psStream | Output device |
| uint8                 | u8Char   | Output byte   |

## Returns

None.

## Sample code

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

