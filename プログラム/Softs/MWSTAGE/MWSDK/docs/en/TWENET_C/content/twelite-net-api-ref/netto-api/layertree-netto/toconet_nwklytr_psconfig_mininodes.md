# ToCoNet_NwkLyTr_psConfig_MiniNodes()

## 解説

[中継ネット](../../../twelite-net-api-expl/nettowkunitsuite/netto-1/)としてLayerTreeネットワークを選択し、送信専用として振る舞う LAYERTREE_MONINODES を構成する。

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
		// 始動時の処理
		memset(&sNwkLayerTreeConfig, 0, sizeof(sNwkLayerTreeConfig));
		sNwkLayerTreeConfig.u8Role = 
				TOCONET_NWK_ROLE_ENDDEVICE;
		// ネットワークの初期化
		pContextNwk = 
			ToCoNet_NwkLyTr_psConfig_MiniNodes(&sNwkLayerTreeConfig);

		if (sAppData.pContextNwk) {
			ToCoNet_Nwk_bInit(pContextNwk);
			ToCoNet_Nwk_bStart(pContextNwk);
		}

```

