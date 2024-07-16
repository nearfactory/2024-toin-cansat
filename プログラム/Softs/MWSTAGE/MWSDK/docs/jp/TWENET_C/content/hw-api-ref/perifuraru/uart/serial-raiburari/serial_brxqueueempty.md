# SERIAL_bRxQueueEmpty()

## 解説

SERIALライブラリにより、UART から受信が FIFO バッファーに格納されているかを確認する。

{% hint style="info" %}
FIFOバッファーからのデータの取り出しには [SERIAL_i16RxChar](serial_i16rxchar.md)() を用います。
{% endhint %}

## 引数

| 型     | 名前           | 詳細                                              |
| ----- | ------------ | ----------------------------------------------- |
| uint8 | u8SerialPort | シリアルポート (E_AHI_UART\_0 または E_AHI_UART\_1 を指定する） |

## 戻り値

| 型      | 詳細                                                                 |
| ------ | ------------------------------------------------------------------ |
| bool_t | TRUE ならSERIALライブラリが管理するFIFOキューに受信データが存在する。FALSEの場合、キューにデータが残っています。 |

## サンプル

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
