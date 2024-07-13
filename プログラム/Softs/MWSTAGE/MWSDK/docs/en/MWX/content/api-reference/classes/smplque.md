---
title: smplque
author: "Mono Wireless Inc."
description: Container class with FIFO queue structure.
---

# smplque

Container class with FIFO queue structure.

```cpp
template <typename T, int N, class Intr> smplbuf_local
template <typename T, class Intr> smplbuf_attach
template <typename T, class Intr> smplbuf_heap
```

`smplque` is a container class that provides FIFO queue operations for memory areas specified by element type `T` and [memory allocation method `alloc`](alloc.md). Since the specification of `alloc` is complicated, an alias definition using `using` is used.

{% hint style="warning" %}
As a rule, element types are assumed to be structures that store numbers, numerical values, etc. It is not assumed to store objects that need to be destroyed by the destructor (since there is no process to delete the object when the element is deleted from the queue).
{% endhint %}

A class `Intr` can be registered to set interrupt disabling at declaration time. If this class is not specified, normal operation without interrupt disable control is performed.

Example of object declaration. Immediately after the declaration, a method call is made for initialization. The maximum size of any of these objects is 128 bytes immediately after initialization, and the initial size is 0 and nothing is stored. The maximum size cannot be changed.

```cpp

void some_func() {

// alloc locally
smplque_local<uint8_t, 128> q1;

// attach to existing buffer
uint8_t buf[128];
smplque_attach<uint8_t> q2;

// alloc from the HEAP
smplque_heap<uint8_t> q3;

void setup() {
  // Globally defined objects are initialized with setup()
  q1.init_local();
  q2.attach(buf, 128);
  q3.init_heap(128);
}

void some_func() {
  // Local definition of smplque_local can omit init_local()
  smplque_local<uint8_t, 128> q_local;
  ..
}

}
```



Since it is a FIFO queue, it is operated using methods such as `push()`,`pop()`,`front()`.

```cpp
void begin() { // begin() runs only once on startup
	smplque_local<int, 32> q1;
	
	q1.push(1);
	q1.push(4);
	q1.push(9);
	q1.push(16);
	q1.push(25);
	
	while(!q1.empty()) {
		Serial << int(q1.front()) << ',';
		q1.pop();
	}
	// output -> 1,4,9,16,25,
}
```



Access by iterator is also possible.

```cpp
void begin() { // begin() runs only once on startup
	smplque_local<int, 32> q1;
	q1.init_local();
	
	q1.push(1);
	q1.push(4);
	q1.push(9);
	q1.push(16);
	q1.push(25);
	
	// Use iterators
	for(int x : q1) {
		Serial << int(x) << ',';
	}
	
	// Application of STL Algorithm
	auto&& minmax = std::minmax_element(q1.begin(), q1.end());
	Serial <<  "min=" << int(*minmax.first)
		     << ",max=" << int(*minmax.second);
	// output -> 1,4,9,16,25,min=1,max=25[]
}
```

## Declaration and initialization

```cpp
smplbuf_local<T,N>
smplbuf_local<T,N>::init_local()

smplbuf_attach<T>
smplbuf_attach<T>::attach(T* buf, uint16_t N)

smplbuf_heap<T>
smplbuf_heap<T>::init_heap(uint16_t N); 

// Example
// Fixed array inside
smplque_local<uint8_t, 128> q1;
q1.init_local();

// Use an already existing array
uint8_t buf[128];
smplque_attach<uint8_t> q2;
q2.attach(buf, 128);

// Allocate to heap
smplque_heap<uint8_t> q3;
q3.init_heap(128); 
```

Declares a container of type `T` and size `N`. After the declaration, call the initialization methods.



The `smplque_local` allocates the area by a fixed array inside. Initialization by the constructor is also possible.

In `smplque_attach`, specify the first pointer `T* buf` of the buffer to be used, the initial size `size` and the maximum size `N` of the array. Initialization by the constructor is also possible.

smplque_heap` allocates memory in the HEAP area (an area of memory that cannot be released but can be allocated at any time). Once allocated, this area cannot be released, so it is usually defined in the global area. Allocation is done by `init_heap()`. Memory allocation by the constructor is not allowed. Please call `init_heap()` to use this function.

{% hint style="danger" %}
When creating a global object, initialization by the constructor is not possible (due to compiler limitation). Please call initialization functions `init_local()`,`attach()`,`init_heap()` at the beginning of execution (`setup()` is recommended).
{% endhint %}



## Methods

### push(), pop(), front(), back()

```cpp
inline void push(T&& c)
inline void push(T& c)
inline void pop()
inline T& front()
inline T& back()

inline T& pop_front()
```

push()` adds an entry to the queue.

pop()` removes an entry from the queue.

front()` refers to the first entry (the first one added).

back()` refers to the last entry (the last one added).

pop_front()` refers to the first entry as a return value and deletes it from the queue.



### empty(), size(), is\_full()

```cpp
inline bool empty()
inline bool is_full()
inline uint16_t size()
inline uint16_t capacity()
```

`empty()` returns `true` if the array contains no elements. `is_full()` returns `true` when the array is full.

`size()` returns the number of elements stored in the queue.

`capacity()` returns the maximum number of elements stored in the queue.



### clear()

```cpp
inline void clear()
```

Erase all elements of the queue.



### operator \[]

```cpp
inline T& operator[] (int i)
```

element. `0` is the first element added.



## Iterator

```cpp
inline smplque::iterator begin()
inline smplque::iterator end()
```

You can get an iterator by `begin()` and `end()`. The beginning of the iterator is the first registered element of the queue. By using the iterator, range for statements and algorithms can be used.

One application is [Access by iterator focusing on a specific member of the structure axis\_xyzt](axis\_xyzt.md).

