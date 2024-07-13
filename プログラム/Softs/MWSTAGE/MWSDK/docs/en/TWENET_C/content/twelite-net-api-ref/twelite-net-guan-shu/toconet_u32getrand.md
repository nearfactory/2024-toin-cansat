# ToCoNet_u32GetRand()

## Explanation

Obtains a 32bit random number. The method of random number generation is specified by [`sToCoNet_AppContext.u8RandMode`](../gou-zao-ti/stoconet_appcontext.md).

{% hint style="warning" %}
By default, the value is generated based on a hardware 16-bit random number.

If you use a 32Khz crystal oscillator or oscillator for the [WakeTimer](../../hw-api-ref/perifuraru/waketimer.md), no random numbers will be generated. Always use a software random number module.
{% endhint %}

## Argument

None

## Returns

| Type     | Remark              |
| -------- | ------------------- |
| `uint32` | 32bit random number |

## Sample code

```c
uint32 u32rand;
u32rand = ToCoNet_u32GetRand();
```

