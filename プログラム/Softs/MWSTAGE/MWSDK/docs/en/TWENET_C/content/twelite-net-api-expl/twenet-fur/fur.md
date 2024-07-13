# System start-up

Flow at system start-up.

![](<../../.gitbook/assets/image (19).png>)

{% hint style="warning" %}
The E_EVENT_START_UP event is only raised for user event processing functions.
{% endhint %}

* 始動: Starting
* システム内処理: In-system processing
* または: or
* ユーザ定義イベント処理関数: User defined event handling functions
* システムがメインループ動作を始める前に E_EVENT_START_UP イベントが、ユーザ定義イベント処理関数に発生します。: The E_EVENT_START_UP event is raised to the user-defined event processing function before the system starts the main loop operation.
