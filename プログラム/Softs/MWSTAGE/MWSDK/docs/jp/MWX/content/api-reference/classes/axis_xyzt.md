---
title: "axis_xyzt"
author: "Mono Wireless Inc."
description: "axis_xyzt"
---

# axis\_xyzt

３軸の加速度センサーの値を格納するための構造体ですが、コンテナクラスに格納したときの利便性を上げるための手続きを追加しています。

```cpp
struct axis_xyzt {
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t t;
};
```



### get\_axis\_{x,y,z}\_iter()

```cpp
/*戻り型は長いテンプレート型名なのでauto&&と記載します*/
auto&& get_axis_x_iter(Iter p)
auto&& get_axis_y_iter(Iter p)
auto&& get_axis_z_iter(Iter p)
```

`axis_xyzt`を格納したコンテナクラスのイテレータをパラメータとして、X, Y, Z 軸のいずれかの要素にアクセスするイテレータを生成します。

以下の例では、`buf.begin()`, `buf.end()`をX軸用のイテレータとしてアルゴリズム`std::minmax_element`に用いています。

```cpp
#include <algorithm>

void myfunc() {  
  // コンテナクラス
  smplbuf_local<axis_xyzt, 10> buf;
  
  // テスト用にデータを投入
  buf[0] = { 1, 2, 3, 4 };
  buf[1] = { 2, 3, 4, 5 };
  ...
  
  // 最大、最小値を得るアルゴリズム
  auto&& minmax = std::minmax_element(
    get_axis_x_iter(buf.begin()),
    get_axis_x_iter(buf.end()));
  
  Serial << "min=" << int(*minmax.first)
        << ",max=" << int(*minmax.second) << mwx::crlf;
}
```



### get\_axis\_{x,y,z}()

```cpp
/*戻り型は長いテンプレート型名なのでauto&&と記載します*/
auto&& get_axis_x(T& c)
auto&& get_axis_y(T& c)
auto&& get_axis_z(T& c)
```

`axis_xyzt`を格納したコンテナクラスのXYZ軸のいずれかの軸を取り出した仮想的なコンテナクラスを生成する関数です。この生成したクラスには`begin()`と`end()`メソッドのみ実装されています。この`begin()`と`end()`メソッドで取得できるイテレータは前節[get\_axis\_{x,y,z}\_iter()](axis\_xyzt.md#get\_axis\_-x-y-z-\_iter)のイテレータと同じものになります。

```cpp
#include <algorithm>

void myfunc() {
  // コンテナクラス
  smplbuf_local<axis_xyzt, 10> buf;
  
  // テスト用にデータを投入
  buf[0] = { 1, 2, 3, 4 };
  buf[1] = { 2, 3, 4, 5 };
  ...
  
  // キューの中の X 軸を取り出す
  auto&& vx = get_axis_x(que);
  
  // 範囲for文の利用
  for (auto&& e : vx) { Serial << int(e) << ','; }
  
  // 最大、最小値を得るアルゴリズム
  auto&& minmax = std::minmax_element(
      vx.begin(), vx.end());
                          
  Serial << "min=" << int(*minmax.first)
        << ",max=" << int(*minmax.second) << mwx::crlf;
}
```
