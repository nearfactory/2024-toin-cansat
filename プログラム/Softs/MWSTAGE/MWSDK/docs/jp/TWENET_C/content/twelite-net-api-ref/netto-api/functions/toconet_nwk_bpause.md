# ToCoNet_Nwk_bPause()

## 解説

中継ネットを中断する。

{% hint style="info" %}
スリープを実行するまえに呼び出します。
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
	case E_STATE_IDLE:
		if (eEvent == E_EVENT_START_UP) {
			if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
				// RESUME
				ToCoNet_Nwk_bResume(pContextNwk);
			} else {
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
			}
...
	case E_STATE_APP_SLEEP:
		if (eEvent == E_EVENT_NEW_STATE) {
			// 中継ネットのポーズ処理を行う
			ToCoNet_Nwk_bPause(sAppData.pContextNwk);
​
			// 周期スリープに入る
			//  - 初回は５秒あけて、次回以降はスリープ復帰を基点に５秒
			vSleep(5000, TRUE, FALSE);
		}
```

