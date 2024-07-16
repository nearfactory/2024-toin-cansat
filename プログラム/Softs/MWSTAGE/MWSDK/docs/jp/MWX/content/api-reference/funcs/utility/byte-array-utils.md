---
title: "Byte array utils"
author: "Mono Wireless Inc."
description: "Byte array utils"
---
# Byte array utils

### 読み出し

バイト配列から、`uint8_t,` ビッグエンディアン並びとして、`uint16_t`, `uint32_t `の値を取得する。

```cpp
	inline uint8_t G_BYTE(const uint8_t*& p) {
		return *(p)++; 
	}
	inline uint16_t G_WORD(const uint8_t*& p) {
		uint32_t r = *p++;
		r = (r << 8) + *p++;
		return r;
	}
	inline uint32_t G_DWORD(const uint8_t*& p) {
		uint32_t r = *p++;
		r = (r << 8) + *p++;
		r = (r << 8) + *p++;
		r = (r << 8) + *p++;
		return r;
	}
```

`p` は読み出したバイト数分だけインクリメントされる。



### 書き込み

ポインタ`q`で指定するバイト配列に`uint8_t`,ビッグエンディアンで`uint16_t`,`uint32_t`の値を書き込む。

```cpp
	inline uint8_t& S_OCTET(uint8_t*& q, uint8_t c) {
		*q++ = c;
		return *q;
	}
	inline uint8_t& S_WORD(uint8_t*& q, uint16_t c) {
		*(q) = ((c) >> 8) & 0xff; (q)++;
		*(q) = ((c) & 0xff); (q)++;
		return *q;
	}
	inline uint8_t& S_DWORD(uint8_t*& q, uint32_t c) {
		*(q) = ((c) >> 24) & 0xff; (q)++;
		*(q) = ((c) >> 16) & 0xff; (q)++;
		*(q) = ((c) >>  8) & 0xff; (q)++;
		*(q) = ((c) & 0xff); (q)++;
		return *q;
	}
```

`q` は書き込んだバイト数分だけインクリメントされる。



## 背景

無線パケットのデータペイロードの生成・分解時の操作を簡略化するため。

より簡略化した`pack_bytes()`, `expand_bytes()`も利用できます。
