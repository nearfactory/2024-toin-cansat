# SERIAL_vInit()

## 解説

UART （シリアルポート）の初期化を行う。

本関数では１つの UART ポートの初期化を行い SERIAL ライブラリによるアクセスを可能とします。

{% hint style="info" %}
初期化時に SERIAL ライブラリが内部的に利用する FIFO バッファーのメモリ領域を静的に確保する必要があります。
{% endhint %}

{% hint style="info" %}
同時に、出力用の [vfPrintf](../fprintf-raiburari/vfprintf.md)(), [vPutChar](../fprintf-raiburari/vputchar.md)() を行うため [tsFILE](../fprintf-raiburari/tsfile.md) 構造体の初期化を行っておきます。
{% endhint %}

## 引数

| 型                                           | 名前      | 詳細             |
| ------------------------------------------- | ------- | -------------- |
| [tsSerialPortSetup](tsserialportsetup.md)\* | psSetup | UART の設定用の構造体。 |

## 戻り値

なし

## サンプル

[tsSerialPortSetup](tsserialportsetup.md) 参照。
