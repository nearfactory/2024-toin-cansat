---
title: "用語"
author: "Mono Wireless Inc."
description: "用語"
---
# 用語

本資料で利用する用語について補足します。

{% hint style="warning" %}
用語の解説は、規格などで定められる定義に沿っていない場合があります。
{% endhint %}



## 一般的な用語

### SDK (TWELITE SDK, MWSDK)

ソフトウェア開発環境

{% hint style="info" %}
TWELITE無線マイコンのソフトウェア開発用のSDKをTWELITE SDK (またはMWSDK)と呼称します。
{% endhint %}



### IEEE802.15.4

TWELITE無線モジュールが利用する無線規格です。MWXライブラリを使用する限り、無線規格の詳細を意識する必要はありません。



### パケット

無線通信における最小の通信単位です。

最大量は通信方式や通信時の設定によって変わりますが、MWXライブラリ標準の通信\<NWK\_SIMPLE>では、ユーザが１パケットで送信できるデータ量は９０バイトです。



### ペイロード

「貨物」といった意味合いですが、無線パケットに含まれるデータ本体のことをいいます。



### ノード

「点・節」といった意味合いですが、無線ネットワーク内の無線局のことを言います。



## MWXライブラリ特有の用語

### アクト

本ライブラリを用いて作成したプログラム。そのソースコードまたは動作するプログラムのことを言います。



### [ビヘイビア](../api-reference/behavior/)

アクトの中でも特にイベント形式のプログラム。そのソースコードまたは動作するプログラムのことを言います。

ビヘイビアは１つのクラス定義による記述で、TWENETからのコールバック関数やイベントや割り込み処理を記述しひとまとめにしています。MWXライブラリでは以下の３種類のビヘイビアがあります。

* アプリケーションビヘイビア：イベントドリブンでのアプリケーション記述を行い、ユーザが定義するクラス。
* ボードビヘイビア：TWELITE無線モジュールを実装するボードの機能利用を簡素化するためのクラス。
* ネットワークビヘイビア：無線ネットワークの手続きを簡素化するためのクラス。

ビヘイビア名は `< >` で括って表記します。例えばシンプル中継ネットワークのビヘイビア名は `<NWK_SIMPLE>` です。



### クラスオブジェクト

本ライブラリの解説では、ライブラリで最初からグローバル宣言されたオブジェクトをクラスオブジェクトと呼称します。`Serial`, `Wire` などです。これらクラスオブジェクトは手続きなしまたは開始手続きを行うことで利用できます。

メモリを比較的多く消費するクラスオブジェクトは、初期化手続きの際(`.setup()`または`.begin()`メソッド)に初期化パラメータに沿ったメモリを確保します。



## C++に関する用語

{% hint style="info" %}
一般の用語です。C言語の知識を前提に解説します。
{% endhint %}



### C++

C++言語のこと。

{% hint style="info" %}
MWXライブラリはC++とC言語によって記述されています。
{% endhint %}



### C++11

C++規格のバージョンの一つ。2011年式のC++といった意味合いで、2011年にISOで規格化されています。直前のC++03から大きく機能拡張されています。C++14, C++17といったより新しいバージョンがあります。

{% hint style="info" %}
MWXライブラリはC++11で追加された機能や構文を用いて実装されています。[MWSDKのコンパイラ対応](design\_policy.md#c-yan-yu-gui-ge)はC++11までとなります。
{% endhint %}



### クラス

あるデータに注目して、その手続きをひとまとめにしたもの。構造体に、その構造体を取り扱うための手続きが含まれています。実際にはもっと深い話題に発展しますが、専門書を参考にしてください。

C++においては、キーワードの `struct`と `class`は本質的には同じもので、いずれのキーワードで宣言してもクラスとなります。

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); }
};
```

上記のクラス定義をC言語でも行った場合、例えば以下のようになります。

```cpp
typedef struct _c_myhello {
  int _i;
  void (*pf_say_hello)(struct _c_myhello *);
} c_myhello;

void say_hello(c_myhello*p) { p->pf_say_hello(); }
void init_c_my_hello(c_myhello*p) {
  p->pf_say_hello = say_hello;
}
```



### ラッパークラス

既存のC言語のライブラリやその内部の構造体などをクラスに包含し、C++特有の機能性を追加するなどして、利用の便を図ったものです。解説中でも「～構造体をラップした」といった記述をする場合があります。

{% hint style="info" %}
MWXライブラリは、TWENETのCライブラリのラッパークラスと、新たに実装したクラス群との組み合わせになっています。
{% endhint %}



### メソッド・メンバー関数

クラスに定義される関数で、クラスに紐付いています。

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); } //メソッド
};
```



### オブジェクト（インスタンス）

クラスを実体化（メモリ確保）したもの。

```cpp
void func() {
    myhello obj_hello; // obj_helloがmyhelloクラスのオブジェクト
    obj_hello._i = 10;
    obj_hello.say_hello();
}
```

本解説ではオブジェクトとインスタンスは同じ意味合いとして取り扱っています。



### コンストラクタ

