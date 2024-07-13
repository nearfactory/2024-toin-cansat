---
title: "smplbuf"
author: "Mono Wireless Inc."
description: "smplbuf"
---
# smplbuf

内部が配列構造のコンテナクラスです。初期化時にバッファの最大サイズを決定しますが、その最大サイズまでの範囲で可変長の配列として振る舞います。

```cpp
template <typename T, int N> smplbuf_local
template <typename T> smplbuf_attach
template <typename T> smplbuf_heap
```

`smplbuf`は要素の型`T`と[メモリの確保方法`alloc`](../alloc.md)で指定したメモリ領域に対して配列の操作を提供するコンテナクラスです。`alloc`の指定は煩雑であるため`using`を用いた別名定義が行っています。

オブジェクトの宣言例です。宣言の直後に初期化用のメソッド呼び出しを行います。いずれも初期化直後の最大サイズは128バイトで、サイズは0です。必要に応じてサイズを拡張しながら使用します。

```cpp
// 配列領域は、クラスメンバー変数の固定配列
smplbuf_local<uint8_t, 128> b1;

// 配列領域は、すでにある領域を参照
uint8_t buf[128];
smplbuf_attach<uint8_t> b2(;

// 配列領域は、ヒープに確保
smplbuf_heap<uint8_t> b3;

// 初期化（グローバル定義の場合はsetup()で行う）
void setup() {
    b1.init_local();
    b2.attach(buf, 0, 128);
    b3.init_heap(128);
} 

// 処理関数内
void some_func() {
    smplbuf_local<uint8_t, 128> bl;
    // bl.init_local(); // smplbuf_localがローカル定義の場合は省略可能
    
    bl.push_back('a');
}
```



上記の`uint8_t`型に限り別名定義があります。

```cpp
template <int N>
smplbuf_u8
// smplbuf<uint8_t, alloc_local<uint8_t, N>>

smplbuf_u8_attach
// smplbuf<uint8_t, alloc_attach<uint8_t>>

smplbuf_u8_heap
// smplbuf<uint8_t, alloc_heap<uint8_t>>
```



通常の配列のように\[]演算子などを用いて要素にアクセスできますし、イテレータを用いたアクセスも可能です。

```cpp
void begin() { // begin()は起動時１回だけ動作する
  smplbuf_u8<32> b1;
  b1.reserve(5); // 5バイト分利用領域に初期化(b1[0..5]にアクセスできる)
  
  b1[0] = 1;
  b1[1] = 4;
  b1[2] = 9;
  b1[3] = 16;
  b1[4] = 25;
  
  for(uint8_t x : b1) { // 暗黙に .begin() .end() を用いたループ
    Serial << int(x) << ",";
  }
}
```



`push_back()`メソッドを定義しています。末尾にデータを追記するタイプのアルゴリズムが使用可能になります。



## 宣言・初期化

```cpp
smplbuf_local<T,N>()
smplbuf_local<T,N>::init_local()

smplbuf_attach<T>(T* buf, uint16_t size, uint16_t N)
smplbuf_attach<T>::attach(T* buf, uint16_t size, uint16_t N)

smplbuf_heap<T>()
smplbuf_heap<T>::init_heap(uint16_t N)

// 例
// 内部に固定配列
smplbuf_local<uint8_t, 128> b1;
b1.init_local();

// すでにある配列を利用する
uint8_t buf[128];
smplbuf_attach<uint8_t> b2;
b2.attach(buf, 0, 128);

// ヒープに確保する
smplbuf_heap<uint8_t> b3;
b3.init_heap(128); 
```

型`T`でサイズ`N`のコンテナを宣言します。宣言後に初期化のメソッドを呼び出します。

`smplbuf_local`は、内部に固定配列により領域を確保します。コンストラクタによる初期化も可能です。

`smplbuf_attach`では、使用するバッファの先頭ポインタ`T* buf`と配列の初期サイズ`size`と最大サイズ`N`を指定します。コンストラクタによる初期化も可能です。

`smplbuf_heap`は、HEAP領域（解放は不可能だが随時確保可能なメモリ領域）にメモリを確保します。一度確保したら開放できない領域ですので通常はグローバル領域に定義します。領域確保は`init_heap()`で行います。コンストラクタによるメモリ確保はできません。必ず`init_heap()`を呼び出して利用してください。

