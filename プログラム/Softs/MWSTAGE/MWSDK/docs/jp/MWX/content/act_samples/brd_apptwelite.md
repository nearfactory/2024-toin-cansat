---
title: BRD_APPTWELITE
author: "Mono Wireless Inc."
description: IO通信(標準アプリケーションApp_Tweliteの基本機能)
---

# BRD\_APPTWELITE

App\_TweLite で必要な配線と同じ配線想定したボードサポート `<BRD_APPTWELITE>` を用いたサンプルです。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの送受信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ディジタル（ボタン）入力 - [Buttons](../api-reference/predefined\_objs/buttons.md)
* アナログ入力 - [Analogue](../api-reference/predefined\_objs/analogue.md)
{% endhint %}

{% hint style="warning" %}
このサンプルは App\_TweLite と通信できません。
{% endhint %}

## アクトの機能

* M1を読み取り、親機か子機を決める。
* DI1-DI4 の値を読み取ります。Buttons クラスにより、チャタリングの影響を小さくするため、連続で同じ値になったときにはじめて変化が通知されます。変化があったときには通信を行います。
* AI1-AI4 の値を読み取ります。
* DIの変化または１秒おきに、DI1-4, AI1-4, VCC の値を、自身が親機の場合は子機へ、子機の場合は親機宛に送信します。
* 受信したパケットの値に応じで DO1-4, PWM1-4 に設定する。

## アクトの使い方

### 必要なTWELITEと配線例

| 役割 | 例                                                                                                                                           |
| -- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>最低限 M1=GND, DI1:ボタン, DO1:LEDの配線をしておく。  |
| 子機 | <a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>最低限 M1=オープン, DI1:ボタン, DO1:LEDの配線をしておく。 |

![配線例 (AI1-AI4は省略可)](<../.gitbook/assets/image (19).png>)



## アクトの解説

### インクルード

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <BRD_APPTWELITE>
#include <STG_STD>
```

全てのアクトで`<TWELITE>`をインクルードします。ここでは、シンプルネットワーク [`<NWK_SIMPLE>`](../networks/nwk\_simple.md) とボードサポート `<BRD_APPTWELITE>`をインクルードしておきます。

またインタラクティブモードを追加するために `<STG_STD>` をインクルードしています。

### 宣言部

```cpp
/*** Config part */
// application ID
const uint32_t DEFAULT_APP_ID = 0x1234abcd;
// channel
const uint8_t DEFAULT_CHANNEL = 13;
// option bits
uint32_t OPT_BITS = 0;
// logical id
uint8_t LID = 0;

/*** function prototype */
MWX_APIRET transmit();
void receive();

/*** application defs */
const char APP_FOURCHAR[] = "BAT1";

// sensor values
uint16_t au16AI[5];
uint8_t u8DI_BM;
```

* サンプルアクト共通宣言
* 長めの処理を関数化しているため、そのプロトタイプ宣言（送信と受信）
* アプリケーション中のデータ保持するための変数

### セットアップ setup()

```cpp
void setup() {
	/*** SETUP section */	
	// init vars
	for(auto&& x : au16AI) x = 0xFFFF;
	u8DI_BM = 0xFF;

	// load board and settings
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

	// settings: configure items
	set << SETTINGS::appname("BRD_APPTWELITE");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
	set.reload(); // load from EEPROM.
	OPT_BITS = set.u32opt1(); // this value is not used in this example.
	LID = set.u8devid(); // logical ID

	// the twelite main class
	the_twelite
		<< set                      // apply settings (appid, ch, power)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	if (brd.get_M1()) { LID = 0; }

	// Register Network
	nwk << set // apply settings (LID and retry)
			;

	// if M1 pin is set, force parent device (LID=0)
	nwk << NWK_SIMPLE::logical_id(LID); // write logical id again.
	
	/*** BEGIN section */
	// start ADC capture
	Analogue.setup(true, ANALOGUE::KICK_BY_TIMER0); // setup analogue read (check every 16ms)
	Analogue.begin(pack_bits(
						BRD_APPTWELITE::PIN_AI1,
						BRD_APPTWELITE::PIN_AI2,
						BRD_APPTWELITE::PIN_AI3,
						BRD_APPTWELITE::PIN_AI4,
				   		PIN_ANALOGUE::VCC)); // _start continuous adc capture.

	// Timer setup
	Timer0.begin(32, true); // 32hz timer

	// start button check
	Buttons.setup(5); // init button manager with 5 history table.
	Buttons.begin(pack_bits(
						BRD_APPTWELITE::PIN_DI1,
						BRD_APPTWELITE::PIN_DI2,
						BRD_APPTWELITE::PIN_DI3,
						BRD_APPTWELITE::PIN_DI4),
					5, 		// history count
					4);  	// tick delta (change is detected by 5*4=20ms consequtive same values)	


	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial 	<< "--- BRD_APPTWELITE ---" << mwx::crlf;
	Serial	<< format("-- app:x%08x/ch:%d/lid:%d"
					, the_twelite.get_appid()
					, the_twelite.get_channel()
					, nwk.get_config().u8Lid
				)
			<< mwx::crlf;
	Serial 	<< format("-- pw:%d/retry:%d/opt:x%08x"
					, the_twelite.get_tx_power()
					, nwk.get_config().u8RetryDefault
					, OPT_BITS
			)
			<< mwx::crlf;
}
```

大まかな流れは、各部の初期設定、各部の開始となっています。



#### 各種ビヘイビアオブジェクトの登録

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
```

