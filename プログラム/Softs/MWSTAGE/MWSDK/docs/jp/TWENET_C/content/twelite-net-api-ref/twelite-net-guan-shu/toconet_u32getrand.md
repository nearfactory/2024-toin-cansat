# ToCoNet_u32GetRand()

## 解説

32bitの乱数を得ます。乱数の生成方法は [`sToCoNet_AppContext.u8RandMode`](../gou-zao-ti/stoconet_appcontext.md)  で指定します。

{% hint style="warning" %}
デフォルトはハードウェアによる16bit乱数に基づき値が生成されます。

[WakeTimer](../../hw-api-ref/perifuraru/waketimer.md) に 32Khz 水晶発振子や発振器を用いた場合、乱数は生成されません。必ずソフトウェア乱数[モジュール](../../twelite-net-api-expl/mojru.md)を利用してください。
{% endhint %}

## 引数

なし。

## 戻り値

| 型        | 詳細       |
| -------- | -------- |
| `uint32` | 32bitの乱数 |

## サンプル

```c
uint32 u32rand;
u32rand = ToCoNet_u32GetRand();
```

