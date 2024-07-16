---
title: "パケット種別定義"
author: "Mono Wireless Inc."
description: "パケット種別定義"
---

# E\_PKT

以下のパケットに対応します。

{% hint style="info" %}
App\_Wings の親機で出力されるアスキー書式に対応します。
{% endhint %}

| 名前            | 解説                                                                                                                                                                           |
| ------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PKT\_ERROR    | パケット解釈前やパケット種別が特定できないなど、TwePacketには意味のあるデータが格納されていない                                                                                                                         |
| PKT\_TWELITE  | 標準アプリ App\_Twelite の [0x81 コマンド](https://mono-wireless.com/jp/products/TWE-APPS/App\_Twelite/step3-81.html)を解釈したもの                                                           |
| PKT\_PAL      | [TWELITE PAL](https://mono-wireless.com/jp/products/TWE-APPS/App\_pal/index.html)のシリアル形式を解釈したもの                                                                              |
| PKT\_APPIO    | リモコンアプリ [App\_IO ](https://mono-wireless.com/jp/products/TWE-APPS/App\_IO/)の[UARTメッセージ](https://mono-wireless.com/jp/products/TWE-APPS/App\_IO/uart.html)を解釈したもの             |
| PKT\_APPUART  | シリアル通信アプリ [App\_UART ](https://mono-wireless.com/jp/products/TWE-APPS/App\_Uart/)の[拡張書式](https://mono-wireless.com/jp/products/TWE-APPS/App\_Uart/mode\_format.html)を解釈したもの。 |
| PKT\_APPTAG   | 無線タグアプリApp\_TagのUARTメッセージを解釈したもの。センサ固有部分は解釈されずpayloadとしてバイト列を報告します。                                                                                                          |
| PKT\_ACT\_STD | [アクト(Act)](https://mono-wireless.com/jp/products/act/index.html)のサンプルなどで使用される出力書式。                                                                                           |