&#x20;システムの振る舞いを決めるためのビヘイビアオブジェクトを登録します。インタラクティブモードの設定管理で合ったり、ボードサポート、また無線パケットのネットワーク記述です。

{% hint style="warning" %}
`setup()`内で登録しないと動作しません。
{% endhint %}



#### インタラクティブモードの設定

```cpp
// インタラクティブモードの初期化
auto&& set = the_twelite.settings.use<STG_STD>();

set << SETTINGS::appname("BRD_APPTWELITE");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.
LID = set.u8devid(); // logical ID;
```

インタラクティブモードの初期化を行います。まず`set`オブジェクトを取得しています。続いて以下の処理を行っています。

* アプリケーション名を`"BRD_APPTWELITE"`に設定（メニューで利用される）
* デフォルトのアプリケーションIDとチャネル値を書き換える
* 不要な項目を削除する
* `set.reload()`により保存された設定値を読み出す
* `OPT_BITS`と`LID`の値を変数にコピーする

{% hint style="info" %}
読み出したインタラクティブモードの設定の反映については後述します。
{% endhint %}



以下は画面例です。+ + + と + を三回 0.2秒から 1 秒の間をあけて入力するとインタラクティブモード画面を出すことが出来ます。

```
[CONFIG/BRD_APPTWELITE:0/SID=8XXYYYYY]
a: (0x1234ABCD) Application ID [HEX:32bit]
i: (        13) Device ID [1-100,etc]
c: (        13) Channel [11-26]
x: (      0x03) RF Power/Retry [HEX:8bit]
o: (0x00000000) Option Bits [HEX:32bit]

 [ESC]:Back [!]:Reset System [M]:Extr Menu    
```

{% hint style="info" %}
※ Option Bits は、メニューに表示はしていますが、このサンプリでは使用していません。
{% endhint %}



#### the\_twelite

このオブジェクトはTWENETの中核としてふるまいます。

```cpp
auto&& brd = the_twelite.board.use<BRD_APPTWELITE>();
```

ボードの登録（このアクトでは`<BRD_APPTWELITE>`を登録しています）。以下のように use の後に `<>` で登録したいボードの名前を指定します。

ユニバーサル参照(`auto&&`)にて得られた戻り値として、参照型でのボードオブジェクトが得られます。このオブジェクトにはボード特有の操作や定義が含まれます。以下ではボードオブジェクトを用い、M1ピンの状態を確認しています。Ｍ1ピンがLOであれば、LID=0、つまり親機アドレスと設定します。

```cpp
	if (brd.get_M1()) { LID = 0; }
```



the\_twelite を動作させるには初期設定が必要です。アプリケーションIDや無線チャネルの設定は必須といえます。

```cpp
	// the twelite main class
	the_twelite
		<< set
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)
```

`the_twelite` に設定を反映するには `<<` を用います。

