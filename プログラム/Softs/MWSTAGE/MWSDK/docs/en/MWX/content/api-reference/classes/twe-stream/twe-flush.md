---
title: mwx::flush
author: "Mono Wireless Inc."
description: Flush buffered output to twe::stream.
---

# mwx::flush

Flush the output buffer of `mwx::stream`. Instance to a helper class that calls the `flush()` method.

```cpp
for (int i = 0; i < 127; ++i) {
    Serial << "hello world! (" << i << ")" << twe::endl << twe::flush;
}
```

* For serial ports, wait polling until output completes.
* For `mwx::simpbuf` buffers, output `0x00` at the end (size is not changed)

