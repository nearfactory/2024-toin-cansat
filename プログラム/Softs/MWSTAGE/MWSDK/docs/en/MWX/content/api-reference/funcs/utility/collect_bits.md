---
title: collect_bits()
author: "Mono Wireless Inc."
description: Generating bitmaps with specified order.
---
# collect\_bits()

Obtains the value of a specified bit position from an integer and creates a bitmap of the specified order.

```cpp
constexpr uint32_t collect_bits(uint32_t bm, ...)
```

The value corresponding to the bit position 0..31 specified by the subsequent variable number parameter is extracted from the value specified in the parameter bm. The extracted values are arranged in parameter order and returned as a bitmap.

The bitmap is ordered with the first parameter as the upper bit and the last parameter as bit 0.

```cpp
uint32_t b1 = 0x12; // (b00010010)
uint32_t b2 = collect_bits(b1, 4, 2, 1, 0); 
  // bit4->1, bit2->0, bit1->1, bit0->0
  // b2=0x10 (b1010)
```

In the example, taking out bits 4,2,1,0 of b1 yields (1,0,1,0). This is calculated as 0x10 as b1010.

## Background

There are situations where values are referenced and set in various bitmaps, such as the status of IO ports (DI, DO), in order to simplify the description.
