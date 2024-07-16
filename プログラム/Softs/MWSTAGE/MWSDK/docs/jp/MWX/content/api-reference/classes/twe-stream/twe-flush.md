---
title: "mwx::flush"
author: "Mono Wireless Inc."
description: "twe::stream へのバッファ出力をフラッシュする。"
---

# mwx::flush

`mwx::stream` の出力バッファをフラッシュする。`flush()` メソッドを呼び出すヘルパークラスへのインスタンス。

```cpp
for (int i = 0; i < 127; ++i) {
    Serial << "hello world! (" << i << ")" << twe::endl << twe::flush;
}
```

* シリアルポートの場合は出力完了までポーリング待ちを行う
* `mwx::simpbuf` バッファの場合は `0x00` を末尾に出力する（サイズは変更しない）

