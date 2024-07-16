---
title: "SPI (ヘルパークラス版)"
author: "Mono Wireless Inc."
description: "SPI (ヘルパークラス版)"
---
# SPI (ヘルパークラス版)

ヘルパークラス版はより抽象度が高い実装です。読み書きを行うオブジェクト `transceiver `を生成することが、バスの利用開始となり、オブジェクトを破棄するとバス利用の終了手続きを行います。

if文の判定式内でオブジェクトの生成を行うことで、オブジェクトの有効期間はif節内のスコープに限定され、if節を抜けた時点でオブジェクトは破棄され、その時点でバスの利用終了の手続きを行います。

```cpp
uint8_t c;
if (auto&& trs = SPI.get_rwer()) { // オブジェクトの生成とデバイスの通信判定
   // このスコープ(波かっこ)内が trs の有効期間。
   trs << 0x00; // 0x00 を mwx::stream インタフェースで書き出し
   trs >> c;    // 読み出したデータをcに格納。
} 
// if 節を抜けるところで wrt は破棄され、バスの利用終了
```

また、読み書きオブジェクトは、`mwx::stream`インタフェースを実装しているため`<<`演算子などを利用することができます。

* バスの利用開始と終了をオブジェクトの有効期間と一致させることで、ソースコードの見通しを良くし、また終了手続きの記述漏れなどを防ぎます
* `mwx::stream`インタフェースによる読み書き手続きを統一します

## 読み書き

読み込み処理とその終了手続きをスコープ内 `if() { ... }` で行うためのヘルパークラスを用いた読み込み方法。

```cpp
inline uint8_t _spi_single_op(uint8_t cmd, uint8_t arg) {
    uint8_t d0, d1;
    if (auto&& x = SPI.get_rwer()) {
        d0 = x.transfer(cmd); (void)d0;
        d1 = x.transfer(arg);
        // (x << (cmd)) >> d0;
        // (x << (arg)) >> d1;
    }

    return d1;
}
```

上記では `get_rwer()`  メソッドにより生成された `x`  オブジェクトを用いて１バイトずつ読み書きを行っています。

1. `if(...)` 内で `x` オブジェクトを生成します。同時にSPIバスのセレクトピンをセットします。（型は、型推論によるユニバーサル参照 `auto&&` で解決しています。）
2. 生成した `x` オブジェクトには `operator bool ()` が定義されており、判定式の評価として利用される。SPIバスの場合は常に `true` となる。
3. `x` オブジェクトには `uint8_t transfer(uint8_t)` メソッドが定義されていて、これを呼び出すことでSPIに対して8bitの読み書き転送を行。
4. `if() { ... }` スコープの末尾で `x` のデストラクタが呼び出され、SPIバスのセレクトピンを解除します。



### get\_rwer()

```cpp
periph_spi::transceiver get_rwer()
```

SPIバスの読み書きに用いるワーカーオブジェクトを取得します。



## ワーカーオブジェクト

### transfer() transfer16() transfer32()

```cpp
uint8_t transfer(uint8_t val)
uint16_t transfer16(uint16_t val)
uint32_t transfer32(uint32_t val)
```

それぞれ8bit,16bit,32bitの転送を行い、読み取り結果を書き込んだデータ幅と同じデータ幅で返す。



### <<演算子

```cpp
operator << (int c)
operator << (uint8_t c)
operator << (uint16_t c) 
operator << (uint32_t c)
```

`int`型,`uint8_t`型は8bitの転送を行います。

`uint16_t`型、`uint32_t`型は、それぞれ16bitの転送、32bitの転送を行います。

転送結果は最大16バイトの内部FIFOキューに格納され `>>` 演算子により読み出します。バッファが大きくないので、転送都度読み出すことを想定します。



### >>演算子

```cpp
operator >> (uint8_t& c)
operator >> (uint16_t& c)
operator >> (uint32_t& c)

null_stream(size_t i = 1)
operator >> (null_stream&& p)
```

直前の転送と同じデータ幅の変数を指定します。

読み出した結果が不要の場合はnull\_stream()オブジェクトを使用します。iで指定したデータバイト分だけ読み飛ばします。



