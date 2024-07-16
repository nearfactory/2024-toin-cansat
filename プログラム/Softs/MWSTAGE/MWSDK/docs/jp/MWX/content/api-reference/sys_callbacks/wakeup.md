---
title: "wakeup()"
author: "Mono Wireless Inc."
description: "wakeup()"
---
# wakeup()

スリープから起床したときに`loop()`に移行する前に呼ばれ、スリープ復帰後の初期化処理や復帰状態によって処理を分岐するための手続きを含めます。

{% hint style="info" %}
センサーの読み出しなどの処理のみで`loop()`での処理がないときは、この関数内で再びスリープを実行できます。
{% endhint %}

{% hint style="info" %}
このコールバック関数定義は省略可能です。
{% endhint %}
