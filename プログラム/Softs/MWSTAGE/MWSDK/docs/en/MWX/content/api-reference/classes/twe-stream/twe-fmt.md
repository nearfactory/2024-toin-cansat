---
title: format (mwx::mwx_format)
author: "Mono Wireless Inc."
description: Enter printf format in mwx::stream
---

# format (mwx::mwx\_format)

Helper class for writing format format to the << operator of `mwx::stream`. In the library, it is alias defined as `Using format=mwx::mwx_format;`.

```cpp
Serial << format("formatted print: %.2f", (double)3123 / 100.) << mwx::crlf;

// formatted print: 31.23[改行]
```

{% hint style="warning" %}
The maximum number of arguments that can be registered in the variable number argument list is 8. 64-bit parameters such as double and uint64\_t type are limited in number. If the limit is exceeded, a compile error will result due to static↵_assert.
{% endhint %}

* Store the argument list received in the constructor in a variable internal to the class using the expand function of the parameter pack.
* Call `fctprintf()` at the point `operator <<` is called to write data to the stream



### Constructor

```cpp
format(const char *fmt, ...)
```

The constructor stores the format pointer and parameters. The subsequent call with the `<<` operator interprets the format and processes the output.

| Parameters | Description                                                                                                          |
| ----- | ----------------------------------------------------------------------------------------------------------- |
| `fmt` | Format format. See TWESDK/TWENET/current/src/printf/README.md |
| `... ` | Parameters according to the format format. <br>* The maximum number of parameters is 4. 5 or more parameters will result in a compile error. <br>* Since consistency with the format is not checked, it is not safe for inconsistent input.  |

{% hint style="warning" %}
The `fmt` must remain accessible until this object is destroyed.
{% endhint %}



