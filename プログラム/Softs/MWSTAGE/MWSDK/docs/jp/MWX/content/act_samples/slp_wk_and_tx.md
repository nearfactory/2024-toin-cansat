---
title: Slp_Wk_and_Tx 
author: "Mono Wireless Inc."
description: "Slp_Wk_and_Tx"
---

# Slp\_Wk\_and\_Tx

`Slp_Wk_and_Tx `は、定期起床後、何か実行（センサーデータの取得など）を行って、その結果を無線パケットとして送信するようなアプリケーションを想定した、テンプレートソースコードです。

`setup(), loop()` の形式では、どうしても `loop()` 中が判読しづらい条件分岐が発生しがちです。本Actでは、`loop()`中を[SM\_SIMPLEステートマシン](../api-reference/classes/smsimple-suttomashin.md)を用いて _switch_構文による単純な状態遷移を用いることで、コードの見通しを良くしています。

{% hint style="success" %}
このアクトには以下が含まれます。

* 代表的な間欠動作（スリープ→起床→計測→無線送信→スリープ）の制御構造について
* 送信パケットの生成と送信手続き、完了待ちについて
{% endhint %}



## アクトの機能

* 起動後、初期化処理を経て、一旦スリープする
  1. `setup()`初期化する
  2. `begin()` スリープ実行する
* スリープ起床後、状態変数を初期化し、以下の順に動作を行う
  1. `wakeup()`スリープからの起床、各初期化を行う
  2. `loop()`状態`INIT`->`WORK_JOB`に遷移: 何らかの処理を行う（このActでは 1ms ごとの _TickCount_ ごとにカウンタを更新し乱数で決めたカウント後に`TX`状態に進む）
  3. `loop()` 状態`TX`送信要求を行う
  4. `loop()` 状態`WAIT_TX`送信完了待ちを行う
  5. `loop()` 状態`EXIT_NORMAL`スリープする (1. に戻る)
* `loop()` 状態`EXIT_FATAL` エラーが発生した場合は、モジュールリセットする

## アクトの解説

### 宣言部

#### インクルード

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <SM_SIMPLE>

#include "Common.h"
```

パケット送信を行うため `<NWK_SIMPLE>` をインクルードしています。また、アプリケーションIDなど基本的な定義は `"Common.h" `に記述しています。



#### 状態定義

`loop()`内の順次処理を記述うするため、このサンプルではステートマシン（状態遷移）の考え方を用います。ごく単純な状態遷移の処理をまとめた`<SM_SIMPLE>`を用います。

`Common.h` に以下の状態に対応する列挙体 `STATE `が定義されています。

```cpp
enum class STATE {
    INIT = 0,    // INIT STATE
    WORK_JOB,    // do some job (e.g sensor capture)
    TX,          // reuest transmit
    WAIT_TX,     // wait its completion
    EXIT_NORMAL, // normal exiting.
    EXIT_FATAL   // has a fatal error (will do system reset)
};
```

状態を示す列挙体`STATE`を用いて[`SM_SIMPLE`ステートマシン（状態遷移）](../api-reference/classes/smsimple-suttomashin.md)を宣言します。

```cpp
SM_SIMPLE<STATE> step;
```

ここで宣言された`step`は、状態の管理、タイムアウト、処理待ちを行うための機能が含まれています。



#### センサーデータ

このサンプルではセンサーデーターの処理は行いませんが、ダミーデータを用意しておきます。

```cpp
struct {
	uint16_t dummy_work_ct_now;
	uint16_t dummy_work_ct_max;  // counter for dummy work job. 
} sensor;
```



### setup()

```cpp
void setup() {
	/*** SETUP section */
	step.setup(); // init state machine
	
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle(false);  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(DEVICE_ID); // set Logical ID. 

	/*** BEGIN section */
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- Sleep an Tx Act ---" << crlf;
}
```

変数やクラスオブジェクトの初期化を行います。

* `step`ステートマシンの初期化
* `the_twelite`クラスオブジェクトの初期化
* ネットワーク `<NWK_SIMPLE>` の登録と初期化(`DEVICE_ID`の登録)を行います。



つづいてクラスオブジェクトやハードウェアなどの開始処理を行います。

```cpp
the_twelite.begin(); // start twelite!
```

`the_twelite`を開始するための手続きです。act0..4では出てきませんでしたが`the_twelite`の設定や各種ビヘイビアの登録を行った場合は、必ず呼び出すようにしてください。



### begin()

```cpp
void begin() {
	Serial << "..begin (run once at boot)" << crlf;
	SleepNow();
}
```

`setup()`の直後に一度だけ呼び出されます。`SleepNow()`関数夜を呼び出して初回のスリープ手続きを行います。



### wakeup()

```cpp
void wakeup() {
　memset(&sensor, 0, sizeof(sensor));
	Serial << crlf << int(millis()) << ":wake up!" << crlf;
}
```

起床直後に呼び出されます。ここではセンサーデータ領域の初期化と、起床時のメッセージを出力しています。



### loop()

```cpp
void loop() {
	do {
		switch(step.state()) {
		case STATE::INIT:
			sensor.dummy_work_ct_now = 0;
			sensor.dummy_work_ct_max = random(10,1000);
			step.next(STATE::WORK_JOB);
		break;

		...
		}
	} while (step.b_more_loop());
}

