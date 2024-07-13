---
title: "delayMicroseconds()"
author: "Mono Wireless Inc."
description: "delayMicroseconds()"
---

# delayMicroseconds()

{% hint style="success" %}
MWSDK2020\_05 には含まれません。対応パッケージはMWSDK\_2020\_07\_UNOFFICIAL以降となります。
{% endhint %}

ポーリングによる時間待ちを行います（μ秒指定）。

```cpp
void delayMicroseconds(uint32_t microsec)
```

`microsec`にて与えられた期間待ち処理を行います。

時間の計測はTickTimerのカウントによって行っています。また長い時間待ちを行う場合はCPUのクロックを低下してポーリング処理を行います。

{% hint style="warning" %}
`setup(), wakeup()`関数内では、TickTimerがまだ動作していないため、whileループによる時間待ち処理になります。この場合、指定値との誤差は大きくなります。このループカウンタは32Mhzに合わせて調整しています。これら関数内でCPUクロックを変化させた場合は、そのクロックに比例した誤差が発生します。

パラメータに10以下といった短い時間を指定した場合は、誤差が大きくなる場合があります。
{% endhint %}
