---
title: "シンプル中継ネット NWK_SIMPLE"
author: "Mono Wireless Inc."
description: "シンプル中継ネット NWK_SIMPLE"
---

# シンプル中継ネット \<NWK\_SIMPLE>

シンプルな中継ネットワークを実装したネットワークビヘイビアです。

```cpp
auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
nwksmpl << NWK_SIMPLE::logical_id(0xFE)
        << NWK_SIMPLE::repeat_max(3);
```

上記はネットワークの利用宣言と設定例です。詳細は後述しますが、ネットワークのアドレスについての考え方など基本的な内容をまず解説します。

{% hint style="info" %}
実際の利用例は[サンプルアクトの解説](../act\_samples/)をご覧ください。
{% endhint %}



このネットワークでの各無線局は8bitの論理IDで識別されます。この値は起動時に各無線局が独自に設定します。論理IDは重複しても構いませんが、重複したことを前提とした通信を行う必要があります。

各無線局のIDを設定します。通常はネットワークには親機の役割の無線局と子機の役割の無線局を配置します。子機のみのネットワークも運用できます。

また子機は中継機の役割にもなります。

| 無線局の識別ID       | 役割          |
| -------------- | ----------- |
| `0x00`         | 親局          |
| `0x01`..`0xEF` | 子局          |
| `0xFE`         | IDを割り振らない子局 |

論理IDを宛先として指定できますが、`0xFE`,`0xFF`は特別な意味を持ちます。下表に宛先指定についてまとめます。

| 宛先ID           | 意味                          |
| -------------- | --------------------------- |
| `0x00`         | 子局から親局を指定する。親局からの指定は無効。     |
| `0x01`..`0xEF` | 特定の子局を指定する。                 |
| `0xFE`         | すべての子局を指定する同報通信（ブロードキャスト）。  |
| `0xFF`         | すべての無線局を指定する同報通信（ブロードキャスト）。 |

また、無線局を特定するのに32bitで指定するシリアル番号も利用できます。



パケットの配送は、IEEE802.15.4のブロードキャストを用います。ACKを用いないため、配送の成功が送信元では判別できませんが、替わりに要求に合った成功率が得られる適当な再送回数を設定したうえ、到達確認が必要な場合は通常パケットの通信を用います。

大規模で頻繁な通信を行う場合は非効率に見えるかもしれませんが、もっぱらデータ収集のみを行い、比較的中継段数が少ないネットワークの場合などより効率的になる場合もあります。

またネットワークを構築するための通信を必要としないため、障害等例外的な状況においても全く通信が止まってしまうといったことが原理的に少なくなります。親機が受信状態かつ子機からの無線到達範囲にあって、子機がパケットを送信しさえすれば、多くの場合親機は受信できます。ネットワークの構築のために通信が必要なネットワークでは、一旦、設定情報などが失われた後は、再度親機と子機間の通信確立のための通信が完了しなければデータが送れません。ネットワークビヘイビア`<NWK_SIMPLE>`がその命名にシンプルとしているのは、こういった理由があります。



このシンプルネットワークを動作させるためには、多くの場合複数回届く再送パケット（同一パケット）を無視する必要があります。`<NWK_SIMPLE>`での同一パケットの識別は送信元のシリアル番号と送信時のパケットの続き番号によって行います（重複チェッカと呼びます）。続き番号は0..63として、順番に割り当てられ、近い時間に届くパケットの続き番号は近い番号であるという仮定を置いています。一定時間以上経過した番号的に遠い(今10番を受信したとしたら40番台のパケットはしばらく前に送信されたと考えられる)続き番号はタイムアウトとして重複対象から除外します。

重複チェッカでの考慮すべきことは以下になります。

{% hint style="info" %}
一般的な使用条件（例：中継段数が３以下で１秒あたりのパケットも１０局未満から到達）では、特別な考慮は不要です。
{% endhint %}

* チェック可能な要素数(数を増やせばメモリ消費とチェックのための処理時間が増える)
* タイムアウト時間の設定

デフォルトではタイムアウトは１秒で、チェックする無線局の数は16です。つまり中継パケットがまわりまわって１秒以上経過した場合、重複パケットとみなされななくなります。また短期的に16を超える無線局からのパケットが到達した場合、超過した無線局については重複チェックが出来なくなります。

中継段数や段数が少なくとも中継局の数が多い場合、再送を非常に長い間隔で行う場合は、設定を考慮すべき場合もあります。



## 宣言・登録

ネットワークビヘイビア`<NWK_SIMPLE>`を利用例を挙げます。

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>