* `set`はインタラクティブモードから読み出した設定の一部（アプリケーションIDや無線チャネルなど）反映させます。反映される項目は[\<STG\_STD>](../settings/stg\_std.md#no-1)の解説を参照してください。
* `TWENET::rx_when_idle()` 受信回路をオープンにする指定です。

{% hint style="info" %}
`<<, >>`演算子は本来ビットシフト演算子ですが、その意味合いと違った利用とはなります。MWXライブラリ内では、C++標準ライブラリでの入出力利用に倣ってライブラリ中では上記のような設定やシリアルポートの入出力で利用しています。

```cpp
// 以下の記述は MWX ライブラリでは利用できません。
#include <iostream>
std::cout << "hello world" << std::endl;
```
{% endhint %}



次にネットワークを登録します。

```cpp
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
nwk << set;
nwk << NWK_SIMPLE::logical_id(LID);
```

1行目は、ボードの登録と同じ書き方で `<>` には `<NWK_SIMPLE>`を指定します。

2,3行目は、`<NWK_SIMPLE>`の設定です。先にインタラクティブモードの設定値を反映させます。反映される項目はLIDと再送回数です。このアプリケーションではM1ピンの状態によってLID=0にする場合があるため、3行目で再度LIDを設定しています。



#### Analogue

ADC(アナログディジタルコンバータ)を取り扱うクラスオブジェクトです。

```cpp
Analogue.setup(true, ANALOGUE::KICK_BY_TIMER0);
```

初期化`Analogue.setup()`で行います。パラメータの`true`はADC回路の安定までその場で待つ指定です。2番目のパラメータは、ADCの開始をTimer0に同期して行う指定です。

```cpp
	Analogue.begin(pack_bits(
						BRD_APPTWELITE::PIN_AI1,
						BRD_APPTWELITE::PIN_AI2,
						BRD_APPTWELITE::PIN_AI3,
						BRD_APPTWELITE::PIN_AI4,
				   	PIN_ANALOGUE::VCC));
```

ADCを開始するには`Analogue.begin()`を呼びます。パラメータはADC対象のピンに対応するビットマップです。

ビットマップを指定するのに`pack_bits()`関数を用います。可変数引数の関数で、各引数には１を設定するビット位置を指定します。例えば`pack_bits(1,3,5)`なら２進数で `101010`の値が戻ります。この関数は`constexpr`指定があるため、パラメータが定数のみであれば定数に展開されます。

パラメータと指定される`BRD_APPTWELITE::`には`PIN_AI1..4`が定義されています。App\_Tweliteで用いるAI1..AI4に対応します。AI1=ADC1, AI2=DIO0, AI3=ADC2, AI4=DIO2 と割り当てられています。`PIN_ANALOGUE::`にはADCで利用できるピンの一覧が定義されています。

{% hint style="info" %}
初回を除き ADC の開始は、割り込みハンドラ内で行います。
{% endhint %}



#### Buttons

DIO (ディジタル入力) の値の変化を検出します。Buttonsでは、メカ式のボタンのチャタリング（摺動）の影響を軽減するため、一定回数同じ値が検出されてから、値の変化とします。

```cpp
Buttons.setup(5);
```

初期化は `Buttons.setup()`で行います。パラメータの 5 は、値の確定に必要な検出回数ですが、設定可能な最大値を指定します。内部的にはこの数値をもとに内部メモリの確保を行っています。

```cpp
Buttons.begin(pack_bits(
						BRD_APPTWELITE::PIN_DI1,
						BRD_APPTWELITE::PIN_DI2,
						BRD_APPTWELITE::PIN_DI3,
						BRD_APPTWELITE::PIN_DI4),
					5, 		// history count
					4);  	// tick delta
```

開始は `Buttons.begin()` で行います。１番目のパラメータは検出対象のDIOです。`BRD_APPTWELITE::`に定義される`PIN_DI1-4` (DI1-DI4) を指定しています。２番めのパラメータは状態を確定するのに必要な検出回数です。３番めのパラメータは検出間隔です。`4`を指定しているので4msごとに5回連続で同じ値が検出できた時点で、HIGH, LOWの状態が確定します。

{% hint style="info" %}
ButtonsでのDIO状態の検出はイベントハンドラで行います。イベントハンドラは、割り込み発生後にアプリケーションループで呼ばれるため割り込みハンドラに比べ遅延が発生します。
{% endhint %}



#### Timer0

```cpp
Timer0.begin(32, true); // 32hz timer
```

App\_Twelite ではアプリケーションの制御をタイマー起点で行っているため、このアクトでも同じようにタイマー割り込み・イベントを動作させます。もちろん1msごとに動作しているシステムのTickTimerを用いても構いません。

上記の例の１番目のパラメータはタイマーの周波数で32Hzを指定しています。２番目のパラメータを`true`にするとソフトウェア割り込みが有効になります。

`Timer0.begin()`を呼び出したあと、タイマーが稼働します。



#### the\_tweliteの動作開始

```cpp
the_twelite.begin(); // start twelite!
```

`setup()` 関数の末尾で `the_twelite.begin()` を実行しています。



#### Serial

Serial オブジェクトは、初期化や開始手続きなく利用できます。

```cpp
	Serial 	<< "--- BRD_APPTWELITE ---" << mwx::crlf;
	Serial	<< format("-- app:x%08x/ch:%d/lid:%d"
					, the_twelite.get_appid()
					, the_twelite.get_channel()
					, nwk.get_config().u8Lid
				)
			<< mwx::crlf;
	Serial 	<< format("-- pw:%d/retry:%d/opt:x%08x"
					, the_twelite.get_tx_power()
					, nwk.get_config().u8RetryDefault
					, OPT_BITS
			)
			<< mwx::crlf;
```

このサンプルでは始動時のメッセージとしていくつかのシステム設定値を表示しています。`Serial`オブジェクトには _const char\*_ 型の文字列や、_int_型（他の整数型はNG)、_printf_とほぼ同じ振る舞いをする`format()`、改行文字を出力する`crlf`などを_<<_演算子に与えます。

{% hint style="info" %}
サンプル中では名前空間`mwx::`を省略している場合もあります。上記では`mwx::crlf`と記載していますが`crlf`と記載しても構いません。mwx::名前空間は、一部を省略可能とするように設計しています。
{% endhint %}



