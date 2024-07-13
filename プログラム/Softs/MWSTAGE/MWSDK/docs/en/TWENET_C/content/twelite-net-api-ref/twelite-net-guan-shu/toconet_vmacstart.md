# ToCoNet_vMacStart()

## Explanation

Initialization process of MAC layer.

{% hint style="info" %}
When you wake up from sleep, if you do not transmit/receive radio and only do microcomputer processing, do not call this function.
{% endhint %}

It is usually written at the end of the second call processing part with `TRUE` argument in [`cbAppColdStart()`](../krubakku/cbappcoldstart.md) , [`cbAppWarmStart()`](../krubakku/cbappwarmstart.md) at startup.

{% hint style="warning" %}
For calls other than `cbAppColdStart()` and `cbAppWarmStart()`, the internal design is not such that problems can occur, but the behavior is undefined. If necessary, it is recommended to perform a sleep and then perform radio transmission after the sleep is restored.
{% endhint %}

{% hint style="info" %}
If `sToCoNet_AppContext.u8MacInitPending` is set, this function should not be called.
{% endhint %}

## Argument

None.

## Returns

None.

## Sample code

```c
void cbAppWarmStart(bool_t bStart) {
    if (!bStart) {
        // very initial call
    } else {
        // initializing...
        
        // finally start the MAC layer
        ToCoNet_vMacStart();
    }
}
```

