---
title: "expand_bytes()"
author: "Mono Wireless Inc."
description: "バイト列を分解し変数に格納"
---
# expand\_bytes()

バイト列を分解し変数に格納します。

```cpp
const uint8_t* expand_bytes(
        const uint8_t* b, const uint8_t* e, ...)
```

`expand_bytes()`は、パラメータに`uint8_t*`型のイテレータの組み合わせを指定します。これは解析対象の先頭と末尾の次のイテレータの指定となります。`e`の位置まで解析が進んだ場合はエラーとなり`nullptr`を返します。

展開にエラーがない場合は、次の読み出しを行うイテレータを戻します。

可変数パラメータには以下を指定します。

| バイト数                 | データ長 | 解説                                                |
| -------------------- | :--: | ------------------------------------------------- |
| `uint8_t`            |   1  |                                                   |
| `uint16_t`           |   2  | ビッグエンディアン並びとして展開する                                |
| `uint32_t`           |   4  | ビッグエンディアン並びとして展開する                                |
| `uint8_t[N]`         |   N  | `uint8_t` 型の固定長配列                                 |
| `std::pair<char*,N>` |   N  | `char*`,`uint8_t*`型の配列と配列長Nのペア`make_pair()`で生成できる |



## 例

```cpp
auto&& rx = the_twelite.receiver.read();

char fourchars[5]{}; 
auto&& np = 
	expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((uint8_t*)fourchars, 4)
    );

// read rest of payload
uint8_t u8DI_BM_remote = 0xff;
uint16_t au16AI_remote[5];
expand_bytes(np, rx.get_payload().end()
	, u8DI_BM_remote
	, au16AI_remote[0]
	, au16AI_remote[1]
	, au16AI_remote[2]
	, au16AI_remote[3]
	, au16AI_remote[4]
);
```

この例では、まず４バイトの文字列を読み出しています。ここでは`make_pair()`を用いて明示的に４バイト分のデータを読み出します。

戻されたイテレータ`np`をもとに、次のデータを読み出します。次のデータは`uint8_t`型、あとは`uint16_t`型が５つ続いています。



## 背景

無全パケットのデータペイロードの生成やデータの取り出しで用いられる`uint8_t`型のバイト配列の記述を簡素化するため。

```cpp
auto&& rx = the_twelite.receiver.read();
char fourchars[5]{}; 
uint8_t u8DI_BM_remote = 0xff;
uint16_t au16AI_remote[5];

uint8_t *p = rx.get_payload().begin();
fourchars[0] = p[0];
fourchars[1] = p[1];
fourchars[2] = p[2];
fourchars[3] = p[3];
fourchars[4] = 0;
p += 4;

u8DI_BM_remote = (p[0] << 8) + p[1]; p+=2;
au16AI_remote[0] = (p[0] << 8) + p[1]; p+=2;
...
```

上記はもっとも単純な記述だが、以下のように[Byte array utils](byte-array-utils.md)を用いてバイト配列から読み出せる。

```cpp
auto&& rx = the_twelite.receiver.read();
char fourchars[5]{}; 
uint8_t u8DI_BM_remote = 0xff;
uint16_t au16AI_remote[5];

uint8_t *p = rx.get_payload().begin();
fourchars[0] = G_BYTE(p);
fourchars[1] = G_BYTE(p);
fourchars[2] = G_BYTE(p);
fourchars[3] = G_BYTE(p);
fourchars[4] = 0;

u8DI_BM_remote = G_WORD(p);
au16AI_remote[0] = G_WORD(p);
...
```
