---
title: "on_rx_packet()"
author: "Mono Wireless Inc."
description: "on_rx_packet()"
---
# on_rx_packet()

受信パケットを受け取ります。

```cpp
void on_rx_packet(mwx::packet_rx& pkt, bool_t &b_handled) 
```

無線パケットを受信したときに[packet_rx](../classes/packet_rx.md)として `pkt` にデータが格納された状態で、本関数が MWX ライブラリ内から呼び出されます。アプリケーション中で本関数が定義されない場合は何もしない weak 関数がリンクされます。

本関数中で `b_handled` に *true* をセットすると、MWX ライブラリに受信パケットがアプリケーション内で処理されたことを伝達します。処理済みとした場合、不要な処理を抑制します。（`the_twelite.receiver` の処理を行わない）


{% hint style="warning" %}
[ビヘイビア](../behavior/README.md)を用いる場合は、ビヘイビア中のコールバック関数を用いてください。
{% endhint %}

{% hint style="warning" %}
`the_twelite.receiver`は推奨されません。

従来`loop()`内での記述を意図して `the_twelite.receiver` による処理を行っていましたが、キューによる遅延処理である点で原理的に取りこぼしが発生し、また記述も煩雑になりがちである点から `on_rx_packet()` を追加しました。
{% endhint %}