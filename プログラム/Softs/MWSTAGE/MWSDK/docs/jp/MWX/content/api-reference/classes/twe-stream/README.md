---
title: "入出力ストリーム"
author: "Mono Wireless Inc."
description: "入出力ストリーム"
---

# mwx::stream

入出力ストリームを処理する上位クラスです。

* CRTP (Curiously Recurring Template Pattern) 手法を用いたポリモーフィズムにより、いくつかのクラス(`Serial, Wire, SPI, smplbuf`) にインタフェースを提供します。
  * CRTP では下位クラスは `template class Derived : public stream<Derived>;`のように定義し、上位クラスからも下位クラスのメソッドを参照します。
* 本クラスでは `print` メソッド、`<<` 演算子などの共通処理の定義を行い、下位クラスで実装した`  write()  `メソッドなどを呼び出すことで、仮想関数を用いるのと近い実装を行っています。

## インタフェース（下位クラスで実装）

下位クラスでは、以下に列挙する関数を実装します。

### available()

```cpp
int available()

// example
while(Serial.available()) {
  int c = Serial.read();
  // ... any
}
```

入力が存在する場合は 1、存在しない場合は 0 を返します。

| パラメータ     | 解説               |
| --------- | ---------------- |
| 戻り値 `int` | 0: データなし 1:データあり |

{% hint style="warning" %}
本実装の戻り値はバッファ長ではありません。
{% endhint %}

###

### flush()

```cpp
void flush()

// example
Serial.println("long long word .... ");
Serial.flush();
```

出力をフラッシュ（出力完了まで待つ）します。



### read()

```cpp
int read()

// example
int c;
while (-1 != (c = read())) {
    // any
}
```

ストリームより１バイトデータを入力します。データが存在しない場合は `-1` を戻します。



### write()

```cpp
size_t write(int c)

// example
Serial.write(0x30);
```

ストリームに１バイト出力します。

| パラメータ        | 解説                  |
| ------------ | ------------------- |
| `n`          | 出力したい文字。            |
| 戻り値 `size_t` | 出力が成功すれば 1、失敗すれば 0。 |



### vOutput()

```cpp
static void vOutput(char out, void* vp)
```

１バイト出力を行うスタティック関数です。クラスメソッドではないため、メンバー変数等の情報は利用できません。替わりにパラメータとして渡される vp にクラスインスタンスへのポインタを渡します。

このスタティック関数は内部的に利用され`fctprintf()`の１バイト出力関数として関数ポインタが渡ります。これを用いて`print`メソッドなどを実装しています。

| パラメータ | 解説                                                           |
| ----- | ------------------------------------------------------------ |
| `out` | 出力したい文字                                                      |
| `vp`  | クラスインスタンスへのポインタ<br>通常は、元のクラスにキャストして write() メソッドを呼び出す |



## インタフェース

### putchar()

```cpp
void mwx::stream::putchar(char c)

// example
Serial.putchar('A');
// result -> A
```

１バイト出力します。

### print(), println()

```cpp
size_t print(T val, int base = DEC) // T: 整数型
size_t print(double val, int place = 2)
size_t print(const char*str)
size_t print(std::initializer_list<int>)

// example
Serial.print("the value is ");
Serial.print(123, DEC);
Serial.println(".");
// result -> the value is 123.

Serial.print(123.456, 1);
// result -> 123.5

Serial.print({ 0x12, 0x34, 0xab, 0xcd });
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.
```

各種整形出力を行います。

| パラメータ        | 解説                                                        |
| ------------ | --------------------------------------------------------- |
| `val`        | 整形出力したい数値型                                                |
| `base`       | 出力形式BIN 二進数 / OCT ８進数 / DEC 10進数 / HEX 16進数 |
| `place`      | 小数点以下の桁数                                                  |
|              |                                                           |
| 戻り値 `size_t` | 書き出したバイト数                                                 |



### printfmt()

```cpp
size_t printfmt(const char* format, ...);

// example 
Serial.printfmt("the value is %d.", 123);
// result -> the value is 123.
```

printf 形式での出力を行います。

> TWESDK/TWENET/current/src/printf/README.md 参照



### operator <<

```cpp
// examples
Serial << "this value is" // const char*
       << int(123)
       << '.';
       << mwx::crlf;
// result -> this value is 123.

Serial << fromat("this value is %d.", 123) << twe::crlf;
// result -> this value is 123.

Serial << mwx::flush; // flush here

Serial << bigendian(0x1234abcd);
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Serial << int(0x30) // output 0x30=48, "48"
       << '/'
       << uint8_t(0x31); // output '1', not "48"
// result -> 48/1

smplbuf<char,16> buf = { 0x12, 0x34, 0xab, 0xcd };
Serail << but.to_stream();
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Seiral << make_pair(buf.begin(), buf.end());
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Serial << bytelist({ 0x12, 0x34, 0xab, 0xcd });
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

```

