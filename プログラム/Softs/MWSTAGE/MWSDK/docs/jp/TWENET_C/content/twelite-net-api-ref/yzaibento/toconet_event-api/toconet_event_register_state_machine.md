# ToCoNet_Event_Register_State_Machine()

## 解説

ユーザ定義イベント処理関数をシステムに登録する。本関数は [cbAppColdStart](../../krubakku/cbappcoldstart.md)(TRUE) で実行する。

## 引数

| 型      | 名前     | 詳細                       |
| ------ | ------ | ------------------------ |
| void\* | pvFunc | ユーザ定義イベント処理関数(\*1)を指定する。 |

```
*1: pvFunc の関数プロトタイプは以下である。
void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg)
```

## 戻り値

| 型     | 詳細                    |
| ----- | --------------------- |
| uint8 | ユーザ定義イベント処理関数のハンドル番号。 |

## サンプル

```c
void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg);

void cbAppColdStart(bool_t bInit) {
    if (bInit == FALSE) {
 		...
    } else {
    	...
    	ToCoNet_Event_Register_State_Machine(vProcessEvCore);
    }
}
```
