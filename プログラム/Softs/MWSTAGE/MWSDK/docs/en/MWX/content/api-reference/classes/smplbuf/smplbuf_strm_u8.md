---
title: smplbuf_strm_u8
author: "Mono Wireless Inc."
---

# smplbuf\_strm\_u8

The `uint8_t` type `smplbuf_strm_u8?? `is also available in the [stream(stream)](../twe-stream/README.md) interface, so several methods for streams can be used.

```cpp
// smplbuf_strm_u8<N> : locally allocated
template <int N> using smplbuf_strm_u8 
  = _smplbuf_stream<uint8_t, mwx::alloc_local<uint8_t, N>>;

// smplbuf_strm_u8_attach : attach to an existing buffer
using smplbuf_strm_u8_attach 
  = mwx::_smplbuf_stream<uint8_t, mwx::alloc_attach<uint8_t>>;

// smplbuf_strm_u8_heap : allocate at HEAP
using smplbuf_strm_u8_heap 
  = mwx::_smplbuf_stream<uint8_t, mwx::alloc_heap<uint8_t>>;

// definition of operator <<
template <class L_STRM, class ALOC>
mwx::stream<L_STRM>& operator << (
        mwx::stream<L_STRM>& lhs, 
        mwx::_smplbuf_stream<uint8_t, ALOC>& rhs)
{
		lhs << rhs.to_stream();
		return lhs;
}
```



Example

```cpp
smplbuf_strm_u8<128> sb1;

sb1 << "hello";
sb1 << uint32_t(0x30313233);
sb1 << format("world%d",99);
sb1.printfmt("Z!");

Serial << sb1;
// hello0123world99Z!
```

