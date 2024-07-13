# utils.h

Here are the macros you can use by including `utils.h`.

## S_OCTET(x)

Write one byte of memory.

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

Declare `uint8 *q` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the assignment operator, `q++` is executed.

## S_BE_WORD(x)

Write 2 bytes of memory.

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

Declare `uint8 *q` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the assignment operator, `q+=2` is executed.

BE represents Big Endian.

## S_BE_DWORD(x)

Write 4 bytes of memory.

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

Declare `uint8 *q` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the assignment operator, `q+=4` is executed.

BE represents Big Endian.

## G_OCTET()

Reads a single byte of memory and stores the value in a variable of type `uint8`.

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

Declare `uint8 *p` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the `=` operator, `p++` is executed.

## G_BE_WORD()

２バイトメモリを読み込み uint16 型の変数に値を格納する。

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

Declare `uint8 *p` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the `=` operator, `p+=2` is executed.

BE represents Big Endian.

## G_BE_DWORD()

１バイトメモリを読み込み uint8 型の変数に値を格納する。

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

Declare `uint8 *p` as a local variable and use it as a pointer to the area where you want to read the data. After evaluating the `=` operator, `p+=4` is executed.

BE represents Big Endian.

## ENCODE_VOLT(x)

Converts values between 2000 and 3600 into 8-bit values.

* 1.95\~2.80V in steps of 5mV
* 2.81\~3.65V in steps of 10mV

```c
// definition at utils.h
#define ENCODE_VOLT(m) \
	(m < 1950 ? 0 : \
		(m > 3650 ? 255 : \
			(m <= 2802 ? ((m-1950+2)/5) : ((m-2800-5)/10+171)) ))

...
uint16 u16Volt = 2860;
uint8 u8Volt_enc = ENCODE_VOLT(u16Volt);
uint16 u16Volt_dec = DECODE_VOLT(u8Volt_Enc);
```

Values between 2000 and 2800 are assigned to 8-bit values in steps of 5 and from 2800 in steps of 10.

## DECODE_VOLT(x)

Returns the 8-bit value obtained by `ENCODE_VOLT()` to its original value.

* 1.95\~2.80V in steps of 5mV
* 2.81\~3.65V in steps of 10mV

```c
// definition at utils.h の定義
#define DECODE_VOLT(i) \
	(i <= 170 ? (1950+i*5) : (2800+(i-170)*10) )

...
uint16 u16Volt = 2860;
uint8 u8Volt_enc = ENCODE_VOLT(u16Volt);
uint16 u16Volt_dec = DECODE_VOLT(u8Volt_Enc);
```

Values between 2000 and 2800 are assigned to 8-bit values in steps of 5 and from 2800 in steps of 10.

## vPortAsInput(c)

Set port `c` as input

```c
#define vPortAsInput(c) vAHI_DioSetDirection(1UL << (c), 0)
```

## vPortAsOutput(c) <a href="vportasinput-c" id="vportasinput-c"></a>

Set port `c` as output

```c
#define vPortAsOutput(c) vAHI_DioSetDirection(0, 1UL << (c))
```

## ​vPortSetHi(c)

Set port `c` to Hi state

```c
#define vPortSetHi(c) vAHI_DioSetOutput(1UL << (c), 0)
```

## ​​vPortSetLo(c)

Set port `c` to Lo state

```c
#define vPortSetLo(c) vAHI_DioSetOutput(0, 1UL << (c))
```

## ​​​vPortSet_TrueAsLo(c, s)

Set port `c` to Lo if `s` is `TRUE`, Hi if `FALSE`.

```c
#define vPortSet_TrueAsLo(c, s)  vAHI_DioSetOutput((s) ? \
    0 : 1UL << (c), s ? 1UL << (c) : 0)
```

## bPortRead(c)

Reads port `c`, returns `TRUE` if Lo level.

```c
#define bPortRead(c) ((u32AHI_DioReadInput() & \
    (1UL<<(c))) ? FALSE : TRUE)
```

## ​u32PortReadBitmap()

Reads port `c`, returning `TRUE` if Lo level.

```c
#define u32PortReadBitmap() (u32AHI_DioReadInput())
```

{% hint style="warning" %}
Bitmap `1` is Hi, `0` is Lo.
{% endhint %}

## ​bPortCheckBitmap(bitmap, c)

If the bit corresponding to port `c` of the read bitmap is Lo level, `TRUE` is returned.

```c
#define bPortCheckBitmap(bitmap, c) \
    (bitmap & (1UL<<(c))) ? FALSE : TRUE)
```

## vPortDisablePullup(c)

Stop the pull-up on port `c`.

```c
#define vPortDisablePullup(c) vAHI_DioSetPullup(0x0, 1UL << (c))
```

## \_C

If you want to define a scope with `switch`, you have to write `_C { ... }`.

```c
#define _C if(1)

// for example
switch(c) {
case 1:
  _C {
    uint8 u8work;
    ; // work
  } break;
default:
}
```

## LB

A newline code (CRLF) string.

{% hint style="warning" %}
This is a 2-byte string literal, so it cannot be used with [`vPutChar()`](../../hw-api-ref/perifuraru/uart/fprintf-raiburari/vputchar.md).
{% endhint %}

```c
#define LB "\r\n"

// for example
vfPrintf(&sSerStream, "HELLO WORLD!" LB);
```
