# ToCoNet_NwkLyTr_psConfig()

## 解説

中継ネットとしてLayerTreeネットワークを選択し、親機または中継器を構成する。

## 引数

| 型                                                            | 名前          | 詳細               |
| ------------------------------------------------------------ | ----------- | ---------------- |
| [`tsToCoNet_NwkLyTr_Context*`](tstoconet_nwklytr_context.md) | pContextNwk | 中継ネットの設定情報格納構造体。 |

## 戻り値

| 型                                                                 | 詳細                |
| ----------------------------------------------------------------- | ----------------- |
| [`tsToCoNet_Nwk_Context*`](../structure/tstoconet_nwk_context.md) | 設定済みの内部構造体へのポインタ。 |

## サンプル

```c
static tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
static tsToCoNet_Nwk_Context* pContextNwk;
...
		// layer 数の決定
		sNwkLayerTreeConfig.u8Layer = 4;
​
		// NBビーコン方式のネットワークを使用する
		sNwkLayerTreeConfig.u8StartOpt =
			TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON;

		// Router として始動
		sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ROUTER;
		pContextNwk = ToCoNet_NwkLyTr_psConfig(&sNwkLayerTreeConfig);
		if (pContextNwk) {
			ToCoNet_Nwk_bInit(pContextNwk);
			ToCoNet_Nwk_bStart(pContextNwk);
		} else {
			// fatal error
		}

```

