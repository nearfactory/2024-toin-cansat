---
title: PingPong
author: "Mono Wireless Inc."
description: "PingPong"
---
# PingPong

2台のシリアル接続しているTWELITEの片方からPING(ピン)の無線パケットを送信すると、他方からPONG(ポン)の無線パケットが返ってきます。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの受信からの速やかな応答送信
* 相手のアドレスを直接指定した送信
* シリアルポートからの入力 - [Serial](../api-reference/classes/twe-stream/#read)
* ディジタル（ボタン）入力 - [Buttons](../api-reference/predefined\_objs/buttons.md)
* アナログ入力 - [Analogue](../api-reference/predefined\_objs/analogue.md)
{% endhint %}





## アクトの使い方

### 必要なTWELITE

いずれかを２台。

* [MONOSTICK BLUE または RED](https://mono-wireless.com/jp/products/MoNoStick/index.html)
* [TWELITE R](https://mono-wireless.com/jp/products/TWE-LITE-R/index.html) でUART接続されている[TWELITE DIP](https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html)など



## アクトの解説

### インクルード

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
```

全てのアクトで`<TWELITE>`をインクルードします。ここでは、シンプルネットワーク [`<NWK_SIMPLE>`](../networks/nwk\_simple.md) をインクルードしておきます。

### 宣言部

```cpp
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

// DIO pins
const uint8_t PIN_BTN = 12;

/*** function prototype */
void vTransmit(const char* msg, uint32_t addr);

/*** application defs */
// packet message
const int MSG_LEN = 4;
const char MSG_PING[] = "PING";
const char MSG_PONG[] = "PONG";
```

* サンプルアクト共通宣言
* 長めの処理を関数化しているため、そのプロトタイプ宣言（送信と受信）
* アプリケーション中のデータ保持するための変数

### セットアップ setup()

```cpp
void setup() {
	/*** SETUP section */
	Buttons.setup(5); // init button manager with 5 history table.
	Analogue.setup(true, 50); // setup analogue read (check every 50ms)

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	/*** BEGIN section */
	Buttons.begin(pack_bits(PIN_BTN), 5, 10); // check every 10ms, a change is reported by 5 consequent values.
	Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC)); // _start continuous adc capture.

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
}
```

大まかな流れは、各部の初期設定、各部の開始となっています。

#### the\_twelite

このオブジェクトはTWENETを操作するための中核クラスオブジェクトです。

```cpp
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)
```

`the_twelite` に設定を反映するには `<<` を用います。

* `TWENET::appid(APP_ID)` アプリケーションIDの指定
* `TWENET::channel(CHANNEL)` チャネルの指定
* `TWENET::rx_when_idle()` 受信回路をオープンにする指定

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
auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
nwksmpl << NWK_SIMPLE::logical_id(0xFE);
        << NWK_SIMPLE::repeat_max(3);
```

1行目は、ボードの登録と同じ書き方で `<>` には `<NWK_SIMPLE>`を指定します。

2行目は、`<NWK_SIMPLE>`の設定で、`0xFE`(**ID未設定の子機**)という指定を行います。

3行目は、中継回数の最大値を指定しています。この解説では中継には触れませんが、複数台で動作させたときにパケットの中継が行われます。



```cpp
the_twelite.begin(); // start twelite!
```

`setup()` 関数の末尾で `the_twelite.begin()` を実行しています。



#### Analogue

ADC(アナログディジタルコンバータ)を取り扱うクラスオブジェクトです。

```cpp
Analogue.setup(true);
```

初期化`Analogue.setup()`で行います。パラメータの`true`はADC回路の安定までその場で待つ指定です。



```cpp
Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC), 50); 
```

ADCを開始するには`Analogue.begin()`を呼びます。パラメータはADC対象のピンに対応するビットマップです。

