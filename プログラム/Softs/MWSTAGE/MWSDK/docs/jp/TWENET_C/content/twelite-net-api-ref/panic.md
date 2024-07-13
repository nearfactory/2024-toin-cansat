# PANIC

PANIC は、 TWELITE NET がシステム動作の継続を不可能と考えた場合に発生し、ネットワークイベントして通知されます。

### PANIC構造体定義

```c
typedef struct {
	bool_t bCancelReset; //!< TRUE を格納するとリセットをキャンセルする
	uint8 u8ReasonCode; //!< 要因コード
	uint32 u32ReasonInfo; //!< 要因の補助情報
	string strReason; //!< 要因の文字列情報
} tsPanicEventInfo;
```



### PANIC 処理の参考コード

```c
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg) {
	switch(eEvent) {
	case E_EVENT_TOCONET_PANIC:
		if (u32arg) {
			tsPanicEventInfo *pInfo = (void*)u32arg;
			V_PRINTF( “PANIC! %d/%s“, pInfo-> u8ReasonCode, pInfo->strReason);
			pInfo->bCancelReset = TRUE; 
			      // TRUE を設定すると、直後にリセットしない。
                  // ただしその後の動作は未定義である。
		} break;
	...
```



### PANIC 要因のID

| `TOCONET_PANIC_TX_FAIL_COUNT` | MAC層内で送信処理の失敗が継続した。通常想定しないエラーです。 |
| ----------------------------- | -------------------------------- |

