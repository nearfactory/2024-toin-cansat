---
title: "TickTimer"
author: "Mono Wireless Inc."
description: "システムタイマー (mwx::periph_ticktimer)"
---

# TickTimer

TickTimerはTWENETの内部制御用に利用され、暗黙に実行されています。タイマーの周期は1msです。`loop()`中でTickTimerイベントにより1msごとの処理を記述する目的で`available()`メソッドのみを定義しています。

{% hint style="warning" %}
必ず1ms刻みでavailableになる訳ではない点に注意してください。

ユーザプログラムの記述内容や、システム内部の割り込み処理などが要因で、大きな遅延が発生することもあり、イベントが飛ばされるような場合もあります。

```cpp
void loop() {
  if (TickTimer.available()) {
    if ((millis() & 0x3FF) == 0) { // これは処理されない場合がある
      Serial << '*';
    }
  }
}
```
{% endhint %}



## メソッド

available()

```cpp
inline bool available()
```

TickTimer割り込み発生後にセットされ、その直後の`loop()`で`true`になります。`loop()`終了後にクリアされます。

