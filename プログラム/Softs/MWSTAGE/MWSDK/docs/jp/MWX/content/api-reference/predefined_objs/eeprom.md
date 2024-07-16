---
title: "EEPROM"
author: "Mono Wireless Inc."
description: "TWELITE 無線マイコンの内蔵EEPROMに対して読み書きを実行"
---
# EEPROM

TWELITE 無線マイコンの内蔵EEPROMに対して読み書きを実行します。

内蔵EEPROMはアドレス0x000～0xEFFまでの3480バイトが利用可能です。

先頭部分は[設定（インタラクティブモード）](../../settings/)に利用されるため、併用する場合は後半のアドレスの利用を推奨します。設定（インタラクティブモード）でどの程度の領域を消費するかは、その実装に依存します。最小限度の設定であっても先頭から256バイトまでは利用されるため、それ以降の利用を推奨します。

## メソッド

### read()

```cpp
uint8_t read(uint16_t address)
```

EEPROMから`address`に対応するデータを読み出します。

{% hint style="warning" %}
エラーの検出は行いません。
{% endhint %}



### write()

```cpp
void write(uint16_t address, uint8_t value)
```

EEPROMから`address`に対して`value`を書き込みます。

{% hint style="warning" %}
エラーの検出は行いません。
{% endhint %}



### update()

```cpp
void update(uint16_t address, uint8_t value)
```

`write()`と同じく書き込みを行いますが、先に`address`にあるデータを読み出して`value`と違う場合のみ、書き込みを行います。EEPROMの書き換え寿命を考慮し、書換回数を減らしたいときに用います。



### get\_stream\_helper()

```cpp
auto&& get_stream_helper()
// 戻り値型は長くなるためauto&&と省略しています。
```

後述の`mwx::stream`を用いた読み書きを行うために、ヘルパーオブジェクトを取得します。



## mwx::streamインタフェースを用いた入出力

[stream\_helper](../classes/twe-stream/stream\_helper.md) [ヘルパーオブジェクト](../classes/twe-stream/stream\_helper.md)を経由して、[`mwx::stream`](../classes/twe-stream/)による演算子やメソッドを用います。`mwx::stream`を用いると`uint16_t`や`uint32_t`型といった整数型の読み書き、`uint8_t`の固定長配列型の読み書き、`format()`オブジェクトによる書式整形などが可能になります。

```cpp
auto&& strm = EEPROM.get_stream_helper();
// ヘルパーオブジェクトの型名は長くなるためauto&&により解決しています。
```

このオブジェクトに対して`<<`演算子など`mwx::stream`で定義されたインタフェースを利用できます。



```cpp
strm.seek(1024); // 1024バイト目に移動

strm << format("%08x", 12345678); // 12345678を16進数の8文字で記録
strm << uint32_t(0x12ab34cd);     // 0x12ab34cd の4バイトを記録
uint8_t msg_hello[16] = "HELLO WORLD!";
strm << msg_hello;                // バイト列 "HELLO WORLD!" を記録(終端なし)

// 結果
// 0400: 30 30 62 63 36 31 34 65 12 ab 34 cd 48 45 4c 4c
//        0  0  b  c  6  1  4  e  0x12ab34cd  H  E  L  L
// 0410: 4f 20 57 4f 52 4c 44 21 00 00 00 00 ff ff ff ff
//        O SP  W  O  R  L  D  ! 
```

`.seek()`を用いてEEPROMのアドレスを1024に移動しています。

上記では８バイト文字列(`00bc614e`)、４バイト整数(`0x12ab34cd`)、16バイトバイト列(`HELLO WORLD!...`)、1バイト終端文字を書き込んでいます。



```cpp
strm.seek(1024);

uint8_t msg1[8];
strm >> msg1;
Serial << crlf << "MSG1=" << msg1;
// MSG1=00bc614e

uint32_t var1;
strm >> var1;
Serial << crlf << "VAR1=" << format("%08x", var1);
// VAR1=12ab34cd

uint8_t msg2[16]; // "HELLO WORLD!"の文字数
strm >> msg2;
Serial << crlf << "MSG2=" << msg2;
// MSG2=HELLO WORLD!
```

`.seek()`を用いてEEPROMのアドレスを1024に移動しています。

先ほど書き出したデータ列を読み出します。順番に8バイト文字、4バイト整数、16バイト文字列を`>>`演算子を用いて読み出します。
