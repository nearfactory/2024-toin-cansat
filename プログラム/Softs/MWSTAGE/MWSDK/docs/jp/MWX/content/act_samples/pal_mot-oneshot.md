---
title: PAL_MOT-single
author: "Mono Wireless Inc."
description: "PAL_MOT-single"
---

# PAL\_MOT-single

このアクトではスリープ復帰後に数サンプル加速度データを取得しそのデータを送ります。

{% hint style="success" %}
のアクトには以下が含まれます。

* 無線パケットの送受信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ステートマシンによる状態遷移制御 - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MOT>](../boards/pal/pal\_mot.md)または[\<CUE>](../boards/cue.md)ボードビヘイビアによるボード操作
{% endhint %}



## アクトの解説

起床→加速度センサーの取得開始→加速度センサーのFIFO割り込み待ち→加速度センサーのデータの取り出し→無線送信→スリープという流れになります。

{% hint style="info" %}
加速度センサーは、FIFOキューが一杯になるとFIFOキューへのデータ追加を停止します。
{% endhint %}



### 宣言部

#### インクルード

```cpp
#include <TWELITE>    // MWXライブラリ基本
#include <NWK_SIMPLE> // ネットワーク
#include <SM_SIMPLE>  // ステートマシン（状態遷移）
#include <STG_STD>    // インタラクティブモード

/*** board selection (choose one) */
#define USE_PAL_MOT
//#define USE_CUE
// board dependend definitions.
#if defined(USE_PAL_MOT)
#define BRDN PAL_MOT
#define BRDC <PAL_MOT>
#elif defined(USE_CUE)
#define BRDN CUE
#define BRDC <CUE>
#endif
// include board support
#include BRDC
```

MOT PALまたはTWELITE CUEに対応するため、インクルード部分はマクロになっています。`USE_PAL_MOT`または、`USE_CUE`のいずれかを定義します。

`USE_PAL_MOT` が定義されている場合は動作センサーパルのボードビヘイビア[`<PAL_MOT>`](../boards/pal/pal\_mot.md)をインクルードしています。



#### 状態定義

```cpp
enum class E_STATE : uint8_t {
	INTERACTIVE = 255,
	INIT = 0,
	START_CAPTURE,
	WAIT_CAPTURE,
	REQUEST_TX,
	WAIT_TX,
	EXIT_NORMAL,
	EXIT_FATAL
};
SM_SIMPLE<E_STATE> step;
```

`loop()`中の順次処理を行うために状態を定義し、また[ステートマシン](../api-reference/classes/smsimple-suttomashin.md)`step`を宣言します。



#### センサーデータ格納

```cpp
struct {
	int32_t x_ave, y_ave, z_ave;
	int32_t x_min, y_min, z_min;
	int32_t x_max, y_max, z_max;
	uint16_t n_seq;
	uint8_t n_samples;
} sensor;
```

センサーデータを格納するためのデータ構造です。



### setup()

```cpp
/// load board and settings objects
auto&& brd = the_twelite.board.use BRDC (); // load board support
auto&& set = the_twelite.settings.use<STG_STD>(); // load save/load settings(interactive mode) support
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>(); // load network support
```

ボード、設定、ネットワークの各ビヘイビアオブジェクトの登録を行います。



#### インタラクティブモード

```cpp
// settings: configure items
set << SETTINGS::appname("MOT");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set << SETTINGS::lid_default(0x1); // set default LID
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

// if SET=LOW is detected, start with intaractive mode.
if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
	set << SETTINGS::open_at_start();
	brd.set_led(LED_TIMER::BLINK, 300); // slower blink
	step.next(STATE::INTERACTIVE);
	return;
}

// load settings
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.
```

インタラクティブモードの初期化を行います。

まず、設定項目の調整を行います。ここでは、メニュー項目で表示されるタイトル名`SETTINGS::appname`、アプリケーションIDのデフォルト値の設定`SETTINGS::appid_default`、チャネルのデフォルト`SETTINGS::ch_default`、論理デバイスIDのデフォルト`SETTINGS::lid_default`、非表示項目の設定`.hide_items()`を行います。

このサンプルでは起動時にSETピンがLOである場合にインタラクティブモードに遷移します。`digitalRead(brd.PIN_SET)`によりピンがLOであることを確認できた場合は、`SETTINGS::open_at_start()`を指定します。この指定により`setup()`を抜けた後に速やかにインタラクティブモード画面が表示されます。画面が表示されても`begin()`や`loop()`が実行されます。このサンプルでは状態`STATE::INTERACTIVE`として`loop()`中ではスリープなどの動作はせず何もしないようにします。

続いて設定値を読み出します。設定値を読むには必ず`.reload()`を実行します。このサンプルではオプションビット設定`.u32opt1()`を読み出します。



#### the\_twelite

```cpp
the_twelite << set;
```

`the_twelite`は、システムの基本的な振る舞いを管理するクラスオブジェクトです。このオブジェクトは、`setup()`内でアプリケーションIDやチャネルなど様々な初期化を行います。

