---
title: "digitalRead()"
author: "Mono Wireless Inc."
description: "DESC"
---
# digitalRead()

入力設定のポートの値を読み出す。

```cpp
static inline E_PIN_STATE digitalRead(uint8_t u8pin)
```

事前に入力に設定したピンの入力値を`LOW`または`HIGH`で得ます。

{% hint style="info" %}
`E_PIN_STATE`型から`int`型への変換演算子は定義していないため、数値型への直接的な代入はできません。
{% endhint %}
