# u8CCITT8()

## Explanation

Calculate the CRC8.

## Argument

| Type     | Name      | Remark                                   |
| -------- | --------- | ---------------------------------------- |
| `uint8*` | `pu8Data` | A string of bytes to calculate the CRC8. |
| `unit8`  | `size`    | Number of bytes to calculate.            |

## Returns

| Type    | Remark                        |
| ------- | ----------------------------- |
| `uint8` | The calculated value of CRC8. |

## Sample code

```c
#include "ccitt8.h"

...
char str[] = "abcABCDE";
uint8 u8crc = u8CCITT8(str, strlen(str));
```