### ループ loop()

ループ関数は TWENET ライブラリのメインループからコールバック関数として呼び出されます。ここでは、利用するオブジェクトが available になるのを待って、その処理を行うのが基本的な記述です。ここではアクトで使用されているいくつかのオブジェクトの利用について解説します。

{% hint style="warning" %}
TWENET ライブラリのメインループは、事前にFIFOキューに格納された受信パケットや割り込み情報などをイベントとして処理し、そののち`loop()`が呼び出されます。`loop()`を抜けた後は CPU が DOZE モードに入り、低消費電流で新たな割り込みが発生するまでは待機します。

したがってCPUが常に稼働していることを前提としたコードはうまく動作しません。
{% endhint %}

```cpp
/*** loop procedure (called every event) */
void loop() {
	if (Buttons.available()) {
		uint32_t bp, bc;
		Buttons.read(bp, bc);

		u8DI_BM = uint8_t(collect_bits(bp, 
							BRD_APPTWELITE::PIN_DI4,   // bit3
							BRD_APPTWELITE::PIN_DI3,   // bit2
							BRD_APPTWELITE::PIN_DI2,   // bit1
							BRD_APPTWELITE::PIN_DI1)); // bit0

		transmit();
	}

	if (Analogue.available()) {
		au16AI[0] = Analogue.read(PIN_ANALOGUE::VCC);
		au16AI[1] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI1);
		au16AI[2] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI2);
		au16AI[3] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI3);
		au16AI[4] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI4);
	}

	if (Timer0.available()) {
		static uint8_t u16ct;
		u16ct++;

		if (u8DI_BM != 0xFF && au16AI[0] != 0xFFFF) { // finished the first capture
			if ((u16ct % 32) == 0) { // every 32ticks of Timer0
				transmit();
			}
		}
	}
}
```



#### Buttons

DIO(ディジタルIO)の入力変化を検出したタイミングで available になり、`Buttons.read()`により読み出します。

```cpp
	if (Buttons.available()) {
		uint32_t bp, bc;
		Buttons.read(bp, bc);
```

１番目のパラメータは、現在のDIOのHIGH/LOWのビットマップで、bit0から順番にDIO0,1,2,.. と並びます。例えば DIO12 であれば `bp & (1UL << 12)` を評価すれば HIGH / LOW が判定できます。ビットが1になっているものがHIGHになります。

{% hint style="info" %}
初回のIO状態確定時は MSB (bit31) に１がセットされます。スリープ復帰時も初回の確定処理を行います。
{% endhint %}



次にビットマップから値を取り出して`u8DI_BM`に格納しています。ここではMWXライブラリで用意した`collect_bits()`関数を用いています。

```cpp
u8DI_BM = uint8_t(collect_bits(bp, 
		BRD_APPTWELITE::PIN_DI4,   // bit3
		BRD_APPTWELITE::PIN_DI3,   // bit2
		BRD_APPTWELITE::PIN_DI2,   // bit1
		BRD_APPTWELITE::PIN_DI1)); // bit0

/* collect_bits は以下の処理を行います。
u8DI_BM = 0;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI1)) u8DI_BM |= 1;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI2)) u8DI_BM |= 2;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI3)) u8DI_BM |= 4;
if (bp & (1UL << BRD_APPTWELITE::PIN_DI4)) u8DI_BM |= 8;
*/
```

`collect_bits()` は、上述の`pack_bits()`と同様のビット位置の整数値を引数とします。可変数引数の関数で、必要な数だけパラメータを並べます。上記の処理では bit0 は DI1、bit1 は DI2、bit2 は DI3、bit3 は DI4の値としてu8DI\_BMに格納しています。



App\_Twelite では、DI1から4に変化があった場合に無線送信しますので、`Buttons.available()`を起点に送信処理を行います。`transmit()`処理の内容は後述します。

```cpp
transmit();
```



#### Analogue

ADCのアナログディジタル変換が終了した直後の`loop()`で available になります。次の ADC が開始するまでは、データは直前に取得されたものとして読み出すことが出来ます。

```cpp
if (Analogue.available()) {
	au16AI[0] = Analogue.read(PIN_ANALOGUE::VCC);
	au16AI[1] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI1);
	au16AI[2] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI2);
	au16AI[3] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI3);
	au16AI[4] = Analogue.read_raw(BRD_APPTWELITE::PIN_AI4);
}
```

ADC値を読むには `Analogue.read()` または `Analogue.read_raw()` メソッドを用います。`read()`はmVに変換した値、`read_raw()`は 0..1023 のADC値となります。パラメータにはADCのピン番号を指定します。ADCのピン番号は`PIN_ANALOGUE::`や`BRD_APPTWELITE::`に定義されているので、こちらを利用しています。

