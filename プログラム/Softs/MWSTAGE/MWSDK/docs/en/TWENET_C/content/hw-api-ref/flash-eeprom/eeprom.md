# EEPROM

TWELITE This is a procedure for using the built-in EEPROM of the wireless microcontroller.

{% hint style="warning" %}
A maximum of 64x56=3584 bytes are available.
{% endhint %}

## Definitions

```c
#include "eeprom_6x.h"
```

| `EEPROM_6X_USER_SIZE`     | Maximum number of bytes available (64x60 bytes, the remaining space is reserved for the system) |
| ------------------------- | ----------------------------------------------------------------------------------------------- |
| `EEPROM_6X_SEGMENT_SIZE`  | EEPROM segment size (64 bytes)                                                                  |
| `EEPROM_6X_USER_SEGMENTS` | Number of segments in EEPROM                                                                    |

## functions

### EEP\_6x_bRead()

Reads out the EEPROM.

```
bool_t EEP_6x_bRead(uint16 u16StartAddr, uint16 u16Bytes, uint8 *pu8Buffer);
```

#### Arguments

| Type      | Name           | Remark                         |
| --------- | -------------- | ------------------------------ |
| `uint16`  | `u16StartAddr` | Read start address (in bytes). |
| `uint16`  | `u16Bytes`     | Number of bytes to read.       |
| `uint8 *` | `pu8Buffer`    | Buffer to store the read data. |

#### Returns

| Type     | Remark                                              |
| -------- | --------------------------------------------------- |
| `boot_t` | If `TRUE`, the read succeeds; if `FALSE`, it fails. |



### EEP\_6x_bWrite()

Write to EEPROM.

```
bool_t EEP_6x_bWrite(uint16 u16StartAddr, uint16 u16Bytes, uint8 *pu8Buffer);
```

#### Arguments

| Type      | Name           | Remark                          |
| --------- | -------------- | ------------------------------- |
| `uint16`  | `u16StartAddr` | Write start address (in bytes). |
| `uint16`  | `u16Bytes`     | Number of bytes written.        |
| `uint8 *` | `pu8Buffer`    | Buffer to store write data.     |

#### Returns

| Type     | Remark                                               |
| -------- | ---------------------------------------------------- |
| `boot_t` | If `TRUE`, the write succeeds; if `FALSE`, it fails. |

## Sample code

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