| 引数型                                                                                          | 解説                                                                                                                                        |
| -------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| `char`                                                                                       | １バイト出力 （数値としてフォーマットはしない）                                                                                                                  |
| `int`                                                                                        | 整数出力 (printf の "%d")                                                                                                                      |
| `double`                                                                                     | 数値出力 (printf の "%.2f")                                                                                                                    |
| `uint8_t`                                                                                    | １バイト出力する(char型と同様)                                                                                                                        |
| `uint16_t`                                                                                   | ２バイト出力する(ビッグエンディアン順)                                                                                                                      |
| `uint32_t`                                                                                   | ４バイト出力する(ビッグエンディアン順)                                                                                                                      |
| `const char*``uint8_t*``const char[S]` | 終端文字までを出力します。出力には終端文字は含まれません。(`S`は固定配列のサイズ指定)                                                                    |
| `uint8_t[S]`                                                                                 | 配列サイズ`S`バイト分をそのまま出力します。(`S`は固定配列のサイズ指定)                                                               |
| `format()`                                                                                   | printf 形式での出力                                                                                                                             |
| `mwx::crlf`                                                                                  | 改行 CRLF の出力                                                                                                                               |
| `mwx::flush`                                                                                 | 出力のフラッシュ                                                                                                                                  |
| `bigendian()`                                                                                | 数値型をビッグエンディアン順で出力する。(右辺値)                                                                                                                 |
| `std::pair<T*, T*>`                                                                          | バイト型の `begin(), end()` ポインタを格納したペア。`make_pair `により生成できる。`T`は `uint8_t` 型を想定する。(右辺値)                                                       |
| `bytelist()`                                                                                 | `std::initializer_list `を用いるバイト列の出力                                                                                                       |
| `smplbuf<uint8_t,AL>&`                                                                       | `uint8_t`型の配列クラスの内容を出力する。`ALC`は<a href="../alloc.md">メモリ確保手段</a>。                                     |
| `smplbuf<uint8_t, AL>::to_stream()`                                                          | `smplbuf&#x3C;T> `のデータを出力する<br>`T`は `uint8_t`型、`AL`は<a href="../alloc.md">メモリ確保手段</a>。 |

{% hint style="info" %}
バイト列として出力する際は、`uint8_t, uint16_t, uint32_t` 型にキャストします。また文字列として数値出力する場合は明示的に`int`形にキャストするようにしてください。
{% endhint %}

{% hint style="info" %}
１バイト型は型名によって取り扱いが違います。通常はサイズを意識した`uint8_t[S]`型を用いるようにしてください。
{% endhint %}



### set\_timeout(), get\_error\_status(),  clear\_error\_status()

```cpp
uint8_t get_error_status()
void clear_error_status()
void set_timeout(uint8_t centisec)

// example
Serial.set_timeout(100); // 1000msのタイムアウトを設定
uint8_t c;
Serial >> c;
```

`>>`演算子を用いた入力タイムアウトとエラーを管理します。

`set_timeout()` によりタイムアウト時間を指定し、`>>`演算子により入力処理を行います。所定時間内までに入力が得られない場合は `get_error_status()` によりエラー値を読み出せます。`clear_error_status()`によりエラー状況をクリアします。

| 引数型        | 解説                                                                            |
| ---------- | ----------------------------------------------------------------------------- |
| `centisec` | 1/10秒単位でタイムアウト時間を設定します。`0xff`を指定した場合は、タイムアウトを無効とします。 |

#### エラー値

| 値 | 意味    |
| - | ----- |
| 0 | エラーなし |
| 1 | エラー状況 |



### operator >>

```cpp
inline D& operator >> (uint8_t& v)
inline D& operator >> (char_t& v)
template <int S> inline D& operator >> (uint8_t(&v)[S])
inline D& operator >> (uint16_t& v)
inline D& operator >> (uint32_t& v)
inline D& operator >> (mwx::null_stream&& p)

//// 例
uint8_t c;

the_twelite.stop_watchdog(); // ウォッチドッグの停止
Serial.set_timeout(0xFF); // タイムアウト無し

// １バイト読み出す
Serial >> c;
Serial << crlf << "char #1: [" << c << ']';

// 読み捨てる
Serial >> null_stream(3); // 3バイト分読み捨てる
Serial << crlf << "char #2-4: skipped";

// ４バイト分読み出す (uint8_t 型固定長配列限定)
uint8_t buff[4];
Serial >> buff;
Serial << crlf << "char #5-8: [" << buff << "]";
```

入力処理を行います。

{% hint style="danger" %}
* `setup()` 内では実行できません。
* ポーリング待ちを行うため、タイムアウトの時間設定（タイムアウト無しなど）によっては、ウォッチドッグタイマーが発動してリセットする場合があります。
{% endhint %}

{% hint style="info" %}
通常は`loop()`中で以下のような読み出しを行います。

```cpp
void loop() {
  uint8_t c;
  while(Serial.available()) {
    Serial >> c;
    // または c = Serial.read();
    
    switch(c) { ... }  // cの値によって処理を分岐する
  }
}
```
{% endhint %}



以下に読み出し格納できる型を列挙します。

| 引数型                  | 解説                                                            |
| -------------------- | ------------------------------------------------------------- |
| `uint8_t, char_t`    | １バイト入力                                                        |
| `uint16_t`           | ２バイト入力（ビッグエンディアン順）                                            |
| `uint32_t`           | ４バイト入力（ビッグエンディアン順）                                            |
| `uint8_t[S]`         | `S`バイト分入力(`S`は固定配列のサイズ指定) |
| `null_stream(int n)` | `n`バイト読み捨てる                                                   |

