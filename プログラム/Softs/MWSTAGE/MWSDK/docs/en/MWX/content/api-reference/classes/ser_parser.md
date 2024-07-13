---
title: serparser
author: "Mono Wireless Inc."
description: serial format input-output (mwx::serial_parser)
---

# serparser

Used for serial format input/output. It has an internal buffer that holds the interpreted binary series. On input, the series is stored in the internal buffer one byte at a time according to the read format, and becomes complete when the interpretation of the series is complete. Conversely, on output, the buffer is output from the internal buffer according to the specified output format.



Three class names are defined according to the memory buffer handling method ([`alloc`](alloc.md)).

```cpp
// serparser_attach : Use existing buffers
serparser_attach

// serparser : Allocate N bytes of buffer internally
serparser_local<N>

// serparser_heap : Allocate buffers in the heap area
serparser_heap
```



## constants (format type)

The type of format to pass in the initialization parameter of `begin()`. There are two types here: ASCII format and binary format.

| constant | type      |
| ---------------------------- | ------- |
| `uint8_t PARSER::ASCII = 1` | ASCII format |
| `uint8_t PARSER::BINARY = 2` | binary format |

{% hint style="warning" %}
Binary format is generally more complicated to handle than ASCII format, including the necessary tools and confirmation methods. Normally, ASCII format should be used.
{% endhint %}



## About format

### ASCII format

The ASCII format is a way to represent a sequence of data in binary as a string.

For example, `00A01301FF123456` in ASCII format is expressed as follows. The beginning is `:`, `B1` is the checksum, and the end is `[CR:0x0d][LF:0x0a]`.

> `:00A01301FF123456B1[CR][LF]`

You can omit the terminating checksum. Replace the CRLF series from the checksum with `X`. This is useful when you want to send data for experiments, etc., although it is less vulnerable to wrong data series due to garbled characters.

> `:00A01301FF123456X`

#### Definition.

| ====== | Bytes of original data | Bytes | Description                                                                                                                                                                                                                                    |
| ------ | :-------: | :--: | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| header | | | 1 | `:`(0x3A) Specifies a colon.                                                                                                                                                                                                                  | 1 | 1
| Data part | N | 2N | Each byte of the original data is represented by two ASCII characters (A-F in upper case). <br> For example, 0x1F is represented as `1` (0x31) `F` (0x46).  |
| checksum | | 2 | The sum of each byte of the data part is calculated in 8-bit width and taken as 2's complement. In other words, the sum of each byte of the data part plus the checksum byte is calculated to be 0 in 8-bit width. <br>The checksum byte is represented by two ASCII characters. <br>For example, in `00A01301FF123456`, 0x00 + 0xA0 + ... + 0x56 = 0x4F, the two's complement of which is 0xB1. (i.e. 0x4F + 0xB1 = 0x00) |
| footer | | 2 | \[CR] (0x0D) \[LF] (0x0A)                                                                                                                                                                                                      | | 2 | \[CR] (0x0D)



### Binary Format

{% hint style="warning" %}
Normally, **ASCII format** should be used.

However, to check the transmission and reception in experiments, it is necessary to prepare a special terminal that supports binary communication, and checksum calculation is also required. It is more difficult to use than the ASCII format.
{% endhint %}

Binary format is a method of sending a sequence of data consisting of binary data with a header and a checksum.

For example, `00A01301FF123456` is expressed in binary format as follows.

> `0xA5 0x5A 0x80 0x08 0x00 0xA0 0x13 0x01 0xFF 0x12 0x34 0x56 0x3D`



#### Definition.

| ====== | Bytes of original data | Bytes in format | Description                                                                                                                    |
| ------ | :-------: | :--------: | --------------------------------------------------------------------------------------------------------------------- |
| header | | | 2 | `0xA5 0x5A`.                                                                                                   | 2 | The data length is two bytes in big-endian format.
| Data Length | | 2 | The data length is two bytes in big-endian format, with MSB (0x8000) set and the length of the data part specified. <br>For example, if the length of the data part is 8 bytes, specify `0x80 0x08`. | N | N | N | N | N | N | N | N | N | N | N
| Data part | N | N | Specifies the original data.                                                                                                           | N | N | Original data.
| Checksum | | 1 | Calculates the XOR of each byte of the data part. <br> For example, if the data part is `00A01301FF123456`, 0x00 xor 0xA0 xor ... 0x56 = 0x3D.    | (1)
| footer | | (1) | The checksum is effectively the end of the line. The output from the radio module will be appended with `0x04` (EOT).                                                                  | 



