# 型 typedef, よく使うマクロ

ソース中で利用される主な typedef型は jendefs.h 定義されている。

| typedef | 型              | 解説          |
| ------- | -------------- | ----------- |
| bool_t  | unsigned char  | ブール型として利用する |
| string  | char \*        |             |
| uint8   | unsigned char  |             |
| int8    | char           |             |
| uint16  | unsigned short |             |
| int16   | short          |             |
| uint32  | unsigned long  |             |
| int32   | long           |             |



以下に、コード中でよく利用されるマクロ定義を列挙します。

| 定義名     |        |                  |
| ------- | ------ | ---------------- |
| FALSE   | 0      |                  |
| TRUE    | 1      |                  |
| PUBLIC  |        | 公開関数であることを明示する。  |
| PRIVATE | static | 非公開関数であることを明示する。 |
