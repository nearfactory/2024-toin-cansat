---
title: "mwx::crlf"
author: "Mono Wireless Inc."
description: "twe::stream に改行コードを出力する"
---

# mwx::crlf

`mwx::stream` の `<<` 演算子に対して改行コード (CR LF) を出力するためのヘルパークラスのインスタンスです。

```cpp
Serial << "hello world!" << mwx::crlf;
```
