---
title: "Printf utils"
author: "Mono Wireless Inc."
description: "printfの実装"
---
# Printf utils

mwxライブラリでは、書式出力をC言語で常用されるprintfの実装を用いています。いくつかの関数の関数定義が利用できます。

```cpp
int mwx_printf(const char* format, ...)
int mwx_snprintf(char* buffer, size_t count, const char* format, ...)
```

`mwx_printf()`はSerialオブジェクトに対してprintf出力を行います。`Serial.printfmt()`と同じ処理になります。

`mwx_snprintf()`はバッファに対してsnprintfを行います。

{% hint style="info" %}
配列クラス[smplbuf](../../classes/smplbuf/)に対してprintfでバッファを構築したい場合は、[mwx::streamでデータ生成](../../classes/smplbuf/#mwxstreamdedta)を参照してください。
{% endhint %}
