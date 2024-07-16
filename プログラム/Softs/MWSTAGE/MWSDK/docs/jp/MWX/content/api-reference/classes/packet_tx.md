---
title: "packet_tx"
author: "Mono Wireless Inc."
description: "packet_tx"
---

# packet\_tx

このクラスはTWENET Cライブラリの`tsTxDataApp`構造体のラッパクラスで、このクラスをベースとした派生クラスのオブジェクトをネットワークビヘイビアまたは[`on_tx_comp()`](../sys_callbacks/on_tx_comp.md)により取得します。

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
	pkt << tx_addr(0x00)
		<< tx_retry(0x1)
		<< tx_packet_delay(0,50,10);
		
	pack_bytes(pkt.get_payload()
		, make_pair("APP1", 4)
		, uint8_t(u8DI_BM)
	);
  
  pkt.transmit();
}
```



## オブジェクトの生成

ネットワークビヘイビアの `.prepare_tx_packet()` によって行います。

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  ...
}
```

上記の例では`the_twelite.network.use<NWK_SIMPLE>()`によってネットワークビヘイビアのオブジェクトを取り出します。このオブジェクトの`.prepare_tx_packet()` によってオブジェクト`pkt`が生成されます。型名はauto&&で推論されていますが`packet_tx`の派生クラスになります。

この`pkt`オブジェクトは、まず、`()`内の条件判定にて`true`か`false`を返します。`false`が返ってくるのは、送信用のキューが一杯でこれ以上要求が追加できない時です。



## 送信設定

無線パケットには宛先情報など相手に届けるための様々な設定を行います。設定には設定内容を含むオブジェクトを<<演算子の右辺値に与えます。

```cpp
pkt << tx_addr(0x00)
	  << tx_retry(0x1)
  	<< tx_packet_delay(0,50,10);
```



以下に設定に用いるオブジェクトについて記載します。

{% hint style="warning" %}
各設定の利用可否や意味合いは、ネットワーク ビヘイビアの仕様によります。
{% endhint %}

### tx\_addr

```cpp
tx_addr(uint32_t addr)
```

宛先アドレス`addr`を指定します。宛先アドレスの値については、ネットワークビヘイビアの仕様を参照してください。

* `<NWK_SIMPLE>`
  MSB(bit31=`0x80000000`)がセットされるアドレスは、無線モジュールのシリアル番号宛という意味になります。
  `0x00`..`0xEF`は、8bitの論理IDを意味します。0xFEは子機宛(`0x01`..`0xEF`)の同報通信（ブロードキャスト）、`0xFF`は親機子機関係なく同報通信（ブロードキャスト）します。



### tx\_retry

```cpp
tx_retry(uint8_t u8count, bool force_retry = false)
```

再送回数の指定を行います。再送回数はu8countで指定します。force\_retryは、送信が成功しようがしまいが、指定回数の再送を行う設定です。

* `<NWK_SIMPLE>`
  ネットワークビヘイビア`<NWK_SIMPLE>`では、同じ内容のパケットを`u8count+1`回送信します。
  `force_retry`の設定は無視されます。


### tx\_packet\_delay

```cpp
tx_packet_delay(uint16_t u16DelayMin,
                uint16_t u16DelayMax,
                uint16_t u16RetryDur)
```

パケットを送信するまでの遅延と再送間隔を設定します。`u16DelayMin`と`u16DelayMax`の２つの値をミリ秒\[ms]で指定します。送信要求をしてからこの間のどこかのタイミングで送信を開始します。再送間隔を`u16RetryDur`の値\[ms]で指定します。再送間隔は一定です。

{% hint style="warning" %}
内部処理の都合で指定通りのタイミングで送信処理が始まらない場合もあります。また、IEEE802.15.4の処理でもパケット創出までの時間ブレが発生します。これらのタイミングのブレは、多くのシステムではパケットの衝突回避を行う上で有効な手立てとなります。

厳格なタイミングでのパケット送信は、IEEE802.15.4の規格の性質上、例外的な使用方法とお考え下さい。
{% endhint %}

* `<NWK_SIMPLE>`
  この指定は有効です。
  最初の送信から１秒を超えて再送され到達した同一パケットについては、新たなパケットが到達したとして重複除外が為されません。再送間隔を長く設定したり、中継でのパケット到達遅延により１秒を超えて同じパケットを受信する場合があります。
  重複パケットの処理の設定は`<NWK_SIMPLE>`ビヘイビアの初期化で設定できます。



### tx\_process\_immediate

```cpp
tx_process_immediate()
```

パケット送信を「できるだけ速やかに」実行するように要求する設定です。TWENETでのパケット送信処理は、1msごとに動作するTickTimer起点で行われています。この設定をすることで、要求後速やかにパケット送信要求が処理されます。もちろん、`tx_packet_delay(0,0,0)`以外の設定では意味がない指定になります。

他のパケット送信処理が行われている場合は、通常の処理になります。

* `<NWK_SIMPLE>`
  この指定は有効です。



### tx\_ack\_required

```cpp
tx_ack_required()
```

無線パケット通信では、送信完了後、送信相手先からACK（アック）という短い無線電文を得て、送信成功とする送信方法があります。このオプションを設定することで、ACK付き送信を行います。

* `<NWK_SIMPLE>`
  `<NWK_SIMPLE>`では、この指定は**無効**です。コンパイルエラーになります。
  `<NWK_SIMPLE>`は、シンプルに動作する中継ネットワークの実装を目的としており、ACK付きの通信は行いません。



### tx\_addr\_broadcast

```
tx_addr_broadcast()
```

ブロードキャストの指定を行います。

* `<NWK_SIMPLE>`
  `<NWK_SIMPLE>`では、この指定は**無効**です。コンパイルエラーになります。
  替わりに宛先アドレス`tx_addr(0xFF)` （ブロードキャスト）または`tx_addr(0xFE)`（子機宛のブロードキャスト）を指定します。



### tx\_packet\_type\_id

```cpp
tx_packet_type_id(uint8_t)
```

0..7の指定ができるTWENET無線パケットのタイプIDを指定します。

* `<NWK_SIMPLE>`
  `<NWK_SIMPLE>`では、この指定は**無効**です。コンパイルエラーになります。
  `<NWK_SIMPLE>`ではタイプIDを内部的に使用しています。ユーザは使用できません。





