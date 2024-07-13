---
title: "Terms"
author: "Mono Wireless Inc."
---

# Terms

The following is a supplement to the terminology used in this document.

{% hint style="warning" %}
The explanation of terms may not be in accordance with the definitions provided in standards and other documents.
{% endhint %}

## General terms

### SDK (TWELITE SDK, MWSDK)

Software Development Environment

{% hint style="info" %}
The SDK for software development of TWELITE wireless microcontrollers is called TWELITE SDK (or MWSDK).
{% endhint %}

### IEEE802.15.4

This is the radio standard used by the TWELITE radio module; as long as you use the MWX library, you do not need to be aware of the details of the radio standard.



### packet

The smallest unit of communication in wireless communications.

The maximum amount varies depending on the communication method and settings during communication, but in the MWX library standard communication \<NWK\_SIMPLE>, the amount of data a user can send in one packet is 90 bytes.



### payload (of a packet)

It refers to the body of data contained in a wireless packet.



### node

It refers to a radio station in a wireless network.



## MWX library specific terms

### ACT

A program created using this library. This refers to its source code or the program that runs.



### [BEHAVIOR](../api-reference/behavior/)

A program in the form of an event, among other ACTs. The source code or the program that runs it.

BEHAVIORs are described by a single class definition, which describes callback functions from TWENET, events, and interrupt processing, all in one place. There are three types of behaviors available in the MWX library:

* Application BEHAVIOR: A class defined by the user that describes the application in an event-driven manner.
* Board BEHAVIOR: A class to simplify the use of the functionality of the board that implements the TWELITE radio module.
* Network BEHAVIOR: A class for simplifying procedures in wireless networks.

Behavior names are enclosed in < >. For example, the behavior name for a simple relay network is \<NWK\_SIMPLE>.



### Class objects

In the description of this library, objects that are declared globally from the beginning in the library are referred to as class objects: `Serial`, `Wire`, etc. These class objects can be used without any procedure or by performing the start procedure.

Class objects that consume relatively large amounts of memory allocate memory along with the initialization parameters during the initialization procedure (.setup() or .begin() method).



## C++ terms

{% hint style="info" %}
This is a general term, and assumes knowledge of the C language.
{% endhint %}



### C++

The C++ language.

{% hint style="info" %}
The MWX library is written in C++ and C.
{% endhint %}



### C++11

One of the versions of the C++ standard, meaning C++ as of 2011, which was standardized by ISO in 2011. It has been greatly enhanced since its predecessor, C++03. Newer versions such as C++14 and C++17 are available.

{% hint style="info" %}
The MWX library is implemented using features and syntax added in C++11, and MWSDK compiler support is limited to C++11.
{% endhint %}



### Class

A collection of procedures that focus on some data in a single place. A structure contains the procedures for handling that structure. It actually develops into a much deeper topic, but please refer to technical books.

In C++, the keywords struct and class are essentially the same thing, and a class is a class regardless of which keyword it is declared with.

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); }
};
```

If the above class definition was also done in C, for example, it would look like the following:

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



### Wrapper class

It is a class that includes existing C libraries and their internal structures, and adds C++ specific functionality to make it easier to use. In some cases, the description "wrapped \~structure" is used in the explanation.

{% hint style="info" %}
The MWX library is a combination of wrapper classes from TWENET's C library and a set of newly implemented classes.
{% endhint %}



### Method, Member function

A function that is defined in a class and is associated with the class.

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); } //Method
};
```



### Object (instance)

A class is materialized (allocated memory).

```cpp
void func() {
    myhello obj_hello; // obj_hello is an object of myhello class
    obj_hello._i = 10;
    obj_hello.say_hello();
}
```

In this explanation, object and instance are treated as having the same meaning.



### Constructor

Initialization procedure at the time of object creation.

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello %d\n", _i); }
  
  myhello(int i = 0) : _i(i) {} // constructor
};

void my_main() {
  myhello helo(10); // Here, the constructor is called and set to _i=10
}
```



### Destructor

This is the procedure when the object is destroyed, paired with the constructor.struct myhello {

```cpp
  int _i;
  void say_hello() { printf("hello! %d\n", _i); }
  
  myhello(int i = 0) : _i(i) {} // constructor
  ~myhello() {
    printf("good bye! %d\n", _i);
  } // destructor
};
```



### abstract class

In C++, polymorphism is achieved by virtual classes. Specifically, a class that defines a pure virtual function specified by the virtual keyword.

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
The MWX library does not use virtual functions due to compiler limitations and performance reasons. We use a [differnet approach](design\_policy.md#crtpnishitatenpurtopatn) to achieve polymorphism.
{% endhint %}



### Scope

In the C/C++ language, think of it as a scope enclosed in { }. The objects created in this scope are destroyed when they leave the scope. The destructor is called at this time.

The following is an explicitly scoped version of helo2, which is discarded after line 8 and the destructor is called.

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



MThe MWX library uses the following notation. Here, the validity period of an object declared within the conditional expression of an if statement (older C languages such as C89 do not allow declarations in such a place) is within {} of the if statement.

```cpp
struct myhello {
  int _i;
  void say_hello() { printf("hello! %d\n", _i); }
  operator bool() { return true; } // Operators for judgment in if()
  
  myhello(int i = 0) : _i(i) {} // constructor
  ~myhello() { printf("good bye! %d\n", _i); } // constructor
};

// Function to create a myhello object (generator)
myhello gen_greeting() { return my_hello(); }