オブジェクト生成時の初期化手続き。

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); }
  
  myhello(int i = 0) : _i(i) {} // コンストラクタ
};

void my_main() {
  myhello helo(10); // ここでコンストラクタが呼び出され_i=10にセットされる
}
```



### デストラクタ

コンストラクタと対になってオブジェクトが破棄されるときの手続きです。

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello! %d\n", _i); }
  
  myhello(int i = 0) : _i(i) {} // コンストラクタ
  ~myhello() {
    printf("good bye! %d\n", _i);
  } //デストラクタ
};
```



### 抽象クラス

C++では仮想クラスによりポリモーフィズム（多態性）を実現します。具体的には`virtual`キーワードで指定た純粋仮想関数を定義したクラスです。

```cpp
struct Base {
  virtual void say_hello() = 0;
};

struct DeriveEng : public Base {
  void say_hello() { printf("Hello!"); }
};

struct DeriveJpn : public Base {
  void say_hello() { printf("Kontiwa!"); }
};
```

{% hint style="warning" %}
MWXライブラリでは、コンパイラの制限や性能上の理由で仮想関数を使用しません。ポリモーフィズムを実現するのに[別の手法](design\_policy.md#crtpnishitatenpurtopatn)を用いています。
{% endhint %}



### スコープ

C/C++言語では `{ }` で括った範囲と考えてください。この中で生成したオブジェクトは、スコープから出るときに破棄されます。この時デストラクタが呼び出されます。

以下は、明示的にスコープを設定したものです。`helo2`は8行目まで実行された時点で破棄され、デストラクタが呼び出されます。

```cpp
void my_main() {
  myhello helo1(1);
  helo1.say_hello();
  
  {
    myhello helo2(2);
    helo2.say_hello();
  }
}

// hello! 1
// hello! 2
// good bye! 2
// good bye! 1
```



MWXライブラリでは以下のような記法を用いています。ここではif文の条件判定式内で宣言（C89といった旧いC言語ではこういった場所での宣言はできません）されたオブジェクトの有効期間は、if文の`{}`内になります。

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello! %d\n", _i); }
  operator bool() { return true; } // if()での判定用の演算子
  
  myhello(int i = 0) : _i(i) {} // コンストラクタ
  ~myhello() { printf("good bye! %d\n", _i); } // コンストラクタ
};

// myhello オブジェクトを生成する関数 (ジェネレータ)
myhello gen_greeting() { return my_hello(); }

void my_main() {
  if (myhello x = gen_greeting()) {
    // myhelloのオブジェクト x は if文中有効
    x.say_hello();
  }
  // if 分を抜けるときにオブジェクトxは破棄される
}
```

例えば二線シリアルバスなど、開始と終了の手続きがあって、その間だけオブジェクトによってバスを操作するような手続きです。オブジェクトの生成後、バスの接続が適切であればif文のtrue節が実行され、生成したオブジェクトによってバスの書き込みまたは読み出しを行います。バスの読み書き操作が終了したらif文を脱出し、この時デストラクタが呼び出され、バスの利用終了手続きが行われます。

```cpp
const uint8_t DEV_ADDR = 0x70;
if (auto&& wrt = Wire.get_writer(DEV_ADDR)) { //バスの初期化、接続判定
	wrt(SHTC3_TRIG_H); // 書き出し
	wrt(SHTC3_TRIG_L);
} // バスの利用終了手続き
```



### 名前空間 (namespace)

定義名の重複を避けるためC++では名前空間が積極的に用いられます。名前空間にある定義にアクセスするには`::`を用います。

```cpp
namespace MY_NAME { // 名前空間の宣言
  const uint8_t MYVAL1 = 0x00;
}

...
void my_main() {
  uint8_t i = MY_NAME::MYVAL1; // MY_NAME の参照
}

```



### テンプレート (template)

テンプレートはC言語のマクロを拡張したものと考えてください。

```cpp
template <typename T, int N>
class myary {
  T _buf[N];
public:
  myary() : _buf{} {}
  T operator [] (int i) { return _buf[i % N]; }
};

myary<int, 10> a1; // int 型で要素数10の配列
myary<char, 128> a2; // char 型の要素数128の配列
```

この例では、簡単な配列を定義しています。`T`と`N`はテンプレートのパラメータで、`T`は型名を`N`は数値を指定し、`T`型で要素数`N`の配列クラスを定義しています。



### nullptr

C++11ではNULLポインタを`nullptr`と記述するようになりました。



### 参照型

C++では、参照型を利用できます。これはポインタによるアクセスに似ていますが、必ずオブジェクトを参照しなければならないという制約があります。

以下のような参照渡しのパラメータを持つ関数では`i`の値を`incr()`内で書き換えることが出来ます。

```cpp
void incr(int& lhs, int rhs) { lhs += rhs; }

void my_main() {
  int i = 10; j = 20;
  incr(i, j);
}
```



テンプレートの解説例ですが`operator[]`の戻り型を`T&`に変更しています。こうすることで`a[0]=1`のように配列内部のデータに対して直接代入操作ができるようになります。

```cpp
template <typename T, int N>
class myary {
  T _buf[N];
public:
  myary() : _buf{} {}
  T& operator [] (int i) { return _buf[i % N]; }
};

