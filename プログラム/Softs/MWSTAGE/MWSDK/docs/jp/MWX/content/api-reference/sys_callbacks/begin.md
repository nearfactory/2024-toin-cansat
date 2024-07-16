---
title: "begin()"
author: "Mono Wireless Inc."
description: "begin()"
---
# begin()

`loop()`関数の初回コールの手前で一度だけ呼び出されます。TWENET の初期化は終了しているので`setup()`のような制約を考慮する必要はありません。

主な使い方は、

* 始動メッセージの表示
* テスト用のコードを記述
* 始動直後のスリープ遷移
* `setup()`で不都合がある処理（無線パケット処理・タイマー動作など）

{% hint style="info" %}
このコールバック関数定義は省略可能です。
{% endhint %}

