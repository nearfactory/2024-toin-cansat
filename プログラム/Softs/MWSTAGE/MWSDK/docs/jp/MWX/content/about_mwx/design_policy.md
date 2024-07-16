---
title: "設計情報"
author: "Mono Wireless Inc."
description: "MWX ライブラリ内で用いる C++ 言語について、その仕様、制限事項、本書記載留意事項、設計メモを記載します。"
---
# 設計情報

MWX ライブラリ内で用いる C++ 言語について、その仕様、制限事項、本書記載留意事項、設計メモを記載します。

{% hint style="danger" %}
**MWXライブラリでアプリケーション記述する場合は、このページを読み飛ばしても差し支えありません。**
{% endhint %}

{% hint style="info" %}
このページはライブラリの動作の理解や改造などのためライブラリソースコードを参照する場面を想定しています。ライブラリを利用するのと比べより高度なC++言語に関する知識を前提となります。
{% endhint %}

## 設計方針について

* アプリケーションのループ記述では、一般によく用いられる API 体系に近い記述を出来ることを目的とするが、TWELITEの特性に合わせた実装とする。
* TWENET はイベントドリブンによるコード記述であり、これを扱えるようにクラス化を行う。上記クラス化によりアプリケーションのふるまいをカプセル化できるようにする。
* イベントドリブンとループの記述は共存できる形とする。
* 代表的なペリフェラルはクラス化して手続きを簡素化する。可能な限りループ記述でアクセスできるようにする。
* 当社で販売する MONOSTICK/PAL といったボードを利用する手続きをクラス化し手続きを簡素化する。（例えば外部のウォッチドッグタイマーの利用を自動化する）
* アプリケーションクラスやボードクラスは、ポリモーフィズムの考え方を導入し、統一した手続きによって利用できるようにする。（例えば、いくつかの振る舞いをするアプリケーションクラスを始動時にロードするような場合、また TWENET C ライブラリの接続部のコードを都度定義しなくてよいようにするため）。
* C++の機能については、特に制限を設けず利用する。例えば、無線パケットを取り扱うにあたり煩雑なパケット構築、分解といった代表的な手続きを簡略化する手段を提供する。
* 演算子 `->` を極力使用しないようにし、原則として参照型による API とする。



{% hint style="info" %}
限られた時間で実装を進めているため、細かい点まで網羅した設計ではありませんが、設計・実装等でお気づきの点がありましたら、当社サポートにご連絡ください。
{% endhint %}

## C++ コンパイラについて

#### バージョン

gcc version 4.7.4



#### C++言語規格

