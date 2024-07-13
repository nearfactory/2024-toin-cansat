# ToCoNet_Nwk_bStart()

## Explanation

Starts the RelayNet.

{% hint style="info" %}
The `E_EVENT_TOCONET_NWK_START` event is received after the network starts.
{% endhint %}

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

