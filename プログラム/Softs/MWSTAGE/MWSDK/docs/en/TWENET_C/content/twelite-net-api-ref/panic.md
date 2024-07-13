# PANIC

PANIC occurs when TWELITE NET believes that continued system operation is not possible and is reported as a network event.

### PANIC structure definition

```c
typedef struct {
	bool_t bCancelReset; //!< Store TRUE to cancel the reset.
	uint8 u8ReasonCode; //!< Factor code
	uint32 u32ReasonInfo; //!< Supporting Information for Factors
	string strReason; //!< String information of factors
} tsPanicEventInfo;
```



### Reference codes for PANIC processing

```c
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	switch(eEvent) {
	case E_EVENT_TOCONET_PANIC:
		if (u32arg) {
			tsPanicEventInfo *pInfo = (void*)u32arg;
			V_PRINTF( “PANIC! %d/%s“, pInfo-> u8ReasonCode, pInfo->strReason);
			pInfo->bCancelReset = TRUE; 
	// If set to TRUE, it will not reset immediately afterwards.
        // However, the subsequent behaviour is undefined.
		} break;
	...
```



### ID of the PANIC factor

| `TOCONET_PANIC_TX_FAIL_COUNT` | The transmission process continued to fail within the MAC layer. This is an error that is not normally expected. |
| ----------------------------- | ---------------------------------------------------------------------------------------------------------------- |

