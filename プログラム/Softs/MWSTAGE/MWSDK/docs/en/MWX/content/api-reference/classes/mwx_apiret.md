---
title: MWX_APIRET
author: "Mono Wireless Inc."
---

# MWX\_APIRET

API return value class that wraps 32-bit type. MSB (bit31) is a flag for failure or success. bit0..30 is used to store the return value.

```cpp
class MWX_APIRET {
	uint32_t _code;
public:
	MWX_APIRET() : _code(0) {}
	MWX_APIRET(bool b) {
	  _code = b ? 0x80000000 : 0;
  }
	MWX_APIRET(bool b, uint32_t val) {
		_code = (b ? 0x80000000 : 0) + (val & 0x7fffffff);
	}
	inline bool is_success() const { return ((_code & 0x80000000) != 0); }
	inline bool is_fail() const { return ((_code & 0x80000000) == 0); }
	inline uint32_t get_value() const { return _code & 0x7fffffff; }
	inline operator uint32_t() const { return get_value(); }
	inline operator bool() const { return is_success(); }
};
```



## constructor

```cpp
MWX_APIRET()
MWX_APIRET(bool b)
MWX_APIRET(bool b, uint32_t val)
```

The default constructor is constructed with a combination of `false` and `0`.

It can also be explicitly constructed with `bool` and `uint32_t` types as parameters.

Since the constructor of type `bool` is implemented, you can use `true`/`false` as follows.

```cpp
MWX_APIRET myfunc() {
  if (...) return true;
  else false;
}
```



## Methods

### is\_success(), operator bool()

```cpp
inline bool is_success() 
inline operator bool()
```

Return `true` if `1` is set in MSB.



```cpp
inline bool is_fail()
```

Return `true` if MSB is `0`.



```cpp
inline uint32_t get_value()
inline operator uint32_t()
```

Obtain the value part of bit0..30.





