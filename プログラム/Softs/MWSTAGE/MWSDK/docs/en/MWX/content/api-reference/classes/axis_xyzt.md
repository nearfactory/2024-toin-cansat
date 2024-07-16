---
title: axis_xyzt
author: "Mono Wireless Inc."
---

# axis\_xyzt

This structure is used to store the values of 3-axis accelerometers, but procedures have been added to increase convenience when stored in a container class.

```cpp
struct axis_xyzt {
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t t;
};
```



### get\_axis\_{x,y,z}\_iter()

```cpp
/*The return type is described as auto&& because it is a long template type name.*/
auto&& get_axis_x_iter(Iter p)
auto&& get_axis_y_iter(Iter p)
auto&& get_axis_z_iter(Iter p)
```

Generates an iterator that accesses an element on the X, Y, or Z axis using the iterator of the container class containing `axis_xyzt` as a parameter.

In the example below, `buf.begin()` and `buf.end()` are used as iterators for the X axis in the algorithm `std::minmax_element`.

```cpp
#include <algorithm>

void myfunc() {  
  // container class
  smplbuf_local<axis_xyzt, 10> buf;
  
  // Submit data for testing
  buf[0] = { 1, 2, 3, 4 };
  buf[1] = { 2, 3, 4, 5 };
  ...
  
  // Algorithm to obtain maximum and minimum values
  auto&& minmax = std::minmax_element(
    get_axis_x_iter(buf.begin()),
    get_axis_x_iter(buf.end()));
  
  Serial << "min=" << int(*minmax.first)
        << ",max=" << int(*minmax.second) << mwx::crlf;
}
```



### get\_axis\_{x,y,z}()

```cpp
/*The return type is described as auto&& because it is a long template type name.*/
auto&& get_axis_x(T& c)
auto&& get_axis_y(T& c)
auto&& get_axis_z(T& c)
```

This function generates a virtual container class from which one of the XYZ axes of the container class containing `axis_xyzt` is taken. Only the `begin()` and `end()` methods are implemented in this generated class. The iterator that can be obtained by these `begin()` and `end()` methods is the same as the iterator in the previous section [get\_axis\_{x,y,z}\_iter()](axis\_xyzt.md#get\_axis\_-x-y-z-\_iter).

```cpp
#include <algorithm>

void myfunc() {
  // container class
  smplbuf_local<axis_xyzt, 10> buf;
  
  // Submit data for testing
  buf[0] = { 1, 2, 3, 4 };
  buf[1] = { 2, 3, 4, 5 };
  ...
  
  // Extract the X axis in the queue
  auto&& vx = get_axis_x(que);
  
  // Use of ranged for statement
  for (auto&& e : vx) { Serial << int(e) << ','; }
  
  // Algorithm to obtain maximum and minimum values
  auto&& minmax = std::minmax_element(
      vx.begin(), vx.end());
                          
  Serial << "min=" << int(*minmax.first)
        << ",max=" << int(*minmax.second) << mwx::crlf;
}
```
