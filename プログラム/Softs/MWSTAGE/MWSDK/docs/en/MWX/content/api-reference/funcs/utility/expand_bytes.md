---
title: expand_bytes()
author: "Mono Wireless Inc."
description: Decompose a sequence of bytes and store it in a variable.
---
# expand\_bytes()

Decompose a sequence of bytes and store it in a variable.

```cpp
const uint8_t* expand_bytes(
        const uint8_t* b, const uint8_t* e, ...)
```

The `expand_bytes()` parameter is a combination of iterators of type `uint8_t*`. This specifies the next iterator after the beginning and end of the parsed target. If the parsing proceeds to the `e` position, `nullptr` is returned.

If there is no error in expansion, the next iterator to be read is returned.

The variable number parameters can be the following

| Byte count | Data length | Explanation                                                |
| -------------------- | :--: | ------------------------------------------------- |
| `uint8_t`            |   1  |                                                   |
| `uint16_t`           |   2  | Expand as a big-endian sequence                                |
| `uint32_t`           |   4  | Expand as a big-endian sequence                                |
| `uint8_t[N]`         |   N  | Fixed-length array of type `uint8_t`.                                 |
| `std::pair<char*,N>` |   N  | Pairs of an array of type `char*`,`uint8_t*` and array length N can be generated with `make_pair()`. |



## Example

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

In this example, a 4-byte string is read out first. Here, `make_pair()` is used to explicitly read 4 bytes of data.

The next data is read out based on the returned iterator `np`. The next data is of type `uint8_t`, followed by five more of type `uint16_t`.



## Background

To simplify the description of byte arrays of type `uint8_t` used in generating data payloads and extracting data from non-volatile packets.

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

The above is the simplest description, but it can be read from a byte array using [Byte array utils](byte-array-utils.md) as follows

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