ここでは[インタラクティブモードの設定値の一部](../settings/stg\_std.md#no-1)を反映しています。

{% hint style="info" %}
インタラクティブモード設定で反映した項目を別の設定に変更したい場合は、続いて上書きしたい設定を行います。

```cpp
the_twelite << set;// インタラクティブモード
the_twelite << twenet::channel(19); // chを19に上書き設定
```
{% endhint %}



#### NWK\_SIMPLEオブジェクト

```cpp
nwk << set;
```

ネットワークビヘイビアオブジェクトに対しても設定を行います。インタラクティブモードの論理デバイスID(LID)と再送設定が反映されます。



#### その他、ハードウェアの初期化など

```cpp
brd.set_led(LED_TIMER::BLINK, 100);
```

LEDのブリンク設定などを行います。



### begin()

```cpp
void begin() { 
	auto&& set = the_twelite.settings.use<STG_STD>();
	if (!set.is_screen_opened()) {
		// sleep immediately, waiting for the first capture.
		sleepNow();
	}
}
```

`setup()`を終了した後に呼ばれます。ここでは初回スリープを実行しています。ただしインタラクティブモードの画面が表示される場合はスリープしません。



### wakeup()

```cpp
void wakeup() {
	Serial << crlf << "--- PAL_MOT(OneShot):" 
	       << FOURCHARS << " wake up ---" << crlf;
	eState = E_STATE::INIT;
}
```

起床後は状態変数`eState`を初期状態INITにセットしています。この後`loop()`が実行されます。



### loop()

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();

	do {
		switch(step.state()) {
			case STATE::INTERACTIVE:
			break;
		...
	} while(step.b_more_loop());
}
```

`loop()` の基本構造は`<SM_STATE>`ステートマシン`state`を用い_switch ... case_節での制御です。初期状態は`STATE::INIT`または`STATE::INTERACTIVE`です。



#### STATE::INTERACTIVE

インタラクティブモード画面が表示されているときの状態です。何もしません。この画面ではSerialの入出力はインタラクティブモードが利用します。



#### STATE::INIT

初期状態のINITです。

```cpp
case STATE::INIT:
	brd.sns_MC3630.get_que().clear(); // clear queue in advance (just in case).
	memset(&sensor, 0, sizeof(sensor)); // clear sensor data
	step.next(STATE::START_CAPTURE);
break;
```

状態INITでは、初期化（結果格納用のキューのクリア）や結果格納用のデータ構造の初期化を行います。STATE::START\_CAPTUREに遷移します。この遷移設定後、もう一度_while_ループが実行されます。



#### STATE::CAPTURE

```cpp
case STATE::START_CAPTURE:
	brd.sns_MC3630.begin(
		// 400Hz, +/-4G range, get four samples and will average them.
		SnsMC3630::Settings(
			SnsMC3630::MODE_LP_400HZ, SnsMC3630::RANGE_PLUS_MINUS_4G, N_SAMPLES)); 

	step.set_timeout(100);
	step.next(STATE::WAIT_CAPTURE);
break;
```

状態START\_CAPTUREでは、MC3630センサーのFIFO取得を開始します。ここでは400Hzで4サンプル取得できた時点でFIFO割り込みが発生する設定にしています。

例外処理のためのタイムアウトの設定と、次の状態`STATE::WAIT_CAPTURE`に遷移します。



#### STATE::WAIT\_CAPTURE

```cpp
case STATE::WAIT_CAPTURE:
	if (brd.sns_MC3630.available()) {
		brd.sns_MC3630.end(); // stop now!
```

状態WAIT\_CAPTUREでは、FIFO割り込みを待ちます。割り込みが発生し結果格納用のキューにデータが格納されると`sns_MC3630.available()`が`true`になります。`sns_MC3630.end()`を呼び出し処理を終了します。



```cpp
sensor.n_samples = brd.sns_MC3630.get_que().size();
if (sensor.n_samples) sensor.n_seq = brd.sns_MC3630.get_que()[0].get_t();
...
```

サンプル数とサンプルのシーケンス番号を取得します。



```cpp
// get all samples and average them.
for (auto&& v: brd.sns_MC3630.get_que()) {
	sensor.x_ave  += v.x;
	sensor.y_ave  += v.y;
	sensor.z_ave  += v.z;
}

if (sensor.n_samples == N_SAMPLES) {
	// if N_SAMPLES == 2^n, division is much faster.
	sensor.x_ave /= N_SAMPLES;
	sensor.y_ave /= N_SAMPLES;
	sensor.z_ave /= N_SAMPLES;
}
...
```

すべてのサンプルデータに対して読み出し、平均値をとる処理をします。



```cpp
// can also be:
//	int32_t x_max = -999999, x_min = 999999;
//	for (auto&& v: brd.sns_MC3630.get_que()) {
//		if (v.x >= x_max) x_max = v.x;
//		if (v.y <= x_min) x_min = v.x;
//		...
//	}	
auto&& x_minmax = std::minmax_element(
	get_axis_x_iter(brd.sns_MC3630.get_que().begin()),
	get_axis_x_iter(brd.sns_MC3630.get_que().end()));
sensor.x_min = *x_minmax.first;
sensor.x_max = *x_minmax.second;
...
```

ここでは取得されたサンプルに対して、各軸に対応するイテレータを用い最大・最小を得ています。

{% hint style="info" %}
C++ Standard Template Library のアルゴリズムを使用する例として`std::mimmax_element`紹介していますが、コメント内のようにforループ内で最大、最小を求めても構いません。
{% endhint %}



```cpp
if (brd.sns_MC3630.available()) {
  ...
  brd.sns_MC3630.get_que().clear(); // clean up the queue
  step.next(STATE::REQUEST_TX); // next state
} else if (step.is_timeout()) {
  Serial << crlf << "!!!FATAL: SENSOR CAPTURE TIMEOUT.";
  step.next(STATE::EXIT_FATAL);
}
break;
```

`.sns_MC3630.get_que().clear()`を呼び出し、キューにあるデータをクリアします。これを呼び出さないと続くサンプル取得ができません。その後`STATE::REQUEST_TX`状態に遷移します。

.`is_timeout()`はタイムアウトをチェックします。タイムアウト時は異常として`STATE::EXIT_FATAL`に遷移します。



#### STATE::REQUEST\_TX

```cpp
case STATE::REQUEST_TX:
	if (TxReq()) {
		step.set_timeout(100);
		step.clear_flag();
		step.next(STATE::WAIT_TX);
	} else {
		Serial << crlf << "!!!FATAL: TX REQUEST FAILS.";
		step.next(STATE::EXIT_FATAL);
	}
break;
```

状態`REQUEST_TX`ではローカル定義関数`TxReq()`を呼び出し、得られたセンサーデータの処理と送信パケットの生成・送信を行います。送信要求は送信キューの状態などで失敗することがあります。送信要求が成功した場合、TxReq()はtrueとして戻りますが、まだ送信は行われません。送信完了は`on_tx_comp()`コールバックが呼び出されます。

また`.clear_flag()`により送信完了を知らせるためのフラグをクリアしておきます。同時にタイムアウトも設定します。



#### E\_SATE::WAIT\_TX

```cpp
case STATE::WAIT_TX:
	if (step.is_flag_ready()) {
		step.next(STATE::EXIT_NORMAL);
	}
	if (step.is_timeout()) {
		Serial << crlf << "!!!FATAL: TX TIMEOUT.";
		step.next(STATE::EXIT_FATAL);
	}
break;
```

状態`STATE::WAIT_TX`では、無線パケットの送信完了を待ちます。フラグは`on_tx_comp()`コールバック関数でセットされ、セット後に`.is_flag_ready()`が_true_になります。



#### E\_SATE::EXIT\_NORMAL, FATAL

```cpp
case STATE::EXIT_NORMAL:
	sleepNow();
break;

case STATE::EXIT_FATAL:
	Serial << flush;
	the_twelite.reset_system();
break;
```

一連の動作が完了したときは状態`STATE::EXIT_NORMAL`に遷移しローカル定義の関数`sleepNow()`を呼び出しスリープを実行します。またエラーを検出した場合は状態`STATE::EXIT_FATAL`に遷移し、システムリセットを行います。



### MWX\_APIRET TxReq()

```cpp
MWX_APIRET TxReq() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();
	MWX_APIRET ret = false;

	// prepare tx packet
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {		
		// set tx packet behavior
		pkt << tx_addr(0x00)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x1 send two times in total)
			<< tx_packet_delay(0, 0, 2); // send packet w/ delay
		
		// prepare packet (first)
		pack_bytes(pkt.get_payload() // set payload data objects.
				, make_pair(FOURCHARS, 4)  // just to see packet identification, you can design in any.
				, uint16_t(sensor.n_seq)
				, uint8_t(sensor.n_samples)
				, uint16_t(sensor.x_ave)
				, uint16_t(sensor.y_ave)
				, uint16_t(sensor.z_ave)
				, uint16_t(sensor.x_min)
				, uint16_t(sensor.y_min)
				, uint16_t(sensor.z_min)
				, uint16_t(sensor.x_max)
				, uint16_t(sensor.y_max)
				, uint16_t(sensor.z_max)
			);

		// perform transmit
		ret = pkt.transmit();

		if (ret) {
			Serial << "..txreq(" << int(ret.get_value()) << ')';
		}
	}

	return ret;
}
```

最期にパケットの生成と送信を要求を行います。パケットには続き番号、サンプル数、XYZの平均値、XYZの最小サンプル値、XYZの最大サンプル値を含めます。



### sleepNow()

```cpp
void sleepNow() {
	Serial << crlf << "..sleeping now.." << crlf;
	Serial.flush();
	step.on_sleep(false); // reset state machine.
	the_twelite.sleep(3000, false); // set longer sleep (PAL must wakeup less than 60sec.)
}
```

スリープの手続きです。

* シリアルポートはスリープ前に`Serial.flush()`を呼び出してすべて出力しておきます。
* `<SM_SIMPLE>`ステートマシンは`on_sleep()`を行う必要があります。







