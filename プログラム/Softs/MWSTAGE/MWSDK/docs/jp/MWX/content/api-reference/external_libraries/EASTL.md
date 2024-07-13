---
title: EASTL
author: Mono Wireless Inc.
description: EASTL
---

# EASTL

[EASTL](https://github.com/electronicarts/EASTL) は Electronic Arts 社が整備した標準テンプレートライブラリ(コンテナ・アルゴリズム)で、C++ の STL (Standard Template Library) に倣い実装されていますが、制約の多いゲーム機の開発で整備されてきた経緯があり、メモリの取り扱いに制約が大きい環境を意識したコンテナやアルゴリズムが用意されています。

本ライブラリは EASTL を TWENET 内で利用できるようにしています。

以下はバッファ最大長が固定の配列とソートアルゴリズムの適用です。

以下にEASTLの特徴を記載します。

* メモリ固定確保のコンテナ (`fixed_`) : 動的確保を行わず、固定長の要素数を持つコンテナの宣言が可能です。グローバル宣言すれば、コンパイル時に固定的にメモリ領域が確保され、ローカル宣言すればスタックエリアに確保されそのスコープ内で利用できます。
* Intrusive コンテナ：通常のコンテナは任意のデータ構造を格納できますが、Intrusiveコンテナはデータ構造に対して専用の基底クラスを継承することで、コンテナ内のリンク構造などを維持するためのリンク情報などを保持します。コンテナ内の各要素はそのコンテナ専用になりますが、リストやマップ構造では非常にメモリ利用効率の良くなります。（参考: [Intrusive and non-intrusive containers](https://www.boost.org/doc/libs/1\_35\_0/doc/html/intrusive/intrusive\_vs\_nontrusive.html)）

2007年の記事[EASTL (open-std.org)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html)に開発動機などが記されています。(関連記事：[EASTL から垣間見るゲームソフトウェア開発現場の現状 その 1](https://i-saint.hatenablog.com/entry/20101012/1286822888), [その 2](https://i-saint.hatenablog.com/entry/20101026/1288028957))

## TWENETでの利用

以下に留意してください。

**当社ではライブラリの動作については包括的な検証は行っておりません。お客様での動作検証をお願いいたします。また EASTL の利用方法についてのお問い合わせについても当社では対応できません。配布元の開設資料・ライブラリソースコードなどの情報を参照してください。**

* [EASTL3.07](https://github.com/electronicarts/EASTL/releases/tag/3.07.00) (2018/1/31) のバージョンを利用します。（C++11でコンパイルできる一番最後のバージョン）
* 以下のライブラリは組み込んでいません。
  * `test/packages/EAAssert`, `source/assert.cpp`
  * `test/packages/EATest`
  * `test/packages/EAThread`, `source/thread_support.cpp`
* テストコード `test/source` の動作移植はしていません。
* _sprintf_関連では`EA::StdC::Vsnprintf(char8_t*, ...)` のみを printf.h ライブラリ中の `vsnprintf_()` を呼び出すことで解決しています。

### 組み込み・コンパイル方法

EASTL はアクト Act の記述の際に利用できます。

TWELITE 向けの開発環境で必要なインクルードパスの追加、ライブラリの追加は行います。作成するコード中でライブラリヘッダのインクルードを行ってください。

```
#include <TWELITE>
#include <EASTL/fixed_string.h>

using namespace eastl;
using tstr128 = fixed_string<char, 127 + 1, false>;

void setup() {
    tstr128 s1;
    s1 = "Hello World";
    Serial << s1.c_str();
}
void loop() {
    ;
}
```

### 組み込み方法(詳細)

ライブラリのコンパイルやインクルードパスの設定は、MWSDK/TWENET 以下のディレクトリで実施済みですが、内部的な設定を以下に記載します。

* EASTL/source 内のコードをコンパイルして、ライブラリアーカイブとしておく (`libEASTL.a`)。リンク時にはこのライブラリの参照が必須です。
* コンパイル時に以下のインクルードパスを追加しておく。

`$(PATH_EASTL)`を EASTL ディレクトリとした場合、インクルードパスは以下となります。

```bash
-I$(PATH_EASTL)/include
-I$(PATH_EASTL)/test/packages/EAAssert/include
-I$(PATH_EASTL)/test/packages/EABase/include/Common
-I$(PATH_EASTL)/test/packages/EAMain/include
-I$(PATH_EASTL)/test/packages/EAStdC/include
-I$(PATH_EASTL)/test/packages/EATest/include
-I$(PATH_EASTL)/test/packages/EAThread/include
```

## コーディングについて

### `std::` と `eastl::` について

MWXライブ内部でも `std::` 名前空間の標準ライブラリを利用しています。

標準ライブラリ (`std::`) と EASTL(`eastl::`)では同じ名前で同じ機能を持つものが定義されています。これらは混在できる場合もありますが、使用するとエラーになる場合もあります。つまりEASTLで使用するものは、通常はEASTL内の定義を用います（例：`std::unique_ptr` に `eastl::fixed_string` を格納しようとするとコンパイラエラーになります）。

また`using namespace std;`といったような記述を行う場合は、名前の衝突に注意してください。

### グローバルオブジェクトの初期化1 (配置new)

TWENET の開発では、コンパイラの制約により、グローバル宣言のオブジェクトのコンストラクタが実行されません。グローバル宣言宣言したオブジェクトのメモリ領域がゼロクリアされるだけです。そのまま、コードを実行すると大抵の場合 null pointer access によりハングアップします。

このオブジェクトを初期化するためには\*placement new (配置new)\*を用います。

```
#include <TWELITE>
#include <EASTL/fixed_string.h>

using namespace eastl;
using tstr128 = fixed_string<char, 127 + 1, false>;

tstr128 g_str1; // constructor is NOT called! needs to be initialized before use.

void setup() {
    (void) new ((void*)&g_str1) tstr128("Hello World");
    Serial << g_str1.c_str();
}
```

_placement new_ のコードは少し乱雑に見えるため、補助関数`mwx::pnew()` を用意しています。先ほどの例を以下のように書き換えることができます。

```
(void) new ((void*)&g_str1) tstr128("Hello World");
// ↓
mwx::pnew(g_str1, "Hello World");
```

※ ２番目の引数以降は可変数で、コンストラクタにそのまま渡されます。

### グローバルオブジェクトの初期化2 (unique\_ptr)

グローバルオブジェクトの初期化方法として `unique_ptr`([std::unique\_ptrの解説](https://cpprefjp.github.io/reference/memory/unique\_ptr.html))を用いる方法もあります。`unique_ptr`は `std::` にも `eastl::` にもありますが、EASTLのクラスでは`eastl::`のものを使用します。

以下のように初期化のタイミングで `.reset()` を呼び出します。

```
#include <TWELITE>
#include <EASTL/unique_ptr.h>
#include <EASTL/fixed_string.h>

using namespace eastl;
using tstr128 = fixed_string<char, 127 + 1, false>;

eastl::unique_ptr<tstr128> uq_str1;

void setup() {
	uq_str1.reset(new tstr128("Hello World"));
    if (uq_str1) { // true: object is stored.
        Serial << uq_str1->c_str();
    }
}
```

### intrusive コンテナについて

以下の例は `intrusive_list` の要素定義例です。メンバーは `int mX` のみです。

```
struct IntNode : public eastl::intrusive_list_node { 
    int mX;
    IntNode(int x = 0) : mX(x) { }
        // no need to call super class's constructor eastl::intrusive_list_node()
};

inline bool operator<(const IntNode& a, const IntNode& b) { return a.mX < b.mX; }
inline bool operator>(const IntNode& a, const IntNode& b) { return a.mX > b.mX; }
```

`intrusive_list`の要素は、必ず `intrusive_list_node` を基底クラスに持っている必要があります。基底クラス内にはリストを維持するためのリンクポインタが含まれます。ここではさらに `sort`などで使用する比較演算子の定義も行います。

```
using tiList = intrusive_list<IntNode>;

void setup() {
    IntNode nodeA(5);
    IntNode nodeB(1);
    IntNode nodeC(9);
    IntNode nodeD(2);
    IntNode nodeE(4);

    tiList l; // intrusive_list body
    
    l.push_front(nodeA); // forming list strucure 
                         //   by updating link info in intrusive_list_node.
    l.push_front(nodeB);
    l.push_front(nodeC);
    l.push_front(nodeD);
    l.push_front(nodeE);
    
    l.sort(); // sort, using < operator
    l.sort(eastl::greater<tilist::value_type>()); // sort, using > operator
}
```

## 参考資料

* [EA Standard Template Library](https://eastl.docsforge.com/) ーTWENET に収録されているライブラリは EASTL 3.07 用で、それ以降に実装・変更された要素が含む点に注意してください）
* [cpprefjp - C++日本語リファレンス](https://cpprefjp.github.io/) ーC++ リファレンスですが STL の解説が参考になります。

## 本サンプルについて

EASTLのライセンス記述は以下です。

_Modified BSD License (3-Clause BSD license) see the file LICENSE in the project root._

```
/*
Copyright (C) 2015 Electronic Arts Inc.  All rights reserved.
  
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
  
1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
```

サンプルコードは MWSLA-1J/E を適用します。

## コード例

### fixed\_vector

最大長が固定された（つまり拡張しない）配列。 （※ `mwx::smplbuf`も同様に最大長固定の配列ですが、MWXライブラリの内部処理に一部特化しています)

```cpp
#include <TWELITE>
#include <EASTL/fixed_vector.h>
#include <EASTL/sort.h>

using tvct = eastl::fixed_vector<uint16_t, 64, false>;
tvct v;

void setup() {
    mwx::pnew(v); // initialize
    v = { 3, 1, 2 ,4 }; // set initial list.

    // push and pop
    v.pop_back();   // 3, 1, 2
    v.push_back(5); // 3, 1, 2, 5
    
    // sort
    eastl::sort(v.begin(), v.end(), eastl::less<tvct::value_type>());
    				// 1, 2, 3, 5
    
    // disp all
    for (auto x : v) { Serial << format(" %d", x); }
    Serial << crlf;
    // using [] operator
    for (int i = 0; i < v.size(); i++) { Serial << format(" %d", v[i]); }
}
```

`fixed_vector` のテンプレート引数は３つあり、１番目が型、２番目が最大数、３番目は_false_にします。 配列の操作については、一般の `std::vector` と類似した `.puch_back()` や `.pop_back()`、`[]演算子`などが利用できます。

また、ソートアルゴリズムなどの適用も可能です。上記の例では `eastl::sort` を昇順 `eastl::less` にて行っています。

### fixed\_list

最大エレメント数が固定されたリスト構造(intrusive\_list についても要参照)。

```cpp
#include <TWELITE>
#include <EASTL/fixed_list.h>
#include <EASTL/sort.h>

using tdata = eastl::pair<uint8_t, void (*)(uint8_t)>; // element data type
using tlst = eastl::fixed_list<tdata, 3, false>; // fixed_list with 3 elements.
tlst l; // list object

void setup() {
    mwx::pnew(l); // initialize (call constructor)
    // add
    if (!l.full()) l.insert(l.begin(), eastl::make_pair('A', [](uint8_t v){ Serial << format("(1:%c)", v); } ));
    if (!l.full()) l.insert(l.begin(), eastl::make_pair('B', [](uint8_t v){ Serial << format("(2:%c)", v); } ));
    if (!l.full()) l.insert(l.begin(), eastl::make_pair('C', [](uint8_t v){ Serial << format("(3:%c)", v); } ));
    if (!l.full()) l.insert(l.begin(), eastl::make_pair('D', [](uint8_t v){ Serial << format("(4:%c)", v); } )); // fails
    Serial << crlf << "init: "; for(auto &x: l) x.second(x.first);
    // find & erase
    auto p = eastl::find_if(l.begin(), l.end(), [](tdata& x) { return (x.first == 'B'); } );
    if (p != l.end()) l.erase(p);
    Serial << crlf << "find&erase: "; for(auto &x: l) x.second(x.first);
    // append
    if (!l.full()) l.insert(l.end(), eastl::make_pair('D', [](uint8_t v){ Serial << format("(4:%c)", v); } ));
    Serial << crlf << "append: "; for(auto &x: l) x.second(x.first);
    // sort
    eastl::sort(l.begin(), l.end(), eastl::less<tlst::value_type>());
    Serial << crlf << "sort:"; for(auto &x: l) x.second(x.first);
    // sort reverse
    eastl::sort(l.begin(), l.end(), eastl::greater<tlst::value_type>());
    Serial << crlf << "sort(rev):"; for(auto &x: l) x.second(x.first);
}
```

`fixed_list` のテンプレート引数は３つあり、１番目が型、２番目が最大数、３番目は_false_にします。リストの操作については、一般の `std::list` に類似した `.insert()`, `.erase()` などが利用できます。

上記のコードでは、リストにはペア `eastl::pair` の要素を格納し、ペアの first を `uint8_t` 型の整数、second を `void (*)(uint8_t)` の関数ポインタとしています。コード中はラムダ式を直接記述しています。コード中の `x.second(x.first);` は second から得られる関数に対して first の値を与えるという意味合いになります。

このリストに対して `eastl::find_if` による要素の検索を行ったり、`bubble_sort`によるソートが可能です。

### intrusive\_list

通常のリストは任意のデータ構造を要素とできますが、intrusive\_list は要素に特定のデータを付与し、そのデータを用いることでデータ構造を構築します。

以下の例では、intruslve\_list データ構造の要素となるには `eastl::intrusive_list_node` を継承したデータ要素型であることを要件とします。`eastl::intrusive_list_node`には前後の要素に対するポインタを格納できるような拡張です。

```cpp
#include <TWELITE>
#include <EASTL/fixed_vector.h>
#include <EASTL/intrusive_list.h>
#include <EASTL/unique_ptr.h>

// list element of intrusive_list.
struct IntNode : public eastl::intrusive_list_node { 
    int mX;
    IntNode(int x = 0) : mX(x) { }
};
inline bool operator>(const IntNode& a, const IntNode& b) { return a.mX > b.mX; } // for sort

using tpool = eastl::fixed_vector<IntNode, 16, false>;
using tlst = eastl::intrusive_list<IntNode>;

tpool pool; // instance pool.
tlst l; // list object

void setup() {
    mwx::pnew(pool); // prepare isntances
    mwx::pnew(l); // initialize (call constructor)

    pool.resize(5); // create 4 instances into pool

    // insert an IntNode element into List.
    int i = 0;
    pool[i].mX = 5; l.push_front(pool[i]); i++;
    pool[i].mX = 1; l.push_front(pool[i]); i++;
    pool[i].mX = 2; l.push_front(pool[i]); i++;
    pool[i].mX = 4; l.push_front(pool[i]); i++;
    pool[i].mX = 3; l.push_front(pool[i]); i++;
    Serial << crlf << "init: ";
    for(auto& x : l) { Serial << format(" %d", x.mX); }

    l.remove(pool[2]);
    Serial << crlf << "remove: ";
    for(auto& x : l) { Serial << format(" %d", x.mX); }

    l.sort(eastl::greater<tlst::value_type>());
    Serial << crlf << "sort: ";
    for(auto& x : l) { Serial << format(" %d", x.mX); }
}
```

この例で `eastl::fixed_vector<>` が使用されているのは、`IntNode` の要素を必要数確保する目的で、`fixed_vector`が必要だったわけではありません。５つの要素にテストの値を格納して intrusive\_list を構築します。例では `l.push_pront()` を呼び出し要素をひとつずつリストに格納しています。実際は格納するのではなく、各要素 `IntNode` が持つポインタの繋ぎ変えです。

ソートの記述は `l.sort()` のようにメンバ関数の呼び出しで行います。

### ring\_buffer

リングバッファ `ring_buffer` は、他のコンテナ（例では `fixed_vector`） との組み合わせで構築されます。

```cpp
#include <TWELITE>
#include <EASTL/fixed_vector.h>
#include <EASTL/bonus/ring_buffer.h>

const size_t N_RING_ELE = 4; // element max for RING BUFFER.
using tvec = eastl::fixed_vector<uint8_t, N_RING_ELE + 1, false>; // One extra element is required.
using tring = eastl::ring_buffer<uint8_t, tvec>;
tring rb;

void setup() {
    mwx::pnew(rb, N_RING_ELE);

    rb.push_front(5);
    rb.push_front(1);
    rb.push_front(4);
    rb.push_front(2);
    Serial << crlf; for (auto x : rb) Serial << format(" %d", x);
    rb.push_front(3);
    Serial << crlf; for (auto x : rb) Serial << format(" %d", x);
    rb.push_front(8);
    Serial << crlf; for (auto x : rb) Serial << format(" %d", x);
    rb.push_front(9);
    Serial << crlf; for (auto x : rb) Serial << format(" %d", x);

    Serial << crlf << format("back=%d", rb.back()) << crlf;
    rb.pop_back();
    for (auto x : rb) Serial << format(" %d", x);
}
```

`ring_buffer` の定義は、要素型とそのコンテナ型の組み合わせです。要素型は余分に一つ要素を持たせておきます。

上記の例では `.push_front()` で先頭に要素を挿入します。オーバーフローすると末尾は消えてしまいます。 `.back()`により一番古い要素を取り出します。`pop_back()`で一番古い要素を削除します。

### intrusive\_hash\_map

マップ構造は、キーと値をもつデータ構造で、キー値により効率よく要素を抽出できるように設計されたデータ構造です。 intrusive\_hash\_map は intrusive 構造とハッシュ値を用いて実装しています。いくばくか定義は煩雑ですが、メモリ消費量は抑制できます。

intrusive\_list と同様独自の要素型 `IntNode` を `eastl::intrusive_hash_node_key<要素型>` を継承した形で定義する必要があります。またハッシュを用いるためハッシュ最大値 (`N_BUCKET_CT`) を定めておく必要があります。この値は、想定される格納要素数に応じて適切に素数の値を設定します。

```cpp
#include <TWELITE>
#include <EASTL/internal/intrusive_hashtable.h>
#include <EASTL/intrusive_hash_map.h>

static const unsigned N_BUCKET_CT = 7;

// intrusive element type
struct IntNode : public eastl::intrusive_hash_node_key<uint8_t> {
    using SUP = intrusive_hash_node_key;
    void (*m_func)(); // member variable is func pointer.
    IntNode(uint8_t key = 0) { SUP::mKey = key; } // key will be passed by the constructor.
};

// intrusive map type
using tmap = eastl::intrusive_hash_map<uint8_t, IntNode, N_BUCKET_CT>;

tmap mp;
IntNode nd_a, nd_b, nd_c, nd_d;

void setup() {
    mwx::pnew(mp); // initialize (call constructor)

    mwx::pnew(nd_a, 'A')->m_func = []() { Serial << "FuncA"; };
    mwx::pnew(nd_b, 'B')->m_func = []() { Serial << "FuncB"; };
    mwx::pnew(nd_c, 'C')->m_func = []() { Serial << "FuncC"; };
    mwx::pnew(nd_d, 'D')->m_func = []() { Serial << "FuncD"; };

    mp.insert(nd_a);
    mp.insert(nd_b);
    mp.insert(nd_c);
    mp.insert(nd_d);
}

void loop() {
    int c = Serial.read();
    if(c != -1) {
        Serial << crlf << '[' << uint8_t(c) << ']';
        auto&& it = mp.find(uint8_t(c));
        if (it != mp.end()) it->m_func();
    }
}
```

上記の例は、マップのキーを `uint8_t` 型の１文字とし、マップの値部分を関数ポインタとします。`loop()`ではキーの入力に応じた関数を実行するといった処理を行います。

最初に、グローバルオブジェクトとしてテーブルと要素を定義したため、`setup()`中で `mwx::pnew()` を呼び出すことでデータ要素(`nd_a, nd_b, nd_c, nd_d`)の初期化、ハッシュマップの初期化 (`mp`) を行っておきます。`mwx::pnew()` の戻り値は、構築したオブジェクトへのポインタですので、初期化後直接メンバ変数に値（ラムダ式）を書き込んでいます。

要素(`nd_a, nd_b, nd_c, nd_d`)の初期と値の設定が終わったら `mp.insert(nd_a)` のようにマップに要素を挿入していきます。

`loop()`ではシリアルから文字を入力するたびに、ハッシュマップの検索を行います。検索は `mp.find()` メソッドを呼び出し、戻り値はイテレータで、検索失敗時は `mp.end()` が戻ります。検索が成功したら `(*it)` により検索出来た要素を参照できます。

{% hint style="info" %}
`intrusive_hash_multimap` は値の重複を許容するマルチマップです。利用方法はハッシュマップとほぼ同じですが、検索時に `.equal_range()` 用い、イテレータのペアとして取り扱う点が違います。

```cpp
using tmap = eastl::intrusive_hash_multimap<uint8_t, IntNode, N_BUCKET_CT>;
...
    // find emelents by key `c'
    auto ip = mp.equal_range(uint8_t(c));
    for(auto&& it = ip.first; it != ip.second; it++) {
        it->m_func();
    }
```
{% endhint %}
