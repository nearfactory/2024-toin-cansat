# ToCoNet_u32GetVersion()

## 解説

ライブラリのバージョンを取得します。

以下の３関数が利用できます。

```
ToCoNet_u32GetVersion() .. 基本ライブラリ
ToCoNet_u32GetVersion_LibEx() .. EXT
ToCoNet_u32GetVersion_LibUtils() .. UTILS
```

## 引数

なし

## 戻り値

| 型        | 詳細                                                 |
| -------- | -------------------------------------------------- |
| `uint32` | バージョン番号 `0x00XXYYZZ` (XX:メジャー番号 YY:マイナー番号 ZZ:派生番号) |

## サンプル

```c
uint32 u32ver = ToCoNet_u32GetVersion();
```

