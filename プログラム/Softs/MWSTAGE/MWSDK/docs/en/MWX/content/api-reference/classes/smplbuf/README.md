---
title: smplbuf
author: "Mono Wireless Inc."
---

# smplbuf

This is a container class with an array structure inside. The maximum size of the buffer is determined at initialization, and it behaves as a variable-length array up to that maximum size.

```cpp
template <typename T, int N> smplbuf_local
template <typename T> smplbuf_attach
template <typename T> smplbuf_heap
```

`smplbuf` is a container class that provides array operations on memory areas specified by element type `T` and [memory allocation method `alloc`](./alloc.md). Since the specification of `alloc` is complicated, alias definitions using `using` are used.

Here is an example of object declaration. Immediately after the declaration, a method call is made for initialization. The maximum size of each object is 128 bytes immediately after initialization, and the size is 0. Use while extending the size as needed.

```cpp
// Array area is a fixed array of class member variables
smplbuf_local<uint8_t, 128> b1;

// Refer to buffer memory that already exist
uint8_t buf[128];
smplbuf_attach<uint8_t> b2;

// Allocate buffer memory at heap
smplbuf_heap<uint8_t> b3;

// Initialize (must do initlialize at setup() for global objects)
void setup() {
    b1.init_local();
    b2.attach(buf, 0, 128);
    b3.init_heap(128);
} 

// In some function
void some_func() {
    smplbuf_local<uint8_t, 128> bl;
    // bl.init_local(); // It can omit if smplbuf_local is declared locally.
    
    bl.push_back('a');
}
```


Alias classes are defined for `uint8_t` type.

```cpp
template <int N>
smplbuf_u8
// smplbuf<uint8_t, alloc_local<uint8_t, N>>

smplbuf_u8_attach
// smplbuf<uint8_t, alloc_attach<uint8_t>>

smplbuf_u8_heap
// smplbuf<uint8_t, alloc_heap<uint8_t>>
```



Elements can be accessed like normal arrays, using the \[] operator, etc., and iterators can also be used to access elements.

```cpp
void begin() { // begin() runs only once at startup
  smplbuf_u8<32> b1; // initially b1 behaves empty buffer (32bytes is allocated).
  b1.reserve(5); // reserves 5bytes in b1.
  
  b1[0] = 1;
  b1[1] = 4;
  b1[2] = 9;
  b1[3] = 16;
  b1[4] = 25;
  
  for(uint8_t x : b1) { // can use ranged for.
    Serial << int(x) << ",";
  }
}
```


The class also has `push_back()` method, which enables some type of algorithm from standard C++ library.



## Declaration, Initialize

```cpp
smplbuf_local<T,N>()
smplbuf_local<T,N>::init_local()

smplbuf_attach<T>(T* buf, uint16_t size, uint16_t N)
smplbuf_attach<T>::attach(T* buf, uint16_t size, uint16_t N)

smplbuf_heap<T>()
smplbuf_heap<T>::init_heap(uint16_t N)

// Example
// Allocated internally as fixed length array.
smplbuf_local<uint8_t, 128> b1;
b1.init_local();

// attach to an existing array.
uint8_t buf[128];
smplbuf_attach<uint8_t> b2;
b2.attach(buf, 0, 128);

// allocate array at heap area.
smplbuf_heap<uint8_t> b3;
b3.init_heap(128); 
```

Declares a container of type `T` and size `N`. After the declaration, call the initialization method.

`smplbuf_local` allocates an area by a fixed array inside. It can also be initialized by the constructor.

In `smplbuf_attach`, specify the first pointer `T* buf` of the buffer to be used, the initial size `size` and maximum size `N` of the array. Initialization by the constructor is also possible.

The `smplbuf_heap` allocates memory in the HEAP area (a memory area that cannot be released but can be allocated at any time). Once allocated, this area cannot be released, so it is usually defined as a global area. Allocation is done by `init_heap()`. Memory allocation by the constructor is not allowed. Please call `init_heap()` to use this function.

