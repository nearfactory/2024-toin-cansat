# TickTimer

TWELITE NET internally used 16Mhz hardware counter. The timeout time is calculated by [`sToCoNet_AppContext.u16TickHz`](../../twelite-net-api-ref/gou-zao-ti/stoconet_appcontext.md).

{% hint style="danger" %}
TWELITE NET is used internally and should not be stopped, reconfigured or otherwise changed by the user.
{% endhint %}

{% hint style="info" %}
It can be used for very short time measurements by reading the count value with `uint32 u32AHI_TickTimerRead()`.

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

 



