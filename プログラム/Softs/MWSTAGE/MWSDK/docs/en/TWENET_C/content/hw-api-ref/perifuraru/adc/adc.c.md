# adc.c

`adc.c` is included as `common/Source/adc.c` in much of the sample code.

The following code is relevant:

| Filename        | Remark                                          |
| --------------- | ----------------------------------------------- |
| sensor_driver.h | Abstraction of sensor processing (header files) |
| sensor_driver.c | Sensor processing using state transitions       |
| adc.h           | ADC processing section (header file)            |
| adc.c           | ADC processing                                  |

{% hint style="warning" %}
There may be differences in the code for each sample.
{% endhint %}

### Usage

See the comments in the code below.

```c
#include "sensor_driver.h"
#include "adc.h"

tsObjData_ADC sObjADC; // ADC management structure (data part)
tsSnsObj sADC; // ADC Management Structure (Control Unit)
int16 a1,a2,ab; // For storing results

...
	// ADC initialisation
	vSnsObj_Init(&sADC);
	vADC_Init(&sObjADC, &sADC, TRUE);
	vADC_WaitInit(); // Waiting for hardware initialisation

...
	// Specify the port on which you want to make ADC measurements 
	//(supply voltage and ADC1,2 in the following)
	sObjADC.u8SourceMask = TEH_ADC_SRC_VOLT
    	| TEH_ADC_SRC_ADC_1 | TEH_ADC_SRC_ADC_2;
    
	// start ADC
	vSnsObj_Process(&sADC, E_ORDER_KICK); // Kick measurement

// Wait for the ADC to finish processing one channel 
//(=E_AHI_DEVICE_ANALOGUE interrupt).
// vSnsObj_Process() is called sequentially.
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap) {
	switch (u32DeviceId) {
	case E_AHI_DEVICE_ANALOGUE:
		// ADC completion interrupt
		vSnsObj_Process(&sADC, E_ORDER_KICK);
		if (bSnsObj_isComplete(&sADC)) {
			// All channels have been processed.
			// Values are stored in:
			a1=sObjADC.ai16Result[TEH_ADC_IDX_ADC_1]; // ADC1[mV]
			a2=sObjADC.ai16Result[TEH_ADC_IDX_ADC_2]; // ADC2[mV]
			ab=sObjADC.ai16Result[TEH_ADC_IDX_VOLT]; // VCC[mV]
			
			// Restore the initial state before the ADC started
			vSnsObj_Process(&sADC, E_ORDER_KICK);
			
			// E_ORDER_KICK again for successive runs
			vSnsObj_Process(&sADC, E_ORDER_KICK);
		}
		break;

	default:
		break;
	}
}
```

### functions

#### void vSnsObj_Init(tsSnsObj _\*_pSnsObj)

Initializes the sensor management structure. Called just before `vADC_Init()`.

#### void vADC_Init(tsObjData_ADC \*pData,_ _tsSnsObj _\*_pSnsObj, bool_t bInitAPR)

This function initializes ADC. The `tsObjData` structure (for storing results) and the `tsSnsObj` structure (for managing ADC) are prepared in advance.

If `bInitAPR` is `TRUE`, the ADC hardware is initialized. Hardware initialization takes some time, so be sure to run `vADC_WaitInit()` and wait for the initialization.

#### void vSnsObj_Process(tsSnsObj \*pObj, teEvent eEv)

This process is called every time the conversion of one ADC port is completed. To wait for the completion of ADC conversion, `E_AHI_DEVICE_ANALOGUE` interrupt is used. This can be done simply after a certain period of time (approximately 100 usec) has elapsed.

In this process, the acquisition of ADC value and the operation to mV value are performed, and the value is stored in the `tsSnsObj` structure.

This process is an event processing for the state transition managed by the `tsSnsObj` structure. Immediately after this processing, `bSnsObj_isComplete()` is called to check whether the processing is completed. To restore the initial state again, this process is executed again with `E_ORDER_KICK` as an argument (that is, to execute ADC again, `E_ORDER_KICK` is executed twice after completion).

#### tsObjData_ADC structure

| Members          | Remark                                                                                                                                                                                                                       |
| ---------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| u8SourceMask     | A bitmap that specifies the port to be ADCed. The specified port becomes the ADC target.<ul><li>`TEH_ADC_SRC_VOLT` : supply voltage</li><li>`TEH_ADC_SRC_ADC_1-4` : ADC1,2,3,4</li></ul> |
| u8InputRangeMask | Specify the range (0-Vref or 0-2Vref) of the ADC target port. The specified port becomes 0-Vref. If not specified, it becomes 0-2Vref.                                                                                       |
| ai16Result\[]    | Structure for storing ADC values. The result is stored as mV value.<ul><li>`TEH_ADC_IDX_VOLT`: supply voltage.</li><li>`TEH_ADC_IDX_ADC_1-4`: ADC1,2,3,4</li></ul>                       |

####

