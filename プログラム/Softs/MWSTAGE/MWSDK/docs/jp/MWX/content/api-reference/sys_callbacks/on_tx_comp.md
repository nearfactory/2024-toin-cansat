---
title: "on_tx_comp()"
author: "Mono Wireless Inc."
description: "on_tx_comp()"
---
# on_tx_comp()

送信完了時に呼び出されます。

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) 
```

無線パケットの送信が終了したときに`packet_ev_tx`として `ev` にデータが格納された状態で、本関数が MWX ライブラリ内から呼び出されます。アプリケーション中で本関数が定義されない場合は何もしない weak 関数がリンクされます。

`ev.u8CbId`は送信時のID、`ev.bStatus`は送信の成功（1）または失敗（0）を示すフラグです。

本関数中で `b_handled` に *true* をセットすると、MWX ライブラリに受信パケットがアプリケーション内で処理されたことを伝達します。処理済みとした場合、不要な処理を抑制します。（`the_twelite.app`, `.board`, `.settings` に対してはイベントコールバック関数を呼び出さない）