```

上記のコードは、実際のコードを簡略化したものです。

この制御構造は[SM\_SIMPLEステートマシン](../api-reference/classes/smsimple-suttomashin.md)を利用しています。_do..while()_ 構文のループになっています。ループの中は_switch case_節となっていて、`.state()`で得られた状態により処理を分岐しています。状態の遷移は`.next()`を呼び出しステートマシン内の内部変数を新しい状態値に書き換えます。

`step.b_more_loop()`は、`.next()`により状態遷移があった場合_true_に設定されます。これは状態遷移が発生したとき`loop()`を脱出せずに次の状態のコード（case節）を実行する目的です。

以下に各状態の解説を行います。

#### STATE::INIT

```cpp
sensor.dummy_work_ct_now = 0;
sensor.dummy_work_ct_max = random(10,1000);

step.next(STATE::WORK_JOB);
```

ダミーーのセンサー値を初期化します。一つは加算カウンタ、一つはカウンター停止値でランダムに決定しています。



#### STATE::WORK\_JOB

```cpp
if (TickTimer.available()) {
	Serial << '.';
	sensor.dummy_work_ct_now++;
	if (sensor.dummy_work_ct_now >= sensor.dummy_work_ct_max) {
		Serial << crlf;
		step.next(STATE::TX);
	}
}
```

WORK\_JOB状態では1msごとのタイマー単位で処理します。Tickタイマーごとに`TickTimer.available()`になります。Tickタイマーごとにカウンタを加算し`dummy_work_ct_max`になったら、次の状態`STATE::TX`に遷移します。



#### STATE::TX

```cpp
if (Transmit()) {
	Serial << int(millis()) << ":tx request success!" << crlf;
	step.set_timeout(100);
	step.clear_flag();
	step.next(STATE::WAIT_TX);
} else {
	// normall it should not be here.
	Serial << int(millis()) << "!FATAL: tx request failed." << crlf;
	step.next(STATE::EXIT_FATAL);
}
```

`Transmit()`関数を呼び出しパケット送信要求を行います。送信要求が成功した場合は`STATE::WAIT_TXEVENT`に遷移して送信完了を待つことになります。ここでは完了待ちとしてSM\_SIMPLEステートマシンのタイムアウトとフラッグ機能を用います（待ちループ中での変数値の変化により判定する単純なものです）。

単一の送信要求が失敗することは通常想定しませんが、失敗時は`STATE::EXIT_FATAL`として例外処理する状態に遷移します。

{% hint style="warning" %}
この時点ではまだパケットが送信されていないため、この時点でスリープをしてはいけません。多くの場合、**送信完了**を待ってから、続く処理を行います。
{% endhint %}

{% hint style="info" %}
`Transmit()`関数は`MWX_APIRET`オブジェクトを返しますが、このオブジェクトは_bool_型の成功の可否と、最大３１ビットの値を保持しています。_bool_型として評価できますから、_if_文の判定は送信要求が成功したら_true_、失敗したら_false_を返します。
{% endhint %}



#### STATE::WAIT\_TX

```cpp
if (step.is_flag_ready()) {
	Serial << int(millis()) << ":tx completed!" << crlf;
	step.next(STATE::EXIT_NORMAL);
} else if (step.is_timeout()) {
	Serial << int(millis()) << "!FATAL: tx timeout." << crlf;
	step.next(STATE::EXIT_FATAL);
}
```

送信完了待ちは後述の`on_tx_comp()`によりステートマシン機能のフラッグをセットすることで判定しています。タイムアウトは`.is_timeout()`を呼び出すことで`.set_timeout()`を行ったときからの経過時間により判定します。

送信が成功しても失敗しても通常は完了通知がありますが、タイムアウトを設け例外処理のための状態`STATE::EXIT_FATAL`に遷移します。



#### STATE::EXIT\_NORMAL

```
SleepNow();
```

`SleepNow()`を呼び出して、スリープ処理に入ります。



#### STATE::EXIT\_FATAL

```cpp
Serial << crlf << "!FATAL: RESET THE SYSTEM.";
delay(1000); // wait a while.
the_twelite.reset_system();
```

重大なエラーとして、システムリセットを行います。



### SleepNow()

```cpp
void SleepNow() {
	uint16_t u16dur = SLEEP_DUR;
	u16dur = random(SLEEP_DUR - SLEEP_DUR_TERMOR, SLEEP_DUR + SLEEP_DUR_TERMOR);

	Serial << int(millis()) << ":sleeping for " << int(u16dur) << "ms" << crlf;
	Serial.flush();

	step.on_sleep(); // reset status of statemachine to INIT state.

	the_twelite.sleep(u16dur, false);
}
```

周期スリープを行います。スリープ時間は`random()`関数を用いて、一定の時間ブレを作っています。これは複数のデバイスの送信周期が同期した場合、著しく失敗率が上がる場合があるためです。

スリープ前にはSM\_SIMPLEステートマシンの状態を`.on_sleep()`を呼び出してセットしておきます。



### Transmit()

```cpp
MWX_APIRET vTransmit() {
	Serial << int(millis()) << ":vTransmit()" << crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x3 send four times in total)
			<< tx_packet_delay(0,0,2); // send packet w/ delay (send first packet with randomized delay from 0 to 0ms, repeat every 2ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(FOURCC, 4) // string should be paired with length explicitly.
			, uint32_t(millis()) // put timestamp here.
			, uint16_t(sensor.dummy_work_ct_now) // put dummy sensor information.
		);
		
		// do transmit 
		//return nwksmpl.transmit(pkt);
		return pkt.transmit(); 
	}

	return MWX_APIRET(false, 0);
}
```

ID=`0x00`の親機宛に無線パケットの送信要求を行います。格納されるデータはActサンプルで共通に使われている４文字識別子(`FOURCC`)に加え、システム時間\[ms]とダミーセンサー値(`sensor.dummy_work_ct_now`)を格納します。

まず最初に送信パケットを格納するオブジェクトを取得します。このオブジェクトを操作し、送信データや条件を設定します。

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

mwx ライブラリでは、_if_文中でオブジェクトを取得し、そのオブジェクトの_bool_判定で_true_の場合に処理を行う記述を採用しています。ここでは`the_twelite.network.use<NWK_SIMPLE>()`によりボードオブジェクトを取得し、ボードオブジェクトの`.prepare_tx_packet()`によりパケットオブジェクトを取得しています。パケットオブジェクトの取得失敗は通常想定しませんが、失敗時は送信キューが一杯で送信要求が受け付けられない場合です。このサンプルは単一の送信のみですから、エラーは想定外の重大な問題に限られます。

```cpp
pkt << tx_addr(0x00) // 宛先
		<< tx_retry(0x1) // 再送回数
		<< tx_packet_delay(0,0,2); // 送信遅延
