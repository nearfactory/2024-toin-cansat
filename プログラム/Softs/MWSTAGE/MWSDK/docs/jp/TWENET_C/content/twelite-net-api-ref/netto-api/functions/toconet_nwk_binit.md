# ToCoNet_Nwk_bInit()

## 解説

中継ネットの初期化を行う。

事前に中継ネットの構成手続き(LayerTree を構成する場合は [`ToCoNet_NwkLyTr_psConfig()`](../layertree-netto/toconet_nwklytr_psconfig.md)) [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md) 構造体へのポインタを得ておく。

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

