# Flash

In principle, flash memory is used as an area for storing programs. Unused sectors can also be used by programs.

{% hint style="danger" %}
There is no denying the possibility of destroying the entire program area due to power failure during the writing process. For applications that require frequent writing, please perform sufficient verification.
{% endhint %}

## About Sectors

TWELITE BLUE provides 5 blocks (160KB) of 32KB sectors, while TWELITE RED provides 16 blocks (512KB).

## Read out

This is a method to read from a user-defined tsFlash structure. In the example, CRC check and data check by MAGIC NUMBER are also performed.

```c
#define FLASH_TYPE E_FL_CHIP_INTERNAL
#define FLASH_SECTOR_SIZE (32L* 1024L) // 32KB
#define FLASH_SECTOR_NUMBER 5 // 0..4　(for TWELITE BLUE)

/** @ingroup FLASH
 * Read out from Flash
 * @param psFlash data buffer for storing.
 * @param sector readout sector
 * @param offset Offset from the beginning of the sector
 * @return TRUE: Read successfully FALSE: Failure
 */
bool_t bFlash_Read(tsFlash *psFlash, uint8 sector, uint32 offset) {
    bool_t bRet = FALSE;
    offset += (uint32)sector * FLASH_SECTOR_SIZE; // calculate the absolute address

    if (bAHI_FlashInit(FLASH_TYPE, NULL) == TRUE) {
        if (bAHI_FullFlashRead(offset, sizeof(tsFlash), (uint8 *)psFlash)) {
            bRet = TRUE;
        }
    }

    // validate content
    if (bRet && psFlash->u32Magic != FLASH_MAGIC_NUMBER) {
    	bRet = FALSE;
    }
    if (bRet && psFlash->u8CRC != u8CCITT8((uint8*)&(psFlash->sData), sizeof(tsFlashApp))) {
    	bRet = FALSE;
    }

    return bRet;
}

```

## Writing

Writing is performed in sectors. Sector erase is performed beforehand.

```c
#define FLASH_TYPE E_FL_CHIP_INTERNAL
#define FLASH_SECTOR_SIZE (32L* 1024L) // 32KB
#define FLASH_SECTOR_NUMBER 5 // 0..4　(for TWELITE BLUE)

/**  @ingroup FLASH
 * Flash Write
 * @param psFlash Data to be written
 * @param sector write sector
 * @param offset Offset from the beginning of the write sector
 * @return TRUE:Write success FALSE: Failure
 */
bool_t bFlash_Write(tsFlash *psFlash, uint8 sector, uint32 offset)
{
    bool_t bRet = FALSE;
    offset += (uint32)sector * FLASH_SECTOR_SIZE; // calculate the absolute address

    if (bAHI_FlashInit(FLASH_TYPE, NULL) == TRUE) {
        if (bAHI_FlashEraseSector(sector) == TRUE) { // erase a corresponding sector.
        	psFlash->u32Magic = FLASH_MAGIC_NUMBER;
        	psFlash->u8CRC = u8CCITT8((uint8*)&(psFlash->sData), sizeof(tsFlashApp));
            if (bAHI_FullFlashProgram(offset, sizeof(tsFlash), (uint8 *)psFlash)) {
                bRet = TRUE;
            }
        }
    }

    return bRet;
}
```

## Erase

This is an example of performing sector erasure.

```c
/**  @ingroup FLASH
 * Flash Sector Erase
 * @return TRUE: Write success FALSE: Failure
 */
bool_t bFlash_Erase(uint8 sector)
{
	int i;
    bool_t bRet = FALSE;

    if (bAHI_FlashInit(FLASH_TYPE, NULL) == TRUE) {
        if (bAHI_FlashEraseSector(sector) == TRUE) { // erase a corresponding sector.
        	bRet = TRUE;
        }
    }

    return bRet;
}
```
