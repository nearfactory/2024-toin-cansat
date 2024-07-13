# TickTimer

TWELITE NET 内部利用の 16Mhz ハードウェアカウンタ。タイムアウト時間は [sToCoNet_AppContext](../../twelite-net-api-ref/gou-zao-ti/stoconet_appcontext.md).u16TickHz により計算されます。

{% hint style="danger" %}
TWELITE NET では内部的に使用されるため、ユーザが停止・再設定など変更を行ってはなりません。
{% endhint %}

{% hint style="info" %}
uint32 u32AHI_TickTimerRead() によりカウント値を読むことで、ごく短い時間計測に利用できます。

```c
uint32 s, e, d, cyc;

// 1ms = 16000 count
cyc = 1000 / sToCoNet_AppContext.u16TickHz;
cyc *= 16000;

// check the process time
s = u32AHI_TickTimerRead();
.. some process ..
e = u32AHI_TickTimerRead();

// calculate the time
if (e > s) {
    d = e - s;
} else {
    d = 64000 - e + s;
}
d /= 16; // now `d' represents micro sec.

```
{% endhint %}

 



