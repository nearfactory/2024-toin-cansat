---
title: Byte array utils
author: "Mono Wireless Inc."
description: Byte array utils
---
# Byte array utils

### Read

Get `uint16_t`, `uint32_t` values from the byte array as `uint8_t,` big-endian sequence.

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

`p` is incremented by the number of bytes read.



### Writing

Write `uint8_t`, big-endian `uint16_t` and `uint32_t` values to the byte array specified by pointer `q`.

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

`q` is incremented by the number of bytes written.



## Background

To simplify operations when generating and disassembling the data payload of wireless packets.

More simplified `pack_bytes()` and `expand_bytes()` are also available.
