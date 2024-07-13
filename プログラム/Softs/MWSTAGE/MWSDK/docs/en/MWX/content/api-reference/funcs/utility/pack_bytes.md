---
title: pack_bytes()
author: "Mono Wireless Inc."
description: Generates a sequence of bytes by arranging element data.
---
# pack\_bytes()

Generates a sequence of bytes by arranging element data.

```cpp
uint8_t* pack_bytes(uint8_t* b, uint8_t* e, ...)
```

`pack_bytes` takes a `begin()`,`end()` iterator of the container class as a parameter and writes the data specified by the following parameters to the container as a sequence of bytes.

The data given for the variable argument parameters are as follows

| Data Type | Number of Bytes | Description                                                 |
| -------------------- | :--: | -------------------------------------------------- |
| `uint8_t`            |   1  |                                                    |
| `uint16_t`           |   2  | Stored in big-endian order                                  |
| `uint32_t`           |   4  | Stored in big-endian order                                  |
| `uint8_t[N]`         |   N  | Fixed-length array of type `uint8_t`.                                  |
| `std::pair<char*,N>` |   N  | A pair of an array of type `char*`,`uint8_t*` and array length. Can be generated with `make_pair()`. |



```cpp
smplbuf_u8& pack_bytes(smplbuf_u8& c, ...)
```

The `pack_bytes` takes a container object as a parameter and writes the data specified by the subsequent parameters as a sequence of bytes into the container. It is appended to the end by the container's `.push_back()` method.

The data given as variable argument parameters is shown below.

| Data Type | Number of Bytes | Description                                                    |
| -------------------- | :-------: | ----------------------------------------------------- |
| `uint8_t`            |     1     |                                                       |
| `uint16_t`           |     2     | Stored in big-endian order                                     |
| `uint32_t`           |     4     | Stored in big-endian order                                     |
| `uint8_t[N]`         |     N     | Fixed-length array of type `uint8_t`.                                     |
| `std::pair<char*,N>` |     N     | A pair of an array of type `char*`,`uint8_t*` and array length. Can be generated with `make_pair()`.    |
| `smplbuf_u8?`        | `.size()` | smplbuf<>` container of type `uint8_t`. Container length (`.size()`) of data. |



## Example

```cpp
auto&& rx = the_twelite.receiver.read();

smplbuf<uint8_t, 128> buf;
mwx::pack_bytes(buf
	, uint8_t(rx.get_addr_src_lid())	    // src addr (LID)
	, uint8_t(0xCC)							// cmd id (0xCC, fixed)
	, uint8_t(rx.get_psRxDataApp()->u8Seq)	// seqence number
	, uint32_t(rx.get_addr_src_long())		// src addr (long)
	, uint32_t(rx.get_addr_dst())			// dst addr
	, uint8_t(rx.get_lqi())					// LQI
	, uint16_t(rx.get_length())				// payload length
	, rx.get_payload() 						// payload
);
```

In this example, each attribute and payload of the received packet is re-stored in a separate buffer `buf`.



## Background

To simplify the description of byte arrays of type `uint8_t` used in the generation of data payloads and extraction of data in non-volatile packets.

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

The above is the simplest description, but byte arrays can be generated using [Byte array utils](byte-array-utils.md) as follows.

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
