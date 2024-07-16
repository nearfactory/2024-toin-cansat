---
title: "コールバック関数"
author: "Mono Wireless Inc."
description: "コールバック関数"
---

# コールバック関数

アプリケーションの記述を行うコールバック関数です。コールバックはシステム（ライブラリ）から呼び出されるという意味です。ユーザがいくつかのコールバック関数を定義することでシステムの振る舞いを記述します。

以下のコールバック関数は必須定義です。

* `setup()`
* `loop()`

それ以外の関数は定義しない場合は、何も実行しない空の関数が替わりにリンクされます。



### 通常のコールバック呼び出し順序

```
init_coldboot()
  ↓ (TWENET内部処理:初期化1)
setup()
  ↓（TWENET内部処理:初期化2)
begin() --- 初回のみ
  ↓
loop() <--+
  ↓       |イベント処理、ビヘイビア処理
CPU DOZE -+
```

{% hint style="warning" %}
正確なふるまいを参照したい方はソースコード`mwx_appcore.cpp`を参照してください。
{% endhint %}



### スリープ復帰時のコールバック呼び出し順序

```
the_twelite.sleep()
  ↓ sleeping...
 
 
init_warmboot()
  ↓ (TWENET内部処理:初期化3)
wakeup()
  ↓（TWENET内部処理:初期化4)
loop() <--+
  ↓       |イベント処理、ビヘイビア処理
CPU DOZE -+
```

{% hint style="warning" %}
正確なふるまいを参照したい方はソースコード`mwx_appcore.cpp`を参照してください。
{% endhint %}

