---
title: "レイヤーツリーネット NWK_LAYERED"
author: "Mono Wireless Inc."
description: "レイヤーツリーネット NWK_LAYERED"
---

# レイヤーツリーネット  \<NWK\_LAYERED>

シンプルな中継ネットワークを実装したネットワークビヘイビアです。

{% hint style="warning" %}
現時点での実装では、このビヘイビアはレイヤーツリーネットの親機で受信を行うためのものです。親機からの送信や中継器、子機の利用は実装されていません。
{% endhint %}



以下のように `setup()` 中で初期化します。`NWK_LAYERED::ROLE_PARENT`として親機としてのロールを割り当てます。

```cpp
#include <NWK_LAYERED>
void setup() {
  ...
  auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
  nwk_ly << NWK_LAYERED::network_role(NWK_LAYERED::ROLE_PARENT);
            // set a role as parent.
}
```



パケットの受信が行われたときは、`NWK_SIMPLE`と同様に `on_rx_packet()` が呼び出されます。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
  auto type = rx.get_network_type();
    
  if (type == mwx::NETWORK::LAYERED) {
    ; // レイヤーツリーネットのパケット
    handled = true; // 処理済みにする
  }
}
```

`rx`はパケット情報をラップしたクラスです。内部的には `_get_network_type()`の処理用の内部フラグを設定する以外はパケット情報等の加工は行っていません。

つまり`tsRxDataApp* `を返す`rx.get_psRxDataApp()`を参照すれば、TWENET C ライブラリと同様のパケット情報が得られます。`packet_rx`はこの情報にアクセスするためのいくつかの手続きが定義されていますが、得られる情報に変わりはありません。



## `NWK_SIMPLE`との併用

`NWK_SIMPLE`と併用する場合は、`the_twelite.network`に`NWK_LAYERED`を、`the_twelite.newwork2`に`NWK_SIMPLE`を割り当てます。

```cpp
#include <NWK_LAYERED>
#include <NWK_SIMPLE>

void setup() {
    ...
	auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
	auto&& nwk_sm = the_twelite.network2.use<NWK_SIMPLE>();
}

void on_rx_packet(packet_rx& rx, bool_t &handled) {
  auto type = rx.get_network_type();
    
  if (type == mwx::NETWORK::LAYERED) {
      ; // レイヤーツリーネットのパケット
  }
  else if (type == mwx::NETWORK::SIMPLE) {
      ; // NWK_SIMPLE のパケット
  }
  else if (type == mwx::NETWORK::NONE) {
      ; // 通常のアプリ (App_Twelite など)
  }
  else {
      ; // 解釈できなかったもの
  }
  
  // パケットを処理済みにし、これ以上 MWX ライブラリの介入はしない。
  handled = true;
}
```

各パケット種別は上記のように `.get_network_type()` により判別します。

* `mwx::NETWORK::LAYERED` : そのままパケット情報を参照します。
* `mwx::NETWORK::SIMPLE` : `NWK_SIMPLE` の処理に倣います。
* `mwx::NETWORK::NONE` : ネットワーク処理や重複パケットの処理など一切が行われません。例えば App_Twelite 標準アプリケーションの場合、例えば１送信ごとに再送を含め３パケットずつ送出されます。この際、すべてのパケットの受信が成功した場合`on_rx_packt()`が3回呼び出されることになります。通常は、3回受信できたからと言って2回目、3回目のデータは必要ありません。こういった重複パケットの処理を追加する必要があります。



実例はAct\_Samplesの`Rcv_Univsl`を参照してください。TWELITE PAL, Act\_samples, App\_Twelite　といった無線チャネルとアプリケーションIDが同一だが、種別の違うパケットの受信処理しています。さらに App\_Twelite のために重複チェッカーの処理も用意しています。
