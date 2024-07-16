---
title: Scale utils
author: "Mono Wireless Inc."
description: scaling function between 0..1000 and 0..255.
---
# Scale utils

Scale (expand and contract) with 8-bit values (0..25, etc.) and user-friendly 0..1000 (thousandths, ‰) values. Approximate computation by multiplication and bit shift instead of division (`x*1000/255`) for low computational cost.

```cpp
static inline uint8_t scale_1000_to_127u8(uint16_t x)
static inline uint16_t scale_127u8_to_1000(uint8_t x)
static inline uint8_t scale_1000_to_255u8(uint16_t x)
static inline uint16_t scale_255u8_to_1000(uint8_t x)
static inline uint8_t scale_1000_to_256u8(uint16_t x)
static inline uint16_t scale_256u16_to_1000(uint16_t x)
```



### scale\_1000\_to\_127u8()

Scale 0..1000 to 0..127. Approximate calculation using `(16646*x+65000)>>17`.

### scale\_127u8\_to\_1000()

Scale 0..127 to 0..1000. Approximate calculation using `(2064000UL*x+131072)>>18`.

### scale\_1000\_to\_255u8()

Scales 0..1000 to 0..255. Approximate calculation using `(33423*x+65000)>>17`.

### scale\_255u8\_to\_1000()

Scales 0..255 to 0..1000. Approximate calculation using `(1028000UL*uint32_t(x)+131072)>>18`.

### scale\_1000\_to\_256u8()

Scales 0..1000 to 0..256. Approximate calculation using `(33554*x+66000) >> 17`.

Note: x=999,1000 returns 255 as the range of `uint8_t` even though the calculated value is 256.

### scale\_256u16\_to\_1000()

Scale 0..256 to 0..1000. Approximate calculation using `(1028000UL*uint32_t(x)+131072)>>18`.



## Background

Values to be set in hardware are often based on binary numbers such as 0...255, while numbers to be handled in user applications are easier to handle based on decimal numbers such as 0...1000. We defined an expression that does not perform division for these scale conversions.