{% hint style="danger" %}
When creating a global object, initialization by the constructor is not possible. Please call initialization functions `init_local()`,`attach()`,`init_heap()` at the beginning of execution (`setup()` is recommended).
{% endhint %}



## List of initializers 

```cpp
void in_some_func() {
    smplbuf_local<uint8_t, 5> b1;
    b1.init_local();
    
    b1 = { 0, 1, 2, 3, 4 };
    
    smplbuf_local<uint8_t, 5> b2{0, 1, 2, 3, 4};
}
```

Initializer list `{ ... }` can be used to initialize members. Except for use in the constructor in a local declaration of `smplbuf_local`, it is valid after calling the initialization method.

* right side value of assignment operator (`smplbuf_local`, `smplbuf_attach`, `smplbuf_heap`)
* constructor (local declaration of `smplbuf_local`, global declaration is not allowed)



## Methods

### append(), push\_back(), pop\_back()

```cpp
inline bool append(T&& c)
inline bool append(const T& c)
inline void push_back(T&& c)
inline void push_back(const T& c)
inline void pop_back()
```

Add a member `c` at the end. The return value of `append()` is `bool`, which returns `false` if the buffer is full and cannot be added.

The `pop_back()` deletes the trailing entry. However, it does not clear the entry.



### empty(), size(), capacity()

```cpp
inline bool empty()
inline bool is_end()
inline uint16_t size()
inline uint16_t capacity()
```

`empty()` returns `true` when no elements are stored in the array. `is_end()` returns `true` when the array is full.

`size()` returns the number of array elements.

`capacity()` returns the maximum number of elements in the array.



### reserve(), reserve\_head(), redim()

```cpp
inline bool reserve(uint16_t len)
inline void reserve_head(uint16_t len)
inline void redim(uint16_t len)
```

`reserve()` extends the size of the array. The area where the array is not stored is initialized by default.

`reserve_hear()` allocates an area of the specified size at the top of the array. This area cannot be referenced by the container object. For example, it can be used as a container to access a sub-array of a packet payload whose header part is skipped. To return the container to the allocated area so that all of it can be accessed again, give it the same negative value as when it was allocated.

`redim()` resizes the allocated area. Unlike `reserve()`, it does not initialize the unused area.



### operator \[]

```cpp
inline T& operator [] (int i) 
inline T operator [] (int i) const
```

element.

A negative value for `i` makes the element from the end of the buffer. For `-1`, the element is the last element, and for `-2`, it is one before the end.



## Output to mwx::stream

Array objects of type `uint8_t` (`smplbuf<uint8_t, *>`) can be output as is to derived objects of `mwx::stream`.



### << operator

```cpp
template <class L_STRM, class AL>
	mwx::stream<L_STRM>& operator << (
			mwx::stream<L_STRM>& lhs, mwx::_smplbuf<uint8_t, AL>& rhs) 
			
//例
smplbuf_u8<128> buf;
buf.push_back('a');
buf.push_back('b');
buf.push_back('c');

Serial << buf;
// 出力: abc

```

Outputs a sequence of bytes for `mwx::stream` derived objects such as `Serial`.



### to\_stream()

```cpp
inline std::pair<T*, T*> to_stream()

//Example
smplbuf_u8<128> buf;
buf.push_back('a');
buf.push_back('b');
buf.push_back('c');

Serial << buf.to_stream();
// Output: 0123
```

Used for output to stream purposes. Used to implement the << operator.



## Data generation with `mwx::stream

mwx::stream` defines functions and operators to output bytes to a stream, such as `<<` operator and `printfmt()` method. You can use the stream output procedure with a smplbuf array of type `uint8_t` as the output destination.

There are two methods.

* Using helper object generated by [`.get_stream_helper()`](get\_stream_helper.md).
* Use the smplbuf class that [inherits `mwx::stream`](smplbuf\_strm\_u8.md).
