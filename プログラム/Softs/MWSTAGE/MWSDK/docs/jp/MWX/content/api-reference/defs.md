---
title: "定義"
author: "Mono Wireless Inc."
description: "ライブラリ中で共通的に読み込まれる定義"
---
# 定義

ライブラリ中で共通的に読み込まれる定義について、定義内容を引用します。

### mwx\_common.h

```cpp
#include <cstdint> // for type name
typedef char char_t;
typedef uint8_t byte;
typedef uint8_t boolean;

#ifndef NULL
#define NULL nullptr
#endif
```
