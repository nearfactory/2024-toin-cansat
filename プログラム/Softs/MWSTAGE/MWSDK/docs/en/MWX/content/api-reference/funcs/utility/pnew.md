---
title: "pnew()"
author: "Mono Wireless Inc."
description: "pnew() - Simplify the description of placement new"
---

# pnew()

Simplifies the description of placement new (placement new).

```cpp
template <class T, class... Args>
T* pnew(T& obj, Args&&... args) {
    return (T*)new ((void*)&obj) T(std::forward<Args&&>(args)...);
}
```



For example, use the following Constructor arguments can also be given.

```cpp
class my_class {
    int _a;
public:
    my_class(int a = -1) : _a(a) {}
};

my_class obj_1; // This constructor will not be called!
my_class obj_2; // This constructor will not be called!

void setup() {
    mwx::pnew(obj_1);    // my_class obj_1;  
	mwx::pnew(obj_2, 2); // my_class obj_2(2);
    ...
}
```



## background

Since the constructor of the global object is not called due to compiler constraints, one way to initialize it is to use placement new. However, since the syntax of placement new (placement new) appears to be complicated.

Another method is to use `std::unique_ptr` (or `eastl::unique_ptr`).

```cpp
std::unique_ptr<my_class> obj_3;

void setup() {
    obj_3.reset(new my_class(3));
    		//  Since the TWELITE microcontroller only allocates new once and cannot delete It is effectively equivalent to a global object.
}
```