{% hint style="warning" %}
周期的に実行されるADCの値は、タイミングによってはavailable通知前のより新しい値が読み出されることがあります。

このアクトでは32Hzと比較的ゆっくりの周期で処理しているため、available判定直後に処理すれば問題にはなりませんが、変換周期が短い場合、`loop()`中で比較的長い時間のかかる処理をしている場合は注意が必要です。

`Analogue`には、変換終了後に割り込みハンドラ内から呼び出されるコールバック関数を指定することが出来ます。例えば、このコールバック関数からFIFOキューに値を格納する処理を行い、アプリケーションループ内ではキューの値を逐次読み出すといった非同期処理を行います。
{% endhint %}



#### Timer0

`Timer0`は32Hzで動作しています。タイマー割り込みが発生直後の `loop()` で available になります。つまり、秒３２回の処理をします。ここでは、ちょうど１秒になったところで送信処理をしています。

```cpp
if (Timer0.available()) {
	static uint8_t u16ct;
	u16ct++;

	if (u8DI_BM != 0xFF && au16AI[0] != 0xFFFF) { // finished the first capture
		if ((u16ct % 32) == 0) { // every 32ticks of Timer0
			transmit();
		}
	}
}
```

AppTweliteでは約１秒おきに定期送信を行っています。`Timer0`がavailableになったときに`u16ct`をインクリメントします。このカウンタ値をもとに、３２回カウントが終われば`transmit()`を呼び出し無線パケットを送信しています。

`u8DI_BM`と`au16AI[]`の値判定は、初期化直後かどうかの判定です。まだDI1..DI4やAI1..AI4の値が格納されていない場合は何もしません。



### transmit()

無線パケットの送信要求をTWENETに行う関数です。本関数が終了した時点では、まだ無線パケットの処理は行われません。実際に送信が完了するのは、送信パラメータ次第ですが、数ms後以降になります。ここでは代表的な送信要求方法について解説します。

```cpp
MWX_APIRET transmit() {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
	  auto&& set = the_twelite.settings.use<STG_STD>();
		if (!set.is_screen_opened()) {
			Serial << "..DI=" << format("%04b ", u8DI_BM);
			Serial << format("ADC=%04d/%04d/%04d/%04d ", au16AI[1], au16AI[2], au16AI[3], au16AI[4]);
			Serial << "Vcc=" << format("%04d ", au16AI[0]);
			Serial << " --> transmit" << mwx::crlf;
		}

		// set tx packet behavior
		pkt << tx_addr(u8devid == 0 ? 0xFE : 0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x1 send two times in total)
			<< tx_packet_delay(0,50,10); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(APP_FOURCHAR, 4) // string should be paired with length explicitly.
			, uint8_t(u8DI_BM)
		);

		for (auto&& x : au16AI) {
			pack_bytes(pkt.get_payload(), uint16_t(x)); // adc values
		}
		
		// do transmit 
		return pkt.transmit();
	}
	return MWX_APIRET(false, 0);
}
```

####

#### 関数プロトタイプ

```
MWX_APIRET transmit()
```

`MWX_APIRET`は`uint32_t`型のデータメンバを持つ戻り値を取り扱うクラスです。MSB(bit31)が成功失敗、それ以外が戻り値として利用するものです。



#### ネットワークオブジェクトとパケットオブジェクトの取得

```cpp
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

ネットワークオブジェクトを`the_twelite.network.use<NWK_SIMPLE>()`で取得します。そのオブジェクトを用いて`.prepare_tx_packet()`により`pkt`オブジェクトを取得します。

ここではif文の条件判定式の中で宣言しています。宣言した`pkt`オブジェクトはif節の終わりまで有効です。pktオブジェクトはbool型の応答をし、ここではTWENETの送信要求キューに空きがあって送信要求を受け付ける場合に`true`、空きがない場合に`false`となります。



#### インタラクティブモード画面表示中の表示抑制

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
if (!set.is_screen_opened()) {
    //インタラクティブモード画面中ではない！
}
```

インタラクティブモードの画面が表示されているときは、画面出力を抑制します。



#### パケットの送信設定

```cpp
pkt << tx_addr(u8devid == 0 ? 0xFE : 0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
		<< tx_retry(0x1) // set retry (0x3 send four times in total)
		<< tx_packet_delay(0,50,10); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)
```

パケットの設定は`the_twelite`の初期化設定のように`<<`演算子を用いて行います。

* `tx_addr()` パラメータに送信先アドレスを指定します。`0x00`なら自分が子機で親機宛に、`0xFE`なら自分が親機で任意の子機宛のブロードキャストという意味です。
* `tx_retry()` パラメータに再送回数を指定します。例の`1`は再送回数が１回、つまり合計２回パケットを送ります。無線パケット１回のみの送信では条件が良くても数％程度の失敗はあります。
* `tx_packet_delay()` 送信遅延を設定します。一つ目のパラメータは、送信開始までの最低待ち時間、２番目が最長の待ち時間です。この場合は送信要求を発行後におよそ0msから50msの間で送信を開始します。３番目が再送間隔です。最初のパケットが送信されてから10ms置きに再送を行うという意味です。



