# Flash

フラッシュメモリは、原則としてプログラムを格納するための領域として利用します。未使用のセクターはプログラムで利用することも可能です。

{% hint style="danger" %}
書き込み処理中の電源断などで、プログラム領域ごと破壊してしまう可能性も否定できません。頻繁に書き込みを行う用途については十分な検証を行ってください。
{% endhint %}

## セクターについて

32KB 単位のセクターが TWELITE BLUE では 5ブロック(160KB)、TWELITE RED では 16 ブロック (512KB) 用意されます。

## 読み込み

ユーザ定義の tsFlash 構造体から読み出す方法です。例では CRC チェックや MAGIC NUMBER によるデータチェックも行っています。

```c
#define FLASH_TYPE E_FL_CHIP_INTERNAL
#define FLASH_SECTOR_SIZE (32L* 1024L) // 32KB
#define FLASH_SECTOR_NUMBER 5 // 0..4　(for TWELITE BLUE)

/** @ingroup FLASH
 * フラッシュの読み込み
 * @param psFlash 読み込み格納データ
 * @param sector 読み出しセクタ
 * @param offset セクタ先頭からのオフセット
 * @return TRUE:読み出し成功 FALSE:失敗
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

## 書き込み

書き込みはセクター単位で実施します。事前にセクター消去を行ってから実行します。

```c
#define FLASH_TYPE E_FL_CHIP_INTERNAL
#define FLASH_SECTOR_SIZE (32L* 1024L) // 32KB
#define FLASH_SECTOR_NUMBER 5 // 0..4　(for TWELITE BLUE)

/**  @ingroup FLASH
 * フラッシュ書き込み
 * @param psFlash 書き込みたいデータ
 * @param sector 書き込みセクタ
 * @param offset 書き込みセクタ先頭からのオフセット
 * @return TRUE:書き込み成功 FALSE:失敗
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

## 消去

セクター消去を行う例です。

```c
/**  @ingroup FLASH
 * フラッシュセクター消去
 * @return TRUE:書き込み成功 FALSE:失敗
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
