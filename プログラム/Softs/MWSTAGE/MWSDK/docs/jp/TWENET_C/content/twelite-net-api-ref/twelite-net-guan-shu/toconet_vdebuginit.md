# ToCoNet_vDebugInit()

## 解説

TWELITE NET のデバッグメッセージ出力の初期化を行います。

指定した [UART](../../hw-api-ref/perifuraru/uart/) 出力は、アプリケーションの出力と共用で利用され、アプリケーションの出力の間にデバッグメッセージが出力されます。

{% hint style="warning" %}
[Makefile](../../twelite-sdk-howto/birudonitsuite/makefile-nitsuite.md) にて`TOCONET_DEBUG`を`1`に設定して、ビルドを実行します。
{% endhint %}

{% hint style="danger" %}
デバッグメッセージは TWELITE NET の内部開発向けで、出力書式等については開示しません。問題の解析等を行う際に、お客様に出力をご連絡いただく場合があります。
{% endhint %}

## 引数

| 型                                                                                 | 名前     | 詳細                     |
| --------------------------------------------------------------------------------- | ------ | ---------------------- |
| [`tsFILE`](../../hw-api-ref/perifuraru/uart/fprintf-raiburari/tsfile.md)`*` | `pSer` | UART出力先を定義した構造体へのポインタ。 |

## 戻り値

なし。

## サンプル

```c
#include "serial.h"
#include "fprintf.h"

tsFILE sSerStream; // use for fprintf
tsSerialPortSetup sSerPort;

void initser() {
  //... setup serial port
  sSerPort.u8SerialPort = E_AHI_UART_0;
  //...
  SERIAL_vInitEx(&sSerPort, pUartOpt);
  
  // setup fprintf
  sSerStream.bPutChar = SERIAL_bTxChar; // lib function
  sSerStream.u8Device = E_AHI_UART_0; // UART0
  
  // setup TWELITE NET debug message.
  ToCoNet_vDebugInit(&sSerStream);
  ToCoNet_vDebugLevel(0); // start with no debug message.
}
```