#### パケットのデータペイロード

ペイロードは積載物という意味ですが、無線パケットでは「送りたいデータ本体」という意味でよく使われます。無線パケットのデータにはデータ本体以外にもアドレス情報などいくつかの補助情報が含まれます。

送受信を正しく行うために、データペイロードのデータ並び順を意識するようにしてください。ここでは以下のようなデータ順とします。このデータ順に合わせてデータペイロードを構築します。

```cpp
# 先頭バイトのインデックス: データ型 : バイト数 : 内容

00: uint8_t[4] : 4 : ４文字識別子
04: uint8_t    : 1 : DI1..4のビットマップ
06: uint16_t   : 2 : Vccの電圧値
08: uint16_t   : 2 : AI1のADC値 (0..1023)
10: uint16_t   : 2 : AI2のADC値 (0..1023)
12: uint16_t   : 2 : AI3のADC値 (0..1023)
14: uint16_t   : 2 : AI4のADC値 (0..1023)
```

{% hint style="info" %}
データペイロードには90バイト格納できます（実際にはあと数バイト格納できます）。

IEEE802.15.4の無線パケットの１バイトは貴重です。できるだけ節約して使用することを推奨します。１パケットで送信できるデータ量に限りがあります。パケットを分割する場合は分割パケットの送信失敗などを考慮する必要がありコストは大きくつきます。また１バイト余分に送信するのに、およそ16μ秒×送信時の電流に相当するエネルギーが消費され、特に電池駆動のアプリケーションには大きく影響します。

上記の例は、解説のためある程度の妥協をしています。節約を考える場合 00: の識別子は１バイトの簡単なものにすべきですし、Vccの電圧値は８ビットに丸めてもかまわないでしょう。また各AI1..AI4の値は10bitで、合計40bit=5バイトに対して6バイト使用しています。
{% endhint %}



上記のデータペイロードのデータ構造を実際に構築してみます。データペイロードは `pkt.get_payload()` により `simplbuf<uint8_t>` 型のコンテナとして参照できます。このコンテナに上記の仕様に基づいてデータを構築します。

```cpp
auto&& payl = pkt.get_payload();
payl.reserve(16); // １６バイトにリサイズ
payl[00] = APP_FOURCHAR[0];
payl[01] = APP_FOURCHAR[1];
...
payl[08] = (au16AI[0] & 0xFF00) >> 8; //Vcc
payl[09] = (au16AI[0] & 0xFF);
...
payl[14] = (au16AI[4] & 0xFF00) >> 8; // AI4
payl[15] = (au16AI[4] & 0xFF);
```



上記のように記述できますがMWXライブラリでは、データペイロード構築のための補助関数`pack_bytes()`を用意しています。

```cpp
// prepare packet payload
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(APP_FOURCHAR, 4) // string should be paired with length explicitly.
	, uint8_t(u8DI_BM)
);

for (auto&& x : au16AI) {
	pack_bytes(pkt.get_payload(), uint16_t(x)); // adc values
}
```

`pack_bytes`の最初のパラメータはコンテナを指定します。この場合は`pkt.get_payload()`です。

そのあとのパラメータは可変数引数で`pack_bytes`で対応する型の値を必要な数だけ指定します。`pack_bytes`は内部で`.push_back()`メソッドを呼び出して末尾に指定した値を追記していきます。

3行目の`make_pair()`は標準ライブラリの関数で`std::pair`を生成します。文字列型の混乱（具体的にはペイロードの格納時にヌル文字を含めるか含めないか）を避けるための指定です。`make_pair()`の1番目のパラメータに文字列型(`char*`や`uint8_t*`型、`uint8_t[]`など)を指定します。2番目のパラメータはペイロードへの格納バイト数です。

4行目は`uint8_t`型でDI1..DI4のビットマップを書き込みます。

7-9行目では`au16AI`配列の値を順に書き込んでいます。この値は`uint16_t`型で2バイトですが、ビッグエンディアンの並びで書き込みます。

{% hint style="info" %}
7行目のfor文はC++で導入された**範囲for**文です。サイズのわかっている配列や`begin(), end()`によるイテレータによるアクセスが可能なコンテナクラスなどは、この構文が使用できます。au16AIの型もコンパイル時に判定できるため `auto&&` （ユニバーサル参照）で型の指定も省略してます。

通常のfor文に書き換えると以下のようになります。

```cpp
for(int i = 0; i < sizeof(au16AI)/sizeof(uint16_t)); i++) {
  pack_bytes(pkt.get_payload(), au16AI[i]);
}
```
{% endhint %}



これでパケットの準備は終わりです。あとは、送信要求を行います。

