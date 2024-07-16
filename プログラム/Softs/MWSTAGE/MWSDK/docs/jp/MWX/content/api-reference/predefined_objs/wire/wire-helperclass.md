---
title: "Wire (ヘルパークラス版)"
author: "Mono Wireless Inc."
description: "Wire (ヘルパークラス版)"
---
# Wire (ヘルパークラス版)

ヘルパークラス版はより抽象度が高い実装です。読み書きに対応するオブジェクト `reader, writer `を生成することがバスの利用開始となり、オブジェクトを破棄するとバス利用の終了手続きを行います。

if文の判定式内でオブジェクトの生成を行うことで、オブジェクトの有効期間はif節内のスコープに限定され、if節を抜けた時点でオブジェクトは破棄され、その時点でバスの利用終了の手続きを行います。

```cpp
if (auto&& wrt = Wire.get_writer(...)) { // オブジェクトの生成とデバイスの通信判定
   // このスコープ(波かっこ)内が wrt の有効期間。
   wrt << 0x00; // 0x00 を mwx::stream インタフェースで書き出し
} 
// if 節を抜けるところで wrt は破棄され、バスの利用終了
```

また読み書きオブジェクトは`mwx::stream`インタフェースを実装しているため`<<`演算子などを利用することができます。

* バスの利用開始と終了をオブジェクトの有効期間と一致させることで、ソースコードの見通しを良くし、また終了手続きの記述漏れなどを防ぐ
* `mwx::stream`インタフェースによる読み書き手続きの統一

## 読み込み

読み込み処理とその終了手続きをスコープ内 `if() { ... }` で行うためのヘルパークラスを用いた読み込み方法です。

```cpp
const uint8_t DEV_ADDR = 0x70;
uint8_t au8data[6];
if (auto&& rdr = Wire.get_reader(DEV_ADDR, 6)) {
		for (auto&& c: au8data) {
			c = rdr();
		}
}

// same above
uint16_t u16temp, u16humd;
uint8_t u8temp_csum, u8humd_csum;
if (auto&& rdr = Wire.get_reader(SHTC3_ADDRESS, 6)) {
		rdr >> u16temp;
		rdr >> u8temp_csum;
		rdr >> u16humd;
		rdr >> u8humd_csum;
}
```

上記では `get_readr()`  メソッドにより生成された `rdr`  オブジェクトを用いて１バイトずつ読み出しします。 メソッドのパラメータには読み込みたい二線シリアル ID を指定します。

1. `if(...)` 内で `rdr` オブジェクトを生成。（型は、型推論によるユニバーサル参照 `auto&&` で解決しています。）
2. 生成した `rdr` オブジェクトには `operator bool ()` が定義されており、判定式の評価として利用される。指定された ID により通信が可能であれば `true` となる。
3. `rdr` オブジェクトには `int operator () (void)` 演算子が定義されていて、これを呼び出すことで２線シリアルバスから１バイトのデータを読み出す。読み込みに失敗したときは `-1` が戻り、成功した場合は読み込んだバイト値が戻る。
4. `if() { ... }` スコープの末尾で `rdr` のデストラクタが呼び出され、二線シリアルバスの `STOP` を行う。



### get\_reader(addr, read\_count=0)

```cpp
periphe_wire::reader
get_reader(uint8_t addr, uint8_t read_count = 0)
```

I2C 読み出しに用いるワーカーオブジェクトを取得します。

| パラメータ        | 解説                                                                                    |
| ------------ | ------------------------------------------------------------------------------------- |
| `addr`       | 読み込み用のI2Cアドレス                                                                         |
| `read_count` | 読み出しバイト数（この値を指定すると最後の転送で STOP ビットを発行する）。0を指定した場合は STOP ビットなしとなる（デバイスによっては動作するものもあります） |



## 書き出し (writer)

書き出し処理とその終了手続きをスコープ内 `if() { ... }` で行うためのヘルパークラスを用いた読み込み方法です。

```cpp
const uint8_t DEV_ADDR = 0x70;
if (auto&& wrt = Wire.get_writer(DEV_ADDR)) {
	wrt(SHTC3_TRIG_H);
	wrt(SHTC3_TRIG_L);
}

// same above
if (auto&& wrt = Wire.get_writer(DEV_ADDR)) {
	wrt << SHTC3_TRIG_H; // int type is handled as uint8_t
	wrt << SHTC3_TRIG_L;
}

```

