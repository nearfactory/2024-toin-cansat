# cbToCoNet_vMain()

## Explanation

Called from the application main loop in TWELITE NET. It is called after returning from a power saving DOZE and describes processes that need to be checked frequently, such as processing input from the UART.

{% hint style="info" %}
It is recommended that the main processing of the application be described by user-defined event processing functions.
{% endhint %}

The main interruptors are [UART](../../hw-api-ref/perifuraru/uart/), [TickTimer](../../hw-api-ref/perifuraru/ticktimer.md), radio event (transmission complete, received). The main interrupt sources are [UART](../../hw-api-ref/perifuraru/uart/), [TickTimer](../../hw-api-ref/perifuraru/ticktimer.md) and radio events (transmission complete, reception).

## Argument

None

## Returns

None

## Sample code

```c
void cbToCoNet_vMain(void) {
	// Handle serial (uart) input
	vHandleSerialInput(SERIAL_FROM_QUEUE);
}
```