ビットマップを指定するのに`pack_bits()`関数を用います。可変数引数の関数で、各引数には１を設定するビット位置を指定します。例えば`pack_bits(1,3,5)`なら２進数で `101010`の値が戻ります。この関数は`constexpr`指定があるため、パラメータが定数のみであれば定数に展開されます。

パラメータには`PIN_ANALOGUE::A1`（ADC0）と`PIN_ANALOGUE::VCC`（モジュール電源電圧）が指定されています。

2番目のパラメータには`50`が指定されています。ADCの動作はデフォルトではTickTimerで開始されていて、

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
Buttons.begin(pack_bits(PIN_BTN),
					5, 		// history count
					10);  	// tick delta
```

開始は `Buttons.begin()` で行います。１番目のパラメータは検出対象のDIOです。`BRD_APPTWELITE::`に定義される`PIN_BTN` (12) を指定しています。２番めのパラメータは状態を確定するのに必要な検出回数です。３番めのパラメータは検出間隔です。`10`を指定しているので10msごとに5回連続で同じ値が検出できた時点で、HIGH, LOWの状態が確定します。

{% hint style="info" %}
ButtonsでのDIO状態の検出はイベントハンドラで行います。イベントハンドラは、割り込み発生後にアプリケーションループで呼ばれるため割り込みハンドラに比べ遅延が発生します。
{% endhint %}



#### Serial

Serial オブジェクトは、初期化や開始手続きなく利用できます。

```cpp
Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
```

シリアルポートへの文字列出力を行います。`mwx::crlf`は改行文字です。



### ループ loop()

ループ関数は TWENET ライブラリのメインループからコールバック関数として呼び出されます。ここでは、利用するオブジェクトが available になるのを待って、その処理を行うのが基本的な記述です。ここではアクトで使用されているいくつかのオブジェクトの利用について解説します。

{% hint style="warning" %}
TWENET ライブラリのメインループは、事前にFIFOキューに格納された受信パケットや割り込み情報などをイベントとして処理し、そののち`loop()`が呼び出されます。`loop()`を抜けた後は CPU が DOZE モードに入り、低消費電流で新たな割り込みが発生するまでは待機します。

したがってCPUが常に稼働していることを前提としたコードはうまく動作しません。
{% endhint %}

```cpp
void loop() {
	  // read from serial
		while(Serial.available())  {
				int c = Serial.read();
				Serial << mwx::crlf << char(c) << ':';
				switch(c) {
				    case 't':
				    	  vTransmit(MSG_PING, 0xFF);
				        break;
				    default:
							  break;
				}
		}


	// Button press
	if (Buttons.available()) {
		uint32_t btn_state, change_mask;
		Buttons.read(btn_state, change_mask);

		// Serial << fmt("<BTN %b:%b>", btn_state, change_mask);
		if (!(change_mask & 0x80000000) && (btn_state && (1UL << PIN_BTN))) {
			// PIN_BTN pressed
			vTransmit(MSG_PING, 0xFF);
		}
	}
}
```



#### Serial

```cpp
		while(Serial.available())  {
				int c = Serial.read();
				Serial << mwx::crlf << char(c) << ':';
				switch(c) {
				    case 't':
				    	  vTransmit(MSG_PING, 0xFF);
				        break;
				    default:
							  break;
				}
		}
