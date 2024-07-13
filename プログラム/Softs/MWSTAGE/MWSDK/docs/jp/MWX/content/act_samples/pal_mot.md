---
title: PAL_MOT-fifo
author: "Mono Wireless Inc."
description: "PAL_MOT-fifo"
---

# PAL\_MOT-fifo

[動作センサーパル MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) を用い、センサー値の取得を行います。

{% hint style="success" %}
のアクトには以下が含まれます。

* 無線パケットの送受信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ステートマシンによる状態遷移制御 - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MOT>](../boards/pal/pal\_mot.md)または[\<CUE>](../boards/cue.md)ボードビヘイビアによるボード操作
{% endhint %}



## アクトの機能

* 動作センサーパル MOTION SENSE PAL を用い、加速度センサーの加速度を連続的に計測し、無線送信します。
* コイン電池で動作させるための、スリープ機能を利用します。



## アクトの使い方

### 必要なTWELITE

| 役割 | 例                                                                                                                                                                                           |
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUEまたはRED</a>アクト<a href="parent_monostick.md">Parent_MONOSTICK</a>を動作させる。                               |
| 子機 | [BLUE PAL または RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) +[動作センサーパル MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |



## アクトの解説

### インクルード

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <PAL_>
```

 動作センサーパルのボードビヘイビア[`<PAL_MOT>`](../boards/pal/pal\_mot.md)をインクルードします。



### setup()

```cpp
void setup() {
	/*** SETUP section */
	// board
	auto&& brd = the_twelite.board.use<PAL_MOT>();
	brd.set_led(LED_TIMER::BLINK, 100);

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)     
		<< TWENET::channel(CHANNEL);

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(0xFE); 
	
	/*** BEGIN section */
	the_twelite.begin(); // start twelite!
	brd.sns_MC3630.begin(SnsMC3630::Settings(
		SnsMC3630::MODE_LP_14HZ, SnsMC3630::RANGE_PLUS_MINUS_4G));

	/*** INIT message */
	Serial << "--- PAL_MOT(Cont):" << FOURCHARS 
				 << " ---" << mwx::crlf;
}

```



最初にボードビヘイビア`<PAL_MOT>`を登録します。ボードビヘイビアの初期化時にセンサーやDIOの初期化が行われます。最初に行うのは、ボードのDIP SWなどの状態を確認してから、ネットワークの設定などを行うといった処理が一般的だからです。

```cpp
auto&& brd = the_twelite.board.use<PAL_MOT>();

u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE
```

ここでは、ボード上の４ビットDIP SWのうち３ビットを読み出して子機のIDとして設定しています。0の場合は、ID無しの子機(`0xFE`)とします。

LEDの設定を行います。ここでは 10ms おきに ON/OFF の点滅の設定をします（スリープを行い起床時間が短いアプリケーションでは、起床中は点灯するという設定とほぼ同じ意味合いになります）。

```cpp
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



#### 加速度センサーの初期化

```cpp
	brd.sns_MC3630.begin(SnsMC3630::Settings(
		SnsMC3630::MODE_LP_14HZ, SnsMC3630::RANGE_PLUS_MINUS_4G));
```

加速度センサーの計測を開始します。加速度センサーの設定(`SnsMC3630::Settings`)には計測周波数と測定レンジを指定します。ここでは14HZの計測(`SnsMC3630::MODE_LP_14HZ`)で、±4Gのレンジ(`SnsMC3630::RANGE_PLUS_MINUS_4G`)で計測します。