上記では `get_writer()`  メソッドにより生成された `wrt`  オブジェクトを用いて１バイトずつ書き出す。 メソッドのパラメータには読み出したい二線シリアル ID を指定します。

1. `if(...)` 内で `wrt` オブジェクトを生成する。（型名は長くなるため auto で解決）
2. 生成した `wrt` オブジェクトには `operator bool ()` が定義されており、判定式の評価として利用される。指定された ID により通信が可能であれば `true` となる。
3. `wrt` オブジェクトには `int operator () (void)` 演算子が定義されていて、これを呼び出すことで２線シリアルバスに１バイトのデータを書き出しす。失敗したときは `-1` が戻り、成功した場合は書き込んだバイト値が戻る。
4. `if() { ... }` スコープの末尾で `wrt` のデストラクタが呼び出され、二線シリアルバスの `STOP` を行う。

### get\_writer()

```cpp
periph_wire::writer
get_writer(uint8_t addr)
```

I2C書き出しに用いるワーカーオブジェクトを取得します。

| パラメータ  | 解説            |
| ------ | ------------- |
| `addr` | 書き出し用のI2Cアドレス |



## ワーカーオブジェクト (writer)

### <<演算子

```cpp
operator << (int c)
operator << (uint8_t c)
operator << (uint16_t c) 
operator << (uint32_t c)
```

`int`型,`uint8_t`型は8bitの転送を行います。データ並び順はビッグエンディアン形式（上位バイトが先に転送される）です。



### ()演算子

```cpp
operator() (uint8_t val)
operator() (int val)
```

１バイト書き出す。



## ワーカーオブジェクト  (reader)

### >>演算子

```cpp
operator >> (uint8_t& c)
operator >> (uint16_t& c)
operator >> (uint32_t& c)
operator >> (uint8_t(&c)[N]) // Nバイトの固定配列
```

それぞれのデータ型のサイズ分だけ読み出します。データ並び順はビッグエンディアン形式（先に転送されたほうが上位バイトに格納される）です。



### ()演算子

```cpp
int operator() (bool b_stop = false)

//例
uint8_t dat[6];
if (auto&& rdr = Wire.get_reader(0x70)) {
  for(uint8_t& x : dat) {
    x = rdr();
  }
}
```

１バイト読み出します。エラーがある場合は-1を戻し、正常時は読み出したバイト値を戻します。

`b_stop`を`true`にすると、その読み出しにおいてSTOPビットを発行します。



## 例

以下の例は、環境センサーパルの温湿度センサーSHTC3の計測例です。

```cpp
Wire.begin();
// reset (may not necessary...)
if (auto&& wrt = Wire.get_writer(0x70)) {
	wrt << 0x80; // SHTC3_SOFT_RST_H
	wrt << 0x05; // SHTC3_SOFT_RST_L
}

delay(5); // wait some

// start read
if (auto&& wrt = Wire.get_writer(0x70)) {
	wrt << 0x60; // SHTC3_TRIG_H
	wrt << 0x9C; // SHTC3_TRIG_L
}

delay(10); // wait some

// read result
uint16_t u16temp, u16humd;
uint8_t u8temp_csum, u8humd_csum;
if (auto&& rdr = Wire.get_reader(0x70, 6)) {
	rdr >> u16temp;
	rdr >> u8temp_csum;
	rdr >> u16humd;
	rdr >> u8humd_csum;
}

// checksum 0x31, init=0xFF
if (CRC8_u8CalcU16(u16temp, 0xff) != u8temp_csum) {
	Serial << format("{SHTC3 T CKSUM %x}", u8temp_csum); }
if (CRC8_u8CalcU16(u16humd, 0xff) != u8humd_csum) {
	Serial << format("{SHTC3 H CKSUM %x}", u8humd_csum); }

// calc temp/humid (degC x 100, % x 100)
int16_t i16Temp = (int16_t)(-4500 + ((17500 * int32_t(u16temp)) >> 16));
int16_t i16Humd = (int16_t)((int32_t(u16humd) * 10000) >> 16);

Serial << "temp=" << int(i16Temp) 
	   << ",humid=" << int(i16Humd) << mwx::crlf;
```