```

`Serial.available()`が`true`の間はシリアルポートからの入力があります。内部のFIFOキューに格納されるためある程度の余裕はありますが、速やかに読み出すようにします。データの読み出しは`Serial.read()`を呼びます。

ここでは`'t'`キーの入力に対応して`vTransmit()`関数を呼び出しPINGパケットを送信します。



#### Buttons

DIO(ディジタルIO)の入力変化を検出したタイミングで available になり、`Buttons.read()`により読み出します。

```cpp
	if (Buttons.available()) {
		uint32_t btn_state, change_mask;
		Buttons.read(btn_state, change_mask);
```

１番目のパラメータは、現在のDIOのHIGH/LOWのビットマップで、bit0から順番にDIO0,1,2,.. と並びます。例えば DIO12 であれば `btn_state & (1UL << 12)` を評価すれば HIGH / LOW が判定できます。ビットが1になっているものがHIGHになります。

{% hint style="info" %}
初回のIO状態確定時は MSB (bit31) に１がセットされます。スリープ復帰時も初回の確定処理を行います。
{% endhint %}

```cpp
// Serial << fmt("<BTN %b:%b>", btn_state, change_mask);
if (!(change_mask & 0x80000000) && (btn_state && (1UL << PIN_BTN))) {
	// PIN_BTN pressed
	vTransmit(MSG_PING, 0xFF);
```

初回確定以外の場合かつPIN\_BTNのボタンが**離された**タイミングで`vTransmit()`を呼び出しています。押したタイミングにするには`(!(btn_state && (1UL << PIN_BTN)))`のように条件を論理反転します。



### transmit()

無線パケットの送信要求をTWENETに行う関数です。本関数が終了した時点では、まだ無線パケットの処理は行われません。実際に送信が完了するのは、送信パラメータ次第ですが、数ms後以降になります。ここでは代表的な送信要求方法について解説します。

```cpp
void vTransmit(const char* msg, uint32_t addr) {
	Serial << "vTransmit()" << mwx::crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
			, uint16_t(analogRead(PIN_ANALOGUE::A1)) // possible numerical values types are uint8_t, uint16_t, uint32_t. (do not put other types)
			, uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)) // A1 and VCC values (note: alalog read is valid after the first (Analogue.available() == true).)
			, uint32_t(millis()) // put timestamp here.
		);
	
		// do transmit 
		pkt.transmit();
	}
}
```

####

#### ネットワークオブジェクトとパケットオブジェクトの取得

```cpp
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

ネットワークオブジェクトを`the_twelite.network.use<NWK_SIMPLE>()`で取得します。そのオブジェクトを用いて`.prepare_tx_packet()`により`pkt`オブジェクトを取得します。

ここではif文の条件判定式の中で宣言しています。宣言した`pkt`オブジェクトはif節の終わりまで有効です。pktオブジェクトはbool型の応答をし、ここではTWENETの送信要求キューに空きがあって送信要求を受け付ける場合に`true`、空きがない場合に`false`となります。



#### パケットの送信設定

```cpp
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)
```

パケットの設定は`the_twelite`の初期化設定のように`<<`演算子を用いて行います。

* `tx_addr()` パラメータに送信先アドレスを指定します。`0x00`なら自分が子機で親機宛に、`0xFE`なら自分が親機で任意の子機宛のブロードキャストという意味です。
* `tx_retry()` パラメータに再送回数を指定します。例の`3`は再送回数が3回、つまり合計4回パケットを送ります。無線パケット１回のみの送信では条件が良くても数％程度の失敗はあります。
* `tx_packet_delay()` 送信遅延を設定します。一つ目のパラメータは、送信開始までの最低待ち時間、２番目が最長の待ち時間です。この場合は送信要求を発行後におよそ100msから200msの間で送信を開始します。３番目が再送間隔です。最初のパケットが送信されてから20ms置きに再送を行うという意味です。



#### パケットのデータペイロード

ペイロードは積載物という意味ですが、無線パケットでは「送りたいデータ本体」という意味でよく使われます。無線パケットのデータにはデータ本体以外にもアドレス情報などいくつかの補助情報が含まれます。

送受信を正しく行うために、データペイロードのデータ並び順を意識するようにしてください。ここでは以下のようなデータ順とします。このデータ順に合わせてデータペイロードを構築します。

```
# 先頭バイトのインデックス: データ型 : バイト数 : 内容

00: uint8_t[4] : 4 : ４文字識別子
08: uint16_t   : 2 : AI1のADC値 (0..1023)
06: uint16_t   : 2 : Vccの電圧値 (2000..3600)
10: uint32_t   : 4 : millis()システム時間
```

