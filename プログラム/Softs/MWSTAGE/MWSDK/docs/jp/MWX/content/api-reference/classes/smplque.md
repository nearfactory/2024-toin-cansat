---
title: "smplque"
author: "Mono Wireless Inc."
description: "FIFOキューを構造のコンテナクラス"
---

# smplque

FIFOキューを構造のコンテナクラスです。

```cpp
template <typename T, int N, class Intr> smplbuf_local
template <typename T, class Intr> smplbuf_attach
template <typename T, class Intr> smplbuf_heap
```

`smplque`は要素の型`T`と[メモリの確保方法`alloc`](alloc.md)で指定したメモリ領域に対してFIFOキューの操作を提供するコンテナクラスです。`alloc`の指定は煩雑であるため`using`を用いた別名定義が行っています。

{% hint style="warning" %}
要素型は原則として数値や数値などを格納する構造体を想定しています。デストラクタによる破棄手続きが必要なオブジェクトを格納することを想定していません（キューから要素を抹消する際にオブジェクトを抹消する処理をしていないため）。
{% endhint %}

宣言時に割り込み禁止設定を行うクラス`Intr`を登録することが出来ます。このクラスは指定しない場合は、割り込み禁止制御を行わない通常の動作となります。

オブジェクトの宣言例です。宣言の直後に初期化用のメソッド呼び出しを行います。いずれも初期化直後の最大サイズは128バイトで、初期サイズは0で何も格納されていません。最大サイズは変更できません。

```cpp

void some_func() {

// 内部に固定配列
smplque_local<uint8_t, 128> q1;

// すでにある配列を利用する
uint8_t buf[128];
smplque_attach<uint8_t> q2;

// ヒープに確保する
smplque_heap<uint8_t> q3;

void setup() {
  // グローバル定義のオブジェクトは setup() で初期化
  q1.init_local();
  q2.attach(buf, 128);
  q3.init_heap(128);
}

void some_func() {
  // ローカル定義の smplque_local は init_local() は省略できる
  smplque_local<uint8_t, 128> q_local;
  ..
}


```



FIFOキューですので`push()`,`pop()`,`front()`といったメソッドを用いて操作します。

```cpp
void begin() { // begin() は起動時１回のみ動作する
	smplque_local<int, 32> q1;
	
	q1.push(1);
	q1.push(4);
	q1.push(9);
	q1.push(16);
	q1.push(25);
	
	while(!q1.empty()) {
		Serial << int(q1.front()) << ',';
		q1.pop();
	}
	// output -> 1,4,9,16,25,
}
```



イテレータによるアクセスも可能です。

```cpp
void begin() { // begin() は起動時１回のみ動作する
	smplque_local<int, 32> q1;
	q1.init_local();
	
	q1.push(1);
	q1.push(4);
	q1.push(9);
	q1.push(16);
	q1.push(25);
	
	// イテレータを利用
	for(int x : q1) {
		Serial << int(x) << ',';
	}
	
	// STLアルゴリズムの適用
	auto&& minmax = std::minmax_element(q1.begin(), q1.end());
	Serial <<  "min=" << int(*minmax.first)
		     << ",max=" << int(*minmax.second);
	// output -> 1,4,9,16,25,min=1,max=25[]
}
```

## 宣言・初期化

```cpp
smplbuf_local<T,N>
smplbuf_local<T,N>::init_local()

smplbuf_attach<T>
smplbuf_attach<T>::attach(T* buf, uint16_t N)

smplbuf_heap<T>
smplbuf_heap<T>::init_heap(uint16_t N); 

//例
// 内部に固定配列
smplque_local<uint8_t, 128> q1;
q1.init_local();

// すでにある配列を利用する
uint8_t buf[128];
smplque_attach<uint8_t> q2;
q2.attach(buf, 128);

// ヒープに確保する
smplque_heap<uint8_t> q3;
q3.init_heap(128); 
```

型`T`でサイズ`N`のコンテナを宣言します。宣言後に初期化のメソッドを呼び出します。



`smplque_local`は、内部に固定配列により領域を確保します。コンストラクタによる初期化も可能です。

`smplque_attach`では、使用するバッファの先頭ポインタ`T* buf`と配列の初期サイズ`size`と最大サイズ`N`を指定します。コンストラクタによる初期化も可能です。

`smplque_heap`は、HEAP領域（解放は不可能だが随時確保可能なメモリ領域）にメモリを確保します。一度確保したら開放できない領域ですので通常はグローバル領域に定義します。領域確保は`init_heap()`で行います。コンストラクタによるメモリ確保はできません。必ず`init_heap()`を呼び出して利用してください。

{% hint style="danger" %}
グローバルオブジェクトを生成する場合は、コンストラクタによる初期化が行なえません。実行初期（`setup()`を推奨）に初期化関数`init_local()`,`attach()`,`init_heap()`を呼び出すようにしてください。
{% endhint %}



## メソッド

### push(), pop(), front(), back()

```cpp
inline void push(T&& c)
inline void push(T& c)
inline void pop()
inline T& front()
inline T& back()

inline T& pop_front()
```

`push()`はエントリをキューに追加します。

`pop()`はエントリをキューから抹消します。

`front()`は先頭のエントリ（一番最初に追加されたもの)を参照します。

`back()`は末尾のエントリ（一番最後に追加されたもの)を参照します。

`pop_front()`は先頭のエントリを戻り値として参照し、同時にそのエントリをキューから抹消します。



### empty(), size(), is\_full()

```cpp
inline bool empty()
inline bool is_full()
inline uint16_t size()
inline uint16_t capacity()
```

`empty()`は配列に要素が格納されていない場合に`true`を戻します。`is_full()`は反対に配列サイズ一杯まで要素が格納されているときに`true`を戻します。

`size()`はキューに格納されている要素数を返します。

`capacity()`はキューの最大格納数を返します。



### clear()

```cpp
inline void clear()
```

キューのすべての要素を抹消します。



### operator \[]

```cpp
inline T& operator[] (int i)
```

要素にアクセスします。`0`が最初に追加した要素です。



## イテレータ

```cpp
inline smplque::iterator begin()
inline smplque::iterator end()
```

`begin()`と`end()`によるイテレータを取得できます。イテレータの先頭はキューの最初に登録した要素です。イテレータを用いることで範囲for文やアルゴリズムが利用できます。

応用として[axis\_xyzt構造体の特定のメンバーに注目したイテレータによるアクセス](axis\_xyzt.md)があります。