```cpp
return pkt.transmit();
```

パケットを送信するには`pkt`オブジェクトの`pkt.transmit()`メソッドを用います。戻り値として`MWX_APIRET`型を返していますが、このアクトでは使っていません。

{% hint style="info" %}
戻り値には、要求の成功失敗の情報と要求に対応する番号が格納されています。送信完了まで待つ処理を行う場合は、この戻り値の値を利用します。
{% endhint %}



### on\_rx\_packet()

無線パケットが受信できたときは、受信イベントとして`on_rx_packet()`が呼び出されます。

{% hint style="info" %}
`the_twelite.receiver`による手続きでは一旦受信パケットを内部キュー（２パケットまで格納）に格納してからの処理でしたが、`on_rx_packet()`ではTWENETライブラリからのコールバックから直接呼び出され、より取りこぼし等が発生しにくい手続きです。ただし`loop()`文中で長時間処理を止めてしまうような記述を行うと、同じように取りこぼしの原因となります。
{% endhint %}

ここでは、相手方から伝えられたDI1..DI4の値とAI1..AI4の値を、自身のDO1..DO4とPWM1..PWM4に設定します。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {	
	auto&& set = the_twelite.settings.use<STG_STD>();

	Serial << format("..receive(%08x/%d) : ", rx.get_addr_src_long(), rx.get_addr_src_lid());

  // expand the packet payload
	char fourchars[5]{};
	auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
    );

	// check header
	if (strncmp(APP_FOURCHAR, fourchars, 4)) { return; }

	// read rest of payload
	uint8_t u8DI_BM_remote = 0xff;
	uint16_t au16AI_remote[5];
	expand_bytes(np, rx.get_payload().end()
		, u8DI_BM_remote
		, au16AI_remote[0]
		, au16AI_remote[1]
		, au16AI_remote[2]
		, au16AI_remote[3]
		, au16AI_remote[4]
	);

	Serial << format("DI:%04b", u8DI_BM_remote & 0x0F);
	for (auto&& x : au16AI_remote) {
		Serial << format("/%04d", x);
	}
	Serial << mwx::crlf;

	// set local DO
	digitalWrite(BRD_APPTWELITE::PIN_DO1, (u8DI_BM_remote & 1) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO2, (u8DI_BM_remote & 2) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO3, (u8DI_BM_remote & 4) ? HIGH : LOW);
	digitalWrite(BRD_APPTWELITE::PIN_DO4, (u8DI_BM_remote & 8) ? HIGH : LOW);

	// set local PWM : duty is set 0..1024, so 1023 is set 1024.
	Timer1.change_duty(au16AI_remote[1] == 1023 ? 1024 : au16AI_remote[1]);
	Timer2.change_duty(au16AI_remote[2] == 1023 ? 1024 : au16AI_remote[2]);
	Timer3.change_duty(au16AI_remote[3] == 1023 ? 1024 : au16AI_remote[3]);
	Timer4.change_duty(au16AI_remote[4] == 1023 ? 1024 : au16AI_remote[4]);
}

```



まず受信パケットのデータ`rx`をはパラメータとして渡されます。`rx`から無線パケットのアドレス情報やデータペイロードにアクセスします。パラメータ`handled`は通常利用しません。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled)
```



受信パケットデータには、送信元のアドレス（32bitのロングアドレスと8bitの論理アドレス）などの情報を参照しています。インタラクティブモード画面が表示されているときは出力を抑制します。

```cpp
if (!set.is_screen_opened()) {
   Serial << format("..receive(%08x/%d) : ",
      rx.get_addr_src_long(), rx.get_addr_src_lid());
}
```

{% hint style="info" %}
`<NWK_SIMPLE>`では、8bitの論理IDと32bitのロングアドレスの２種類が常にやり取りされます。送り先を指定する場合はロングアドレスか論理アドレスのいずれかを指定します。受信時には両方のアドレスが含まれます。
{% endhint %}



MWXライブラリには`transmit()`の時に使った`pack_bytes()`の対になる関数`expand_bytes()`が用意されています。

```cpp
char fourchars[5]{};
auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
	, make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
  );
```

1行目ではデータ格納のための`char`型の配列を宣言しています。サイズが5バイトなのは末尾にヌル文字を含め、文字出力などでの利便性を挙げるためです。末尾の`{}`は初期化の指定で、5バイト目を0にすれば良いのですが、ここでは配列全体をデフォルトの方法で初期化、つまり`0`にしています。

2行目で`expand_bytes()`により4バイト文字列を取り出しています。パラメータにコンテナ型を指定しない理由は、この続きを読み出すための読み出し位置を把握する必要があるためです。1番目のパラメータでコンテナの先頭イテレータ（`uint8_t*`ポインタ）を指定します。`.begin()`メソッドにより取得できます。2番目のパラメータはコンテナの末尾の次を指すイテレータで`.end()`メソッドで取得できます。2番目はコンテナの末尾を超えた読み出しを行わないようにするためです。