void my_main() {
  if (myhello x = gen_greeting()) {
    // The object x in myhello is valid during the if statement
    x.say_hello();
  }
  // object x is destroyed when exiting the if minute
}
```

For example, a two-wire serial bus is a procedure where there is a start and end procedure and the bus is manipulated by an object only during that time. After the object is created, if the bus is properly connected, the true clause of the if statement is executed and the bus is written or read by the created object. When the bus read/write operation is completed, the if statement is exited and the destructor is called to terminate the bus usage.

```cpp
const uint8_t DEV_ADDR = 0x70;
if (auto&& wrt = Wire.get_writer(DEV_ADDR)) { //バスの初期化、接続判定
	wrt(SHTC3_TRIG_H); // 書き出し
	wrt(SHTC3_TRIG_L);
} // バスの利用終了手続き
```



### Namespace

The namespace are actively used in C++ to avoid duplication of definition names. To access a definition in a namespace, use::.

```cpp
namespace MY_NAME { // 名Namespace declaration
  const uint8_t MYVAL1 = 0x00;
}

...
void my_main() {
  uint8_t i = MY_NAME::MYVAL1; // reference of MY_NAME
}

```



### Template

Think of a template as an extension of a C macro.

```cpp
template <typename T, int N>
class myary {
  T _buf[N];
public:
  myary() : _buf{} {}
  T operator [] (int i) { return _buf[i % N]; }
};

myary<int, 10> a1; // Array of type int with 10 elements
myary<char, 128> a2; // Array of char type with 128 elements
```

This example defines a simple array, where T and N are template parameters, where T is the type name and N is a number, and defines an array class of type T with N elements.



### nullptr

In C++11, NULL pointers are now written as nullptr.



### reference type

In C++, reference types are available. This is similar to access by pointer, but with the restriction that it must refer to an object.

For functions with pass-by-reference parameters like the one below, the value of `i` can be rewritten in `incr()`.

```cpp
void incr(int& lhs, int rhs) { lhs += rhs; }

void my_main() {
  int i = 10; j = 20;
  incr(i, j);
}
```



In the example of the template explanation, the return type of `operator[]` is changed to `T&`. By doing so, it becomes possible to perform assignment operations directly on the data inside the array, such as `a[0]=1`.

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
As a rule, the programming interface of the MWX library does not use pointer types, but reference types.
{% endhint %}



### type inference

C++11 introduces the auto keyword for type inference. This allows the compiler to infer the type of an object from its initialization description, thus omitting the need to specify the specific type name. This is effective in cases where class names using template are very long.

In most of the explanations, auto&&, which is called universal reference, is used. Universal references can be written here without being aware of passing references.

```cpp
auto&& p = std::make_pair("HELLO", 5);
       // const char* と int のペア std::pair
```



### container

A class for storing multiple objects of a specific data type such as arrays is called a container. An array class such as myary mentioned in the template example is also called a container.

{% hint style="info" %}
The MWX library provides an array class smplbuf and a FIFO queue class smplque.
{% endhint %}

### Iterator, .begin(), .end()

A pointer in C can be thought of as a way to access a contiguous set of memory elements in a continuous manner from the beginning to the end. The simplest implementation of a FIFO queue is a ring buffer, but there is no memory continuity. Even such a data structure can be described in the same way as a pointer using an iterator.

The methods `.begin()` and `.end()` are used to get the iterator. The iterator that points to the beginning of the container is obtained with `.begin()`. The iterator that points to the next to the end is obtained with `.end()`. The reason for using the next to the end instead of the end is for the clarity of the loop description in the for and while statements, and to handle the case where the number of elements stored in the container is zero.

```cpp
my_queue que; // my_queue is a class of queue

auto&& p = que.begin();
auto&& e = que.end();

while(p != e) {
  some_process(*p);
  ++p;
}

```

In the above, `some_process()` is applied to each element of the `que` using the iterator `p`. `p` is incremented by the `++` operator as an iterator that points to the next element. Even if the container has a data structure that cannot be described by a pointer, it can be processed in the same way as using a pointer.

Since `.end()` indicates the next to the end, the end decision of the while statement is as simple as `(p ! = e)`, which is concise. If there is no element in the queue, `.begin()` returns the same iterator as .end(). If there are no elements in the queue, `.begin()` will return the same iterator as .end(), which is the next iterator after the iterator for the unstored elements.

For a contiguous container in memory, its iterator will usually be a normal pointer. It is not expected to be a large overhead during its operation.



### C++ standard library

The C++ standard library includes the STL (Standard Template Library), which is part of the MWX library.

{% hint style="warning" %}
Due to the [limitations](./#zhi-xian-shi-xiang) of the C/C++ compiler for TWELITE, only a few features are available.
{% endhint %}



### Algorithm

In C, for example, the process of finding the maximum or minimum value is written separately depending on the type. In C++, you can use templates, iterators, etc. to describe such operations independently of their types. This is called an algorithm.

```cpp
// Return the iterator with the maximum value with any iterator as a parameter.
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

For example, the algorithm to find the maximum value as shown above. This algorithm is type-independent. (It is called generic programming.)



```cpp
#include <algorithm>

auto&& minmax = std::minmax_element( // Algorithm for obtaining the maximum minimum
  que.begin(), que.end());

auto&& min_val = *minmax.first;
auto&& max_val = *minmax.second;
```

Here we specify an iterator for que and apply the algorithm `std::minmax_elenet` to obtain its maximum and minimum. `std::minmax_elemet` is defined in the C++ standard library. Its return value is `std::pair`, which combines any two values. The algorithm calculates the maximum and minimum values if the elements indicated by the iterator can be compared with each other using operators such as `<`,`>`,`==`. The return type is also derived from the iterator type.



