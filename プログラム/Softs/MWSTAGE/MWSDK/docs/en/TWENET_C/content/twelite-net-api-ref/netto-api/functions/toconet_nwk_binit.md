# ToCoNet_Nwk_bInit()

## Explanation

Initialisation of the relay network.

Get a pointer to the [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md) structure of the configuration procedure of the relay net ([`ToCoNet_NwkLyTr_psConfig()`](../layertree-netto/toconet_nwklytr_psconfig.md) in case of configuring a LayerTree) in advance.

## Argument

| Type                                                                | Name        | Remark                                                                  |
| ------------------------------------------------------------------- | ----------- | ----------------------------------------------------------------------- |
| [`tsToCoNet_Nwk_Context`](../structure/tstoconet_nwk_context.md)`*` | pContextNwk | A structure that manages the internal information of the relay network. |

## Returns

| Type   | Remark                         |
| ------ | ------------------------------ |
| bool_t | `TRUE`: Success, `FALSE`: Fail |

## Sample code

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

