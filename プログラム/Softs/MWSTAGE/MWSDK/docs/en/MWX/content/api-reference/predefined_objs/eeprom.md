---
title: EEPROM
author: "Mono Wireless Inc."
description: EEPROM
---
# EEPROM

TWELITE Reads and writes to the built-in EEPROM of the wireless microcontroller.

The built-in EEPROM has 3480 bytes available from address 0x000 to 0xEFF.

The first part is [Settings (Interactive settings mode)](../../settings/), so it is recommended to use the second half of the address when used together. The amount of space consumed by the settings (Interactive settings mode) depends on its implementation. Even with minimal settings, up to 256 bytes are used from the beginning, so use of the later addresses is recommended.

## Methods

### read()

```cpp
uint8_t read(uint16_t address)
```

Read the data corresponding to `address` from EEPROM.

{% hint style="warning" %}
No error detection.
{% endhint %}



### write()

```cpp
void write(uint16_t address, uint8_t value)
```

Write `value` from EEPROM to `address`.

{% hint style="warning" %}
No error detection.
{% endhint %}



### update()

```cpp
void update(uint16_t address, uint8_t value)
```

This function is used when you want to reduce the number of rewrites in consideration of the rewrite life of EEPROM.



### get\_stream\_helper()

```cpp
auto&& get_stream_helper()
// The return type is abbreviated as auto&& due to its length.
```

Obtain a helper object to read and write using `mwx::stream` described below.



## mwx::streamインタフェースを用いた入出力

[stream\_helper](../classes/twe-stream/stream\_helper.md) [helper object](../classes/twe-stream/stream\_helper.md) [`mwx::stream`](../classes/twe-stream/) operators and methods. Using `mwx::stream`, you can read and write integer types such as `uint16_t` and `uint32_t` types, read and write fixed-length array types such as `uint8_t`, and format them using `format()` objects.

```cpp
auto&& strm = EEPROM.get_stream_helper();
// Helper object type names are resolved by auto&& due to their length.
```

Interfaces defined in `mwx::stream`, such as the `<<` operator, can be used on this object.



```cpp
strm.seek(1024); // Move to 1024th byte

strm << format("%08x", 12345678); // Record 12345678 as 8 characters in hexadecimal
strm << uint32_t(0x12ab34cd);     // Record 4 bytes of 0x12ab34cd
uint8_t msg_hello[16] = "HELLO WORLD!";
strm << msg_hello;                // Record "HELLO WORLD!" byte sequence (unterminated)

// result
// 0400: 30 30 62 63 36 31 34 65 12 ab 34 cd 48 45 4c 4c
//        0  0  b  c  6  1  4  e  0x12ab34cd  H  E  L  L
// 0410: 4f 20 57 4f 52 4c 44 21 00 00 00 00 ff ff ff ff
//        O SP  W  O  R  L  D  ! 
```

`.seek()` is used to move the EEPROM address to 1024.

The above writes an 8-byte string (`00bc614e`), a 4-byte integer (`0x12ab34cd`), a 16-byte byte string (`HELLO WORLD!.... `), and a 1-byte terminating character.



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

Move the EEPROM address to 1024 using `.seek()`.

Reads the data sequence written out earlier. In order, 8-byte characters, 4-byte integers, and 16-byte strings are read out using the `>>` operator.
