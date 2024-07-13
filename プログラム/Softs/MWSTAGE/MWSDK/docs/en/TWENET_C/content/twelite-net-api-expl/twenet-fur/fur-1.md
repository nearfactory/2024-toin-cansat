# Wireless events

## Events of receving or transmit complete.

![](<../../.gitbook/assets/image (21).png>)

* 無線割り込み(受信・送信完了など): Radio interrupt (e.g. reception, transmission complete)
* システム内処理: In-system processing
* イベントFIFO: Event FIFO
* 取り出し: extraction
* 無線イベントの処理は、割り込みで（内部処理）得られた無線イベントを割り込みハンドラ中でイベントFIFOに登録します。登録後メインループ中からイベントFIFOを処理します。:The processing of radio events obtained by interrupts (internal processing) is registered in the event FIFO in the interrupt handler. After registration, the event FIFO is processed from the main loop.
* 割り込み待ち: waiting for interruption
* 無線イベント処理: Wireless event handling
  * 受信データなどのシステム処理: System processing of incoming data etc.
* 受信FIFO: Receive FIFO
* 無線イベント処理(送信完了): Radio event processing (transmission complete)
  * システム処理=>cbToCoNet_vTx(): System processing=>cbToCoNet_vTx()
* 無線イベント処理(他): Radio event processing (others)
  * システム処理=>cbToCoNet_vNwkEvent(): System processing=>cbToCoNet_vNwkEvent()
* 無線受信処理(受信パケットの処理): Radio reception process (processing of incoming packets)
  * システム処理=>cbToCoNet_vRx(): System processing=>cbToCoNet_vRx()
* 割り込み待ち: Waiting for interruption
* 取り出したイベント種別により、いずれかが実行される。同時に複数イベントが登録されている場合はフロー順に呼び出される。: Depending on the type of event retrieved, one of them will be executed. If multiple events are registered at the same time, they are called in order of flow.
* 割り込み待ち: Waiting for interrupt.



## Detailes when transmitting

![](<../../.gitbook/assets/image (22).png>)

* 割り込み待ち: Waiting for interrupt.
* 送信完了無線イベント: Transmission complete radio event
* 送信処理中?: Sending in progress?
  * 送信キューから取り出して、送信処理中のデータに格納: Removed from the transmission queue and stored in the data being transmitted.
  * ToCoNet_bMacTxReq(), ToCoNet_Nwk_bTx() APIによりアプリケーションから送信要求を投入。: ToCoNet_bMacTxReq(), ToCoNet_Nwk_bTx() APIs to submit a request for transmission from the application.
  * 送信FIFO: Send FIFO
  * ※ 送信要求が呼び出された直後の TickTimer を起点として送信処理が行われます。全ての処理が終わった時点でcbToCoNet_vTx()により結果が報告されます。: note:  The transmission process is performed starting from the TickTimer immediately after the transmission request is called. When all the processing is finished, the result is reported by cbToCoNet_vTx().
  * 条件に合うもの（送信遅延等）を送信処理中データから一つ選ぶ: Select one of the data in the transmission process that matches the conditions (e.g. transmission delay)
  * 802.15.4 MAC 層へ送信要求を出し、送信完了待ち状態に設定: 802.15.4 Request transmission to MAC layer and set to waiting for completion of transmission
* 送信完了?: Is transmission complete?
  * cbToCoNet_vTx()を呼び出す: Call cbToCoNet_vTx()
  * 送信処理中のデータを空にし、送信処理中を解除: Empties the data in the transmission process and releases the transmission process
  * 送信処理中のデータ: Data in the transmission process
* 割り込み待ち: Waiting for interrupt
