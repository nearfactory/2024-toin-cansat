---
title: "SerialParser"
author: "Mono Wireless Inc."
description: "シリアルポート用書式入力 (mwx::serial_parser)"
---

# SerialParser

この組み込みクラスはシリアルポートでの書式入力に利用することを想定して組み込みオブジェクトとして定義しています。

初期化時(`begin()`)にヒープから内部で使用するバッファ領域を確保する`mwx::serial_parser<mwx::alloc_heap<uint8_t>>`型として定義されています。

詳細はクラス [serparser](../classes/ser\_parser.md) を参照してください。
