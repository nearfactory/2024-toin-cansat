---
title: "digitalReadBitmap()"
author: "Mono Wireless Inc."
description: "digitalReadBitmap()"
---
# digitalReadBitmap()

{% hint style="success" %}
mwxライブラリ 0.1.4 以降に収録
{% endhint %}

入力設定のポートの値を一括読み出しします。

```cpp
uint32_t digitalReadBitmap()
```

LSB側から順にDIO0 ... DIO19 の順に値が格納されます。

HIGH側のピンには 1 が、LOW側のピンには 0 が設定されます。
