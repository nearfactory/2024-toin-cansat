---
title: "pinMode()"
author: "Mono Wireless Inc."
description: "pinMode()"
---

# pinMode()

DIO(汎用ディジタルIO)ピンの設定を行います。

```cpp
void pinMode(uint8_t u8pin, E_PIN_MODE mode)
```

この関数では DIO0..19 と、DO0,1のピンの状態を変更できます。設定内容は `E_PIN_MODE` の列挙値の[DIOの解説](./#pinnomdodio-0-19)と[DOの解説](./#pinnomdodo-0-1)を参照してください。

{% hint style="danger" %}
DO0,1は特殊なピンで、原則として他の目的で利用されるものですが、出力としても設定可能です。ただしハード的な制約があるピンですので、利用には注意が必要です。

両方のピンは、電源投入時にHIGHレベルが担保される必要があります。不安定な電圧をとったりするような回路構成の場合、モジュールが起動しないなどの問題が出ます。
{% endhint %}
