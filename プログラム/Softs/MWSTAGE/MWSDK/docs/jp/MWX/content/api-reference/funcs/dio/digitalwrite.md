---
title: "digitalWrite()"
author: "Mono Wireless Inc."
description: "digitalWrite()"
---

# digitalWrite()

ディジタル出力ピンの設定を変更します。

```cpp
static inline void digitalWrite(uint8_t u8pin, E_PIN_STATE ulVal)
```

事前に`pinMode()`にて設定対象のピンを出力用に設定しておきます。１番目のパラメータは、設定対象のピン番号を指定します。２番目のパラメータは`HIGH`か`LOW`のいずれかを指定します。

{% hint style="info" %}
入力が `E_PIN_STATE` 型となっています。`E_PIN_STATE`から`int`型への変換演算子は定義していませんので、数値による直接の入力はできないようになっています。
{% endhint %}
