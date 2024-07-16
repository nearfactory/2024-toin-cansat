---
title: "PAL_MAG"
author: "Mono Wireless Inc."
description: "PAL_MAG"
---
# PAL\_MAG

[開閉センサーパル OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) を用い、センサー値の取得を行います。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの送受信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ステートマシンによる状態遷移制御 - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MAG>](../boards/pal/pal\_mag.md)または[\<CUE>](../boards/cue.md)ボードビヘイビアによるボード操作
{% endhint %}



## アクトの機能

* 開閉センサーパル OPEN-CLOSE SENSE PAL を用い、磁気センサーの検出時に割り込み起床し、無線送信します。
* コイン電池で動作させるための、スリープ機能を利用します。



## アクトの使い方

### 必要なTWELITE

| 役割 | 例                                                                                                                                                                                               |
| -- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUEまたはRED</a>アクト<a href="parent_monostick.md">Parent_MONOSTICK</a>を動作させる。                                   |
| 子機 | [BLUE PAL または RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) +[開閉センサーパル OPEN-CLOSE SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |



## アクトの解説

### インクルード

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <PAL_MAG>
```

開閉センサーパルのボード ビヘイビア[`<PAL_MAG>`](../boards/pal/pal\_mag.md)をインクルードします。



### setup()

```cpp
void setup() {
	/*** SETUP section */
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_MAG>();
	// now it can read DIP sw status.
	u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
	if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE

	// LED setup (use periph_led_timer, which will re-start on wakeup() automatically)
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)

	// the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID); // set Logical ID. (0xFE means a child device with no ID)

	/*** BEGIN section */
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- PAL_MAG:" << FOURCHARS << " ---" << mwx::crlf;
}
```



最初にボードビヘイビア`<PAL_MAG>`を登録します。ボードビヘイビアの初期化時にセンサーやDIOの初期化が行われます。最初に行うのは、ボードのDIP SWなどの状態を確認してから、ネットワークの設定などを行うといった処理が一般的だからです。

```cpp
auto&& brd = the_twelite.board.use<PAL_MAG>();

u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE
```

ここでは、ボード上の４ビットDIP SWのうち３ビットを読み出して子機のIDとして設定しています。0の場合は、ID無しの子機(`0xFE`)とします。

LEDの設定を行います。ここでは 10ms おきに ON/OFF の点滅の設定をします（スリープを行い起床時間が短いアプリケーションでは、起床中は点灯するという設定とほぼ同じ意味合いになります）。

```cpp
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



### begin()

`begin()`関数は`setup()`関数を終了し(そのあとTWENETの初期化が行われる)一番最初の`loop()`の直前で呼ばれます。

```cpp
void begin() {
	sleepNow(); // the first time is just sleeping.
}
```

`setup()`終了後に`sleepNow()`を呼び出し初回スリープを実行します。



### sleepNow()

```cpp
void sleepNow() {
	uint32_t u32ct = 60000;
	
	pinMode(PAL_MAG::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
	pinMode(PAL_MAG::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);

	the_twelite.sleep(u32ct);
}
```

スリープに入るまえに磁気センサーのDIOピンの割り込み設定をします。`pinMode()`を用います。２番めのパラメータは`PIN_MODE::WAKE_FALLING`を指定しています。これはHIGHからLOWへピンの状態が変化したときに起床する設定です。

7行目で`the_twelite.sleep()`でスリープを実行します。パラメータの60000は、TWELITE PAL ボードのウォッチドッグをリセットするために必要な起床設定です。リセットしないと60秒経過後にハードリセットがかかります。

###

### wakeup()

スリープから復帰し起床すると `wakeup()` が呼び出されます。そのあと`loop()` が都度呼び出されます。`wakeup()`の前に、UARTなどの各ペリフェラルやボード上のデバイスのウェイクアップ処理（ウォッチドッグタイマーのリセットなど）が行われます。例えばLEDの点灯制御を再始動します。

```cpp
void wakeup() {
	if (the_twelite.is_wokeup_by_wktimer()) {
		sleepNow();
	}
}
```

ここではウェイクアップタイマーからの起床の場合(`the_twelite.is_wokeup_by_wktimer()`)は再びスリープを実行します。これは上述のウォッチドッグタイマーのリセットを行う目的のみの起床です。

磁気センサーの検出時の起床の場合は、このままloop()処理に移行します。



### loop()

ここでは、検出された磁気センサーのDIOの確認を行い、パケットの送信を行い、パケット送信完了後に再びスリープを実行します。

```cpp
void loop() {
	if (!b_transmit) {
		if (auto&& pkt = 
      the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet())

			uint8_t b_north = 
			  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
			uint8_t b_south = 
			  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
	
			Serial << "..sensor north=" << int(b_north) 
			       << " south=" << int(b_south) << mwx::crlf;
	
			// set tx packet behavior
			pkt << tx_addr(0x00)
				<< tx_retry(0x1)
				<< tx_packet_delay(0, 0, 2);
	
			// prepare packet payload
			pack_bytes(pkt.get_payload()
				, make_pair(FOURCHARS, 4) 
				, b_north
				, b_south
			);
	
			// do transmit
			MWX_APIRET ret = pkt.transmit();
	
			if (ret) {
				u8txid = ret.get_value() & 0xFF;
				b_transmit = true;
			}
			else {
				// fail to request
				sleepNow();
			}
		} else {
		  sleepNow();
		}
	} else { 
		if (the_twelite.tx_status.is_complete(u8txid)) {		
			b_transmit = 0;
			sleepNow();
		}
	}
}
```



`b_transmit`変数によって`loop()`内の振る舞いを制御しています。送信要求が成功した後、この値を1にセットしパケット送信完了待ちを行います。

```cpp
	if (!b_transmit) {
```



磁気センサーの検出DIOピンの確認を行います。検出ピンは二種類あります。N極検知とS極検知です。単に磁石が近づいたことだけを知りたいならいずれかのピンの検出されたことが条件となります。

```cpp
uint8_t b_north = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_NORTH);
uint8_t b_south = 
  the_twelite.is_wokeup_by_dio(PAL_MAG::PIN_SNS_SOUTH);
```

起床要因のピンを確認するには`the_twelite.is_wokeup_by_dio()`を用います。パラメータはピン番号です。戻り値をuint8\_tに格納しているのはパケットのペイロードに格納するためです。



通信条件の設定やペイロードにデータを格納後、送信を行います。

```cpp
// do transmit
MWX_APIRET ret = pkt.transmit();
```



その後、`loop()` 中 `b_transmit `が `true `になっている場合は、完了チェックを行い、完了すれば `sleepNow()` によりスリープします。

```cpp
if (the_twelite.tx_status.is_complete(u8txid)) {		
	b_transmit = 0;
	sleepNow();
}
```

送信完了に確認は `the_twelite.tx_status.is_complete(u8txid)` で行っています。`u8txid`は送信時に戻り値として戻されたID値です。
