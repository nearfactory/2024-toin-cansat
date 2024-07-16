---
title: "packet_rx"
author: "Mono Wireless Inc."
description: "packet_rx"
---

# packet\_rx

このクラスはTWENETの`tsRxDataApp`構造体のラッパークラスです。

このクラスオブジェクトは、[ビヘイビア](../behavior/README.md)のコールバック関数または[`on_rx_packets()`](../sys_callbacks/on_rx_packet.md)により取得できます。

`packet_rx`では、特にパケットのデータペイロードを`smplbuf`コンテナで取り扱えるようにし、`expand_bytes()`などのユーティリティ関数によりペイロードの解釈記述を簡素化しています。

{% hint style="info" %}
現時点では、シンプル中継ネットワーク`<NWK_SIMPLE>`で必要とされるものを中心にメソッド等のインタフェースを実装しています。
{% endhint %}



## メソッド

### get\_payload()

```cpp
smplbuf_u8_attach& get_payload()
```

パケットのデータペイロードを取得する。

{% hint style="info" %}
`<NWK_SIMPLE>`を用いた場合は、先頭に`<NWK_SIMPLE>`用のヘッダデータがあります。戻りとして参照されるコンテナは、このヘッダ部分を除いた部分配列になります。ヘッダ部分まで参照したいときは`get_psRxDataApp()`により`tsRxDataApp`構造体を参照してください。
{% endhint %}



### get\_psRxDataApp()

```cpp
const tsRxDataApp* get_psRxDataApp() 
```

TWENET Cライブラリの受信構造体を得る。



### get\_length()

```cpp
uint8_t get_length()
```

ペイロードのデータ長を返す。`.get_payload().size()`と同じ値になる。



### get\_lqi()

```cpp
uint8_t get_lqi()
```

LQI値 (Link Quality Indicator)を得る。

{% hint style="info" %}
LQIとは電波通信品質を示す値です。0から255までの数値で表されます。

ちなみに、いくつかの段階で評価する場合は、50未満(悪い -80dbm 未満)、50～100(やや悪い)、100～150(良好)、150以上(アンテナの近傍)といった区分けも可能です。これらは目安である点にご留意ください。
{% endhint %}



### get\_addr\_src\_long(), get\_addr\_src\_lid()

```cpp
uint32_t get_addr_src_long()
uint8_t get_addr_src_lid()
```

送信元のアドレスを得る。

`get_addr_src_long()`は送信元のシリアル番号で、MSB(bit31)が必ず１になります。

`get_addr_src_lid()`は送信元の論理IDで`0x00`-`0xFE`までの値をとります（`<NWK_SIMPLE>`で指定する論理IDです）。



### get\_addr\_dst()

```cpp
uint32_t get_addr_dst()
```

宛先アドレスを得ます。

宛先アドレスは、送信元で指定され、宛先の種別によって値の範囲が変わります。

| 値                   | 解説                        |
| ------------------- | ------------------------- |
| MSB(bit31)がセットされている | 宛先としてシリアル番号を指定しています。      |
| `0x00`-`0xFF`       | 宛先として論理ID(8bit)が指定されています。 |



### is\_secure\_pkt()

```cpp
bool is_secure_pkt()
```

暗号化パケットの場合は `true` を返し、平文の時は`false` を返します。




### get_network_type()
```cpp
uint8_t get_network_type() 
```

ネットワークビヘイビアで識別されるパケットのネットワークタイプを返す。

| 値 | 解説                        |
| ----------------------- | ------------------------- |
| `mwx::NETWORK::LAYERED` | `<NWK_LAYERED>` からのパケット |
| `mwx::NETWORK::SIMPLE`  | `<NWK_SIMPLE>`  からのパケット |
| `mwx::NETWORK::NONE`    | ネットワークを介さないパケット (App\_Tweliteなど) |
| その他                  | エラーまたは識別できないパケット |
