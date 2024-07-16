# EEPROM

TWELITE 無線マイコン内蔵の EEPROM を利用する手続きです。

{% hint style="warning" %}
最大 64x56=3584 バイト利用可能です。
{% endhint %}

## 定義

```c
#include "eeprom_6x.h"
```

| `EEPROM_6X_USER_SIZE`     | 利用可能な最大バイト数 (64x60 バイト, 残領域はシステム予約です) |
| ------------------------- | ------------------------------------- |
| `EEPROM_6X_SEGMENT_SIZE`  | EEPROM のセグメントサイズ (64バイト)              |
| `EEPROM_6X_USER_SEGMENTS` | EEPROM のセグメント数                        |

## 関数

### EEP\_6x_bRead()

EEPROM の読み出しを行います。

```
bool_t EEP_6x_bRead(uint16 u16StartAddr, uint16 u16Bytes, uint8 *pu8Buffer);
```

#### 引数

| 型         | 名前             | 詳細                |
| --------- | -------------- | ----------------- |
| `uint16`  | `u16StartAddr` | 読み出し開始アドレス（バイト）。  |
| `uint16`  | `u16Bytes`     | 読み出しバイト数。         |
| `uint8 *` | `pu8Buffer`    | 読み出しデータを格納するバッファ。 |

#### 戻り値

| 型        | 詳細                             |
| -------- | ------------------------------ |
| `boot_t` | `TRUE` なら読み出しは成功、`FALSE` なら失敗。 |



### EEP\_6x_bWrite()

EEPROM に書き込みます。

```
bool_t EEP_6x_bWrite(uint16 u16StartAddr, uint16 u16Bytes, uint8 *pu8Buffer);
```

#### 引数

| 型         | 名前             | 詳細                |
| --------- | -------------- | ----------------- |
| `uint16`  | `u16StartAddr` | 書き込み開始アドレス（バイト）。  |
| `uint16`  | `u16Bytes`     | 書き込みバイト数。         |
| `uint8 *` | `pu8Buffer`    | 書き込みデータを格納するバッファ。 |

#### 戻り値

| 型        | 詳細                             |
| -------- | ------------------------------ |
| `boot_t` | `TRUE` なら読み出しは成功、`FALSE` なら失敗。 |

## サンプル

```c
#include "eeprom_6x.h"

    // read
    if (EEP_6x_bRead(0, sizeof(tsSaveDat), (uint8 *)psSaveDat)) {
    	bRet = TRUE;
    }

    // write
	psSaveDat->u32Magic = FLASH_MAGIC_NUMBER;
	psSaveDat->u8CRC = u8CCITT8((uint8*)&(psSaveDat->sData), sizeof(tsSaveDat));
	if (EEP_6x_bWrite(0, sizeof(tsSaveDat), (uint8 *)psSaveDat)) {
		bRet = TRUE;
	}
	
    // clean up
    uint8 au8buff[EEPROM_6X_SEGMENT_SIZE];
    memset (au8buff, 0xFF, EEPROM_6X_SEGMENT_SIZE);
    bRet = TRUE;
    for (i = 0; i < EEPROM_6X_USER_SEGMENTS; i++) {
		bRet &= EEP_6x_bWrite(i * EEPROM_6X_SEGMENT_SIZE,
		                      EEPROM_6X_SEGMENT_SIZE,
		                      au8buff);
    }
```