## Methods

### declaration, begin()

```cpp
// serparser_attach : use existing buffer
serparser_attach p1;

uint8_t buff[128];
p1.begin(ARSER::ASCII, buff, 0, 128);


// serparser : allocate N bytes buffer inside
serparser p2<128>;
p2.begin(PARSER::ASCII);


// serparser_heap : allocate buffer in heap area
serparser_heap p3;
p3.begin(PARSER::ASCII, 128);
```

The declaration specifies the memory allocation class. Since this specification is complicated, an alias definition is used as described above.

| Class name (alias definition) <br>Memory Allocation | Contents                                   |
| -------------------------- | ------------------------------------ |
| `serparser_attach` | specify an already existing buffer with `begin()` |
| `serparser_local<N>` | allocate a buffer of N bytes internally |
| `serparser_heap` | allocate a heap of the size specified by the parameters of the `begin()` method |

Calls the `begin()` method according to the memory allocation class.



#### serparser\_attach&#x20;

```cpp
void begin(uint8_t ty, uint8_t *p, uint16_t siz, uint16_t max_siz)
```

The buffer specified by `p` is used in the [format] (ser\_parser.md#nitsuite) specified by `ty`. The maximum length of the buffer is specified by `max_siz` and the effective data length of the buffer by `siz`.

This definition is used especially when you want to format output data columns (see `>>` operator).



#### serparser\_local\<N> - Allocate internal buffer

```cpp
void begin(uint8_t ty)
```

Initialize with the [format](ser\_parser.md#nitsuite) specified by `ty`.



#### serparser\_heap - Allocated to heap

```cpp
void begin(uint8_t ty, uint16_t siz)
```

Initialize the heap by allocating the size specified by `siz` to the heap in the [format](ser\_parser.md#nitsuite) specified by `ty`.

{% hint style="warning" %}
Once allocated, heap space cannot be released.
{% endhint %}





### get\_buf()

```cpp
BUFTYPE& get_buf()
```

Returns an internal buffer. The buffer will be of type `smplbuf<uint8_t, alloc>`.



### parse()

```cpp
inline bool parse(uint8_t b)
```

Processes input characters. Receives a single byte of input string of formatted input and interprets it according to the format. For example, in ASCII format, it receives a series like `:00112233X` as input. X`, one byte at a time, and when the last `X` is entered, the interpretation of the format is completed and reported as done.

The parameter to `parse()` is the input byte, and the return value is `true` if the interpretation is complete.

{% hint style="info" %}
When `parse()` reports that reading is complete, the next `parse()` will return to the reading-in-progress status.
{% endhint %}

#### 例

```cpp
while (Serial.available()) {
    int c = Serial.read();
    
    if (SerialParser.parse(c)) {
        // Format interpretation complete, data sequence obtained in b (smplbuf<uint8_t>)
        auto&& b = SerialParser.get_buf();
        
        // The following is the processing of the resulting data sequence
        if (b[0] == 0xcc) {
          // ...
        }
    }
}
```



### operator bool()

```cpp
operator bool() 
```

If `true`, reading is completed by `parse()`; if `false`, interpretation is in progress.

#### Example (parse() example can be rewritten as follows)

```cpp
while (Serial.available()) {
    int c = Serial.read();
    
    SerialParser.parse(c);
    
    if(SerialParser) {
        // Format interpretation complete, data sequence obtained in b (smplbuf<uint8_t>)
        auto&& b = SerialParser.get_buf();
        // ...
    }
}
```



### << operator

Outputs the internal buffer to the stream (Serial) in a formatted format.

#### Example

```cpp
uint8_t u8buf[] = { 0x11, 0x22, 0x33, 0xaa, 0xbb, 0xcc };

ser_parser pout;
pout.begin(ARSER::ASCII, u8buf, 6, 6); // Specify 6 bytes of u8buf

Serial << pout;// Format output to Serial -> :112233AABBCC??[CR][LF] 
```
