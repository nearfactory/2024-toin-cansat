---
title: "setup()"
author: "Mono Wireless Inc."
description: "setup()"
---
# setup()

コード実行の初期に呼び出され、初期化コードを記述します。

{% hint style="warning" %}
TWENETの初期化は `setup()` 関数が終了した後にも実行されます。多くの処理はTWENETが終了した後に実行するようになっているため、ここでは初期化以外の処理を行わないようにしてください。

注意すべき事項を以下に列挙します。

* スリープ`the_twenet.sleep()`の実行はできません。初期化後速やかにスリープしたいときは`begin()`関数内に最初のスリープ処理を記述してください。
* `delay()`関数は後述の処理\*に置き換えられます。この場合、パラメータの`ms`はミリ秒を指定するものではありません。\


\*` delay()`の代替処理

```cpp
static inline void delay(uint32_t ms) {
		volatile uint32_t ct = ms * 4096;
		while (ct > 0) {
			--ct;
		}
}
```
{% endhint %}

