# WakeTimer

The WakeTimer is driven by a 32KHz (or 32768Hz for an external oscillator) oscillator and has two hardware counters. Use [`ToCoNet_vSleep()`](../../twelite-net-api-ref/twelite-net-guan-shu/toconet_vsleep.md) to perform sleep using WakeTimer.

The counter is a subtractive type, and when the value reaches 0, an interrupt is generated and the counter continues to subtract.

{% hint style="info" %}
If you check the value of WakeTimer used for sleep wakeup, you can know the elapsed time after waking up. This value can be used to adjust the timing of the next periodic wake-up.

The periodic wakeup option in `ToCoNet_vSleep()` uses this mechanism to adjust the timing of the next wakeup.
{% endhint %}

### Relationship between count value and real time

The calculation of the WakeTimer count value and real time is as follows.

{% hint style="info" %}
The calibration value is used for count compensation when WakeTimer is used to execute sleep.

The number of `Tms` counts can be calculated using the following formula

```
Ct = Tms * 32 * 10000 / cal
  Ct  : Number of counts after correction
  Tms : Time in milliseconds
  cal : calibration value
```
{% endhint %}

{% hint style="info" %}
When using an external oscillator (32768Hz), perform the following calculations.

```
Ct = (Tms * 32768) / 1000
  Ct  : Number of counts after correction
  Tms : Time in milliseconds
```
{% endhint %}



##  <a href="yin-shu" id="yin-shu"></a>