3番目に読み出す変数を指定しますが、ここでも`make_pair`によって文字列配列とサイズのペアを指定します。

{% hint style="info" %}
このアクトでは、パケット長が間違っていた場合などのエラーチェックを省いています。チェックを厳格にしたい場合は、`expand_bytes()`の戻り値により判定してください。

`expand_bytes()`の戻り値は `uint8_t*` ですが、末尾を超えたアクセスの場合は`nullptr（ヌルポインタ）`を戻します。
{% endhint %}



読み出した4バイト文字列の識別子が、このアクトで指定した識別子と異なる場合は、このパケットを処理しません。

```cpp
if (strncmp(APP_FOURCHAR, fourchars, 4)) { return; }
```

{% hint style="info" %}
TWENETではアプリケーションIDと物理的な無線チャネルが合致する場合は、どのアプリケーションもたとえ種別が違ったとしても、受信することが出来ます。他のアプリケーションで作成したパケットを意図しない形で受信しない目的で、このような識別子やデータペイロードの構造などのチェックを行い、偶然の一致が起きないように対処することを推奨します。

シンプルネットワーク`<NWK_SIMPLE>`でのパケット構造の要件も満足する必要があるため、シンプルネットワークを使用しない他のアプリケーションが同じ構造のパケットを定義しない限り（非常にまれと思われます）、パケットの混在受信は発生しません。
{% endhint %}



続いて、データ部分の取得です。DI1..DI4の値とAI1..AI4の値を別の変数に格納します。

```cpp
	// read rest of payload
	uint8_t u8DI_BM_remote = 0xff;
	uint16_t au16AI_remote[5];
	expand_bytes(np, rx.get_payload().end()
		, u8DI_BM_remote
		, au16AI_remote[0]
		, au16AI_remote[1]
		, au16AI_remote[2]
		, au16AI_remote[3]
		, au16AI_remote[4]
	);
```

先ほどの`expand_bytes()`の戻り値`np`を1番目のパラメータにしています。先に読み取った4バイト文字列識別子の次から読み出す指定です。2番目のパラメータは同様です。

3番目以降のパラメータは**データペイロードの並びに一致した型**の変数を、**送り側のデータ構造と同じ順番**で並べています。この処理が終われば、指定した変数にペイロードから読み出した値が格納されます。

確認のためシリアルポートへ出力します。インタラクティブモード画面が表示されているときは出力は抑制します。

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
Serial << format("DI:%04b", u8DI_BM_remote & 0x0F);
for (auto&& x : au16AI_remote) {
	Serial << format("/%04d", x);
}
Serial << mwx::crlf;
```

数値のフォーマット出力が必要になるので`format()`を用いています。`>>`演算子向けに_`printf()`_と同じ構文を利用できるようにしたヘルパークラスですが、引数の数が４つまでに制限されています。（`Serial.printfmt()`には引数の数の制限がありません。）

１行目の `"DI:%04b"` は`"DI:0010"`のようにDI1..DI4のビットマップを４桁で表示します。３行目の`"/%04d"`は`"/3280/0010/0512/1023/1023"`のように Vcc/AI1..AI4の値を順に整数で出力します。５行目の`mwx::crlf`は改行文字列を出力します。



これで必要なデータの展開が終わったので、あとはボード上のDO1..DO4とPWM1..PWM4の値を変更します。

```cpp
// set local DO
digitalWrite(BRD_APPTWELITE::PIN_DO1, (u8DI_BM_remote & 1) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO2, (u8DI_BM_remote & 2) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO3, (u8DI_BM_remote & 4) ? HIGH : LOW);
digitalWrite(BRD_APPTWELITE::PIN_DO4, (u8DI_BM_remote & 8) ? HIGH : LOW);

// set local PWM : duty is set 0..1024, so 1023 is set 1024.
Timer1.change_duty(au16AI_remote[1] == 1023 ? 1024 : au16AI_remote[1]);
Timer2.change_duty(au16AI_remote[2] == 1023 ? 1024 : au16AI_remote[2]);
Timer3.change_duty(au16AI_remote[3] == 1023 ? 1024 : au16AI_remote[3]);
Timer4.change_duty(au16AI_remote[4] == 1023 ? 1024 : au16AI_remote[4]);
```

`digitalWrite()`はディジタル出力の値を変更します。１番目のパラメータはピン番号で、２番目は`HIGH`(Vccレベル)か`LOW`(GNDレベル)を指定します。

`Timer?.change_duty()`はPWM出力のデューティ比を変更します。パラメータにデューティ比 0..1024 を指定します。最大値が1023でないことに注意してください（ライブラリ内で実行される割り算のコストが大きいため2のべき乗である1024を最大値としています）。`0`にするとGNDレベル、`1024`にするとVccレベル相当の出力になります。
