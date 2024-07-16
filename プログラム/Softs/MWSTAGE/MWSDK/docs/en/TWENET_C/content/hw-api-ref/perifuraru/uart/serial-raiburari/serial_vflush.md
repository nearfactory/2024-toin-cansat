# SERIAL_vFlush()

## Explanation

Wait until the output request has been completed by the SERIAL library.

{% hint style="info" %}
Use this if you want to output all messages before going to sleep. Normally, it is recommended to keep a sufficient FIFO buffer for output and to avoid calling the process as much as possible.
{% endhint %}

{% hint style="warning" %}
Waiting for output to complete may cause instability. In this case, remove this waiting process and check the operation.

If the waiting process is omitted and the output is interrupted, an extra FIFO memory is allocated for the output.
{% endhint %}

## Argument <a href="yin-shu" id="yin-shu"></a>

| Type  | Name         | Remark                                                           |
| ----- | ------------ | ---------------------------------------------------------------- |
| uint8 | u8SerialPort | Serial port (E_AHI_UART\_0 or E_AHI_UART\_1 should be specified) |

## Returns

None

## Sample code

```c
void cbToCoNet_vMain(void) {
	while (!SERIAL_bRxQueueEmpty(sSerPort.u8SerialPort)) {
		int16 i16Char;
​		i16Char = SERIAL_i16RxChar(sSerPort.u8SerialPort);
​		vfPrintf(&sSerStream, "\n\r# [%c] --> ", i16Char);
	    SERIAL_vFlush(sSerStream.u8Device);
		...
	}
}
```

