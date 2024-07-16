# User-defined event processing function

![](<../../.gitbook/assets/image (24).png>)

* 開始: Start
* 定義済み関数の処理が全て完了？: All predefined functions have been processed?
* ユーザ定義イベント処理実行(発生イベント): Execution of user-defined event processing (passed a generated event)
* 状態が遷移した?: Has the state transitioned?
* ユーザー定義イベント処理関数の実行 `E_EVENT_NEW_STATE`: Execution of a user-defined event handling function by passing `E_EVENT_NEW_STATE.`
* 終了: End
* ※ ユーザ定義イベントの処理が呼び出された時には、状態が遷移する限りは連続的にユーザ定義イベント処理関数が呼び出されます。これはいったん制御をシステムのメインループに戻すことは無く、システム内の一関数呼び出し内でし実施されます。: When user-defined event processing is called, the user-defined event processing function is called continuously as long as the state transitions. This is done within a single function call in the system, without returning control to the main system loop.
* ※ システム内から呼び出される場合も、アプリケーション側から `ToCoNet_Event_Process()`により呼び出す場合も同じ振る舞いをします。: It behaves the same whether it is called from within the system or from the application side by `ToCoNet_Event_Process()`
