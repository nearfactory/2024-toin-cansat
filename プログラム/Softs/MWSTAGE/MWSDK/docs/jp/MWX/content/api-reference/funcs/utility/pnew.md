---
title: "pnew()"
author: "Mono Wireless Inc."
description: "pnew() - 配置new の記述を簡素化"
---

# pnew()

配置new(placement new)の記述を簡素化します。

```cpp
template <class T, class... Args>
T* pnew(T& obj, Args&&... args) {
    return (T*)new ((void*)&obj) T(std::forward<Args&&>(args)...);
}
```



例えば以下のように利用します。コンストラクタ引数を与えることもできます。

```cpp
class my_class {
    int _a;
public:
    my_class(int a = -1) : _a(a) {}
};

my_class obj_1; // このコンストラクタは呼ばれない！
my_class obj_2; // このコンストラクタは呼ばれない！

void setup() {
    mwx::pnew(obj_1);    // my_class obj_1; に相当    
	mwx::pnew(obj_2, 2); // my_class obj_2(2); に相当
    ...
}
```



## 背景

コンパイラの制約のためグローバルオブジェクトのコンストラクタが呼び出されないため、これを初期化する方法は配置newを使用する方法が挙げられます。しかしながら、配置new(placement new)の構文は煩雑に見えるため。

他に`std::unique_ptr`(または `eastl::unique_ptr`)を用いる方法もある。

```cpp
std::unique_ptr<my_class> obj_3;

void setup() {
    obj_3.reset(new my_class(3));
    		// TWELITE マイコンでは new は１回だけ確保し delete はできないため、
            // 事実上グローバルオブジェクトと同等です。
}
```

