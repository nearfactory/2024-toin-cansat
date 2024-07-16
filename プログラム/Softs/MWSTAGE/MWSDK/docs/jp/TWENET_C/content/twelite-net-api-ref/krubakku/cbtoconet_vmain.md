# cbToCoNet_vMain()

## 解説

TWELITE NET のアプリケーションメインループからの呼び出されます。電力を節約するドーズ(DOZE)から復帰後に呼び出され、UART からの入力処理など頻繁に確認する必要のある処理を記述します。

{% hint style="info" %}
アプリケーションの主処理は、ユーザ定義イベント処理関数で記述することを推奨します。
{% endhint %}

主な割り込み要因は、[UART](../../hw-api-ref/perifuraru/uart/), [TickTimer](../../hw-api-ref/perifuraru/ticktimer.md), 無線イベント(送信完了、受信)です。

## 引数

なし

## 戻り値

なし

## サンプル

```c
void cbToCoNet_vMain(void) {
	// Handle serial (uart) input
	vHandleSerialInput(SERIAL_FROM_QUEUE);
}
```

