# ステート（状態）

## 組み込み状態

E_STATE_IDLE以外の状態名は、ユーザ定義イベント処理関数で自由に使うことができます。また、状態名をユーザ定義することも可能です。

| 状態名              | 解説         |
| ---------------- | ---------- |
| E_STATE_IDLE     | 初期状態。値は 0。 |
| E_STATE_RUNNING  | 実行中。       |
| E_STATE_FINISHED | 完了。        |

## ユーザ独自定義

ToCoNet_EVENT_APP_BASE 定義以降の値を利用してください。

```c
#include "ToCoNet_event.h"

typedef enum
{
    E_STATE_APP_BASE = ToCoNet_STATE_APP_BASE,
    E_STATE_USER_INIT,
    E_STATE_USER_FINISH,
    E_STATE_USER_MEASURING
} teStateApp;
```

{% hint style="warning" %}
enum を 16bit や 8bit に小さくするコンパイルオプションは適用しないでください。
{% endhint %}
