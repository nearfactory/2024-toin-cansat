# ToCoNet_Nwk_bStart()

## 解説

中継ネットを開始する。

{% hint style="info" %}
ネットワーク開始後に、`E_EVENT_TOCONET_NWK_START` イベントを受け取ります。
{% endhint %}

## 引数

| 型                                                                   | 名前          | 詳細                  |
| ------------------------------------------------------------------- | ----------- | ------------------- |
| [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md)`*` | pContextNwk | 中継ネットの内部情報を管理する構造体。 |

## 戻り値

| 型      | 詳細                     |
| ------ | ---------------------- |
| bool_t | `TRUE`: 成功、`FALSE`: 失敗 |

## サンプル

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;
...
			pContextNwk =
				ToCoNet_NwkLyTr_psConfig(&sNwkLayerTreeConfig);
			if (pContextNwk) {
				ToCoNet_Nwk_bInit(pContextNwk);
				ToCoNet_Nwk_bStart(pContextNwk);
			}
```

