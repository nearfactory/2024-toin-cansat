---
title: mwx::stream
author: "Mono Wireless Inc."
description: input-output stream
---

# mwx::stream

Upper-level class that handles input/output streams.

* Interfaces to several classes (`Serial, Wire, SPI, smplbuf`) by polymorphism using CRTP (Curiously Recurring Template Pattern) method.
  * In CRTP, lower classes are defined as `template class Derived : public stream<Derived>;` and methods of lower classes are referenced from upper classes.
* This class defines common processing such as `print` method, `<<` operator, etc., and calls `write()` method, etc. implemented in lower classes, which is similar to using virtual functions.

## Interfaces (implemented in lower classes)

Lower classes implement the functions listed below.

### available()

```cpp
int available()

// example
while(Serial.available()) {
  int c = Serial.read();
  // ... any
}
```

Returns 1 if the input exists, 0 if it does not.

| Parameters | Description |
| --------- | ---------------- |
| Return value `int` | 0: no data 1: data present |

{% hint style="warning" %}
The return value of this implementation is not the buffer length.
{% endhint %}

###

### flush()

```cpp
void flush()

// example
Serial.println("long long word .... ");
Serial.flush();
```

Flush output (wait for output to complete).



### read()

```cpp
int read()

// example
int c;
while (-1 != (c = read())) {
    // any
}
```

Inputs 1-byte data from the stream. If the data does not exist, return `-1`.



### write()

```cpp
size_t write(int c)

// example
Serial.write(0x30);
```

Outputs 1 byte to the stream.

| Parameters | Description |
| ------------ | ------------------- |
| `n` | The character you want to output.            | Return value `size_t` | 1 if output succeeds, 0 otherwise.
| Return value `size_t` | 1 if output succeeds, 0 if it fails.



### vOutput()

```cpp
static void vOutput(char out, void* vp)
```

This is a static function that produces a single byte output. Since this is not a class method, member variables and other information are not available. Instead, a pointer to the class instance is passed to vp, which is passed as a parameter.

This static function is used internally and the function pointer is passed as a one-byte output function of `fctprintf()`. This is used to implement the `print` method, etc.

| Parameters | Description |
| ----- | ------------------------------------------------------------ |
| `out` | the character to output |
| `vp` | pointer to a class instance<br> Usually, cast to the original class and call the write() method |



## Interface

### putchar()

```cpp
void mwx::stream::putchar(char c)

// example
Serial.putchar('A');
// result -> A
```

Output a single byte.

### print(), println()

```cpp
size_t print(T val, int base = DEC) // T: 整数型
size_t print(double val, int place = 2)
size_t print(const char*str)
size_t print(std::initializer_list<int>)

// example
Serial.print("the value is ");
Serial.print(123, DEC);
Serial.println(".");
// result -> the value is 123.

Serial.print(123.456, 1);
// result -> 123.5

Serial.print({ 0x12, 0x34, 0xab, 0xcd });
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.
```

Performs various types of formatted output.

| Paramita | Explanation |
| ------------ | --------------------------------------------------------- |
| `val` | Integer powerups |
| `base` | power form<br>BIN binary / OCT 8 math / DEC 10 math / HEX 16 math |
| `place` | Number of decimals below the decimal point
| | |
| back `size_t` | the number of booklets |



### printfmt()

```cpp
size_t printfmt(const char* format, ...);

// example 
Serial.printfmt("the value is %d.", 123);
// result -> the value is 123.
```

Prints output in printf format.

> TWESDK/TWENET/current/src/printf/README.md 参照



### operator <<

```cpp
// examples
Serial << "this value is" // const char*
       << int(123)
       << '.';
       << mwx::crlf;
// result -> this value is 123.

Serial << fromat("this value is %d.", 123) << twe::crlf;
// result -> this value is 123.

Serial << mwx::flush; // flush here

Serial << bigendian(0x1234abcd);
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Serial << int(0x30) // output 0x30=48, "48"
       << '/'
       << uint8_t(0x31); // output '1', not "48"
// result -> 48/1

smplbuf<char,16> buf = { 0x12, 0x34, 0xab, 0xcd };
Serail << but.to_stream();
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Seiral << make_pair(buf.begin(), buf.end());
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

Serial << bytelist({ 0x12, 0x34, 0xab, 0xcd });
// will output 4byte of 0x12 0x34 0xab 0xcd in binary.

```

