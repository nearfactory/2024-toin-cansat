---
title: "identify_packet_type()"
author: "Mono Wireless Inc."
description: "identify_packet_type()"
---

# idenify\_packet\_type()

パケットデータのバイト列を入力として、パケットの種別を判定します。戻り値は[E\_PKT](e\_pkt.md)です。

```cpp
E_PKT identify_packet_type(uint8_t* p, uint8_t u8len)
```



特定のパケットであると解釈できなかった場合は`E_PKT::PKT_ERROR`が戻ります。