myary<int, 10> a1;
void my_main() {
  a1[0] = 1;
  a1[1] = 2;
}
```



{% hint style="info" %}
MWXライブラリのプログラミングインタフェースは、原則といてポインタ型の利用をせず、参照型を用いています。
{% endhint %}



### 型推論

C++11 では型推論の`auto`キーワードが導入されています。これはコンパイラが初期化の記述からそのオブジェクトの型を推論するため、具体的な型名の記述を省略できます。これはtemplateを用いたクラス名が非常に長くなるような場合に効果的です。

解説中では多くの場合ユニバーサル参照と呼ばれる`auto&&`を用いています。ユニバーサル参照については、ここでは参照渡しの場合も意識せずに記述できるものと考えてください。

```cpp
auto&& p = std::make_pair("HELLO", 5);
       // const char* と int のペア std::pair
```



### コンテナ

配列など特定のデータ型のオブジェクトを複数個格納するためのクラスをコンテナと呼びます。テンプレートの例で挙げた`myary`のような配列クラスもコンテナと呼んでいます。

{% hint style="info" %}
MWXライブラリでは、配列クラス`smplbuf`とFIFOキュークラス`smplque`を用意しています。
{% endhint %}



### イテレータ, .begin(), .end()

C言語で言うところのポインタ（もちろんC++でも同じようにポインタは使えます）を拡張した概念です。C言語のポインタは、メモリが連続した要素を先頭から末尾まで連続的にアクセスする手段と考えることが出来ます。FIFOキューを考えてみます。もっとも単純なキューの実装はリングバッファによるものですが、メモリーの連続性はありません。こういったデータ構造であっても、イテレータを用いるとポインタと同じように記述できます。

イテレータを取得するため`.begin()`,`.end()` のメソッドが用いられます。コンテナの**先頭**を指すイテレータを`.begin()`で取得します。**末尾の次**を指すイテレータを`.end()`で取得します。末尾ではなく、末尾の次である理由にはforやwhile文のループ記述の明快さ、コンテナに格納される要素数が0の場合の取り扱いが挙げられます。

```cpp
my_queue que; // my_queue はキューのクラス

auto&& p = que.begin();
auto&& e = que.end();

while(p != e) {
  some_process(*p);
  ++p;
}

```

上記では、`que`の各要素について、イテレータ`p`を用いて各要素に`some_process()`を適用しています。`p`は`++`演算子によって次の要素を指すイテレータとしてインクリメントしています。本来ポインタでは記述できないデータ構造を持つコンテナであっても、このようにポインタを用いた処理と同じような処理が出来ます。

`.end()`が末尾の次を示すため、while文の終了判定は`(p != e)`のように簡潔です。キューに要素がない場合は`.begin()`は`.end()`と同じイテレータを返します。（何も格納されていない要素のイテレータの次ですから、最初に格納すべき領域を示すイテレータと考えればよいでしょう）

メモリ上で連続したコンテナの場合、通常、そのイテレータは通常のポインタとなります。その操作時に大きなオーバーヘッドにはならないことが期待できます。



### C++標準ライブラリ

C++の標準ライブラリにはSTL(Standard Template Library)が含まれます。MWXライブラリでの一部を利用しています。

{% hint style="warning" %}
TWELITE向けのC/C++コンパイラの[制約](./#zhi-xian-shi-xiang)から、利用できる機能はごく一部です。
{% endhint %}



### アルゴリズム

例えば最大や最小値を求めるといった処理をC言語では型に応じて別々に記述していました。こういったコードは型の部分だけ違って他は同じといったものも少なくありません。C++ではtemplateやイテレータなどを用いて、こういった処理を型に依存せず記述することができます。これをアルゴリズムと呼んでいます。

```cpp
// 任意のイテレータをパラメータとし最大値を持つイテレータを戻す
template <class Iter>
Iter find_max(Iter b, Iter e) {
  Iter m = b; ++b;
  while(b != e) {
    if (*b > *m) { m = b; }
    ++b;
  }
  return m;
}
```

例えば上記のように最大値を求めるアルゴリズムです。このアルゴリズムは型に依存しません。(ジェネリックプログラミングと呼ばれます）



```cpp
#include <algorithm>

auto&& minmax = std::minmax_element( // 最大最小を得るアルゴリズム
  que.begin(), que.end());

auto&& min_val = *minmax.first;
auto&& max_val = *minmax.second;
```

ここでは`que`のイテレータを指定し、その最大と最小を得るアルゴリズム`std::minmax_elenet`を適用しています。`std::minmax_elemet`はC++標準ライブラリ内に定義されています。その戻り値は任意の２つの値を組み合わせる`std::pair`です。このアルゴリズムは、イテレータの示す要素同士で`<,>,==`といった演算子での比較が出来れば最大と最小を計算してくれます。戻り型もイテレータの型から導かれます。



