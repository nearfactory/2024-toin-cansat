---
title: "loop()"
author: "Mono Wireless Inc."
description: "loop()"
---

# loop()

アプリケーションのメインループです。ループ終了後はCPUがDOZEモードに遷移し低消費電流で次の割り込みを待ちます。

アクトの記述では、ほとんどの処理がこのループ内に記述されます。

{% hint style="info" %}
バック関数定義は省略可能です。
{% endhint %}



