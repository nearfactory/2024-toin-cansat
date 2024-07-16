# Hardware interrupts/events

![](<../../.gitbook/assets/image (23).png>)

* ハード割り込み(TICK TIMER 以外): Hard interrupts (other than TICK TIMER)
  * システム内処理: In-system processing
  * イベントFIFO: Events FIFO
  * 取り出し: Extraction
* 割り込み待ち: Waiting for interrupt (TickTimer 以外: other than TickTimer)
* 割り込み待ち: Waiting for interrupt (TICK_TIMER)
  * システム内処理: In-system processing
  * ユーザ定義イベント処理関数 `E_EVENT_TICK_TIMER`: User defined event handling functions (fires `E_EVENT_TICK_TIMER`)
  * 同上(1秒置き) `E_AHI_DEVICE_TICK_SECOND`: Same as above (every second, fires `E_AHI_DEVICE_TICK_SECOND`)
* 左図はハードウェア関連の割り込みのみが発生したときの処理順を示します。メインループ中では、ハードウェア処理は後の方で処理されます。タイミングが重要な割り込み処理は`cbToCoNet_u8HwInt()`に記述してください。: The diagram on the left shows the order of processing when only a hardware-related interrupt occurs. During the main loop, the hardware processing is handled later in the process. A processing which is timing critical should be described in `cbToCoNet_u8HwInt()`.
