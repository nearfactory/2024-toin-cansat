# utils.h

utils.h をインクルードすることで利用できるマクロを紹介します。

## S_OCTET(x)

１バイトをメモリを書き込む。

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

uint8 \*q をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。代入演算子の評価後 q++ が実行される。

## S_BE_WORD(x)

２バイトをメモリを書き込む。

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

uint8 \*q をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。代入演算子の評価後 q+=2 が実行される。

BE はビッグエンディアン、LE はリトルエンディアン。

## S_BE_DWORD(x)

４バイトをメモリを書き込む。

```c
uint8 *q = &sTx.au8Data[0];

S_OCTET(0x12);　
S_BE_WORD(0x1234); 
S_BE_DWORD(0x12345678);
```

uint8 \*q をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。代入演算子の評価後 q+=4 が実行される。

BE はビッグエンディアン、LE はリトルエンディアン。

## G_OCTET()

１バイトメモリを読み込み uint8 型の変数に値を格納する。

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

uint8 \*p をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。=演算子の評価後 p++ が実行される。

## G_BE_WORD()

２バイトメモリを読み込み uint16 型の変数に値を格納する。

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

uint8 \*p をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。=演算子の評価後 p+=2 が実行される。

BE はビッグエンディアン、LE はリトルエンディアン。

## G_BE_DWORD()

１バイトメモリを読み込み uint8 型の変数に値を格納する。

```c
uint8 *p = &sRx.au8Data[0];

uint8 u8data1 = OCTET();　
uint16 u16data2 = G_BE_WORD(); 
uint32 u32data3 = G_BE_DWORD();
```

uint8 \*p をローカル変数として宣言しておき、データを読み込みたい領域のポインタとしておく。=演算子の評価後 p+=4 が実行される。

BE はビッグエンディアン、LE はリトルエンディアン。

## ENCODE_VOLT(x)

2000～3600 の値を 8bit 値に変換します。

* 1.95\~2.80V は 5mV 刻み
* 2.81\~3.65V は 10mV 刻み

```c
// utils.h の定義
#define ENCODE_VOLT(m) \
	(m < 1950 ? 0 : \
		(m > 3650 ? 255 : \
			(m <= 2802 ? ((m-1950+2)/5) : ((m-2800-5)/10+171)) ))

...
uint16 u16Volt = 2860;
uint8 u8Volt_enc = ENCODE_VOLT(u16Volt);
uint16 u16Volt_dec = DECODE_VOLT(u8Volt_Enc);
```

2000～2800 の値は 5 刻み、2800～は10 刻みで 8bit 値に割り当てます。

## DECODE_VOLT(x)

ENCODE_VOLT() により得られた8bit値を元の値に戻します。

* 1.95\~2.80V は 5mV 刻み
* 2.81\~3.65V は 10mV 刻み

```c
// utils.h の定義
#define DECODE_VOLT(i) \
	(i <= 170 ? (1950+i*5) : (2800+(i-170)*10) )

...
uint16 u16Volt = 2860;
uint8 u8Volt_enc = ENCODE_VOLT(u16Volt);
uint16 u16Volt_dec = DECODE_VOLT(u8Volt_Enc);
```

2000～2800 の値は 5 刻み、2800～は10 刻みで 8bit 値に割り当てます。

## vPortAsInput(c)

ポートcを入力に設定する

```c
#define vPortAsInput(c) vAHI_DioSetDirection(1UL << (c), 0)
```

## vPortAsOutput(c) <a href="vportasinput-c" id="vportasinput-c"></a>

ポートcを出力に設定する

```c
#define vPortAsOutput(c) vAHI_DioSetDirection(0, 1UL << (c))
```

## ​vPortSetHi(c)

ポートcをHi状態にする

```c
#define vPortSetHi(c) vAHI_DioSetOutput(1UL << (c), 0)
```

## ​​vPortSetLo(c)

ポートcをLo状態にする

```c
#define vPortSetLo(c) vAHI_DioSetOutput(0, 1UL << (c))
```

## ​​​vPortSet_TrueAsLo(c, s)

ポート c を s が TRUE なら Lo, FALSE なら Hi に設定する

```c
#define vPortSet_TrueAsLo(c, s)  vAHI_DioSetOutput((s) ? \
    0 : 1UL << (c), s ? 1UL << (c) : 0)
```

## bPortRead(c)

ポート c を読み出す。Loレベルなら TRUE が返る

```c
#define bPortRead(c) ((u32AHI_DioReadInput() & \
    (1UL<<(c))) ? FALSE : TRUE)
```

## ​u32PortReadBitmap()

ポート c を読み出す。Loレベルなら TRUE が返る。

```c
#define u32PortReadBitmap() (u32AHI_DioReadInput())
```

{% hint style="warning" %}
ビットマップの1がHi,0がLoとなります。
{% endhint %}

## ​bPortCheckBitmap(bitmap, c)

読みだしたビットマップのポート c に対応するビットがLoレベルならTRUEを返す。

```c
#define bPortCheckBitmap(bitmap, c) \
    (bitmap & (1UL<<(c))) ? FALSE : TRUE)
```

## vPortDisablePullup(c)

ポート c のプルアップを停止する。

```c
#define vPortDisablePullup(c) vAHI_DioSetPullup(0x0, 1UL << (c))
```

## \_C

switch でスコープを定義したい場合  \_C { … } と記述している。

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

改行コード (CRLF) 文字列です。

{% hint style="warning" %}
２バイトの文字列リテラルですので、[vPutChar](../../hw-api-ref/perifuraru/uart/fprintf-raiburari/vputchar.md)() では利用できません。
{% endhint %}

```c
#define LB "\r\n"

// for example
vfPrintf(&sSerStream, "HELLO WORLD!" LB);
```
