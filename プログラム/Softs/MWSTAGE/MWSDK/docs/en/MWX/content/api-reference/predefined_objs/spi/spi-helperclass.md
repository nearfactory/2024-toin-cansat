---
title: SPI access (using helper class)
author: "Mono Wireless Inc."
description: SPI access (using helper class)
---
# SPI access (using helper class)

The helper class version is a more abstract implementation. Creating a `transceiver` object for reading and writing is the start of using the bus, and destroying the object is the end of using the bus.

By creating the object in the decision expression of the if statement, the validity period of the object is limited to the scope of the if clause, and the object is destroyed when it exits the if clause, at which point the bus usage termination procedure is performed.

```cpp
uint8_t c;
if (auto&& trs = SPI.get_rwer()) { // Object creation and device communication determination
   // Within this scope (wave brackets) is the validity period of trs.
   trs << 0x00; // Writes 0x00 with mwx::stream interface
   trs >> c;    // Store the read data in c.
} 
// wrt is discarded at the point where the if clause is exited, and the use of the bus is terminated
```

In addition, read/write objects implement the `mwx::stream` interface, allowing the use of the `<<` operator, etc.

* The start and end of bus usage is aligned with the validity period of the object to improve the visibility of the source code and to prevent omissions in the termination procedure, etc.
* Unify read/write procedures with the `mwx::stream` interface

## read/write

Reading process and its termination procedure in scope `if() { ... }` to do the reading with a helper class.

```cpp
inline uint8_t _spi_single_op(uint8_t cmd, uint8_t arg) {
    uint8_t d0, d1;
    if (auto&& x = SPI.get_rwer()) {
        d0 = x.transfer(cmd); (void)d0;
        d1 = x.transfer(arg);
        // (x << (cmd)) >> d0;
        // (x << (arg)) >> d1;
    }

    return d1;
}
```

In the above, the `x` object created by the `get_rwer()` method is used to read and write one byte at a time. 1.

1. create `x` object in `if(...)` Generate the `x` object in the `get_rwer()` method. At the same time, set the select pin of the SPI bus. (The type is resolved with the universal reference `auto&&` by type inference. 2.)
2. the generated `x` object has a `operator bool ()` defined, which is used to evaluate the decision expression, which is always `true` for the SPI bus.
3. `x` object defines `uint8_t transfer(uint8_t)` method, which is called to perform 8bit read/write transfer to SPI. 4.
4.`if() { ... }` Destructor of `x` is called at the end of the scope to release the select pin of the SPI bus.



### get\_rwer()

```cpp
periph_spi::transceiver get_rwer()
```

Obtains the worker object used to read/write the SPI bus.



## worker object

### transfer() transfer16() transfer32()

```cpp
uint8_t transfer(uint8_t val)
uint16_t transfer16(uint16_t val)
uint32_t transfer32(uint32_t val)
```

Each transfers 8-bit, 16-bit, and 32-bit data, and returns the read result with the same data width as the written data width.



### << operator

```cpp
operator << (int c)
operator << (uint8_t c)
operator << (uint16_t c) 
operator << (uint32_t c)
```

The `int` and `uint8_t` types perform 8-bit transfer.

Types `uint16_t` and `uint32_t` perform 16-bit and 32-bit transfers, respectively.

The transfer result is stored in an internal FIFO queue of up to 16 bytes and read by the `>>` operator. Since the buffer is not large, it is assumed to be read out after each transfer.



### >> operator

```cpp
operator >> (uint8_t& c)
operator >> (uint16_t& c)
operator >> (uint32_t& c)

null_stream(size_t i = 1)
operator >> (null_stream&& p)
```

Specify a variable with the same data width as the previous transfer.

If the result of the read is not needed, use the null\_stream() object; it skips reading by the data byte specified by i. If the result of the read is not needed, use the null\_stream() object.

