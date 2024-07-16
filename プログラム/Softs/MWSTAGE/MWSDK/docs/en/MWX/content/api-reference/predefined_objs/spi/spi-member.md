---
title: SPI access (using member functions)
author: "Mono Wireless Inc."
description: SPI access (using member functions)
---
# SPI access (using member functions)

After initializing the hardware by the `begin()` method, the bus can be read and written by `beginTransaction()`. Executing `beginTransaction()` selects the SPI select pin. Read/write is done with the `transfer()` function; SPI reads and writes at the same time.



### beginTransaction()

```cpp
void beginTransaction()
void beginTransaction(SPISettings settings)
```

Starts the use of the bus; sets the SPI select pin.

If called with the `settings` parameter given, the bus is set.



### endTransaction()

```cpp
void endTransaction()
```

Terminates the use of the bus; releases the SPI select pin.



### transfer(), transfer16(), transfer32()

```cpp
inline uint8_t transfer(uint8_t data)
inline uint16_t transfer16(uint16_t data)
inline uint32_t transfer32(uint32_t data)
```

Reads and writes the bus. `transfer()` transfers 8 bits, `transfer16()` transfers 16 bits, and `transfer32()` transfers 32 bits.
