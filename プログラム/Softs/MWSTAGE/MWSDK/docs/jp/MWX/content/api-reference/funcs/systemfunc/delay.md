---
title: "delay()"
author: "Mono Wireless Inc."
description: "delay()"
---
# delay()

ポーリングによる時間待ちを行います。

```cpp
void delay(uint32_t ms)
```

`ms`にて与えられた期間待ち処理を行います。

時間の計測はTickTimerのカウントによって行っています。また長い時間待ちを行う場合はCPUのクロックを低下してポーリング処理を行います。

{% hint style="info" %}
`delay()`を呼び出してから約5ms経過するごとにTWELITEマイコン内部のウォッチドッグ処理を行います。

※例えば`while(1) delay(1);` を実行した場合は、`delay()`内部で5ms経過しないためウォッチドッグ処理が行われず、一定時間後リセットが実行されます。
{% endhint %}

{% hint style="warning" %}
`setup(), wakeup()`関数内では、TickTimerがまだ動作していないため、whileループによる時間待ち処理になります。この場合、指定値との誤差は大きくなります。このループカウンタは32Mhzに合わせて調整しています。これら関数内でCPUクロックを変化させた場合は、そのクロックに比例した誤差が発生します。

パラメータに1,2といった短い時間を指定した場合は、誤差が大きくなる場合があります。
{% endhint %}

