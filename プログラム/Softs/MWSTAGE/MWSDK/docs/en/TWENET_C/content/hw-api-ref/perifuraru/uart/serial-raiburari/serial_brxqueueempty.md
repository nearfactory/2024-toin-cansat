# SERIAL_bRxQueueEmpty()

## Explanation

The SERIAL library checks whether the reception from the UART is stored in the FIFO buffer.

{% hint style="info" %}
To retrieve data from the FIFO buffer, use [`SERIAL_i16RxChar()`](serial_i16rxchar.md).
{% endhint %}

## Argument

| Type  | Name         | Remark                                                           |
| ----- | ------------ | ---------------------------------------------------------------- |
| uint8 | u8SerialPort | Serial port (E_AHI_UART\_0 or E_AHI_UART\_1 should be specified) |

## Returns

| Type   | Remark                                                                                                                           |
| ------ | -------------------------------------------------------------------------------------------------------------------------------- |
| bool_t | If `TRUE`, there is received data in the FIFO queue managed by the SERIAL library. If `FALSE`, there is still data in the queue. |

## Sample code

```c
void cbToCoNet_vMain(void) {
    while (!SERIAL_bRxQueueEmpty(sSerPort.u8SerialPort)) {
        int16 i16Char;
​        i16Char = SERIAL_i16RxChar(sSerPort.u8SerialPort);
​        vfPrintf(&sSerStream, "\n\r# [%c] --> ", i16Char);
        SERIAL_vFlush(sSerStream.u8Device);
        ...
    }
}
```