void setup() {
  ...
  
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
}
```

２行目で`<NWK_SIMPLE>`の定義情報をインクルードします。７行目で`the_twelite`に`<NWK_SIMPLE>`を登録します。



## 設定

`<NWK_SIMPLE>`登録後に設定を行います。

```cpp
void setup() {
  auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
  nwksmpl << NWK_SIMPLE::logical_id(0xFE);
}
```

設定は`<<`演算子で行います。

### `<<`演算子 (設定)

オブジェクト`the_twelite`の初期設定を行うために`<<`演算子を用います。

以下に挙げる設定用のクラスオブジェクトを入力とし、設定をしなければデフォルト値が適用されます。

#### NWK\_SIMPLE::logical\_id(uint8\_t id)

パラメータ`id`に指定した論理デバイスIDを設定します。デフォルトは`0xFE`(ID未設定子機)です。



#### NWK\_SIMPLE::retry\_default(uint8\_t val)

パラメータ`val`に指定した回数を送信時の再送回数のデフォルト値とします。



#### NWK\_SIMPLE::repeat\_max(uint8\_t val)

パラメータ`val`に指定した回数を最大中継回数とします。デフォルトは`2`です。

中継をさせたくない場合は`0`を指定します。

{% hint style="info" %}
[受信回路を動作させない設定](../api-reference/predefined\_objs/the\_twelite.md#twenet-rx\_when\_idle-uint-8-\_t-benable)をした場合、TWELITE無線マイコンが無線パケットを受信できないため、必然的に、中継動作は行われなくなります。
{% endhint %}



#### NWK\_SIMPLE::dup\_check(uint8\_t maxnodes, uint16\_t timeout\_ms, uint8\_t tickscale)

重複パケットの検出アルゴリズムのパラメータです。

{% hint style="warning" %}
中継段数が多かったり、パケットの再送間隔が長い場合は考慮が必要ですが、**一般的な利用での設定変更は必要ありません。**
{% endhint %}

* `maxnodes`は履歴を保持するため無線局（ノード）の数です。ノード数を少なく設定した場合、短期間に設定以上のノードからのパケットが来た場合、重複除外できないノードが出てきます。重複除外できない場合、受信時に複数回データが表示される、必要以上に再中継してしまうといった問題が出ます。デフォルトは`16`です。１ノード当たり21バイトメモリを消費します。
* `timeout_ms`は履歴を抹消するまでのタイムアウト時間\[ms]です。タイムアウトは続き番号のブロック単位で管理されていて、ブロック単位でタイムアウト処理が行われます。デフォルトは`1000`\[ms]です。
* `tickscale`はタイムアウトを管理するための時間単位で`2^tickscale`\[ms]となります。時間は7bitで管理されるため、`127*(2^tickscale) > timeout_ms`になるように設定します。デフォルトは`5`(32ms)です。



#### NWK\_SIMPLE::secure\_pkt(const uint8\_t \*pukey, bool b\_recv\_plain\_pkt = false)

暗号化パケットの有効化します。

* `pukey` は暗号化の鍵を １６バイト (128bit) で指定します。
* `b_recv_plain_pkt` は`true`を指定すると、同じアプリケーションID、チャネルの平文パケットを受信します。

{% hint style="info" %}
パケットが平文かどうかは `packet_rx::is_secure_pkt()` が `true `(暗号化) か `false `(平文) で判定できます。
{% endhint %}



#### STG\_STD

[インタラクティブモードの設定値](../settings/stg\_std.md#no-1)を反映します。以下の値を反映します。

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
...
set.reload(); // 設定値をロード
nwk << set;   // インタラクティブモードの設定値を反映
```

* **logical\_id**
* **retry\_default**

## メソッド

### prepare\_tx\_packet()

```cpp
// 型名はpacket_tx_nwk_simple<NWK_SIMPLE>ですがauto&&と記載します。
auto&&  preare_tx_packet()

//例
if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  ...
  pkt.transmit();
}
```

送信オブジェクトを取得します。オブジェクトは[`packet_tx`](../api-reference/classes/packet\_tx.md)の派生クラスになります。このオブジェクトに送信アドレスやペイロードを格納し`.transmit()`メソッドで送信を行います。

このオブジェクトには`bool`演算子が定義されています。オブジェクト生成時にTWENETが送信要求を受け付けられない場合は`false`を返します。



## 送信オブジェクト

`.prepare_tx_packet()`メソッドにて取得した送信オブジェクトのメソッドです。

### bool演算子

```cpp
operator bool()

// 例
if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

オブジェクト生成時にTWENETが送信要求を受け付けられない場合は`false`を返します。



### transmit()

```cpp
MWX_APIRET transmit()

// 例
uint8_t txid;

if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  
  ...
  
  MWX_APIRET ret = pkt.transmit();
  if (ret) {
    txid = pkt.get_value();
  }
}
```

パケットの送信処理を行います。`MWX_APIRET`が`true`の場合に送信要求が成功ですが、この要求時点では送信処理が始まりません。

パケットの送信IDは[`MWX_APIRET`](../api-reference/classes/mwx\_apiret.md)の`.get_value()`で得られる値部に格納されます。[`the_twelite.x_status.is_complete()`](../api-reference/predefined\_objs/the\_twelite.md#tx\_status)または[`transmit_complete()`](../api-reference/behavior/#transmit\_complete)より送信完了を確認できます。



## パケット最大長と構造

パケットの最大長を以下に示します。宛先をLID(論理デバイスID)とした場合は、宛先ショートの欄となり、暗号化無しで87バイトまで含めることが可能です。

| ネットワーク層     | 暗号化 | ペイロードの最大 |
| ----------- | --- | -------- |
| NWK\_SIMPLE | なし  | 91       |
| NWK\_SIMPLE | あり  | 89       |

※ 将来のための予備として２バイト分は予備としています。ユーザの判断でこの予備バイトを利用することも可能です。



パケットの構造は以下のようになっています。

```
|MacH:17[........................................]MacF:2|
         TWENET:3[.....................]TwenetF:1
                  NWK_SIMPLE:11|PAYLOAD
                         　　             (:nはバイト数)

1: MacH は IEEE802.15.4 MAC のヘッダ情報
2: TwenetH は TWENET 識別のための情報
3: NWK_SIMPLE は NWK_SIMPLEネットワークの制御情報
  |Type:1|Src LID:1|Src Addr:4|Dest LID:1|Dst Addr:4|Repeat:1|
4: PAYLOAD はペイドード
5: TwenetF は CRC8 のチェックサム(TWENETパケットの弁別を目的とする)
6: MacF は CRC16 のMAC 層のチェックサム
```