```

得られた`pkt`オブジェクトに対して、送信条件（宛先や再送など）を<<演算子を用いて設定します。[`tx_addr`](../api-reference/classes/packet\_tx.md#tx\_addr)はパケットの宛先を指定します。[`tx_retry`](../api-reference/classes/packet\_tx.md#tx\_retry)は再送回数、[`tx_packet_delay`](../api-reference/classes/packet\_tx.md#tx\_packet\_delay)は送信遅延の指定です。

```cpp
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(FOURCC, 4) // string should be paired with length explicitly.
	, uint32_t(millis()) // put timestamp here.
	, uint16_t(sensor.dummy_work_ct_now) // put dummy sensor information.
);	
```

パケットのペイロード（データ部分）は`pkt.get_payload()`により得られる[`smblbuf<uint8_t>派生`](../api-reference/classes/smplbuf/)の配列です。この配列に対して直接値を設定しても構いませんが、ここでは[`pack_bytes()`](../api-reference/funcs/utility/pack\_bytes.md)を用いた値の設定を行います。

{% hint style="info" %}
ペイロードの最大長は上記の例では91バイトですが、詳しくは[NWK\_SIMPLEパケット構造と最大長](../networks/nwk\_simple.md#pakettonoto)を参照ください。
{% endhint %}

この関数は可変数引数により指定できます。一番最初のパラメータは`.get_payload()`より得られた配列オブジェクトです。

* `make_pair(FOURCC,4)` : _make\_pair_はC++標準ライブラリのもので、_std::pair_オブジェクトを生成します。文字列型に対して先頭から４バイト分を書き出すという意味になります。\
  （文字列型の配列は終端を含める、含めないといった話題が混乱を生むため、明示的に書き出すバイト数を指定するために、このような指定をします）
* _`uint32_t`_型のデータを指定するとビッグエンディアン並びで４バイト分のデータを書き込みます。
* _`uint16_t`_型のデータについても同様です。

{% hint style="info" %}
uint8\_t 型のポインタを用いてデータの書き込みを行うことも出来ます。

```cpp
auto&& pay = pkt.get_payload(); // get buffer object.