開始後は加速度センサーの計測が秒14回行われ、その値はセンサー内部のFIFOキューに保存されます。センサーに28回分の計測が終わった時点で通知されます。



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
	pinMode(PAL_MOT::PIN_SNS_INT, WAKE_FALLING);
	the_twelite.sleep(60000, false);
}
```

スリープに入るまえに加速度センサーのDIOピンの割り込み設定をします。FIFOキューが一定数まで到達したときに発生する割り込みです。`pinMode()`を用います。２番めのパラメータは`PIN_MODE::WAKE_FALLING`を指定しています。これはHIGHからLOWへピンの状態が変化したときに起床する設定です。

３行目で`the_twelite.sleep()`でスリープを実行します。パラメータの60000は、TWELITE PAL ボードのウォッチドッグをリセットするために必要な起床設定です。リセットしないと60秒経過後にハードリセットがかかります。

###

### wakeup()

加速度センサーのFIFO割り込みにより、スリープから復帰し起床すると `wakeup()` が呼び出されます。そのあと`loop()` が都度呼び出されます。`wakeup()`の前に、UARTなどの各ペリフェラルやボード上のデバイスのウェイクアップ処理（ウォッチドッグタイマーのリセットなど）が行われます。例えばLEDの点灯制御を再始動します。

```cpp
void wakeup() {
	Serial << "--- PAL_MOT(Cont):" << FOURCHARS
	       << " wake up ---" << mwx::crlf;

	b_transmit = false;
	txid[0] = 0xFFFF;
	txid[1] = 0xFFFF;
}
```

ここでは`loop()`で使用する変数の初期化を行っています。



### loop()

ここでは、加速度センサー内のFIFOキューに格納された加速度情報を取り出し、これをもとにパケット送信を行います。パケット送信完了後に再びスリープを実行します。

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();

	if (!b_transmit) {
		if (!brd.sns_MC3630.available()) {
			Serial << "..sensor is not available." 
					<< mwx::crlf << mwx::flush;
			sleepNow();
		}

		// send a packet
		Serial << "..finish sensor capture." << mwx::crlf
			<< "  seq=" << int(brd.sns_MC3630.get_que().back().t) 
			<< "/ct=" << int(brd.sns_MC3630.get_que().size());

		// calc average in the queue.
		{
			int32_t x = 0, y = 0, z = 0;
			for (auto&& v: brd.sns_MC3630.get_que()) {
				x += v.x;
				y += v.y;
				z += v.z;
			}
			x /= brd.sns_MC3630.get_que().size();
			y /= brd.sns_MC3630.get_que().size();
			z /= brd.sns_MC3630.get_que().size();

			Serial << format("/ave=%d,%d,%d", x, y, z) << mwx::crlf;
		}

		for (int ip = 0; ip < 2; ip++) {
			if(auto&& pkt = 
				the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet())
				
				// set tx packet behavior
				pkt << tx_addr(0x00)
					<< tx_retry(0x1)
					<< tx_packet_delay(0, 0, 2);
			
				// prepare packet (first)
				uint8_t siz = (brd.sns_MC3630.get_que().size() >= MAX_SAMP_IN_PKT)
									? MAX_SAMP_IN_PKT : brd.sns_MC3630.get_que().size();
				uint16_t seq = brd.sns_MC3630.get_que().front().t;
			
				pack_bytes(pkt.get_payload()
					, make_pair(FOURCHARS, 4)
					, seq 
					, siz
				);

				// store sensor data (36bits into 5byts, alas 4bits are not used...)
				for (int i = 0; i < siz; i++) {
					auto&& v = brd.sns_MC3630.get_que().front();
					uint32_t v1;

					v1  = ((uint16_t(v.x/2) & 4095) << 20)  // X:12bits
						| ((uint16_t(v.y/2) & 4095) <<  8)  // Y:12bits
						| ((uint16_t(v.z/2) & 4095) >>  4); // Z:8bits from MSB
					uint8_t v2 = (uint16_t(v.z/2) & 255);   // Z:4bits from LSB
					pack_bytes(pkt.get_payload(), v1, v2); // add into pacekt entry.
					brd.sns_MC3630.get_que().pop(); // pop an entry from queue.
				}

				// perform transmit
				MWX_APIRET ret = pkt.transmit();

				if (ret) {
					Serial << "..txreq(" << int(ret.get_value()) << ')';
					txid[ip] = ret.get_value() & 0xFF;
				} else {
					sleepNow();
				}
			}
		}

		// finished tx request
		b_transmit = true;
	} else {
		if(		the_twelite.tx_status.is_complete(txid[0])
			 && the_twelite.tx_status.is_complete(txid[1]) ) {

			sleepNow();
		}
	}
}

```



`b_transmit`変数によって`loop()`内の振る舞いを制御しています。送信要求が成功した後、この値を1にセットしパケット送信完了待ちを行います。

```cpp
	if (!b_transmit) {
```



最初にセンサーがavailableかどうかを確認します。割り込み起床後であるため、availableでないのは通常ではなく、そのままスリープします。

```cpp
if (!brd.sns_MC3630.available()) {
	Serial << "..sensor is not available." 
			<< mwx::crlf << mwx::flush;
	sleepNow();
}
```



無線送信パケットでは使用しないのですが、取り出した加速度の情報を確認してみます。

