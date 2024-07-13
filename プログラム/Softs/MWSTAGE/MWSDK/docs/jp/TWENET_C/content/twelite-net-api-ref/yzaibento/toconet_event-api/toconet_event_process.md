# ToCoNet_Event_Process()

## 解説

指定したユーザ定義イベント処理関数でのイベント処理を行う。ハンドルにより指定する場合は ToCoNet_Event_ProcessH() を呼び出します。

{% hint style="info" %}
この処理は関数呼び出しとして同期的に実行されます。
{% endhint %}

{% hint style="warning" %}
ユーザ定義イベント処理関数を直接呼び出してはいけません。適切な状態遷移が実施されず、また内部の管理情報と矛盾が発生することがあります。
{% endhint %}

## 引数

### ToCoNet_Event_Process()

| 型                    | 名前       | 詳細            |
| -------------------- | -------- | ------------- |
| teEvent (または uint32) | eEvent   | イベント名         |
| uint32               | u32evarg | イベント引数        |
| void\*               | pvFunc   | ユーザ定義イベント処理関数 |

### ToCoNet_Event_ProcessH()

| 型                    | 名前       | 詳細                  |
| -------------------- | -------- | ------------------- |
| teEvent (または uint32) | eEvent   | イベント名               |
| uint32               | u32evarg | イベント引数              |
| uint8                | u8Handle | ユーザ定義イベント処理関数へのハンドル |

## 戻り値

なし。

## サンプル

```c
static u8hnd; // ハンドル
...
    // 登録
    u8hnd = ToCoNet_Event_Register_State_Machine(vProcessEvCore);
... 
// 送信完了を vProcessEvCore に伝える。
cbToCoNet_vTxEvent(uint8 u8CbId, uint8 u8Stat) {
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, vProcessEvCore);
    // または
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, u8hnd);
}
```

