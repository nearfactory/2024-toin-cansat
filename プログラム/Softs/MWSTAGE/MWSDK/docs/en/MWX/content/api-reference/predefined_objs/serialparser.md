---
title: SerialParser
author: "Mono Wireless Inc."
description: format input parser for serial input (mwx::serial_parser)
---

# SerialParser

This built-in class is defined as a built-in object, intended to be used for format input via serial port.

It is defined as a `mwx::serial_parser<mwx::alloc_heap<uint8_t>>` type that allocates buffer space for internal use from the heap at initialization (`begin()`).

For details, see class [serparser](../classes/ser\_parser.md) for details.
