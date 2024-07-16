# ToCoNet_Event_SetState()

## 解説

ユーザ定義イベント処理関数内で状態遷移を行う。

{% hint style="info" %}
ユーザ定義イベント処理関数を終了後に、続けて新しい状態にて E_EVENT_NEW_STATE イベントが呼び出されます。
{% endhint %}

## 引数

| 型          | 名前        | 詳細                 |
| ---------- | --------- | ------------------ |
| tsEvent \* | pEv       | ユーザイベント定義関数の管理構造体。 |
| teState    | eNewState | 新しい状態。             |

## 戻り値

なし

## サンプル

```c
sToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
```

