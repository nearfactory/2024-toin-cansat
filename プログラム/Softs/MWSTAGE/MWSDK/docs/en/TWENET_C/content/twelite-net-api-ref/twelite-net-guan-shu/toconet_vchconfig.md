# ToCoNet_vChConfig()

## Explanation

Change channel.

{% hint style="info" %}
`ToCoNet_vRfConfig()` applies other settings than the channel, but this function changes only the channel.
{% endhint %}

{% hint style="warning" %}
If the channel cannot be changed at a time such as during transmission or reception, the change will not take place. If the channel is not changed, the value of [`sToCoNet_AppContext.u8Channel`](../gou-zao-ti/stoconet_appcontext.md) is returned to the channel before the change.
{% endhint %}

{% hint style="warning" %}
If the [module](../../twelite-net-api-expl/mojru.md) `ToCoNet_USE_MOD_CHANNEL_MGR` is used, the channel is not changed by this function.
{% endhint %}

## Argument

None

## Returns

None

## Sample code

```c
sToCoNet_AppContext.u8Channel = 25;
ToCoNet_vChConfig();
if (sToCoNet_AppContext.u8Channel != 25) {
  // Oops! Channel is not changed to 25.
}
```