{% hint style="danger" %}
グローバルオブジェクトを生成する場合は、コンストラクタによる初期化が行なえません。実行初期（`setup()`を推奨）に初期化関数`init_local()`,`attach()`,`init_heap()`を呼び出すようにしてください。
{% endhint %}



## 初期化子リスト

```cpp
void in_some_func() {
    smplbuf_local<uint8_t, 5> b1;
    b1.init_local();
    
    b1 = { 0, 1, 2, 3, 4 };
    
    smplbuf_local<uint8_t, 5> b2{0, 1, 2, 3, 4};
}
```

初期化子リスト（イニシャライザリスト）`{ ... }` によるメンバーの初期化をできます。`smplbuf_local`のローカル宣言でのコンストラクタでの利用を除き、初期化のメソッドを呼び出した後に有効です。

* 代入演算子の右辺値 (`smplbuf_local`, `smplbuf_attach`, `smplbuf_heap`)
* コンストラクタ(`smplbuf_local`のローカル宣言、グローバル宣言は不可)



## メソッド

### append(), push\_back(), pop\_back()

```cpp
inline bool append(T&& c)
inline bool append(const T& c)
inline void push_back(T&& c)
inline void push_back(const T& c)
inline void pop_back()
```

末尾にメンバー`c`を追加します。`append()`の戻り値は`bool`で、バッファが一杯で追加できないときに`false`が返ります。

`pop_back()`は末尾のエントリを抹消します。ただしエントリのクリアはしません。



### empty(), size(), capacity()

```cpp
inline bool empty()
inline bool is_end()
inline uint16_t size()
inline uint16_t capacity()
```

`empty()`は配列に要素が格納されていない場合に`true`を戻します。`is_end()`は反対に配列サイズ一杯まで要素が格納されているときに`true`を戻します。

`size()`は配列の要素数を返します。

`capacity()`は配列の最大格納数を返します。



### reserve(), reserve\_head(), redim()

```cpp
inline bool reserve(uint16_t len)
inline void reserve_head(uint16_t len)
inline void redim(uint16_t len)
```

`reserve()`は配列のサイズを拡張します。配列が格納されていない領域はデフォルトで初期化されます。

`reserve_hear()`は配列の先頭部に指定したサイズの領域を確保します。コンテナオブジェクトからは参照できない領域となります。例えばパケットペイロードのヘッダ部分を読み飛ばした部分配列にアクセスするようなコンテナとして利用できるようにします。確保した領域を戻しすべてアクセスできるようにコンテナを戻すには確保時と同じ負の値を与えます。

`redim()`は利用領域のサイズを変更します。`reserve()`と違い、未使用領域の初期化を行いません。



### operator \[]

```cpp
inline T& operator [] (int i) 
inline T operator [] (int i) const
```

要素にアクセスします。

`i`に負の値を与えるとバッファー末尾からの要素となります。`-1`の場合は末尾の要素、`-2`は末尾から一つ手前となります。



## mwx::streamへの出力

`uint8_t`型の配列オブジェクト(`smplbuf<uint8_t, *>`)は、`mwx::stream`の派生オブジェクトに対して、そのまま出力できます。



### << 演算子

```cpp
template <class L_STRM, class AL>
	mwx::stream<L_STRM>& operator << (
			mwx::stream<L_STRM>& lhs, mwx::_smplbuf<uint8_t, AL>& rhs) 
			
//例
smplbuf_u8<128> buf;
buf.push_back('a');
buf.push_back('b');
buf.push_back('c');

Serial << buf;
// 出力: abc

```

`Serial`など`mwx::stream`の派生オブジェクトに対して、バイト列を出力します。



### to\_stream()

```cpp
inline std::pair<T*, T*> to_stream()

//例
smplbuf_u8<128> buf;
buf.push_back('a');
buf.push_back('b');
buf.push_back('c');

Serial << buf.to_stream();
// 出力:0123
```

ストリームへの出力目的で利用します。<<演算子の実装に用いられています。



## mwx::streamでデータ生成

`mwx::stream`では`<<`演算子や`printfmt()`メソッドと行ったストリームに対してバイト列を出力するための関数・演算子が定義されています。`uint8_t`型のsmplbufの配列を出力先と見立ててストリーム出力手続きを行えます。

方法は２種類あります。

* [`.get_stream_helper()`](get\_stream\_helper.md)により生成されるヘルパーオブジェクトを利用する。
* `mwx::stream`を[継承したsmplbufクラス](smplbuf\_strm\_u8.md)を利用する。
