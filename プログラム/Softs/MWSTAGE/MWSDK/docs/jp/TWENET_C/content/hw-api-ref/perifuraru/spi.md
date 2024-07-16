# SPI

SPIバスはセンサーやメモリデバイス等の読み書きに利用されます。I2C に比べ使用するピン数が多いものの、高速な転送が必要なデバイスによく使用されます。

{% hint style="info" %}
Samp_SPI サンプルを参考にしてください。
{% endhint %}

SPIバスを利用する場合、SPI セレクトピンを必要なデバイス分だけ確保されます。ピンは SEISEL0 のピンから順に割り当てられます。

以下に初期化と読み書きの例を挙げます。SPIでは、書き込みと読み出しを同時に行うこともできます。

センサーでのデータ取得時に一般的な例として、コマンド送信して続いて結果を得るという手続きを挙げます。

1. SPIの初期化（デバイス使用開始）
2. SPI のセレクトピンの有効化
3. SPI 転送１ (8ビット)、送信を目的とします。
4. SPI 転送２ (16ビット)、受信を目的とします。

{% hint style="danger" %}
各転送で必ずポーリング待ち(`while(bAHI_SpiPollBusy());`)を行う必要があります。
{% endhint %}

```c
#define SLAVE_ENABLE2		(1)			//	DIO19とDIO0を用いる
#define CS_DIO19			(0x01)		//	SPISEL0:DIO19に接続したものを使う
#define CS_DIO0				(0x02)		//	SPISEL1:DIO0に接続したものを使う

    ...
    // SPI の初期化
    vAHI_SpiConfigure(SLAVE_ENABLE2, // SPISEL0,1 の２つを利用する
					  E_AHI_SPIM_MSB_FIRST,
					  TRUE,
					  TRUE,
					  1,
					  E_AHI_SPIM_INT_DISABLE,
					  E_AHI_SPIM_AUTOSLAVE_DSABL);

	...
    // 転送１（8bitの書き出し)
    vAHI_SpiSelect(CS_DIO0); // SPISEL1 を有効にする
    vAHI_SpiStartTransfer(
					7, // 7+1=8 bit の転送
					0x00000012L ); // 0x12 を送信
    while(bAHI_SpiPollBusy()); // 読み書きのポーリング待ち
    //uint8 u8val = u8AHI_SpiReadTransfer8();
        // SPIMISO の入力データを読み出しは可能。
        // この例では、u8val は使用しない。

    // 必要なら少し時間を置く。
    vWait(100);
    
    //　転送２ (16bitの読み込み)
    vAHI_SpiStartTransfer(
					15, // 15+1=16 bit の転送
					0x00000000L ); // ダミーの送信データ。
    while(bAHI_SpiPollBusy()); // 読み書きのポーリング待ち。
    uint16 u16val = u16AHI_SpiReadTransfer16();
        // 読み出した 16bit データ。

    // セレクトを解除する
    vAHI_SpiStop();

    ...
    // SPIデバイスの利用を中止する。
    vAHI_SpiDisable();
```



## 主要関数

### vAHI_SpiConfigure() 

```c
void vAHI_SpiConfigure(
    uint8 u8SlaveEnable,
    bool_t bLsbFirst,
    bool_t bPolarity,
    bool_t bPhase,
    uint8 u8ClockDivider,
    bool_t bInterruptEnable,
    bool_t bAutoSlaveSelect);
```

本関数は SPI デバイスの初期化を行い利用可能にします。SPIデバイスを解放する場合は `vAHI_SpiDisable()` を呼びます。

{% hint style="warning" %}
セレクトピンは最低でも SPISEL0 がSPI専用として割り当てられます。つまり SPIを使用する場合 SPISEL0(DIO19) は他の用途には使用できません。
{% endhint %}

| 引数名                | 解説                                                                                              |
| ------------------ | ----------------------------------------------------------------------------------------------- |
| `u8SalveEnable`    | 有効にするチップセレクトピンを指定する。`0`を指定すると SPISEL0を予約、`1`を指定すると SPISEL0,1 を予約し、`2`を指定すると SPISEL0,1,2 を予約します。 |
| `bLsbFirst`        | `TRUE` ならLSB(下位ビット)からデータが始まる                                                                    |
| `bPolarity`        | `TRUE` ならクロックを反転する                                                                              |
| `bPhase`           | `TRUE` なら立ち下がりエッジを用いる                                                                           |
| `u8ClockDiviser`   | クロック周波数の指定。0..63 を指定する。0 では 16MHz 駆動し、それ以外は  `2*u8ClockDiviser` で分周する。                          |
| `bInterruptEnable` | SPI転送終了時に割り込みを発生させる。※ サンプル等では使用していません。                                                          |
| `bAutoSlaveSelect` | `TRUE`の場合セレクトピンの制御をSPI転送中のみの自動制御とする。`FALSE`の場合 `vAHI_SpiSelect()` の呼び出しタイミングで設定されます。            |

{% hint style="info" %}
`bLsbFirst`, `bPolarity`, `bPhase`, `u8ClockDiviser` は接続するデバイスのデータシートより値を決めてください。
{% endhint %}

#### SPI Mode と `bPolarity`, `bPhase` の関係

| SPI Mode | `bPolarity` | `bPhase` |
| -------- | ----------- | -------- |
| 0        | `FALSE`     | `FALSE`  |
| 1        | `FALSE`     | `TRUE`   |
| 2        | `TRUE`      | `FALSE`  |
| 3        | `TRUE`      | `TRUE`   |



### vAHI_SpiSelect()

```c
void vAHI_SpiSelect(uint8 u8SlaveMask);
```

セレクトピンを有効化します。

| 引数名           | 解説                                                       |
| ------------- | -------------------------------------------------------- |
| `u8SalveMask` | `1`: SPISEL0, `2`:SPISEL1, `4`:SPISEL3, `0`: セレクトを解除します。 |

{% hint style="info" %}
`vAHI_SpiStop()` は `vAHI_SpiSelect(0)` の呼び出しと同じです。
{% endhint %}



### vAHI_SpiStartTransfer()

```c
void vAHI_SpiStartTransfer(uint8 u8CharLen, uint32 u32Out);
```

SPI転送を開始します。本関数呼び出し直後に転送を開始しますが、転送の終了を待たずに制御を返します。後述の `bAHI_SpiPollBusy()` により転送終了を待ちます。

| 引数名         | 解説                                                                       |
| ----------- | ------------------------------------------------------------------------ |
| `u8CharLen` | 転送長を指定する。`0..31` が指定でき、指定した数 + 1 ビットの転送を行う。例えば 16 ビットの転送であれば `15` を指定する。 |
| `u32Out`    | 出力するビット列。32bitに満たない転送の場合、データは LSB 側に右詰めする。（8bit の場合、bit0 〜 bit7 に格納する）   |



### bAHI_SpiPollBusy()

```c
bool_t bAHI_SpiPollBusy(void);
```

SPI 転送のポーリング待ちを行う。`TRUE` の間は SPI の転送中を意味します。



### u32AHI_SpiReadTransfer32()

```c
 uint32 u32AHI_SpiReadTransfer32(void);
 uint16 u16AHI_SpiReadTransfer16(void);
 uint8 u8AHI_SpiReadTransfer8(void);
```

SPI転送が終了した後に呼び出し、転送時の受信データ (SPIMISO) を読み出します。読み出したデータは LSB 側に右詰めされています。
