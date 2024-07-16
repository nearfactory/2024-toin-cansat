# u8CCITT8()

## 解説

CRC8 を計算する。

## 引数

| 型        | 名前        | 詳細             |
| -------- | --------- | -------------- |
| `uint8*` | `pu8Data` | CRC8を計算するバイト列。 |
| `unit8`  | `size`    | 計算するバイト数。      |

## 戻り値

| 型       | 詳細     |
| ------- | ------ |
| `uint8` | CRC8値。 |

## サンプル

```c
#include "ccitt8.h"

...
char str[] = "abcABCDE";
uint8 u8crc = u8CCITT8(str, strlen(str));
```

