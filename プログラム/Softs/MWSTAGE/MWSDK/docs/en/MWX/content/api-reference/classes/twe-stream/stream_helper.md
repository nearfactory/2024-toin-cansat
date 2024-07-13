---
title: stream_helper
author: "Mono Wireless Inc."
---

# stream\_helper

stream\_helper is a helper object that grants the `mwx::stream` interface. It creates a helper object that references a data class and performs data input/output via the helper object.

The following creates a helper object `bs` from an array `b` of smplbufs and inputs data using the `mwx::stream::operator <<()` operator.

```cpp
smplbuf_u8<32> b;
auto&& bs = b.get_stream_helper(); // helper object

// Data String Generation
uint8_t FOURCHARS[]={'A', 'B', 'C', 'D'};
bs << FOURCHARS;
bs << ';';
bs << uint32_t(0x30313233); // "0123"
bs << format(";%d", 99);

Serial << b << crlf; // Output to Serial via smplbuf_u8<32> class

//result: ABCD;0123;99
```



### Overview

stream\_helper behaves as if the data array were a stream.

Internally, it holds read/write positions in the data array. It behaves as follows.

* After the last data is read or written, it moves to the next read/write position.
* After the last data is read or data is appended to the end, the read/write position becomes the end.
* If the read/write position is the end of the line, then
  * `available()` becomes `false`.
  * Read cannot be performed.
  * Writing is appended if it is within the writable range.



## generating stream\_helper

stream\_helper is a data class ([smplbuf](../smplbuf/README.md), [EEPROM](../../predefined\_objs/eeprom.md)) member functions.

```cpp
auto&& obj_helper = obj.get_stream_helper()
// obj is an object of data class, obj_helper type is accepted by auto&& because it is long.
```



## methods

### rewind()

```cpp
void rewind()
```

Moves the read/write position to the beginning.



### seek()

```cpp
int seek(int offset, int whence = MWX_SEEK_SET)
```

Set the read/write position.

| whence | Set the location                                                     |
| -------------- | -------------------------------------------------------- |
| `MWX_SEEK_SET` | set from the leading position. If `offset` is `0`, it means the same as `rewind()`.      | `0` in `offset` means the same as `rewind()`.
| `MWX_SEEK_CUR` | Move by `offset` with respect to the current position.                                | `MWX_SEEK_CUR` | move the current position by `offset`.
| `MWX_SEEK_END` | end position. Setting `offset` to `0` sets the end position. If `-1` is set, it moves to the last character. | `-1` to move to the last character.



### tell()

```cpp
int tell()
```

Returns the read/write position. Returns `-1` for the end position.



### available()

```cpp
int available()
```

Returns `0` if the read/write position is the end. If it is not the end, it returns any other value.

