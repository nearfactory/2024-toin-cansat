---
title: "シリアルポート選択"
author: "Mono Wireless Inc."
description: "シリアルポート選択"
---

# シリアルポート選択

`Windows` `macOS` `Linux` `RasPi`

![シリアルポート選択](../../.gitbook/assets/img_sel_serial.png)

Windows / macOS / Linux では、始動時にTWELITEが接続されたシリアルポートを選択する画面を表示します。
ただし、シリアルポートは、あとから接続することもできます。

{% hint style="info" %}
TWELITE STAGEアプリでは、シリアルポートの選択と各画面の処理は連携しません。
例えば、シリアルポートの選択をしていない状態で、ビューアを起動しても表示は更新されません。この状態でシリアルポートを選択すると、ビューワの表示が更新されます。

なお、シリアルポートは `Alt(Cmd)+0,1,2,...` のキー操作でいつでも切り替えできます。
{% endhint %}



#### Windows

`c`キーを押すと、リストで反転中のシリアルポートのCOMポート名が表示されます。


#### Raspberry Pi

Raspberry Pi ではUSBデバイスに加えて`/dev/serial0` `/dev/serial1`があれば `serial0`, `serial1` を表示します。なお、通常は`serial0`を使用します。
