---
title: "MWX_APIRET"
author: "Mono Wireless Inc."
description: "MWX_APIRET"
---

# MWX\_APIRET

32bit型をラップしたAPI戻り値クラス。MSB(bit31)は失敗、成功のフラグ。bit0..30は戻り値を格納するために使用します。

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



## コンストラクタ

```cpp
MWX_APIRET()
MWX_APIRET(bool b)
MWX_APIRET(bool b, uint32_t val)
```

デフォルトコンストラクタは`false`,`0`の組み合わせで構築します。

また`bool`型と`uint32_t`型をパラメータとする明示的な構築も可能です。

`bool`型のコンストラクタを実装しているため、以下のように`true`/`false`を用いることができます。

```cpp
MWX_APIRET myfunc() {
  if (...) return true;
  else false;
}
```



## メソッド

### is\_success(), operator bool()

```cpp
inline bool is_success() 
inline operator bool()
```

MSBに`1`がセットされていれば`true`を返す。



```cpp
inline bool is_fail()
```

MSBが`0`の場合に`true`を返す。



```cpp
inline uint32_t get_value()
inline operator uint32_t()
```

bit0..30の値部を取得する。





