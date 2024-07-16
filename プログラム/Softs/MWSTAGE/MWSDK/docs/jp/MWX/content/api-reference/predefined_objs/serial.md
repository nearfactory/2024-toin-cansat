---
title: "Serial"
author: "Mono Wireless Inc."
description: "TWELITE の UART0 ポート  (mwx::serial_jen)"
---

# Serial

``[`mwx::stream`](../classes/twe-stream/) を実装し TWELITE の UART0 で入出力する。

* `Serial`オブジェクトはシステム起動時に UART0, 115200 bps で初期化され、ライブラリ内で初期化処理が行われます。ユーザコード上は、`setup()`から利用できます。
* `Serial1`オブジェクトは、ライブラリ内で用意されていますが、初期化処理は行っていません。UART1を有効化するためには、必要な初期化手続き `Serial1.setup(), Serial1.begin()` を行ってください。

{% hint style="danger" %}
起動直後の `setup(), wakeup()` やスリープ直前の `flush` 処理で、出力が不安定になる場合があります。
{% endhint %}

###

### setup()

```cpp
void setup(uint16_t buf_tx, uint16_t buf_rx)
```

オブジェクトの初期化を行う。

* TX/RX用のFIFOバッファのメモリ確保
* TWE\_tsFILE 構造体のメモリ確保

{% hint style="warning" %}
`Serial`(UART0) は ライブラリ内で `setup() `の呼び出しが自動で行われます。ユーザによる呼び出しを行う必要はありません。

また、`Serial `(UART0) のバッファサイズは、コンパイル時に決定されます。マクロ `MWX_SER_TX_BUFF `(未指定時は 768), `MWX_SER_RX_BUFF`(未指定時 256) により変更できます。
{% endhint %}

| パラメータ    | 解説              |
| -------- | --------------- |
| `buf_tx` | TX用のFIFOバッファサイズ |
| `buf_rx` | RX用のFIFOバッファサイズ |



### begin()

```cpp
void begin(unsigned long speed = 115200, uint8_t config = 0x06)
```

ハードウェアの初期化を行う。

{% hint style="warning" %}
`Serial` (UART0) は ライブラリ内で `begin()` の呼び出しが自動で行われます。ユーザによる呼び出しを行う必要はありません。
{% endhint %}

| パラメータ    | 解説                                                                                                |
| -------- | ------------------------------------------------------------------------------------------------- |
| `speed`  | UART のボーレートを指定する。                                                                                 |
| `config` | `serial_jen::E_CONF::PORT_ALT`ビットを指定したときは、UART1をDIO14,15で初期化します。指定しない場合はDIO11(TxD),9(RxD)で初期化します。 |

{% hint style="info" %}
指定したボーレートの下２桁の数値は０に丸めて処理します。またハードウェアの制限により指定したボーレートより誤差が生じます。

ボーレートの計算には除算が発生し計算時間がかかる場合があります。9600,38400,115200を指定する場合は、除算をせずに計算を行います。処理の詳細は、`constexpr uint16_t _serial_get_hect_baud(uint32_t baud)`を参照してください。
{% endhint %}



### end()

（未実装）ハードウェアの使用を停止する。



### get\_tsFile()

```cpp
TWE_tsFILE* get_tsFile();
```

Cライブラリで利用する`  TWE_tsFILE*  `形式での構造体を得る。

{% hint style="info" %}
Serial (UART) では、`_sSerial` 構造体が定義されています。
{% endhint %}

