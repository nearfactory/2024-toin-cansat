---
title: "smplbuf_strm_u8"
author: "Mono Wireless Inc."
description: "smplbuf_strm_u8"
---

# smplbuf\_strm\_u8

`uint8_t`型の`smplbuf_strm_u8???`は[ストリーム(stream)](../twe-stream/)インタフェースも有しているため、いくつかのストリーム用のメソッドを使用することができます。

```cpp
// smplbuf_strm_u8<N> : ローカル確保
template <int N> using smplbuf_strm_u8 
  = _smplbuf_stream<uint8_t, mwx::alloc_local<uint8_t, N>>;

// smplbuf_strm_u8_attach : 既存バッファへのアタッチ版
using smplbuf_strm_u8_attach 
  = mwx::_smplbuf_stream<uint8_t, mwx::alloc_attach<uint8_t>>;

// smplbuf_strm_u8_heap : HEAP確保
using smplbuf_strm_u8_heap 
  = mwx::_smplbuf_stream<uint8_t, mwx::alloc_heap<uint8_t>>;

// << 演算子の定義
template <class L_STRM, class ALOC>
mwx::stream<L_STRM>& operator << (
        mwx::stream<L_STRM>& lhs, 
        mwx::_smplbuf_stream<uint8_t, ALOC>& rhs)
{
		lhs << rhs.to_stream();
		return lhs;
}
```



例

```cpp
smplbuf_strm_u8<128> sb1;

sb1 << "hello";
sb1 << uint32_t(0x30313233);
sb1 << format("world%d",99);
sb1.printfmt("Z!");

Serial << sb1;
// hello0123world99Z!
```

