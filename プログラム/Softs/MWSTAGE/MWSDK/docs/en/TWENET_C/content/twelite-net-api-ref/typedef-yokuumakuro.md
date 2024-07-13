# typedef, frequently used macros

The main typedef types used in the source are defined in `jendefs.h`.

| typedef | type           | Remark            |
| ------- | -------------- | ----------------- |
| bool_t  | unsigned char  | as a boolean type |
| string  | char \*        |                   |
| uint8   | unsigned char  |                   |
| int8    | char           |                   |
| uint16  | unsigned short |                   |
| int16   | short          |                   |
| uint32  | unsigned long  |                   |
| int32   | long           |                   |



The following is a list of the most commonly used macro definitions in the code.

| name    | value  | remark                                 |
| ------- | ------ | -------------------------------------- |
| FALSE   | 0      |                                        |
| TRUE    | 1      |                                        |
| PUBLIC  |        | Specify that it is a public function.  |
| PRIVATE | static | Specify that it is a private function. |