{% hint style="info" %}
データペイロードには90バイト格納できます（実際にはあと数バイト格納できます）。

IEEE802.15.4の無線パケットの１バイトは貴重です。できるだけ節約して使用することを推奨します。１パケットで送信できるデータ量に限りがあります。パケットを分割する場合は分割パケットの送信失敗などを考慮する必要がありコストは大きくつきます。また１バイト余分に送信するのに、およそ16μ秒×送信時の電流に相当するエネルギーが消費され、特に電池駆動のアプリケーションには大きく影響します。
{% endhint %}



上記のデータペイロードのデータ構造を実際に構築してみます。データペイロードは `pkt.get_payload()` により `simplbuf<uint8_t>` 型のコンテナとして参照できます。このコンテナに上記の仕様に基づいてデータを構築します。

上記のように記述できますがMWXライブラリでは、データペイロード構築のための補助関数`pack_bytes()`を用意しています。

```cpp
// prepare packet payload
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
	, uint16_t(analogRead(PIN_ANALOGUE::A1)) // possible numerical values types are uint8_t, uint16_t, uint32_t. (do not put other types)
	, uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)) // A1 and VCC values (note: alalog read is valid after the first (Analogue.available() == true).)
	, uint32_t(millis()) // put timestamp here.
);
```

`pack_bytes`の最初のパラメータはコンテナを指定します。この場合は`pkt.get_payload()`です。

そのあとのパラメータは可変数引数で`pack_bytes`で対応する型の値を必要な数だけ指定します。`pack_bytes`は内部で`.push_back()`メソッドを呼び出して末尾に指定した値を追記していきます。

3行目の`make_pair()`は標準ライブラリの関数で`std::pair`を生成します。文字列型の混乱（具体的にはペイロードの格納時にヌル文字を含めるか含めないか）を避けるための指定です。`make_pair()`の1番目のパラメータに文字列型(`char*`や`uint8_t*`型、`uint8_t[]`など)を指定します。2番目のパラメータはペイロードへの格納バイト数です。

4,5,6行目は、数値型の値 (`uint8_t`, `uint16_t`, `uint32_t`)を格納します。符号付などの数値型、`char`型など同じ数値型であっても左記の3つの型にキャストして投入します。

`analogRead()`と`analogRead_mv()`は、ADCの結果を取得するものです。前者はADC値(0..1023)、後者は電圧\[mv]\(0..2470)となります。モジュールの電源電圧は内部的に分圧抵抗の値を読んでいるためその変換を行う`adalogRead_mv()`を利用しています。



これでパケットの準備は終わりです。あとは、送信要求を行います。

```cpp
pkt.transmit();
```

パケットを送信するには`pkt`オブジェクトの`pkt.transmit()`メソッドを用います。

{% hint style="info" %}
このアクトでは使用しませんが、戻り値には、要求の成功失敗の情報と要求に対応する番号が格納されています。送信完了まで待つ処理を行う場合は、この戻り値の値を利用します。
{% endhint %}



### on\_rx\_packet()

受信パケットがある場合の処理です。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
		uint8_t msg[MSG_LEN];
		uint16_t adcval, volt;
		uint32_t timestamp;

		// expand packet payload (shall match with sent packet data structure, see pack_bytes())
		expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
					, msg       // 4bytes of msg
											//   also can be -> std::make_pair(&msg[0], MSG_LEN)
					, adcval    // 2bytes, A1 value [0..1023]
				  , volt      // 2bytes, Module VCC[mV]
					, timestamp // 4bytes of timestamp
        );
		
		// if PING packet, respond pong!
    if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
				// transmit a PONG packet with specifying the address.
        vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
    }

		// display the packet
		Serial << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
                    , rx.get_psRxDataApp()->u32SrcAddr
                    , rx.get_lqi()
                    , rx.get_length()
					, rx.get_psRxDataApp()->u8Seq
                    )
				<< format(" %s AD=%d V=%d TS=%dms>" // note: up to 4 args!
					, msg
					, adcval
					, volt
					, timestamp
					)
               << mwx::crlf
			   << mwx::flush;
	}
