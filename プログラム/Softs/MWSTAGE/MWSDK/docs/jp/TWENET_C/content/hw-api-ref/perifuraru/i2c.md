# I2C

I2C はライブラリに収録していませんが SMBus.c というファイル名で I2C を用いるサンプルプログラムで利用されています。

{% hint style="info" %}
Samp_I2C サンプルプログラムを参考にしてください。
{% endhint %}

```c
#include "SMBus.h"
#define EEPROM_24XX01_ADDRESS		0x50

// 初期化
void vInitHardware() {
	// SMBUS
	vSMBusInit();
}

// 24XX EEPROM 書き込み
bool_t b24xx01_Write(uint8 u8Address, uint8 *pu8Data, uint8 u8Length)
{
	uint8 n;
	bool_t bOk = TRUE;
	volatile int x;

	for(n = 0; n < u8Length; n++){
		// u8Addressと1バイトのデータ書き込み
		bOk &= bSMBusWrite(EEPROM_24XX01_ADDRESS, u8Address++, 1, pu8Data++);
		for(x = 0; x < 16000; x++);
	}
	return(bOk);
}


PUBLIC bool_t b24xx01_Read(uint8 u8Address, uint8 *pu8Dest, uint8 u8Length)
{
	bool_t bOk = TRUE;
	
	// u8Address の書き込み
	bOk &= bSMBusWrite(EEPROM_24XX01_ADDRESS, u8Address, 0, NULL);

	if(u8Length > 1){
		// u8Length分連続で読み出す。
		bOk &= bSMBusSequentialRead(EEPROM_24XX01_ADDRESS, u8Length, pu8Dest);
	}

	return(bOk);
}
```
