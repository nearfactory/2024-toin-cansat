---
title: E_PKT
author: "Mono Wireless Inc."
description: packet type definition
---

# E\_PKT

Corresponds to the following packet

{% hint style="info" %}
Corresponds to the ASCII format output by the Parent Node of App\_Wings.
{% endhint %}

| Name | Description                                                                                                                                                                           |
| ------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| PKT\_ERROR | TwePacket does not contain meaningful data such as before packet interpretation or packet type cannot be identified.
| PKT\_TWELITE | [0x81 command](https://mono-wireless.com/jp/products/TWE-APPS/App\_Twelite/step3-81.html) of the standard application App\_Twelite is interpreted.
| PKT\_PAL | Interpret serial format of [TWELITE PAL](https://mono-wireless.com/jp/products/TWE-APPS/App\_pal/index.html)
| PKT\_APPIO | [UART message](https://mono-wireless.com/jp/) of remote control application [App\_IO](https://mono-wireless.com/jp/products/TWE-APPS/App\_IO/) products/ TWE-APPS/App\_IO/uart.html) interpreted by the remote control application [App\_IO](\_IO/).
| PKT\_APPUART | [Extended format](https://mono-wireless.com/jp/) of the serial communication application [App\_UART ](https://mono-wireless.com/jp/products/TWE-APPS/App\_Uart/) products/ TWE-APPS/App\_Uart/mode\_format.html) is interpreted. |
| PKT\_APPTAG | The UART message of the wireless tag application App\_Tag is interpreted. The sensor specific part is not interpreted and the byte string is reported as payload.                                                                                                          | PKT\_ACT\PTTAG
| PKT\_ACT\_STD | Output format used in [Act (Act)](https://mono-wireless.com/jp/products/act/index.html) sample, etc.                                                                                           |