```cpp
Serial << "..finish sensor capture." << mwx::crlf
	<< "  seq=" << int(brd.sns_MC3630.get_que().front().t) 
	<< "/ct=" << int(brd.sns_MC3630.get_que().size());

// calc average in the queue.
{
	int32_t x = 0, y = 0, z = 0;
	for (auto&& v: brd.sns_MC3630.get_que()) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	x /= brd.sns_MC3630.get_que().size();
	y /= brd.sns_MC3630.get_que().size();
	z /= brd.sns_MC3630.get_que().size();

	Serial << format("/ave=%d,%d,%d", x, y, z) << mwx::crlf;
}
```

加速度センサーの計測結果は`brd.sns_MC3630.get_que()`で得られるFIFOキューに格納されます。

加速度センサーの計測結果を格納している構造体 `axis_xyzt` は x, y, z の三軸の情報に加え、続き番号 t が格納されています。

格納されているサンプル数はキューのサイズ(`brd.sns_MC3630.get_que().size()`)を読み出すことで確認できます。通常は２８サンプルですが処理の遅延等によりもう少し進む場合もあります。最初のサンプルは`front()`で取得することができます。その続き番号は`front().t`になります。

ここでは、サンプルをキューから取り出す前にサンプルの平均をとってみます。キューの各要素にはfor文(`for (auto&& v: brd.sns_MC3630.get_que()) { ... }`) でアクセスできます。for文内の `v.x, v.y, v.z` が各要素になります。ここでは各要素の合計を計算しています。for文終了後は要素数で割ることで平均を計算しています。



次にパケットを生成して送信要求を行いますが、データ量が大きいため２回に分けて送信します。そのため送信処理がfor文で２回行われます。

```cpp
		for (int ip = 0; ip < 2; ip++) {
```



送信するパケットに含めるサンプル数とサンプル最初の続き番号をパケットのペイロードの先頭部分に格納します。

```cpp
// prepare packet (first)
uint8_t siz = (brd.sns_MC3630.get_que().size() >= MAX_SAMP_IN_PKT)
? MAX_SAMP_IN_PKT : brd.sns_MC3630.get_que().size();
uint16_t seq = brd.sns_MC3630.get_que().front().t;

pack_bytes(pkt.get_payload()
	, make_pair(FOURCHARS, 4)
	, seq 
	, siz
);
```



最後に加速度データを格納します。先程は平均値の計算のためにキューの各要素を参照のみしましたが、ここではキューから１サンプルずつ読み出してパケットのペイロードに格納します。

```cpp
for (int i = 0; i < siz; i++) {
	auto&& v = brd.sns_MC3630.get_que().front();
	uint32_t v1;

	v1  = ((uint16_t(v.x/2) & 4095) << 20)  // X:12bits
		| ((uint16_t(v.y/2) & 4095) <<  8)  // Y:12bits
		| ((uint16_t(v.z/2) & 4095) >>  4); // Z:8bits from MSB
	uint8_t v2 = (uint16_t(v.z/2) & 255);   // Z:4bits from LSB
	pack_bytes(pkt.get_payload(), v1, v2); // add into pacekt entry.
	brd.sns_MC3630.get_que().pop(); // pop an entry from queue.
}
```

加速度センサーからのデータキューの先頭を読み出すのは`.front()`を用います。読みだした後`.pop()`を用いて先頭キューを開放します。

加速度センサーから取得されるデータは1Gを1000としたミリGの単位です。レンジを±4Gとしているため、12bitの範囲に入るように２で割って格納します。データ数を節約するため最初の4バイトにX,Y軸とZ軸の上位8bitを格納し、次の1バイトにZ軸の下位4bitの合計5バイトを生成します。



２回分の送信待ちを行うため送信IDは`txid[]`配列に格納します。

```cpp
MWX_APIRET ret = pkt.transmit();

if (ret) {
	Serial << "..txreq(" << int(ret.get_value()) << ')';
	txid[ip] = ret.get_value() & 0xFF;
} else {
	sleepNow();
}
```



その後、`loop()` 中 `b_transmit `が `true`になっている場合は、完了チェックを行い、完了すれば `sleepNow()` によりスリープします。

```cpp
} else {
	if(		the_twelite.tx_status.is_complete(txid[0])
		 && the_twelite.tx_status.is_complete(txid[1]) ) {

		sleepNow();
	}
}
```

送信完了に確認は `the_twelite.tx_status.is_complete()` で行っています。`txid[]`は送信時に戻り値として戻されたID値です。

