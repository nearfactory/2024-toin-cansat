---
title: "The design policy"
author: "Mono Wireless Inc."
---
# The design policy

This section describes the specifications, limitations, notes in this document, and design memos for the C++ language used in the MWX library.

{% hint style="info" %}
This page is intended for use in situations where you need to refer to the library resource code to understand how the library works or to modify it. It assumes a higher level of knowledge of the C++ language compared to using the library.
{% endhint %}

## Design Policy

* The application loop description is intended to be similar to the commonly used API system, but the implementation should be tailored to the characteristics of TWELITE.
* TWENET is an event-driven code description, and it should be classed so that it can be handled. The above classifications will encapsulate the behavior of the application.
* Event-driven and loop descriptions should be able to coexist.
* Simplify procedures by classifying typical peripherals. Make them accessible by loop descriptions whenever possible.
* Simplify the procedures for using the boards we sell, such as MONOSTICK/PAL, by creating classes. (For example, to automate the use of an external watchdog timer.
* Application classes and board classes should be made available through a unified procedure, introducing the idea of polymorphism. (For example, to load application classes with several behaviors at startup, and to avoid defining the connection code of the TWENET C library each time).
* There are no restrictions on the use of C++ functionality. For example, it provides a means to simplify typical procedures such as packet construction and decomposition, which are complicated in handling wireless packets.
* The operator `->` should be avoided as much as possible, and the API should be based on reference types in principle.



{% hint style="info" %}
As we are working on the implementation in a limited time, the design does not cover all the details, but if you have any questions about the design or implementation, please contact our support.
{% endhint %}

## About the C++ Compiler

#### Version

gcc version 4.7.4



#### C++ standard

C++11 (For compiler support status, please refer to the general information.)

* [https://gcc.gnu.org/gcc-4.7/cxx0x\_status.html](https://gcc.gnu.org/gcc-4.7/cxx0x\_status.html)&#x20;
* [https://cpprefjp.github.io/implementation-status.html](https://cpprefjp.github.io/implementation-status.html)&#x20;



## C++ limitations

※ This is a description of what we know.

* You can allocate memory with the new and new\[] operators, but you cannot destroy the allocated memory; most C++ libraries that allocate memory dynamically are virtually unusable. It is used for objects that are created only once and not destroyed after that.
* The constructor of the global object is not called.\
  Note: If necessary, you can initialize the constructor call by using the initialization function (`setup()`) as shown in (`new ((void*)&obj_global) class_foo();`).
* Exception cannot be used.
* Unable to use virtual function.



## Design Memo

This section contains information that will help you understand the code when referring to the MWX library code.

### Current implementation

Due to the limited time available for implementation, some of the details may not be sufficiently developed. For example, const is not fully taken into account in many classes.



### namespace

We have the following policy for namespaces.

* In principle, definitions are placed in a common namespace mwx.
* We want to be able to use namespaces without identifiers, but we want to require identifiers for some definitions.
* Class names should be relatively long, and those used by users should be defined as aliases.

Classes, Functions, and constants are defined within the namespace of mwx names (more precisely, mwx::L1 enclosed in inline namespace L1), with a few exceptions. inline namespace is specified so that definitions that require the specification of mwx:: can coexist with those that do not. The reason why inline namespace is specified is to allow definitions that require the specification of mwx:: to coexist with those that do not.

Most of the definitions do not require namespace names to be specified by using namespace. These specifications are made in `using_mwx_def.hpp` in the library.

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
using namespace mwx::L1; // Definitions in mwx::L1 can be accessed without mwx::.
                         // But mwx::L2 needs mwx::.
```

Exceptionally, relatively short names can be specified as `mwx::crlf`, `mwx::flush`. These are placed in the inline namespace mwx::L2; using namespace mwx::L2; will allow them to be used without specifying the namespace name.

Also, some class names have a using specification.

The `std::make_pair` used in the MWX library is specified using.



### CRTP(Curiously recursive template patterns.)&#x20;

Since virtual functions (virtual) and run-time type information (RTTI) are not available, and even if they were available, they would be difficult to perform,  [CRTP (Curiously recurring template pattern) ](https://en.wikipedia.org/wiki/Curiously\_recurring\_template\_pattern)is used as an alternative design method. CRTP is a template pattern for calling methods of a child class from the parent class from which it is inherited.

The following example shows how to implement an interface called `interface()` in a `Derived` class that inherits from `Base`, and calls the `Derived::print()` method from `Base`.

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

The following are the main classes used in the MWX library.

* Basic parts of event processing`mwx::appdefs_crtp`
* state machine `public mwx::processev_crtp`
* stream `mwx::stream`



### Virtualization with CRTP

In the CRTP class, the class from which it inherits is different for each instance. For this reason, it is not possible to cast it to a parent class and treat it as a member of the same family, nor is it possible to use advanced polymorphism such as virtual functions or RTTI (runtime type information).

The following is an example of implementing the above CRTP example with a virtual function: CRTP cannot manage instances together in the same array as in Base\* b\[2].

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



The MWX library solves this problem by defining a dedicated class for storing class instances of CRTP and defining a similar interface to this class. An example code is given below.

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

The VBase class member variable p\_inst stores a pointer to an object of type Base , and pf\_intrface is a member function pointer to Base::s\_intrface. Base::s\_intrface invokes the T::intrface method by being passed an object instance of itself as an argument and static\_casting it to the T type.

Storage in VBase is implemented here by overloading the = operator (see below for source examples).

In the above example, when making a call to b\[0].intrface(), Base::s\_intrface() will be called with reference to the VBase::pf\_intrface function pointer. In addition, a call to Derived1::intrface() will be made. This part is expected to be expanded inline by the compiler.

It is also possible to perform a conversion from the VBase type to the original Derived1 or Derived2 through a forced cast, but there is no way to directly know the type of the pointer stored in void\*. Although there is no completely safe way to do this, a unique ID (TYPE\_ID) is provided for each class as shown below, and the ID is checked when the cast is executed (get() method). If the get() method is called with a different type, an error message will be displayed.

`B`If a pointer is stored as a Base type, it may not be correctly converted to a T type (e.g., when T has multiple inheritance), so a static\_assert is used to determine at compile time that the pointer is derived from a Base type by using is\_base\_of in \<type\_trails>.

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



### new, new\[] operator

The microcontroller in the TWELITE module does not have enough memory nor does it have advanced memory management. However, the area from the end of the microcontroller's memory map to the stack area is available as a heap area, which can be allocated as needed. An overview of the memory map is shown in the figure below, where APP is the RAM area allocated by the application code, HEAP is the heap area, and STACK is the stack area.

```
|====APP====:==HEAP==..   :==STACK==|
0                                  32KB
```

Even if it is not possible to delete, the new operator may be useful in some situations. For this reason, the new and new\[] operators are defined as follows: pvHear\_Alloc() is a function for allocating memory provided by the semiconductor library, and the same is true for u32HeapStart and u32HeapEnd. 0xdeadbeef is a dummy address. Please do not point out that it is strange that beef is dead.

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

Since exceptions cannot be used, there is no way to deal with failures. Also, if you continue to allocate without being aware of the memory capacity, there is a possibility of interference with the stack area.

{% hint style="info" %}
The memory allocated by the system (e.g. MAC layer) is about 2.5KB.
{% endhint %}



### Container class

The MWX library does not use the container classes provided by the standard library, considering the small resources of the microcontroller and the lack of dynamic memory allocation, but defines two simple container classes. The container classes have defined iterators and begin() and end() methods, so you can use some of the range for statements and STL algorithms.

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

| Class name | remark                                                                                                                                                                                                                                       |
| ---------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `smplbuf`  | It is an array class that manages the maximum area (capacity) and the usable area (size) whose size can be specified within the maximum area. This class also implements the stream interface, so data can be written using the << operator. |
| `smplque`  | The FIFO queue is implemented. The size of the queue is determined by template parameters. There is also a template argument to manipulate the queue using interrupt inhibition.                                                             |

####

#### About memory in container classes

In the container class, the memory allocation method is specified as a parameter of the template argument.

| Class name     | Remark                                                                                                                                                                                                              |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `alloc_attach` | Specify the buffer memory that has already been allocated. This is used when you want to manage the memory area allocated for the C library, or when you want to process the same buffer area as a fragmented area. |
| `alloc_static` | Allocate as a static array in the class. The size is determined in advance or used as an area for temporary use.                                                                                                    |
| `alloc_heap`   | Allocate to the heap area. Once allocated to the system heap, it cannot be discarded, but it is suitable for use in initialization to allocate an area according to application settings.                           |



### variable parameter

In the MWX library, variable number arguments are used for operations on byte sequences, bit sequences, and printf equivalent operations. The example below shows the process of setting 1 to the specified bit position.

```cpp
// packing bits with given arguments, which specifies bit position.
//   pack_bits(5, 0, 1) -> (b100011) bit0,1,5 are set.

// The first function of recursive extraction
template <typename Head>
constexpr uint32_t pack_bits(Head head) { return  1UL << head; }

// Extract head and transfer the rest of the parameters to pack_bits 
//by recursive call
template <typename Head, typename... Tail>
constexpr uint32_t pack_bits(Head head, Tail&&... tail) {
  return (1UL << head) | pack_bits(std::forward<Tail>(tail)...);
}

// コンパAfter ILL, the following two will have the same result.
constexpr uint32_t b1 = pack_bits(1, 4, 0, 8);
// b1 and b2 are the same! 
const uint32_t b2 = (1UL << 1)|(1UL << 4)|(1UL << 0)|(1UL << 8);
```

In this process, the parameter pack of template (typename... part of template) to perform recursive processing to expand the arguments. In the above example, since constexpr is specified, the calculation is done at compile time and the result is equivalent to macro definition or const value specification such as b2. It can also behave as a function that dynamically calculates variables as arguments.



In the following example, the expand\_bytes function is used to store a value in a local variable from the received packet data string. In the case of using a parameter pack, since the type of each argument can be known, it is possible to store values of different sizes and types from the byte string of the received packet, as shown below.

```cpp
auto&& rx = the_twelite.receiver.read(); // received packet

// Variable that stores the contents of the packet after expansion
// The payload of a packet is a sequence of bytes, arranged as follows.
//   [B0][B1][B2][B3][B4][B5][B6][B7][B8][B9][Ba][Bb]
//   <message       ><adc*  ><vcc*  ><timestamp*    >
//   * Numerical types are big-endian.
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



### Iterator

An iterator is an abstraction of a pointer, which has the effect of making it possible to access data structures as if they were pointers, even if the data structures are not memory-contiguous, for example.

{% hint style="info" %}
In the C++ STL, the combination of an iterator indicating the beginning of the container obtained by the begin() method and an iterator indicating the "next" end of the container obtained by the end() method is often used.

The reason why we use end() for the "next" at the end of the container is because we expect the following description, and the MWX library follows this in its container implementation.

```cpp
smplque<uint8_t, alloc_local<uint8_t, 5> > que;
que.push('a'); que.push('b'); que.pop(); que.push('c'); ...

auto&& p = que.begin();
auto&& e = que.end();

while(p != e) { // p advanced to e = all elements processed
  Serial << *p;
  ++p; // イThe prefix operator is used for incrementing the terrator.
  　　　// In this case, writing p++ will result in a copy of the iterator 
       // in the code, although it is likely to be optimized by the compiler.
}
```

Conform the iterator to the standard library specification, so that range for statements can be used, and algorithms from the standard library can be used.

```csharp
#include <algorithm>
#include <cctype>

// Character conversion using lambda expressions
std::for_each(que.begin(), que.end(), 
  [](uint8_t& x) { x = std::toupper(x); });

// range-for statement
for (uint8_t x : que) {
  Serial << x;
}
```

(The MWX library has not been tested for compatibility with the C++ standard library. Please check the operation before use.)
{% endhint %}



The following example shows the use of an iterator for a FIFO queue that cannot be accessed continuously with a normal pointer, and also an iterator that extracts only a specific member of the FIFO queue structure (the X axis in the example).

```cpp
//A queue with 5 elements, whose elements are the 4-axis structures of XYZT
smplque<axis_xyzt, alloc_local<axis_xyzt, 5> > que;

// Input data for testing.
que.push(axis_xyzt(1, 2, 3, 4));
que.push(axis_xyzt(5, 2, 3, 4));
...

// Access using iterators as structures
for (auto&& e : v) { Serial << int(e.x) << ','; }

// Extract the X axis in the queue.
auto&& vx = get_axis_x(que);
// Access with X-axis iterator
for (auto&& e : vx) { Serial << int(e) << ','; }

// Since it is an iterator of int16_t elements, 
//the STL algorithm (max-min) can be used.
auto&& minmax = std::minmax_element(vx.begin(), vx.end());
```



 The following is an excerpt of the implementation of an iterator for the smplque class. In this iterator, the queue object is managed by its entity and its index. The part of the queue that is discontiguous in memory (ring buffer structure where the next to the tail must point to the beginning) is solved by smplque::operator \[]. If the addresses of the objects match and the indices match, the iterators point to the same thing.

This implementation part also includes the typedefs required by , allowing more STL algorithms to be applied.

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

Iterators that access only specific structure members in the container that contains the structure are a bit complicated. Define a member function to access the structure members in advance. Next, define a template with this member function as a parameter (`R& (T::*get)()`). "`Iter`" is the iterator type of the container class.

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

// It's long, so shorten it with using
template <typename T, int16_t& (axis_xyzt::*get)()>
using _axis_xyzt_axis_ret = _axis_xyzt_iter_gen<
    _iter_axis_xyzt<typename T::iterator, axis_xyzt, int16_t, get>, T>;

// Generator to extract X axis
template <typename T>
_axis_xyzt_axis_ret<T, &axis_xyzt::get_x>
get_axis_x(T& c) {
    return _axis_xyzt_axis_ret<T, &axis_xyzt::get_x>(c);
}
```

The `operator *` that accesses the value calls the member function described above. The `*_p` is the `axis_xyzt` structure, and `(*_p.*get)()` calls `_p->get_x()` if `&axis_xyzt::get_x` is specified in `T::*get`.

The `_axis_xyzt_iter_gen` class implements only `begin()`, `end()` and generates the above iterators. Now you can use range for statements and algorithms.

This class name is very long and difficult to write in the source code. We will prepare a generator function to generate this class. In the example below, it is `get_axis_x()` in the last line. By using this generator function, the description becomes as simple as `auto&& vx = get_axis_x(que);` as shown in the beginning.

This iterator for extracting only the axes can also be used with the `smplbuf` class of array type as well.



### Implementing interrupt, event, and state handlers

In order to describe the application behavior by user-defined classes, typical handlers need to be defined as mandatory methods, but it is complicated to define all the other numerous interrupt handlers, event handlers, and state machine state handlers. Ideally, only those defined by the user should be defined, and only that code should be executed.

```cpp
class my_app_def {
public: // Define required methods
	void network_event(twe::packet_ev_nwk& pEvNwk) {}
	void receive(twe::packet_rx& rx) {}
	void transmit_complete(twe::packet_ev_tx& pEvTx) {}
	void loop() {}
	void on_sleep(uint32_t& val) {}
	void warmboot(uint32_t& val) {}
	void wakeup(uint32_t& val) {}
	
public: // It is cumbersome to make these descriptions mandatory.
  // DIO interrupt handler: There are 20 types.
  // DIO event handler: There are 20 types.
  // Timer interrupt handler: There are five types
  // Timer event handlers: there are 5 types
  // ...
}
```



In the MWX library, a large number of DIO interrupt handlers (on TWELITE hardware, a single interrupt, but for ease of use, a handler is assigned to each DIO) are defined as empty handlers using templates, and user-defined member functions are defined by specializing the templates.

```cpp
// hpp file
class my_app_def : class app_defs<my_app_def>, ... {
  // Empty handler
  template<int N> void int_dio_handler(uint32_t arg, uint8_t& handled) { ; }

  ...   
  // Implement only number 12.
  
public:
  // Callback function called from TWENET
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

The actual user-described code has been simplified by macroizing and including header files, but the above includes the code necessary for the explanation.

The `my_app_def::cbTweNet_u8HwInt()` is called from the interrupt handler from TWENET. in the cpp file, only `int_dio_handler<12>` is instantiated with the specialization described in it. file is instantiated from a template in the hpp file. The rest are instantiated from templates in the hpp file.

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

Eventually, we can expect that the compiler optimization will determine that codes other than number 12 are meaningless and disappear from the code (however, we do not guarantee that they will be optimized as described above).

In other words, in user code, if you want to define the behavior at interrupt 12, just write `int_dio_handler<12>` (Note: to enable DIO interrupt, you need to call `attachInterrupt()`). Handlers that are not registered are expected to be low-cost calls due to compile-time optimization.

{% hint style="info" %}
One technique to enable this when the user defines a function, and call another function if not defined, is to resolve it at link time. Specify `__attribute__((wake))` as shown below. If the `wakeup()` function is defined in the user code, the user code will be linked to the function, and if it is not defined, the function with empty content will be linked.

```cpp
// mwx_appcore.cpp
void wakeup() __attribute__((weak));
void wakeup() { }
```

In the implementation of the above handler, it is necessary to generate weak member variables explicitly, and it is difficult to optimize by inlining, so it is not used. However, weak functions are defined to receive some callbacks from TWENET, such as `wakeup()`.
{% endhint %}



### Stream class

The stream class is mainly used for input/output of UART (serial port), and MWX library mainly defines procedures for output. But some of them are also defined for input.

This section describes the implementation required by the derived class.

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

The above is an implementation of the `write()` method that writes a single character. The `stream<serial_jen>` of the parent class accesses the `serial_jen::write()` method using the `get_Drived()` method to perform casting.

Define methods such as `write()`, `read()`, `flush()`, and `available()` as needed.

For formatting output, we use Marco Paland's [printf library](https://github.com/mpaland/printf), which needs to be implemented for use with the MWX library. In the following example, the derived class `serial_jen` needs to define the `vOutput()` method for 1-byte output, and save the auxiliary information for output in the parent class `pvOutputContext` since `vOutput()` is a static method. The other is to save the auxiliary information in the `pvOutputContext` of the parent class since `vOutput()` is a static method.

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
	TWE_tsFILE* _psSer; // Low-level structure for serial output
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

By `get_pfcOutput()`, the `vOutput()` function defined in the derived class is specified, and `pvOutputContext` is passed as its parameter. In the above example, when the `<<` operator is called with int type, `serial_jen::vOutput()` and `TWE_tsFILE*` which is already set for UART are passed to the `fctprintf()` function.



### Worker object for Wire, SPI

In the Wire class, it is necessary to manage the communication from start to end when sending and receiving with a 2-wire device. This section describes the contents of the description of using the worker object.

```cpp
if (auto&& wrt = Wire.get_writer(SHTC3_ADDRESS)) {
	Serial << "{I2C SHTC3 connected.";
	wrt << SHTC3_TRIG_H;
	wrt << SHTC3_TRIG_L;
	Serial << " end}";
}
```



This is an excerpt of the `periph_twowire::writer` class, which inherits from `mwx::stream<writer>` to implement the stream interface, and implements the `write()` and `vOutput()` methods to use the steam interface. To use the steam interface, the `write()` and \`vOutput() methods are implemented.

The constructor calls the method to start communication for 2-wire serial and the destructor calls the method to end communication. Also, the `operator bool()` operator returns true if the communication of the 2-wire serial device is successfully started.

```cpp
class periph_twowire {
public:
	class writer : public mwx::stream<writer> {
		friend class mwx::stream<writer>;
		periph_twowire& _wire;
	
	public:
		writer(periph_twowire& ref, uint8_t devid) : _wire(ref) {
	  	_wire.beginTransmission(devid); // Start communication with constructor
		}
	
		~writer() {
			_wire.endTransmission(); // Communication terminated by destructor
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

The `get_writer()` method creates an object `wrt`. Due to the Return Value Optimization (RVO) of the C++ compiler, the `writer` is created directly in the `wrt`, so no copy is made and the bus running in the constructor is not initialized multiple times. However, RVO is not guaranteed by the C++ specification, and just in case, the MWX library defines copy, delete assignment operators, and move constructors (although it is unlikely that move constructors will be evaluated).

The wrt in the if clause is first initialized by the constructor and starts communication at the same time. If there is no error at the start of communication, the bool operator at the time of conditional judgment returns true, and the processing in the scope of the if clause takes place. If there is no error at the start of communication, the bool operator at the conditional judgment returns true, and the processing in the if clause scope is performed. When the scope is exited, the destructor terminates the 2-wire serial bus. If there is no communication partner, false will be returned and the wrt object will be destroyed.

It overrides the definition of `operator << (int)`, which is specific to Wire and SPI. The default behavior of the stream is to convert numeric values to strings and output them, but Wire and SPI rarely write numeric strings to the bus, and on the contrary, we often want to input literals of numeric type such as configuration values. We will change this behavior.

```cpp
			writer& operator << (int v) {
				_wire.write(uint8_t(v & 0xFF));
				return *this;
			}
```

In this example, the int type values are truncated to 8 bits and the values are output.