```



まず受信パケットのデータはパラメータ`rx`として渡されます。`rx`から無線パケットのアドレス情報やデータペイロードにアクセスします。

```cpp
while (the_twelite.receiver.available()) {
		auto&& rx = the_twelite.receiver.read();
```

次の行では、受信パケットデータには、送信元のアドレス（32bitのロングアドレスと8bitの論理アドレス）などの情報を参照しています。

```cpp
Serial << format("..receive(%08x/%d) : ",
   rx.get_addr_src_long(), rx.get_addr_src_lid());
```

{% hint style="info" %}
`<NWK_SIMPLE>`では、8bitの論理IDと32bitのロングアドレスの２種類が常にやり取りされます。送り先を指定する場合はロングアドレスか論理アドレスのいずれかを指定します。受信時には両方のアドレスが含まれます。
{% endhint %}



MWXライブラリには`transmit()`の時に使った`pack_bytes()`の対になる関数`expand_bytes()`が用意されています。

```cpp
uint8_t msg[MSG_LEN];
uint16_t adcval, volt;
uint32_t timestamp;

// expand packet payload (shall match with sent packet data structure, see pack_bytes())
expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
		, msg       // 4bytes of msg
								//   also can be -> std::make_pair(&msg[0], MSG_LEN)
		, adcval    // 2bytes, A1 value [0..1023]
	  , volt      // 2bytes, Module VCC[mV]
		, timestamp // 4bytes of timestamp
    );
```

1行目から3行目までは、データを格納する変数を指定しています。

6行目で`expand_bytes()`によりパケットのペイロードのデータを変数に格納します。1番目のパラメータでコンテナの先頭イテレータ（`uint8_t*`ポインタ）を指定します。`.begin()`メソッドにより取得できます。2番目のパラメータはコンテナの末尾の次を指すイテレータで`.end()`メソッドで取得できます。2番目はコンテナの末尾を超えた読み出しを行わないようにするためです。

3番目以降のパラメータに変数を列挙します。列挙した順番にペイロードの読み出しとデータ格納が行われます。

{% hint style="info" %}
このアクトでは、パケット長が間違っていた場合などのエラーチェックを省いています。チェックを厳格にしたい場合は、`expand_bytes()`の戻り値により判定してください。

`expand_bytes()`の戻り値は `uint8_t*` ですが、末尾を超えたアクセスの場合は`nullptr（ヌルポインタ）`を戻します。
{% endhint %}



`msg`に読み出した4バイト文字列の識別子が`"PING"`の場合はPONGメッセージを送信する処理です。

```cpp
if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
    vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
}
```



続いて到着したパケット情報を表示します。

```cpp
		Serial << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
                    , rx.get_psRxDataApp()->u32SrcAddr
                    , rx.get_lqi()
                    , rx.get_length()
										, rx.get_psRxDataApp()->u8Seq
                    )
           << format(" %s AD=%d V=%d TS=%dms>" // note: up to 4 args!
                    , msg
                    , adcval
                    , volt
                    , timestamp
                    )
         << mwx::crlf
			   << mwx::flush;
```

数値のフォーマット出力が必要になるので`format()`を用いています。`>>`演算子向けに_printf()_と同じ構文を利用できるようにしたヘルパークラスですが、**引数の数は最大8つまで（32bitパラメータの場合)に制限**されています。（制限を超えるとコンパイルエラーが出ます。なお`Serial.printfmt()`には引数の数の制限がありません。）

`mwx::crlf`は改行文字(CR LF)を、`mwx::flush`は出力完了待ちを指定します。（`mxw::flush`は`Serial.flush()`と記述しても構いません）
