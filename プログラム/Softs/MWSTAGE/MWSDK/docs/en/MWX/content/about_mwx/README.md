---
title: "About the MWX library"
author: "Mono Wireless Inc."
---

# About the MWX library

The MWX library is designed to make the programming of TWELITE modules easier and more extensible. Based on the TWENET C library that has been used in the MWSDK, the MWX library has been developed as a library for the application development layer.

```
   Act (USER APPs)... 
+-----------------------+
| MWX C++ LIB           |
+---------------+       |
| TWENET C LIB  |       |
+------------+----------+
| MAC LAYER  | AHI APIs |
+-----------------------+
| TWELITE HARDWARE      |
+-----------------------+
```

The name of the MWX library is Mono Wireless C++ Library for TWELITE, where MW comes from MonoWireless, and C++ -> CXX -> double X -> WX, where MW and WX are combined to form MWX.

The code written using this library is called "ACT".

###

### Notation, etc.

This section describes the notation used in this explanation.

#### auto&&

This is called a universal reference, and is often used in standard libraries. In our library, auto&& is used in most cases

{% hint style="info" %}
The **"**`auto`" is a keyword used to declare local variables (automatic variables) in the C language, but in this case it means to declare them by type inference. `auto` is useful in situations where the C++ template syntax often results in very cumbersome type names, and at the same time can be implemented without explicitly stating the type name.

The following example uses `std::minmax_element`, a standard library algorithm for finding the maximum-minimum value for a type v, and declares the resulting return value by auto. In this case, the type deduced by auto will be `std::pair<int, int>`.

```cpp
#include <algorithm>
int v[] = { 1, 3, -1, 5, 10 };
auto&& result = std::minmax_element(std::begin(v), std::end(v));
Serial << "min=" << int(result.first)
       << ",max=" << int(result.second);
```

For the strict meaning of `&&` in `auto &&`, you will need to refer to technical books, etc. Here, please think of it as "you can declare the return value without worrying about whether it is a reference type (similar to pointer passing in C) or a value.
{% endhint %}



#### about namespace

The `namespace`, `inline namespace`, and `using` are used to redefine names and so on. Some of them are abbreviated in the explanation.



### Restrictions (TWENET)

The MWX library has not been developed to support all the functions of the underlying libraries and functions (functions in the TWNET C library, microcontroller peripheral functions provided by semiconductor vendors, and IEEE802.15.4 functions).



### Restrictions (Using C++)

The MWX library is written in the C++ language, and the act description is also written in C++. However, not all functions are available even in the C++ language. Please pay particular attention to the following points.

* You can allocate memory with the `new` and `new[]` operators, but you cannot destroy the allocated memory, and most C++ libraries that allocate memory dynamically are virtually unusable.
* The constructor of the global object is not called.\
  Note： If necessary, initialization can be done including a call to the constructor by initializing it as in the initialization function ((`void*)&obj_global) class_foo();`).
* `exception` cannot be used.
* Unable to use `virtual` functions.
* Due to the above limitations, only some of the C++ standard libraries such as STL can be used.

※ This is a description of what we know.

{% hint style="warning" %}
We have not conducted a comprehensive verification of the standard library as to whether it can be used or not, nor what might be available. If you find any inconvenience in operation, please try to implement it in another way.
{% endhint %}



### about the library source code

The source code can be found at the followings:

* `{MWSDK install dir}/TWENET/current/src/mwx`
* [`https://github.com/monowireless/mwx`](https://github.com/monowireless/mwx)
