# Main loop

![](<../../.gitbook/assets/image (20).png>)

* 割り込み 無線ハードウェア: Interrupts Wireless Hardware
* システム内処理: In-system processing
* 割り込み待ち: Waiting for interruption
* CPU Doze 解除: release CPU Doze
* ウォッチドッグリセット: Watchdog reset
* 無線イベント処理(送信完了): Radio event processing (transmission complete)
* 無線イベント処理(他): Radio event processing (others)
* ハードウェアイベント処理: Hardware event handling
  * cbToCoNet_vHwEvent(), ユーザ定義イベント処理関数: cbToCoNet_vHwEvent(), user defined event handling function
* 無線受信処理(受信処理): Radio reception process (reception process)
* 割り込み待ち: Waiting for interrupt
* 割り込み待ち状況では、割り込みハンドラにより登録されたイベントFIFOキュー順に処理が行われるが、同時にキューに投入された場合、左記の順で処理が行われる。: In an interrupt waiting situation, processing is carried out in the order of the event FIFO queue registered by the interrupt handler, but if the events are put into the queue at the same time (e.g. Rx event and Harware event comes at same time), processing is carried out in the order shown on the left.
* マイコンに無線・ハードウェア関連のイベント（イベントFIFOに登録されないものも含む）が発生すると、最後にcbToCoNet_vMain()が必ず呼び出されCPU_Doze状態に遷移する: Whenever a wireless/hardware-related event (including those not registered in the event FIFO) occurs in the microcontroller, cbToCoNet_vMain() is called at the end and the microcontroller transitions to the CPU_Doze state.
* 左図はTWENETのメインループで、各コールバック関数で相対的に長い時間のかかる処理を行うと、他の処理が滞るため、角処理は簡潔に止める。: The diagram on the left shows the main loop of TWENET, where the corner processing is kept brief because the relatively long processing time in each callback function will slow down the other processing.



