---
title: .get_stream_helper()
author: "Mono Wireless Inc."
---

# .get\_stream\_helper()

Operators and methods by [`mwx::stream`](../twe-stream/) via a [stream_helper](../twe-stream/stream\_helper.md) [helper object](../twe-stream/stream\_helper.md) that references a smplbuf array of type `uint8_t`.

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

//Result: ABCD;0123;99
```

Helper object type names are resolved by `auto&&` because they are long. The interfaces defined in `mwx::stream`, such as `<<` operator, can be used for this object.

The generated helper object `bs` starts reading/writing from the beginning of the main array `b` when it is created. If it is at the end of the array, data is added by `append()`. Each time a read/write operation is performed, the position is moved to the next one.

Helper functions can use the `>>` operator for reading.

```cpp
//..Continuation of the above example
// ABCD;0123;99 <- stored in b

//Variable for storing read data
uint8_t FOURCHARS_READ[4];
uint32_t u32val_read;
uint8_t c_read[2];

// Read out by operator>>
bs.rewind();                //Rewind the position to the beginning.
bs >> FOURCHARS_READ;      //4 chars
bs >> mwx::null_stream(1); //1 char skipping
bs >> u32val_read;         //32bit data
bs >> mwx::null_stream(1); //1 char skipping
bs >> c_read;              //2 chars

// Result
Serial << crlf << "4chars=" << FOURCHARS_READ;
Serial << crlf << format("32bit val=0x%08x", u32val_read);
Serial << crlf << "2chars=" << c_read;

// 4chars=ABCD
// 32bit val=0x30313233
// 2chars=99
```