C++11 (コンパイラ対応状況は一般の情報を参考にしてください）

* [https://gcc.gnu.org/gcc-4.7/cxx0x\_status.html](https://gcc.gnu.org/gcc-4.7/cxx0x\_status.html)&#x20;
* [https://cpprefjp.github.io/implementation-status.html](https://cpprefjp.github.io/implementation-status.html)&#x20;



## C++ の制限事項

※ 当社で把握しているものについての記載です。

* `new, new[]` 演算子でのメモリ確保は行えますが、確保したメモリを破棄することはできません。C++ライブラリで動的メモリ確保をするものは殆どが事実上利用不可能です。一度だけ生成してそれ以降破棄しないオブジェクトに使用しています。
* グローバルオブジェクトのコンストラクタが呼び出されません。\
  参考：必要な場合は、初期化関数(`setup()`) で `new ((void*)&obj_global) class_foo(); `のように初期化することでコンストラクタの呼び出しを含めた初期化を行えます。
* 例外 `exception`が使用できません。
* 仮想関数 `virtual`が使用できません。



## 設計メモ

本節ではMWXライブラリのコードを参照する際に理解の助けとなる情報を記載します。

### 現状の実装

限られた時間で実装を進めているため、詳細部分の整備が十分でない場合があります。例えば const に対する考慮は多くのクラスで十分なされていません。



### 名前空間

名前空間について、以下の方針としています。

* 定義は原則として共通の名前空間`mwx`に配置する。
* 名前空間の識別子なしで利用できるようにしたいが、一部の定義は識別子を必須としたい。
* クラス名については比較的長い命名とし、ユーザが利用するものは別名定義とする。

クラス・関数・定数は一部の例外を除き`mwx`名(正確には`inline namespace L1` で囲んだ`mwx::L1`)の名前空間内に定義しています。`inline namespace`を指定しているのは、`mwx::`の指定を必須とする定義と、必須としない定義を共存させるためです。&#x20;

殆どの定義は`using namespace`により名前空間名を指定しなくても良いようになっています。これらの指定はライブラリ内の`using_mwx_def.hpp`で行っています。

```cpp
// at some header file.
namespace mwx {
  inline namespace L1 {
    class foobar {
      // class definition...
    };
  }
}

// at using_mwx_def.hpp
using namespace mwx::L1; // mwx::L1 内の定義は mwx:: なしでアクセスできる
                         // しかし mwx::L2 は mwx:: が必要。
```

例外的に比較的短い名前については`mwx::crlf, mwx::flush`のように指定します。これらは`inline namespace`の`mwx::L2`の名前空間に配置されています。`using namespace mwx::L2;`を指定することで名前空間名の指定なしで利用できるようになります。

また、いくつかのクラス名は`using`指定をしています。

MWXライブラリ内で利用する`std::make_pair`を`using`指定しています。



### CRTP(奇妙に再帰したテンプレートパターン)&#x20;

仮想関数 (virtual), 実行時型情報(RTTI) が利用できない、かつ利用できるようにしたとしても、パフォーマンス面で難があるため、これに代わる設計手法として [CRTP (Curiously recurring template pattern : 奇妙に再帰したテンプレートパターン）](https://en.wikipedia.org/wiki/Curiously\_recurring\_template\_pattern)を用いています。CRTPは、継承元の親クラスから子クラスのメソッドを呼び出すためのテンプレートパターンです。

以下の例では `Base `を継承した `Derived `クラスに `interface()` というインタフェースを実装する例です。`Base`からは`Derived::print()`メソッドを呼び出しています。

```cpp
template <class T>
class Base {
public:
  void intrface() {
    T* derived = static_cast<T*>(this);
    derived->prt();
  }
};

class Derived : public class Base<Derived> {
  void prt() {
     // print message here!
     my_print("foo");
  }
}
```

MWXライブラリで利用されている主要クラスは以下です。

* イベント処理の基本部分`mwx::appdefs_crtp`
* ステートマシン`public mwx::processev_crtp`
* ストリーム `mwx::stream`



### CRTP での仮想化

CRTPクラスは、継承元のクラスはインスタンスごとに違います。このため、親クラスにキャストして、同じ仲間として取り扱うといったこともできませんし、仮想関数(`virtual`)やRTTI（実行時型情報）を用いたような高度なポリモーフィズムも使うことが出来ません。

以下は上述のCRTPの例を、仮想関数で実装した例です。CRTPでは`Base* b[2]`のように同じ配列にインスタンスをまとめて管理することは、そのままではできません。

```cpp
class Base {
	virtual void prt() = 0;
public:
	void intrface() { prt(); }
};

class Derived1 : public Base {
	void prt() { my_print("foo"); }
};

class Derived2 : public Base {
	void prt() { my_print("bar"); }
};

Derived1 d1;
Derived2 d2;
Base* b[2] = { &d1, &d2 };

void tst() {
	for (auto&& x : b) { x->intrface(); }
}
```



MWXライブラリでは、CRTP のクラスインスタンスを格納するための専用クラスを定義し、このクラスに同様のインタフェースを定義することで解決しています。以下にコード例を挙げます。

```cpp
class VBase {
public:
	void* p_inst;
	void (*pf_intrface)(void* p);

public:
	void intrface() {
		if (p_inst != nullptr) {
			pf_intrface(p_inst);
		}
	}
};

template <class T>
class Base {
	friend class VBase;
	static void s_intrface(void* p) {
		T* derived = static_cast<T*>(p);
		derived->intrface();
	}
public:
	void intrface() {
		T* derived = static_cast<T*>(this);
		derived->prt();
	}
};

class Derived1 : public Base<Derived1> {
	friend class Base<Derived1>;
	void prt() { my_print("foo"); }
};

class Derived2 : public Base<Derived2> {
	friend class Base<Derived2>;
	void prt() { my_print("bar"); }
};

Derived1 d1;
Derived2 d2;

VBase b[2];

void tst() {
	b[0] = d1;
	b[1] = d2;

	for (auto&& x : b) {
		x.intrface();
	}
}
```

`VBase` クラスのメンバ変数`  p_inst  `は、`Base <T>` 型のオブジェクトへのポインタを格納し、`pf_intrface`は `Base<T>::s_intrface` へのメンバ関数ポインタです。 `Base<T>::s_intrface` は、自身のオブジェクトインスタンスを引数として渡され、`T`型に`static_cast`することで`T::intrface`メソッドを呼び出します。

`VBase`への格納は、ここでは `=` 演算子のオーバーロードによって実装しています（ソース例は後述）。

上記の例では`b[0].intrface()`の呼び出しを行う際には、`VBase::pf_intrface`関数ポインタを参照し`Base<Derived1>::s_intrface()`が呼び出されることになります。さらに`Derived1::intrface()`の呼び出しを行うことになります。この部分はコンパイラによるinline展開が期待できます。

`VBase `型から元の`Derived1`や`Derived2`への変換を行うことも、強制的なキャストにより可能ですが、`void*`で格納されたポインタの型を直接知る方法はありません。完全に安全な方法はないものの、以下のようにクラスごとに一意のID(`TYPE_ID`)を設けて、キャスト実行時(`get()`メソッド)にIDのチェックを行うようにしています。違う型を指定して `get()`メソッドを呼び出したときは、エラーメッセージを表示するといった対処になります。

`Base<T>`型としてのポインタが格納されると`T`型に正しく変換できない可能性（`T`が多重継承している場合など）あるため、`<type_trails>`の`is_base_of`により`Base<T>`型の派生であることをコンパイル時に `static_assert`による判定を行っています。

```cpp
#include <type_trails>

class Derived1 : public Base<Derived1> {
public:
   static const uint8_t TYPE_ID = 1;
}

class Derived1 : public Base<Derived1> {
public:
   static const uint8_t TYPE_ID = 2;
}

class VBase {
  uint8_t type_id;
public:
	
	template <class T>
	void operator = (T& t) {
		static_assert(std::is_base_of<Base<T>, T>::value == true,
						"is not base of Base<T>.");

		type_id = T::TYPE_ID;
		p_inst = &t;
		pf_intrface = T::s_intrface;
	}
	
  template <class T>
  T& get() {
    static_assert(std::is_base_of<Base<T>, T>::value == true,
					  "is not base of Base<T>.");
			
		if(T::TYPE_ID == type_id) {
			return *reinterpret_cast<T*>(p_inst);
		} else {
			// panic code here!
		}
  }
}

Derived1 d1;
Derived2 d2;

VBase b[2];

void tst() {
	b[0] = d1;
	b[1] = d2;
	
  Derived1 e1 = b[0].get<Derived1>(); // OK
  Derived2 e2 = b[1].get<Derived2>(); // OK
  
  Derived2 e3 = b[1].get<Derived1>(); // PANIC!
}

```



### new, new\[] 演算子

TWELITEモジュールのマイコンには十分なメモリもなければ、高度なメモリ管理もありません。しかしマイコンのメモリマップの末尾からスタックエリアまでの領域はヒープ領域として、必要に応じて確保できる領域があります。以下にメモリマップの概要を図示します。APPがアプリケーションコードで確保されたRAM領域、HEAPはヒープ領域、STACKはスタック領域です。

```
|====APP====:==HEAP==..   :==STACK==|
0                                  32KB
```

たとえ`delete`できなくても`new`演算子が有用である場面も想定されます。そのため、以下のように`new, new[]`演算子を定義しています。`pvHear_Alloc()`は半導体ライブラリで提供されているメモリ確保の関数で、`u32HeapStart, u32HeapEnd`も同様です。`0xdeadbeef`はダミーアドレスです。beefがdeadなのは変だとかいう指摘はしないでください。

```cpp
void* operator new(size_t size) noexcept {
    if (u32HeapStart + size > u32HeapEnd) {
        return (void*)0xdeadbeef;
    } else {
        void *blk = pvHeap_Alloc(NULL, size, 0);   
        return blk;
    }
}
void* operator new[](size_t size) noexcept {
    return operator new(size); }
void operator delete(void* ptr) noexcept {}
void operator delete[](void* ptr) noexcept {}
```

例外も使えないため失敗したときの対処はありません。また、メモリ容量を意識せず確保を続けた場合、スタック領域と干渉する可能性もあります。

{% hint style="info" %}
システム（MAC層など）が確保するメモリは約2.5KBです。
{% endhint %}



### コンテナクラス

MWXライブラリでは、マイコンのリソースが小さい点、メモリーの動的確保ができない点を考慮し標準ライブラリで提供されるコンテナクラスの利用はせず、シンプルなコンテナクラスを２種類定義しています。コンテナクラスにはイテレータや`begin(), end()`メソッドを定義しているため、範囲for文やSTLのアルゴリズムの一部を利用できます。

```cpp
smplbuf<int16_t, alloc_local<int16_t, 16>> buf;
buf.push_back(-1); // push_back() は末尾に追加
buf.push_back(2);
...
buf.push_back(10);

//範囲for文
for(auto&& x : buf) { Serial << int(x) << ',' }
//アルゴリズム std::minmax
auto&& minmax = std::minmax_element(buf.begin(), buf.end());
Serial << "Min=" << int(*minmax.first)
       << ",Max=" << int(*minmax.second);

```

| クラス名      | 概要                                                                                                                                             |
| --------- | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `smplbuf` | 配列クラスで、最大領域 (capacity) と最大領域範囲内で都度サイズを指定できる利用領域(size)を管理します。また本クラスは stream インタフェースを実装しているため、&#x3C;&#x3C; 演算子を用いてデータを書き込むことができます。 |
| `smplque` | FIFOキューを実装しています。キューのサイズはテンプレートのパラメータで決定します。割り込み禁止を用いキューを操作するためのテンプレート引数もあります。                                                                  |

####

#### コンテナクラスのメモリについて

コンテナクラスではメモリの確保方法を`template`引数のパラメータとして指定します。

| クラス名           | 内容                                                                                            |
| -------------- | --------------------------------------------------------------------------------------------- |
| `alloc_attach` | すでに確保済みのバッファメモリを指定する。Cライブラリ向けに確保したメモリ領域を管理したいとき、同じバッファ領域の分断領域として処理したい時などに使用します。 |
| `alloc_static` | クラス内に静的配列として確保する。事前にサイズが決まっていたり、一時利用の領域として使用します。                                |
| `alloc_heap`   | ヒープ領域に確保する。<br>システムのヒープに確保後は破棄できませんが、初期化時にアプリケーションの設定などに従い領域を確保するといった使い方に向いています。       |



### 可変数引数

MWXライブラリでは、バイト列やビット列の操作、`printf`相当の処理を行う処理に可変数引数を用いています。下記の例は指定のビット位置に1をセットする処理です。

```cpp
// packing bits with given arguments, which specifies bit position.
//   pack_bits(5, 0, 1) -> (b100011) bit0,1,5 are set.

// 再帰取り出しの一番最初の関数
template <typename Head>
constexpr uint32_t pack_bits(Head head) { return  1UL << head; }

// head を取り出し、残りのパラメータを再帰呼び出しにて pack_bits に転送
template <typename Head, typename... Tail>
constexpr uint32_t pack_bits(Head head, Tail&&... tail) {
  return (1UL << head) | pack_bits(std::forward<Tail>(tail)...);
}

// コンパイル後、以下の２つは同じ結果になります。
constexpr uint32_t b1 = pack_bits(1, 4, 0, 8);
// b1 and b2 are the same! 
const uint32_t b2 = (1UL << 1)|(1UL << 4)|(1UL << 0)|(1UL << 8);
```

この処理では template のパラメータパック (`typename...` の部分) で、再帰的な処理を行い引数の展開を行っています。上記の例では`constexpr`の指定があるため、コンパイル時に計算が行われマクロ定義や`b2`のような`const`値の指定と同等の結果になります。また変数を引数として動的に計算する関数としても振る舞うこともできます。



以下の例では、`expand_bytes`関数により、受信パケットのデータ列からローカル変数に値を格納しています。パラメータパックを用いた場合各引数の型を把握できるため、下記のように、受信パケットのバイト列から、サイズや異なる型にあった値を格納することができます。

```cpp
auto&& rx = the_twelite.receiver.read(); // 受信パケット

// 展開後のパケットの内容を格納する変数
// パケットのペイロードはバイト列で以下のように並んでいる。
//   [B0][B1][B2][B3][B4][B5][B6][B7][B8][B9][Ba][Bb]
//   <message       ><adc*  ><vcc*  ><timestamp*    >
//   * 数値型はビッグエンディアン並び
uint8_t msg[MSG_LEN];
uint16_t adcval, volt;
uint32_t timestamp;

// expand packet payload
expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, msg       // 4bytes of msg
		, adcval    // 2bytes, A1 value [0..1023]
	  , volt      // 2bytes, Module VCC[mV]
	  , timestamp // 4bytes of timestamp
);
```



### イテレータ

イテレータはポインタの抽象化で、例えばメモリの連続性のないようなデータ構造においても、あたかもポインタを使ったようにデータ構造にアクセスできる効果があります。

{% hint style="info" %}
C++のSTLでは、`begin()`メソッドで得られるコンテナの先頭を示すイテレータと、`end()`メソッドで得られるコンテナの末尾の「次」を示すイテレータの組み合わせが良く用いられます。

コンテナの末尾の「次」を`end()`としているのは、以下のような記述を想定しているためです。MWXライブラリでもこれに倣ってコンテナの実装を行っています。

```cpp
smplque<uint8_t, alloc_local<uint8_t, 5> > que;
que.push('a'); que.push('b'); que.pop(); que.push('c'); ...

auto&& p = que.begin();
auto&& e = que.end();

while(p != e) { // pがeまで進んだ＝全要素処理した
  Serial << *p;
  ++p; // イテレータのインクリメントは前置演算子を使います。
  　　　// この場合、p++ と記述すると、コンパイラによる最適化が行われる可能性は
       // 高いものの、コード上はイテレータのコピーが発生します。
}
```

イテレータを標準ライブラリの仕様に適合させることで、範囲for文が利用できたり、標準ライブラリのアルゴリズムを利用できるようになります。

```csharp
#include <algorithm>
#include <cctype>

// ラムダ式による文字変換
std::for_each(que.begin(), que.end(), 
  [](uint8_t& x) { x = std::toupper(x); });

// 範囲for文
for (uint8_t x : que) {
  Serial << x;
}
```

（MWXライブラリではC++標準ライブラリに対する適合度や互換性についての検証は行っていません。動作確認の上利用ください）
{% endhint %}



以下の例では、通常のポインタでは連続的なアクセスができないFIFOキューのイテレータ、さらに、FIFOキューの構造体の特定メンバー(例ではX軸)のみを抽出するイテレータを利用する例です。

```cpp
// XYZTの４軸構造体を要素とする要素数５のキュー
smplque<axis_xyzt, alloc_local<axis_xyzt, 5> > que;

// テスト用にデータを投入
que.push(axis_xyzt(1, 2, 3, 4));
que.push(axis_xyzt(5, 2, 3, 4));
...

// 構造体としてのイテレータを用いたアクセス
for (auto&& e : v) { Serial << int(e.x) << ','; }

// キューの中の X 軸を取り出す
auto&& vx = get_axis_x(que);
// X軸のイテレータを用いたアクセス
for (auto&& e : vx) { Serial << int(e) << ','; }

// int16_t要素のイテレータなので、STLのアルゴリズム（最大最小）が使える
auto&& minmax = std::minmax_element(vx.begin(), vx.end());
```



 以下は `smplque` クラスのイテレータの実装の抜粋です。このイテレータでは、キューオブジェクトの実体と、インデックスにより管理しています。キューのメモリが不連続になる（末尾の次は先頭を指す必要があるリングバッファ構造）部分は`smplque::operator []`で解決しています。オブジェクトのアドレスが一致することとインデックスが一致すればイテレータは同じものを指していることになります。

この実装部分には `<iterator>` が要求する typedef なども含まれ、より多くのSTLのアルゴリズムが適用できるようになります。

```cpp
class iter_smplque {
	typedef smplque<T, alloc, INTCTL> BODY;

private:
	uint16_t _pos; // index
	BODY* _body;   // point to original object

public: // for <iterator>
	typedef iter_smplque self_type;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef std::forward_iterator_tag iterator_category;
	typedef int difference_type;

public: // pick some methods
	inline reference operator *() {
		return (*_body)[_pos];
	}
	
	inline self_type& operator ++() {
		_pos++;
		return *this;
	}
};
```



構造体を格納したコンテナ中の、特定構造体メンバーだけアクセスするイテレータは少々煩雑です。構造体のメンバーにアクセスするメンバー関数を予め定義しておきます。このメンバー関数をパラメータ（`R& (T::*get)()`）としたテンプレートを定義します。`Iter`はコンテナクラスのイテレータ型です。

```cpp
struct axis_xyzt {
    int16_t x, y, z;
    uint16_t t;
    int16_t& get_x() { return x; }
    int16_t& get_y() { return y; }
    int16_t& get_z() { return z; }
};

template <class Iter, typename T, typename R, R& (T::*get)()>
class _iter_axis_xyzt {
    Iter _p;
    
public:
    inline self_type& operator ++() {
        _p++;
        return *this; }

    inline reference operator *() {
        return (*_p.*get)(); }
};

template <class Ixyz, class Cnt>
class _axis_xyzt_iter_gen {
    Cnt& _c;
    
public:
    _axis_xyzt_iter_gen(Cnt& c) : _c(c) {}
    Ixyz begin() { return Ixyz(_c.begin()); }
    Ixyz end() { return Ixyz(_c.end()); }
};

// 長いので using で短縮
template <typename T, int16_t& (axis_xyzt::*get)()>
using _axis_xyzt_axis_ret = _axis_xyzt_iter_gen<
    _iter_axis_xyzt<typename T::iterator, axis_xyzt, int16_t, get>, T>;

// X 軸を取り出すジェネレータ
template <typename T>
_axis_xyzt_axis_ret<T, &axis_xyzt::get_x>
get_axis_x(T& c) {
    return _axis_xyzt_axis_ret<T, &axis_xyzt::get_x>(c);
}
```

値にアクセスする`operator *`この上述のメンバー関数を呼び出しています。（`*_p`は`axis_xyzt`構造体で、`(*_p.*get)()`は、`T::*get`に`&axis_xyzt::get_x`を指定した場合`_p->get_x()`を呼び出します）

`_axis_xyzt_iter_gen`クラスは`begin(), end()`のみを実装し、上記のイテレータを生成します。これで範囲for文やアルゴリズムが利用できるようになります。

このクラス名は非常に長くなりソースコード中に記述するのは困難です。このクラスを生成するためのジェネレータ関数を用意します。下記の例では末尾の行の `get_axis_x()` です。このジェネレータ関数を用いることで冒頭のような`auto&& vx = get_axis_x(que);`といった簡潔な記述になります。

また、この軸だけを抽出するイテレータは、配列型の`smplbuf`クラスでも同様に利用できます。



### 割り込み・イベント・状態ハンドラの実装

ユーザ定義クラスによりアプリケーション動作を記述するため、代表的なハンドラは必須メソッドとして定義が必要ですが、それ以外に多数ある割り込みハンドラ、イベントハンドラ、ステートマシンの状態ハンドラをすべて定義するのは煩雑です。ユーザが定義したものだけ定義され、それのみのコードが実行されるのが理想です。

```cpp
class my_app_def {
public: // 必須メソッドの定義
	void network_event(twe::packet_ev_nwk& pEvNwk) {}
	void receive(twe::packet_rx& rx) {}
	void transmit_complete(twe::packet_ev_tx& pEvTx) {}
	void loop() {}
	void on_sleep(uint32_t& val) {}
	void warmboot(uint32_t& val) {}
	void wakeup(uint32_t& val) {}
	
public: // これらを必須記述とするのは煩雑
  // DIO割り込みハンドラ 20種類ある
  // DIOイベントハンドラ 20種類ある
  // タイマー割り込みハンドラ 5種類ある
  // タイマーイベントハンドラ 5種類ある
  // ...
}
```



MWXライブラリでは、数の多い DIO割り込みハンドラ（TWELITEハード上は単一の割り込みですが、利用しやすさのためDIO一つずつにハンドラを割り当てることにしました）などを、テンプレートによる空のハンドラーとして定義した上、ユーザ定義のメンバー関数をそのテンプレートの特殊化することにより定義する手法を採用しました。

```cpp
// hpp file
class my_app_def : class app_defs<my_app_def>, ... {
  // 空のハンドラ
  template<int N> void int_dio_handler(uint32_t arg, uint8_t& handled) { ; }

  ...   
  // 12番だけ実装する
  
public:
  // TWENET から呼び出されるコールバック関数
  uint8 cbTweNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap);
};

// cpp file
template <>
void my_app_def::int_dio_handler<12>(uint32_t arg, uint8_t& handled) {
  digitalWrite(5, LOW);
  handled = true;
  return;
}

void cbTweNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
  uint8_t b_handled = FALSE;
  switch(u32DeviceId) {
  	case E_AHI_DEVICE_SYSCTRL:
      if (u32ItemBitmap & (1UL << 0)){int_dio_handler<0>(0, b_handled);}
      if (u32ItemBitmap & (1UL << 1)){int_dio_handler<1>(1, b_handled);}
      ...
      if (u32ItemBitmap & (1UL << 12)){int_dio_handler<12>(12, b_handled);}
      ...
      if (u32ItemBitmap & (1UL << 19)){int_dio_handler<19>(19, b_handled);}
    break;
  }
}

```

実際のユーザ記述コードは、マクロ化やヘッダファイルのインクルードを行うことで、簡素化されていますが、上記は解説のために必要なコードを含めています。

TWENETからの割り込みハンドラから`my_app_def::cbTweNet_u8HwInt()` が呼び出されます。cppファイル中では、`int_dio_handler<12>`のみが特殊化されて記載された内容でインスタンス化されます。12番以外はhppファイル中のテンプレートからインスタンス化されます。結局以下のように展開されることになります。

```cpp
  	case E_AHI_DEVICE_SYSCTRL:
      if (u32ItemBitmap & (1UL << 0)){;}
      if (u32ItemBitmap & (1UL << 1)){;}
      ...
      if (u32ItemBitmap & (1UL << 12)){
          int_dio_handler<12>(12, b_handled);}
      ...
      if (u32ItemBitmap & (1UL << 19)){;}
      break;
      
    // ↓　↓　↓
    
    // 結局、このように最適化されることが期待できる。
   	case E_AHI_DEVICE_SYSCTRL:
      if (u32ItemBitmap & (1UL << 12)){
        // int_dio_handler<12> もinline展開
        digitalWrite(5, LOW);
        handled = true;
      }
      break;
    
```

最終的に、コンパイラの最適化により12番以外のコードは無意味と判断されコード中から消えてしまうことが期待できます（ただし、上記のように最適化されることを保証するものではありません）。

つまりユーザコード上では12番の割り込み時の振る舞いを定義したいときは`int_dio_handler<12>` を記述するだけで良い、ということになります（注：DIO割り込みを有効にするには `attachInterrupt()` を呼び出す必要があります）。登録しないハンドラはコンパイル時の最適化により低コストな呼び出しで済むことが期待できます。

{% hint style="info" %}
ユーザが関数を定義したときにこれを有効にし、定義しない場合は別の関数を呼び出す手法として、リンク時の解決方法があります。下記のように`__attribute__((weak))` の指定します。ユーザコードで `wakeup() `関数が定義された場合は、ユーザーコードを関数をリンクし、未定義の場合は中身が空の関数をリンクします。

```cpp
// mwx_appcore.cpp
void wakeup() __attribute__((weak));
void wakeup() { }
```

上記ハンドラの実装においてはweak指定したメンバー変数を明示的に生成する必要があり、またinline化による最適化が行いにくいため使用しませんが、`wakeup()`といったいくつかのTWENETからのコールバック関数の受け皿としてweak指定の関数を定義しています。
{% endhint %}



### Streamクラス

ストリームクラスは、主にUART(シリアルポート)の入出力に用います。MWXライブラリでは、出力用の手続きを主に定義しています。一部入力用の定義もあります。

ここでは派生クラスが必要とする実装について解説します。

```cpp
template <class D>
class stream {
protected:
	void* pvOutputContext; // TWE_tsFILE*
public:
  inline D* get_Derived() { return static_cast<D*>(this); }
	inline D& operator << (char c) {
		get_Derived()->write(c);
		return *get_Derived();
	}
};

class serial_jen : public mwx::stream<serial_jen> {
public:
 	inline size_t write(int n) {
		return (int)SERIAL_bTxChar(_serdef._u8Port, n);
	}
};
```

上記は1文字書き出す`write()`メソッドの実装です。親クラスの`stream<serial_jen>`からはキャストを実行する`get_Drived()`メソッドを用いて、`serial_jen::write()`メソッドにアクセスしています。

必要に応じて `write(), read(), flush(), available()` といったメソッドを定義します。



書式出力にはMarco Paland氏による[printfライブラリ](https://github.com/mpaland/printf)を利用しています。MWXライブラリから利用するための実装が必要になります。下記の例で派生クラスの`serial_jen`で必要なことは1バイト出力のための `vOutput()` メソッドを定義することと、`vOutput()`がstaticメソッドであるため出力のための補助情報を親クラスの`pvOutputContext`に保存することです。

```cpp
template <class D>
class stream {
protected:
	void* pvOutputContext; // TWE_tsFILE*
public:
	inline tfcOutput get_pfcOutout() { return get_Derived()->vOutput; }
	
	inline D& operator << (int i) {
		(size_t)fctprintf(get_pfcOutout(), pvOutputContext, "%d", i);
		return *get_Derived();
	}
};

class serial_jen : public mwx::stream<serial_jen> {
	using SUPER = mwx::stream<serial_jen>;
	TWE_tsFILE* _psSer; // シリアル出力のためのローレベル構造体
public:
  void begin() {
    SUPER::pvOutputContext = (void*)_psSer;
  }
  
	static void vOutput(char out, void* vp) {
		TWE_tsFILE* fp = (TWE_tsFILE*)vp;
		fp->fp_putc(out, fp);
	}
};
```

`get_pfcOutput()`により、派生クラスで定義した`vOutput()`関数を指定し、そのパラメータとして`pvOutputContext`が渡されます。上記の例では`<<`演算子がint型で呼び出されたとき`serial_jen::vOutput()`とUART用に設定済みの`TWE_tsFILE*`を`fctprintf()`関数に渡しています。



### Wire, SPIのワーカーオブジェクト

`Wire`クラスでは、2線デバイスとの送信・受信時に、通信開始から終了までを管理する必要があります。ワーカーオブジェクトを利用する記述について内容を記述します。

```cpp
if (auto&& wrt = Wire.get_writer(SHTC3_ADDRESS)) {
	Serial << "{I2C SHTC3 connected.";
	wrt << SHTC3_TRIG_H;
	wrt << SHTC3_TRIG_L;
	Serial << " end}";
}
```



`periph_twowire::writer` クラスの抜粋です。streamインタフェースを実装するために `mwx::stream<writer>` を継承しています。steamインタフェースを利用するために `write()` と `vOutput()`メソッドの実装を行っています。

コンストラクタでは2線シリアルの通信開始を、デストラクタで通信終了のメソッドを呼び出しています。また、`operator bool()`演算子では、2線シリアルのデバイスの通信開始に成功した場合 `true `を返すようになっています。

```cpp
class periph_twowire {
public:
	class writer : public mwx::stream<writer> {
		friend class mwx::stream<writer>;
		periph_twowire& _wire;
	
	public:
		writer(periph_twowire& ref, uint8_t devid) : _wire(ref) {
	  	_wire.beginTransmission(devid); // コンストラクタで通信開始
		}
	
		~writer() {
			_wire.endTransmission(); // デストラクタで通信終了
		}
	
		operator bool() {
			return (_wire._mode == periph_twowire::MODE_TX);
		}
	
	private: // stream interface
		inline size_t write(int n) {
			return _wire.write(val);
		}
	
		// for upper class use
		static void vOutput(char out, void* vp) {
			periph_twowire* p_wire = (periph_twowire*)vp;
			if (p_wire != nullptr) {
				p_wire->write(uint8_t(out));
			}
		}
	};
	
public:
	writer get_writer(uint8_t address) {
		return writer(*this, address);
	}
};
class periphe_twowire Wire; // global instance

// ユーザコード
if (auto&& wrt = Wire.get_writer(SHTC3_ADDRESS)) {
	Serial << "{I2C SHTC3 connected.";
	wrt << SHTC3_TRIG_H;
	wrt << SHTC3_TRIG_L;
	Serial << " end}";
}
```

`get_writer()`メソッドによりオブジェクト`wrt`を生成します。この時にオブジェクトのコピーは通常発生しません。C++コンパイラのRVO(Return Value Optimization)という最適化により、`writer`は`wrt`に直接生成されるためコピーは発生せず、コンストラクタで実行されているバスの初期化を多重に行ったりすることはありません。ただしRVOはC++の仕様では保証されておらず、念のためMWXライブラリ中ではコピー、代入演算子の削除、moveコンストラクタを定義しています（moveコンストラクタが評価される可能性はないと考えられますが）。

if節の中の `wrt`は、まずコンストラクタにより初期化され同時に通信開始します。通信開始でエラーがなければ、条件判定時のbool演算子が`true`を返し、if節スコープ内の処理が行われます。スコープを脱出するとデストラクタにより、2線シリアルバスの利用終了処理を行います。通信の相手先がない場合は `false`が戻り、`wrt`オブジェクトは破棄されます。

Wire, SPI特有の定義として`operator << (int)`の定義をオーバーライドしています。ストリームのデフォルトの振る舞いは、数値を文字列に変換して出力するのですが、WireやSPIで数値文字列をバスに書き込むことは稀で、反対に設定値など数値型のリテラルをそのまま入力したいことが多いのですが、数値型リテラルは多くの場合int型として評価されるため、この振舞を変更します。

```cpp
			writer& operator << (int v) {
				_wire.write(uint8_t(v & 0xFF));
				return *this;
			}
```

ここではint型の値については8bitに切り詰めて、その値を出力しています。