// the following code will write data directly to internal buffer of `pay' object.
uint8_t *p = pay.begin(); // get the pointer of buffer head.

S_OCTET(p, FOURCC[0]); // store byte at pointer `p' and increment the pointer.
S_OCTET(p, FOURCC[1]);
S_OCTET(p, FOURCC[2]);
S_OCTET(p, FOURCC[3]);

S_DWORD(p, millis()); // store uint32_t data.
S_WORD(p, sensor.dummy_work_ct_now); // store uint16_t data.

pay.redim(p - pay.begin());
```

`.get_payload()`から得られた配列オブジェクトは、何も格納されていないサイズ０の配列ですが、この配列にデータを書き込むことでサイズが拡張され（実際は内部の固定長のバッファに対してデータを書き込み、内部管理のデータサイズを更新します）、最終的なサイズがペイロードのデータサイズです。

ここでは`.begin()`を用いて_`uint8_t*`_のポインタを得て、このポインタを用いてデータを書き込み、最後に書き込んだサイズを`.redim()`で設定します。

`S_OCTET(), S_WORD(), S_DWORD()`といった関数を書き込みに用いていますが、例えば`S_OCTET(p, 'H')`は `*p = 'H'; p++;`と同じ処理を行うポインタを用いたデータ書き込みです。

最後の`.redim()`は配列のサイズを**バッファの初期化をせずに**変更する手続きです。`.resize()`を呼び出すとすべて０クリアされます。
{% endhint %}



最後に`.transmit()`を呼び出して、送信要求を行います。戻り値は`MWX_APIRET`型です。要求後、実際の送信が行われますが、送信パラメータや送信サイズにもよりますが、完了まで数ms～数十ms程度はかかります。完了時には`on_tx_comp()`が呼び出されます。

```cpp
return pkt.transmit(); 
```

{% hint style="info" %}
``[`MWX_APIRET`](../api-reference/classes/mwx\_apiret.md)は`uint32_t`型をラップしたクラスで、MSBを失敗成功のフラグとし、以下31ビットをデータとして用いています。`pkt.transmit()`の戻り型になっており、送信要求の成功と失敗(_bool_型へのキャスト)ならびに送信IDをデータ部(`.get_value()`)に格納しています。
{% endhint %}



### on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}
```

送信完了時に呼び出されるシステムイベントです。ここでは`.set_flag()`により完了としています。
