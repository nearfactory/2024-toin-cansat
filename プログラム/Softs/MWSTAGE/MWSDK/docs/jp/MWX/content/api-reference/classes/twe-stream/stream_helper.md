---
title: "stream_helper"
author: "Mono Wireless Inc."
description: "stream_helper"
---

# stream\_helper

stream\_helperは、`mwx::stream`インタフェースを付与するヘルパーオブジェクトです。データクラスを参照するヘルパーオブジェクトを生成し、ヘルパーオブジェクト経由でデータの入出力を行います。

以下にはsmplbufの配列`b`からヘルパーオブジェクト`bs`を生成し`mwx::stream::operator <<()`演算子によるデータ入力を行っています。

```cpp
smplbuf_u8<32> b;
auto&& bs = b.get_stream_helper(); // ヘルパーオブジェクト

// データ列の生成
uint8_t FOURCHARS[]={'A', 'B', 'C', 'D'};
bs << FOURCHARS;
bs << ';';
bs << uint32_t(0x30313233); // "0123"
bs << format(";%d", 99);

Serial << b << crlf; // Serialへの出力は smplbuf_u8<32> クラス経由で

//結果: ABCD;0123;99
```



### 概要

stream\_helper はデータ配列をストリームに見立てて振舞います。

内部にはデータ配列中の読み書き位置を保持しています。次のようにふるまいます。

* 読み出しまたは書き込みをすると次の読み書き位置に移動します。
* 最期のデータを読み出した後、またはデータを末尾に追記した後には、読み書き位置は終端となります。
* 読み書き位置が終端の場合、
  * `available()`が`false`になります。
  * 読み出しは出来ません。
  * 書き込みは書き込み可能範囲であれば追記します。



## stream\_helperの生成

stream\_helper は、データクラス ([smplbuf](../smplbuf/), [EEPROM](../../predefined\_objs/eeprom.md)) のメンバー関数より生成します。

```cpp
auto&& obj_helper = obj.get_stream_helper()
// obj はデータクラスのオブジェクト、obj_helperの型は長くなるのでauto&&で受けています。
```



## メソッド

### rewind()

```cpp
void rewind()
```

読み書き位置を先頭に移動します。



### seek()

```cpp
int seek(int offset, int whence = MWX_SEEK_SET)
```

読み書き位置を設定します。

| whence         | 設定位置                                                     |
| -------------- | -------------------------------------------------------- |
| `MWX_SEEK_SET` | 先頭位置から設定します。`offset`に`0`を指定すると`rewind()`と同じ意味になります。      |
| `MWX_SEEK_CUR` | 現在位置を基準に`offset`分移動しまします。                                |
| `MWX_SEEK_END` | 終端位置にします。`offset`は`0`にすると終端に設定します。`-1`を設定すると最後の文字に移動します。 |



### tell()

```cpp
int tell()
```

読み書き位置を返します。終端位置の場合は`-1`を返します。



### available()

```cpp
int available()
```

読み書き位置が終端であれば`0`を返します。終端でなければそれ以外の値を返します。

