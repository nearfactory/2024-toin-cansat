---
title: "pack_bytes()"
author: "Mono Wireless Inc."
description: "要素データを並べてバイト列を生成"
---
# pack\_bytes()

要素データを並べてバイト列を生成します。

```cpp
uint8_t* pack_bytes(uint8_t* b, uint8_t* e, ...)
```

`pack_bytes`はコンテナクラスの`begin()`,`end()`イテレータをパラメータとし、続くパラメータで指定されるデータをバイト列としてコンテナに書き込みます。

可変引数パラメータに与えるデータは以下に示すとおりです。

| データ型                 | バイト数 | 解説                                                 |
| -------------------- | :--: | -------------------------------------------------- |
| `uint8_t`            |   1  |                                                    |
| `uint16_t`           |   2  | ビッグエンディアン並びで格納される                                  |
| `uint32_t`           |   4  | ビッグエンディアン並びで格納される                                  |
| `uint8_t[N]`         |   N  | `uint8_t` 型の固定長配列                                  |
| `std::pair<char*,N>` |   N  | `char*`,`uint8_t*`型の配列と配列長のペア。`make_pair()`で生成できる。 |



```cpp
smplbuf_u8& pack_bytes(smplbuf_u8& c, ...)
```

`pack_bytes`はコンテナオブジェクトをパラメータとし、続くパラメータで指定されるデータをバイト列としてコンテナに書き込みます。コンテナの`.push_back()`メソッドで末尾に追加します。

可変引数パラメータに与えるデータは以下に示すとおりです。

| データ型                 |    バイト数   | 解説                                                    |
| -------------------- | :-------: | ----------------------------------------------------- |
| `uint8_t`            |     1     |                                                       |
| `uint16_t`           |     2     | ビッグエンディアン並びで格納される                                     |
| `uint32_t`           |     4     | ビッグエンディアン並びで格納される                                     |
| `uint8_t[N]`         |     N     | `uint8_t` 型の固定長配列                                     |
| `std::pair<char*,N>` |     N     | `char*`,`uint8_t*`型の配列と配列長のペア。`make_pair()`で生成できる。    |
| `smplbuf_u8?`        | `.size()` | `uint8_t`型の`smplbuf<>`コンテナ。コンテナ長(`.size()`)のデータを格納する。 |



## 例

```cpp
auto&& rx = the_twelite.receiver.read();

smplbuf<uint8_t, 128> buf;
mwx::pack_bytes(buf
	, uint8_t(rx.get_addr_src_lid())	// src addr (LID)
	, uint8_t(0xCC)							      // cmd id (0xCC, fixed)
	, uint8_t(rx.get_psRxDataApp()->u8Seq)	// seqence number
	, uint32_t(rx.get_addr_src_long())		// src addr (long)
	, uint32_t(rx.get_addr_dst())			// dst addr
	, uint8_t(rx.get_lqi())					  // LQI
	, uint16_t(rx.get_length())				// payload length
	, rx.get_payload() 						    // payload
);
```

この例では受信パケットの各属性やペイロードを別のバッファ`buf`に再格納しています。



## 背景

無全パケットのデータペイロードの生成やデータの取り出しで用いられる`uint8_t`型のバイト配列の記述を簡素化するため。

```cpp
auto&& rx = the_twelite.receiver.read();

uint8_t data[128];
data[0] = rx.get_addr_src_lid();
data[1] = 0xCC;
data[2] = rx.get_psRxDataApp()->u8Seq;
data[4] = rx.get_addr_src_long() & 0x000000FF;
data[5] = (rx.get_addr_src_long() & 0x0000FF00) >> 8;
data[6] = (rx.get_addr_src_long() & 0x00FF0000) >> 16;
data[7] = (rx.get_addr_src_long() & 0xFF000000) >> 24;
...
```

上記はもっとも単純な記述だが、以下のように[Byte array utils](byte-array-utils.md)を用いてバイト配列を生成できる。

```cpp
auto&& rx = the_twelite.receiver.read();

uint8_t data[128], *q = data;
S_OCTET(q, rx.get_addr_src_lid());
S_OCTET(q, 0xCC);
S_OCTET(q, rx.get_psRxDataApp()->u8Seq);
S_DWORD(q, rx.get_addr_src_long());
S_DWORD(q, rx.get_addr_dst());
S_OCTET(q, rx.get_lqi());
S_WORD(q, rx.get_length());
for (auto x : rx.get_payload()) {
  S_OCTET(q, x);
}
```