| Argument type | Explanation                                                                                                                                        |
| -------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| `char` | 1-byte output (not formatted as a number)
| 1-byte output (not formatted as a number) |
| `double` | numeric output (printf's "%.2f") |
| `uint8_t` | output 1 byte (same as char type) |
| `uint16_t` | output 2 bytes (big-endian order) |
| `uint32_t` | output 4 bytes (big-endian order) | `const char*` `const_t` | output 1 byte (same as char type)
| `const char*` `uint8_t*` `const char[S]` | Output up to the terminating character. Output does not include the terminating character. >(`S` specifies the size of the fixed array) |
| `uint8_t[S]` | Output the array size `S` bytes as is. (`S` is the fixed array size specification) |
| `format()` | output in printf format |
| `mwx::crlf` | output of newline CRLF | `mwx::flush` | output of newline CRLF
| `mwx::flush` | flush output |
| `bigendian()` | output numeric types in big-endian order. (right-hand side value) |
| `std::pair<T*, T*>` | A pair containing `begin(), end()` pointers of byte type. Can be created by `make_pair`. T` is assumed to be of type `uint8_t`. (right-hand side value) |
| output byte string using `bytelist()` | `std::initializer_list`.
| `smplbuf<uint8_t,AL>&` | Output the contents of an array class of type `uint8_t`. `ALC` is <a href=". /alloc.md">memory allocation method</a>.                                     |
| `smplbuf<uint8_t, AL>::to_stream()` | Outputs data of `smplbuf&#x3C;T>` <br>`T` is of type `uint8_t`, `AL` is a <a href=". /alloc.md">memory allocation method</a>. |

{% hint style="info" %}
When outputting as a byte string, cast to `uint8_t, uint16_t, uint32_t` type. When outputting numerical values as strings, explicitly cast to `int` type.
{% endhint %}

{% hint style="info" %}
Single-byte types are handled differently depending on the type name. Usually, use the size-conscious `uint8_t[S]` type.
{% endhint %}



### set\_timeout(), get\_error\_status(),  clear\_error\_status()

```cpp
uint8_t get_error_status()
void clear_error_status()
void set_timeout(uint8_t centisec)

// example
Serial.set_timeout(100); // 1000msのタイムアウトを設定
uint8_t c;
Serial >> c;
```

Manages input timeouts and errors using the `>>` operator.

The timeout period is specified by `set_timeout()`, and input is processed using the `>>` operator. If no input is received within the given timeout period, the error value can be read out with `get_error_status()`. The error status is cleared by `clear_error_status()`.

| Argument type | Description |
| ---------- | ----------------------------------------------------------------------------- |
| `centisec` | Sets the timeout period in units of 1/10 second. If `0xff` is specified, timeout is disabled. | `0xff`.

#### Error Value

| Value | Meaning |
| - | ----- |
0 | No Error | 1 | Error Status
| 1 | Error Status |



### operator >>

```cpp
inline D& operator >> (uint8_t& v)
inline D& operator >> (char_t& v)
template <int S> inline D& operator >> (uint8_t(&v)[S])
inline D& operator >> (uint16_t& v)
inline D& operator >> (uint32_t& v)
inline D& operator >> (mwx::null_stream&& p)

//// Example
uint8_t c;

the_twelite.stop_watchdog(); // stop watchdog
Serial.set_timeout(0xFF); // no timeout

// read out 1 byte
Serial >> c;
Serial << crlf << "char #1: [" << c << ']';

// skipping
Serial >> null_stream(3); // Read away 3 bytes
Serial << crlf << "char #2-4: skipped";

// Read 4 bytes (limited to fixed-length arrays of type uint8_t)
uint8_t buff[4];
Serial >> buff;
Serial << crlf << "char #5-8: [" << buff << "]";
```

Input processing.

{% hint style="danger" %}
* Cannot be executed within `setup()`.
* Because it waits for polling, depending on the timeout time setting (e.g. no timeout), the watchdog timer may be triggered and reset.
{% endhint %}

{% hint style="info" %}
Normally, the following readout is performed during `loop()`.

```cpp
void loop() {
  uint8_t c;
  while(Serial.available()) {
    Serial >> c;
    // Or c = Serial.read();
    
    switch(c) { ... }  // Branch processing according to the value of c
  }
}
```
{% endhint %}



The following is a list of types that can be read and stored.

| Argument type | Explanation                                                            |
| -------------------- | ------------------------------------------------------------- |
| 1-byte input (big-endian order)
| 2-byte input (big-endian order) |
| `uint32_t` | 4-byte input (big-endian order) |
| `uint8_t[S]` | input for `S` bytes (`S` specifies fixed array size) |
| `null_stream(int n)` | read `n` bytes away |

