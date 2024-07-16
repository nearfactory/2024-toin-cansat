---
title: EASTL
author: Mono Wireless Inc.
description: EASTL
---

# EASTL

[EASTL](https://github.com/electronicarts/EASTL) is a standard template library (containers and algorithms) maintained by Electronic Arts, and is implemented in the manner of the STL (Standard Template Library) of C++. It has been developed for the development of game consoles with many restrictions, and containers and algorithms have been prepared for environments with large memory handling restrictions.

This library makes EASTL available within TWENET.

The following is a list of EASTL's features.

* Container with fixed memory allocation (`fixed_`) : Containers with a fixed number of elements can be declared without dynamic allocation. If declared globally, a fixed memory area is allocated at compile time. If declared locally, the memory area is allocated in the stack area and can be used within its scope.
* Intrusive containers：While ordinary containers can store arbitrary data structures, Intrusive containers inherit a dedicated base class for data structures, which holds link information and other information to maintain link structures and other structures within the container. Although each element in a container is dedicated to that container, lists and map structures are very efficient in memory usage.（Reference: [Intrusive and non-intrusive containers](https://www.boost.org/doc/libs/1\_35\_0/doc/html/intrusive/intrusive\_vs\_nontrusive.html)）

The 2007 article [EASTL (open-std.org)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html) describes the motivation behind the development.

## Use in TWENET

Please keep in mind the following

**We do not perform comprehensive verification of the library's operation. Please verify the operation of the library by yourself. We also cannot respond to inquiries about how to use EASTL. Please refer to the information such as opening materials and library resource codes of the distribution source.**

* Use the [EASTL3.07](https://github.com/electronicarts/EASTL/releases/tag/3.07.00) (2018/1/31) version. (the last version that can be compiled in C++11)
* The following libraries are not incorporated.
  * `test/packages/EAAssert`, `source/assert.cpp`
  * `test/packages/EATest`
  * `test/packages/EAThread`, `source/thread_support.cpp`
* The test code `test/source` has not been ported to work.
* In _sprintf_ related matters, `EA::StdC::Vsnprintf(char8_t*, ...)` is only solved by calling `vsnprintf_()` in the printf.h library.

### Embedding and Compiling Methods

EASTL can be used when writing an ACT Act.

Add include paths and libraries as required by the development environment for TWELITE. Please include the library headers in the code you create.

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

### How to incorporate (details)

Library compilation and include path settings have already been performed in the directories under MWSDK/TWENET, but the internal settings are described below.

* Compile the code in EASTL/source as a library archive (`libEASTL.a`). Reference to this library is required at link time.
* Add the following include paths at compile time.

If `$(PATH_EASTL)` is the EASTL directory, the include path would be

```bash
-I$(PATH_EASTL)/include
-I$(PATH_EASTL)/test/packages/EAAssert/include
-I$(PATH_EASTL)/test/packages/EABase/include/Common
-I$(PATH_EASTL)/test/packages/EAMain/include
-I$(PATH_EASTL)/test/packages/EAStdC/include
-I$(PATH_EASTL)/test/packages/EATest/include
-I$(PATH_EASTL)/test/packages/EAThread/include
```

## About Coding

### About `std::` and `eastl::`

MWX Live internally also uses the standard libraries in the `std::` namespace.

The standard library (`std::`) and EASTL (`eastl::`) have the same name and the same functionality defined. They may be mixed, but their use may result in errors. In other words, for EASTL use, the definitions in EASTL are usually used (e.g., trying to store `eastl::fixed_string` in `std::unique_ptr` will result in a compiler error).

Also, when using statements such as `using namespace std;`, be careful of name collisions.

### Global object initialization 1 (placement new)

In the development of TWENET, the constructor of the globally declared object is not executed due to compiler constraints. The memory area of the globally declared object is simply cleared to zero. If the code is executed as is, it will hang due to null pointer access in most cases.

Use _placement new_ to initialize this object。

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

\*The code for _placement new_ looks a bit messy, so we provide an auxiliary function `mwx::pnew()`. The previous example can be rewritten as follows

```
(void) new ((void*)&g_str1) tstr128("Hello World");
// ↓
mwx::pnew(g_str1, "Hello World");
```

The second and subsequent arguments are variable numbers and are passed directly to the constructor.

### Global object initialization 2 (unique\_ptr)

Another way to initialize global objects is to use `unique_ptr` ([description of std::unique\_ptr](https://cpprefjp.github.io/reference/memory/unique\_ptr.html)). Although `unique_ptr` is available in both `std::` and `eastl::`, EASTL classes use the one in `eastl::`.

Call `.reset()` at the time of initialization as follows.

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

### About intrusive containers

The following is an example of an element definition for `intrusive_list`. It has only `int mX` members.

```
struct IntNode : public eastl::intrusive_list_node { 
    int mX;
    IntNode(int x = 0) : mX(x) { }
        // no need to call super class's constructor eastl::intrusive_list_node()
};

inline bool operator<(const IntNode& a, const IntNode& b) { return a.mX < b.mX; }
inline bool operator>(const IntNode& a, const IntNode& b) { return a.mX > b.mX; }
```

Elements of `intrusive_list` must have `intrusive_list_node` in the base class. Within the base class are link pointers to maintain the list. This section further defines the comparison operators used for `sort` and so on.

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

## Reference Information

* [EA Standard Template Library](https://eastl.docsforge.com/) ーNote that the libraries included in TWENET are for EASTL 3.07 and contain elements that have been implemented or modified since then.)
* [C++ reference](https://en.cppreference.com/) ーThis is a C++ reference, but the STL explanation is helpful.

## About this sample

The license description of EASTL is as follows

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

The sample code applies MWSLA-1J/E.

## Code Example

### fixed\_vector

An array with a fixed maximum length (i.e., not expandable). (\* `mwx::smplbuf` is also a fixed maximum length array, but it is partially specialized for internal processing of the MWX library)

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
    
    // using operator []
    for (int i = 0; i < v.size(); i++) { Serial << format(" %d", v[i]); }
}
```

There are three template arguments for `fixed_vector`, the first is the type, the second is the maximum number, and the third is _false_. For manipulating arrays, `.puch_back()`, `.pop_back()`, and `[] operators` similar to the general `std::vector` are available.

It is also possible to apply sorting algorithms, etc. In the above example, `eastl::sort` is applied with ascending order `eastl::less`.

### fixed\_list

A list structure with a fixed maximum number of elements (see also `intrusive_list`).

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

There are three template arguments for `fixed_list`: the first is the type, the second is the maximum number, and the third is _false_. For manipulating the list, `.insert()`, `.erase()`, etc. similar to the general `std::list` are available.

In the above code, the list contains the elements of a pair `eastl::pair`, where the first of the pair is an integer of type `uint8_t` and the second is a function pointer to `void (*)(uint8_t)`. In the code, the lambda expression is written directly. The `x.second(x.first);` in the code implies that the value of first is given to the function obtained from second.

This list can be searched for elements using `eastl::find_if` and sorted using `bubble_sort`.

### intrusive\_list

While a normal list can have any data structure as an element, `intrusive_list` constructs a data structure by assigning specific data to an element and using that data.

In the following example, to be an element of the `intruslve_list` data structure, it must be a data element type that inherits from `eastl::intrusive_list_node`. The `eastl::intrusive_list_node` is an extension that allows pointers to the previous and next elements to be stored。

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

In this example, `eastl::fixed_vector<>` is used for the purpose of allocating the required number of elements of the `IntNode`, not that `fixed_vector` was needed. 5 elements are used to store the test values and Construct an intrusive\_list. The example calls `l.push_pront()` to store the elements one by one in the list. Actually, it is not storing, but rewiring the pointers of each element `IntNode`.

Sorting is described by calling a member function like `l.sort()`.

### ring\_buffer

The ring buffer `ring_buffer` is constructed in combination with another container (`fixed_vector` in the example).

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

The definition of `ring_buffer` is a combination of an element type and its container type. The element type should have one extra element.

In the above example, `.push_front()` inserts the element at the top. If it overflows, the tail will disappear. The oldest element is taken out by `.back()`. Remove the oldest element by `.pop_back()`.

### intrusive\_hash\_map

The map structure is a key-value data structure designed to efficiently extract elements by key value. The `intrusive_hash_map` is implemented using the intrusive structure and hash values. It is somewhat complicated to define, but memory consumption can be reduced.

As with intrusive\_list, you need to define your own element type `IntNode` by inheriting from `eastl::intrusive_hash_node_key<element type>`. Also, a maximum hash value (`N_BUCKET_CT`) must be defined to use hashing. This value should be set to an appropriate prime number depending on the expected number of elements to be stored.

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

The above example assumes that the key of the map is a single character of type `uint8_t` and the value part of the map is a function pointer. In `loop()`, the function is executed according to the key input, and so on.

First, since the table and elements are defined as global objects, we initialize the data elements (`nd_a, nd_b, nd_c, nd_d`) and initialize the hashmap (`mp`) by calling `mwx::pnew()` in `setup()`. The return value of `mwx::pnew()` is a pointer to the constructed object, so we write the values (lambda expressions) to the member variables directly after initialization.

After setting the initials and values of the elements (`nd_a, nd_b, nd_c, nd_d`), insert the elements into the map as in `mp.insert(nd_a)`.

In `loop()`, a search of the hashmap is performed each time a character is entered from the serial. The search calls the `mp.find()` method, which returns an iterator, or `mp.end()` if the search fails. If the search succeeds, `(*it)` can be used to refer to the element that was found.

{% hint style="info" %}
`intrusive_hash_multimap` is a multimap that allows duplicate values. Its usage is almost the same as hashmap, except that it uses `.equal_range()` when searching and treats it as a pair of iterators.

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
