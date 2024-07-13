---
title: "Wire"
author: "Mono Wireless Inc."
description: 二線シリアル(I2C) master の読み書き (mwx::periph_wire)
---

# Wire

二線シリアル(I2C) master の読み書きを行います。



#### 別名定義

```cpp
using TwoWire = mwx::periph_twowire<MWX_TWOWIRE_RCVBUFF>;
```

`mwx::periph_wire<MWX_TWOWIRE_RCVBUFF>`は`TwoWire`として参照可能です。



#### 型定義

以下の定義型で引数や戻り値の型を記載します。

```cpp
typedef uint8_t size_type;
typedef uint8_t value_type;
```

## 注意事項

{% hint style="warning" %}
API 中に STOP ビットの扱いが厳格でない呼び出しを行うものもあります。
{% endhint %}

{% hint style="info" %}
`write(), writer::operator() ()` には、本解説以外にもいくつか引数が定義されてます。

* 固定配列型 \
  `uint8_t cmds[]={11,12};`\
  `...`\
  `Wire.write(cmds);`
* `initializer_list<>` 型\
  `Wire.write({11,12})`
{% endhint %}

## 初期化と終了

### Wire インスタンスの生成

ライブラリ内でインスタンスの生成と必要な初期化は行われます。ユーザコードでは `Wire.begin()` を呼び出すことで利用可能となります。

`requestFrom()` メソッドを用いる場合、データを一時保管するための FIFO キューのサイズを指定できます。コンパイル時にマクロ`MWX_TWOWIRE_BUFF` に必要なバイト数を指定してコンパイルする。デフォルトは 32 バイトです。

> 例： `-DMWX_TWOWIRE_BUFF=16`



### begin()

```cpp
void begin(
    const size_type u8mode = WIRE_100KHZ,
    bool b_portalt = false)
```

ハードウェアの初期化を行います。

{% hint style="danger" %}
初期化せずにWireの操作を行うとTWELITE無線モジュールがハングアップします。
{% endhint %}

{% hint style="warning" %}
スリープからの起床時は、スリープ直前で動作していた場合、直前の状態に復帰します。
{% endhint %}



| パラメータ       | 解説                                                                                                                                                                                                                                                                                                              |
| ----------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `u8mode`    | バス周波数を指定する。デフォルトは100Khz(`WIRE_CONF::WIRE_100KHZ`)<br>周波数は`WIRE_CONF::WIRE_??KHZ`で指定し`??`には`50`,`66`,`80`,`100`,`133`,`160`,`200`,`266`,`320`,`400`を指定できる。 |
| `b_portalt` | ハードウェアのピン割り当てを変更する。                                                                                                                                                                                                                                                                                             |

### 例

```cpp
void setup() {
    ...
    Wire.begin();
    ...
}

void wakeup() {
    ...
    Wire.begin();
    ...
}
```





## 読み書き

読み書きの手続きは、以下の2種類あります。いずれかを選択して利用します。

* [メンバ関数版 (以下のメンバ関数を用いた入出力)](wire-member.md)\
  `requestFrom(), beginTransmission(), endTransmission(), write()`
* [ヘルパークラス版(stream機能が使用可能)](wire-helperclass.md)\
  `reader, writer`

####

## その他

### プローブ（デバイスの存在判定)

```cpp
bool probe(uint8_t address)
```

`address` で指定したデバイスが応答するかを確認します。デバイスが存在する場合は `true` が戻ります。



### setClock()

```cpp
void setClock(uint32_t speed)
```

本来はバス周波数を変更するための手続きですが、何も処理をしません。
