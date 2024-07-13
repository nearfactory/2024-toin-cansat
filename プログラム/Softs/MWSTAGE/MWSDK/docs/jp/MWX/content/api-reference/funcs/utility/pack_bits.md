---
title: "pack_bits()"
author: "Mono Wireless Inc."
description: "指定したビット位置に1をセット"
---
# pack\_bits()

指定したビット位置に1をセットします。

```cpp
constexpr uint32_t pack_bits(...)
```

パラメータは可変数引数で指定でき、各パラメータはビット位置を指定する0..31の整数を指定する。例えば`pack_bits(1,3,6)`と指定すると`((1UL<<1)|(1UL<<3)|(1UL<<6))`を返します。

{% hint style="info" %}
`constexpr`は定数による計算が可能な場合はコンパイル時に定数展開します。
{% endhint %}



## 背景

IOポート(DI,DO)の状態など各種ビットマップに値を参照・設定する場面があり、その記述を簡素化するため。
