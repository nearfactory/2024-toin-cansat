# TWENET working flow

{% hint style="info" %}
We are sorry, the diagram is not translated.
{% endhint %}

The following diagram illustrates the basic flow of TWENET.

![](<../../.gitbook/assets/image (11).png>)

A more detailed description of the operation is given in the child pages of this page.

* 電源投入またはスリープ復帰: Power up or Waking up from sleep
* スリープ復帰: Waking up from sleep
* スリープ実行: Perform sleeping
* cbAppColdStart(FALSE): Declaration of the modules used
* cbAppColdStart(TRUE): Network settings, Hardware initialization
* cbAppWarmStart(FALSE): Determination of waking factors
* cbAppWarmStart(TRUE): Hardware initalization
* TWENET開始: Start the TWENET
* イベント: incoming event
* ユーザ定義イベント処理関数: User defined event handling functions

