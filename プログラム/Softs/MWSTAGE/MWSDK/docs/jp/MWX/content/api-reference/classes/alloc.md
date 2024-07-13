---
title: "alloc"
author: "Mono Wireless Inc."
description: "alloc"
---
# alloc

コンテナクラス(`smplbuf`, `smplque`)のテンプレート引数として指定し、内部で利用するメモリの確保または領域指定します。

{% hint style="warning" %}
このクラスはユーザコードから直接呼び出すものではありませんが、内部的にコンテナの宣言に用いられています。
{% endhint %}

| クラス名                    | 内容                  |
| ----------------------- | ------------------- |
| `alloc_attach<T>`       | すでにあるバッファを指定する      |
| `alloc_local<T, int N>` | Nバイトのバッファを内部に静的確保する |
| `alloc_heap<T>`         | 指定したサイズをヒープに確保する    |

`alloc_attach`や`alloc_heap`ではメモリ確保クラスに応じた初期化メソッド (`init_???()`)を実行する必要があります。

## 初期化

```cpp
void attach(T* p, int n) // alloc_attach
void init_local()        // alloc_local
void init_heap(int n)    // alloc_heap
```

バッファー`p`・サイズ`n`で初期化します。



## メソッド

### alloc\_size()

```cpp
uint16_t alloc_size()
```

バッファのサイズを返す。



### &#x20;\_is\_attach(), \_is\_local(), \_is\_heap()

想定したallocクラスと違うメソッド呼び出し記述に対して、`static_assert`のように、コンパイルエラーを発生させるためのメソッドです。
