---
title: mwx::bigendian
author: "Mono Wireless Inc."
description: Output data of numeric type in big-endian order to twe::stream
---

# mwx::bigendian

Helper class for `mwx::stream` `<<` operator to output numeric types as big-endian byte strings.

```cpp
Serial << mwx::bigendian(0x1234abcdUL);

// output binary -> 0x12 0x34 0xab 0xcd
```



### constructor

```cpp
template <typename T>
bigendian::bigendian(T v)
```



| Parameters | Description                             |
| ----- | ------------------------------ |
| `v` | a value of type `uint16_t` or `uint32_t` | a value of type `uint32_t`.





