---
title: idenify_packet_type()
author: "Mono Wireless Inc."
---
# idenify\_packet\_type()

Determines the type of packet using the packet data byte sequence as input. The return value is [E\_PKT](e\_pkt.md).

```cpp
E_PKT identify_packet_type(uint8_t* p, uint8_t u8len)
```



If the packet cannot be interpreted as a specific packet, `E_PKT::PKT_ERROR` is returned.
