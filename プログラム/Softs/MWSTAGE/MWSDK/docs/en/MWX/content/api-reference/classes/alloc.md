---
title: alloc
author: "Mono Wireless Inc."
---

# alloc

Specified as a template argument of a container class (`smplbuf`, `smplque`) to allocate or specify an area of memory to be used internally.

{% hint style="warning" %}
This class is not called directly from user code, but is used internally to declare containers.
{% endhint %}

| Class Name | Contents |
| ----------------------- | ------------------- |
| `alloc_attach<T>` | specify an already existing buffer |
| `alloc_local<T, int N>` | statically allocate a buffer of N bytes internally |
| `alloc_heap<T>` | allocate a buffer of the specified size in the heap

In `alloc_attach` and `alloc_heap`, initialization methods (`init_???()`) must be executed according to the memory allocation class.

## initialization

```cpp
void attach(T* p, int n) // alloc_attach
void init_local()        // alloc_local
void init_heap(int n)    // alloc_heap
```

Initialize with buffer `p` and size `n`.



## Methods

### alloc\_size()

```cpp
uint16_t alloc_size()
```

Returns the size of the buffer.



### \_is\_attach(), \_is\_local(), \_is\_heap()

This method is used to generate a compile error, like `static_assert`, for a method call description that is different from the expected alloc class.
