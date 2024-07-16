# I2C

I2C is not included in the library, but it is used in sample programs that use I2C under the file name `SMBus.c`.

{% hint style="info" %}
Please refer to the Samp_I2C sample program.
{% endhint %}

```c
#include "SMBus.h"
#define EEPROM_24XX01_ADDRESS		0x50

// initialisation
void vInitHardware() {
	// SMBUS
	vSMBusInit();
}

// 24XX EEPROM write
bool_t b24xx01_Write(uint8 u8Address, uint8 *pu8Data, uint8 u8Length)
{
	uint8 n;
	bool_t bOk = TRUE;
	volatile int x;

	for(n = 0; n < u8Length; n++){
		// Write u8Address and 1 byte of data
		bOk &= bSMBusWrite(EEPROM_24XX01_ADDRESS, u8Address++, 1, pu8Data++);
		for(x = 0; x < 16000; x++);
	}
	return(bOk);
}


PUBLIC bool_t b24xx01_Read(uint8 u8Address, uint8 *pu8Dest, uint8 u8Length)
{
	bool_t bOk = TRUE;
	
	// Writing u8Address
	bOk &= bSMBusWrite(EEPROM_24XX01_ADDRESS, u8Address, 0, NULL);

	if(u8Length > 1){
		// Read continuously for u8Length.
		bOk &= bSMBusSequentialRead(EEPROM_24XX01_ADDRESS, u8Length, pu8Dest);
	}

	return(bOk);
}
```
