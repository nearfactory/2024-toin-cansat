---
title: "PAL_AMB-behavior"
author: "Mono Wireless Inc."
description: "PAL_AMB-behavior"
---
# PAL\_AMB-behavior

[環境センサーパル AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) を用い、センサー値の取得を行います。

* [ビヘイビア](./)を用いた親機子機の記述を行っています。
* センサーを値を得るのに[ボードビヘイビア](../../boards/pal/pal\_amb.md)の機能を使わず[`Wire`](../predefined\_objs/wire/)を用いて直接記述しています。
* 子機はステートマシンによる[状態遷移](./#mashin)により記述しています。

{% hint style="success" %}
このアクトの解説の前に[BRD\_APPTWELITEの解説](../../act\_samples/brd\_apptwelite.md)、[PAL\_AMBの解説](../../act\_samples/pal\_amb.md)、[PAL\_AMB-usenapの解説](../../act\_samples/pal\_amb-usenap.md)を参照してください。また[ビヘイビアの解説](./)についても参照ください。
{% endhint %}

{% hint style="info" %}
このサンプルは[ビヘイビア](./)の記述方法のサンプルです。ビヘイビアはより複雑なアプリケーションを記述する際に用います。
{% endhint %}

## アクトの機能

* 環境センサーパル AMBIENT SENSE PAL を用い、センサー値の取得を行います。
* コイン電池で動作させるための、スリープ機能を利用します。



## アクトの使い方

### TWELITEの準備

| 役割 | 例                                                                                                                                                                                            |
| -- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | [MONOSTICK BLUEまたはRED](https://mono-wireless.com/jp/products/MoNoStick/)                                                                                                                     |
| 子機 | [BLUE PAL または RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) +[環境センサーパル AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |

{% hint style="warning" %}
親機にPALを使用する場合は、コイン電池での動作はできません。目安として50mA以上の電流を安定して得られるような電源環境を用意してください。
{% endhint %}



## ファイル構成

* PAL\_AMB-behavior.hpp : `setup()`のみの定義です。DIP-SWを読み出し、D1..D3が上位置の場合は親機として動作し、それ以外は子機としてDIP SWに対応するIDをセットします。
* Parent/myAppBhvParent.hpp : 親機用のビヘイビアクラス定義
* Parent/myAppBhvParent.cpp : 実装&#x20;
* Parent/myAppBhvParent-handlers.cpp : ハンドラーの実装
* Parent/myAppBhvParent.hpp : 子機用のビヘイビアクラス定義
* Parent/myAppBhvParent.cpp : 実装
* Parent/myAppBhvParent-handlers.cpp : ハンドラーの実装

親機のビヘイビア名は`<MY_APP_PARENT>`、子機は`<MY_APP_CHILD>`です。

{% hint style="info" %}
ビルドファイルの追加は[Makefileの解説](../../install\_n\_build/makefile.md)を参照してください。
{% endhint %}



## 初期化 setup()

```cpp
// now read DIP sw status can be read.
u8ID = (brd.get_DIPSW_BM() & 0x07);

// Register App Behavior (set differnt Application by DIP SW settings)
if (u8ID == 0) {
	// put settings to the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL)  // set channel (pysical channel)
		<< TWENET::rx_when_idle();   // open RX channel

	the_twelite.app.use<MY_APP_PARENT>();
} else {		
	// put settings to the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)

	the_twelite.app.use<MY_APP_CHILD>();
}
```

DIP SWの読み値が0の場合は親機用のビヘイビア`<MY_APP_PARENT>`を、それ以外の場合は子機用のビヘイビア`<MY_APP_CHILD>`を登録します。

{% hint style="warning" %}
親機がMONOSTICKの場合は、PAL用のDIP SWの読み値は0となり、親機としてふるまいます。ただしこの動作はMONOSTICKの仕様として定義されているものではありません。
{% endhint %}



## 親機のビヘイビア

親機はスリープをしない受信機としてふるまい、子機からのパケットを受信したときにシリアルポートにパケットの情報を出力します。



### MY\_APP\_PARENT::receive()

```cpp
void MY_APP_PARENT::receive(mwx::packet_rx& rx) {
	uint8_t msg[4];
	uint32_t lumi;
	uint16_t u16temp, u16humid;

	// expand packet payload (shall match with sent packet data structure, see pack_bytes())
	auto&& np = expand_bytes(rx.get_payload().begin(), rx.get_payload().end(), msg);
	
	// if PING packet, respond pong!
	if (!strncmp((const char*)msg, (const char*)FOURCHARS, 4)) {
		// get rest of data
		expand_bytes(np, rx.get_payload().end(), lumi, u16temp, u16humid);

		// print them
		Serial << format("Packet(%x:%d/lq=%d/sq=%d): ",
							rx.get_addr_src_long(), rx.get_addr_src_lid(),
							rx.get_lqi(), rx.get_psRxDataApp()->u8Seq)
			   << "temp=" << double(int16_t(u16temp)/100.0)
			   << "C humid=" << double(int16_t(u16humid)/100.0)
			   << "% lumi=" << int(lumi)
			   << mwx::crlf << mwx::flush;
    }
}
```

親機用がパケットを受信したときは、パケットの先頭４文字が照合（`FOURCHARS`）できれば、そのパケット内容を表示します。



### MY\_APP\_PARENT::MWX\_TICKTIMER\_INT()

```cpp
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled) {
  // blink LED
  digitalWrite(PAL_AMB::PIN_LED, 
    ((millis() >> 9) & 1) ? PIN_STATE::HIGH : PIN_STATE::LOW);
}
```

親機の割り込みハンドラはLEDの点滅を行います。



### MY\_APP\_PARENT::MWX\_DIO\_EVENT(PAL\_AMB::PIN\_BTN)

```cpp
MWX_DIO_EVENT(PAL_AMB::PIN_BTN, uint32_t arg) {
	Serial << "Button Pressed" << mwx::crlf;
	
	static uint32_t u32tick_last;
	uint32_t tick = millis();

	if (tick - u32tick_last > 100) {
		PEV_Process(E_ORDER_KICK, 0UL);
	}

	u32tick_last = tick;
}
```

PAL上のボタン(5)が押されたときには、状態マシンに対して`E_ORDER_KICK`イベントを発行します。



### MY\_APP\_PARENT::MWX\_STATE(E\_MWX::STATE\_0 .. 3)

状態マシンは、状態遷移の参考として記述したもので、アプリケーションの動作上意味のあるものではありません。ボタンから送付されるE\_ORDER\_KICKイベントによる状態遷移や、タイムアウトなどを実行しています。



## 子機のビヘイビア

子機の動作の流れはPAL\_AMB-usenapと同じです。初回スリープから「起床→センサー動作開始→短いスリープ→起床→センサー値取得→無線送信→無線送信完了待ち→スリープ」を繰り返します。



### MY\_APP\_CHILD::on\_begin()

```cpp
void _begin() {
    // sleep immediately.
    Serial << "..go into first sleep (1000ms)" << mwx::flush;
    the_twelite.sleep(1000);
}
```

`on_begin()`から呼び出される`_begin()`関数では、初回スリープを実行しています。

(※`_begin()`関数で本処理を記述せず`on_begin()`に直接記述してもかまいません)



### MY\_APP\_CHILD::wakeup()

```cpp
void wakeup(uint32_t & val) {
    Serial << mwx::crlf << "..wakeup" << mwx::crlf;
    // init wire device.
    Wire.begin();
    
    // turn on LED
    digitalWrite(PAL_AMB::PIN_LED, PIN_STATE::LOW);

    // KICK it!
    PEV_Process(E_ORDER_KICK, 0); // pass the event to state machine
}
```

スリープからの起床処理を記述しています。

ここで初回の`Wire.begin()`を実行しています。２回目以降のスリープ起床時では冗長な記述です。この処理は`on_begin()`に移動してもかまいません。



### MY\_APP\_CHILD::transmit\_complete()

```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}
```

送信完了時に状態マシンに対して`E_ORDER_KICK`メッセージを処理します。



### MY\_APP\_CHILD::transmit\_complete()

```cpp
static const uint8_t STATE_IDLE = E_MWX::STATE_0;
static const uint8_t STATE_SENSOR = E_MWX::STATE_1;
static const uint8_t STATE_TX = E_MWX::STATE_2;
static const uint8_t STATE_SLEEP = E_MWX::STATE_3;
```

状態名を定義しています。



### MY\_APP\_CHILD::shtc3\_???()

```cpp
MWX_APIRET MY_APP_CHILD::shtc3_start()
MWX_APIRET MY_APP_CHILD::shtc3_read()
```

SHTC3用のセンサー取得実装例です。送付コマンド等の詳細はSHTC3のデータシートなどを参考にしてください。



### MY\_APP\_CHILD::ltr308als\_???()

```cpp
MWX_APIRET MY_APP_CHILD::ltr308als_read()
MWX_APIRET MY_APP_CHILD::ltr308als_start()
static MWX_APIRET WireWriteAngGet(uint8_t addr, uint8_t cmd)
```

LTR308ALSのセンサー取得実装例です。送付コマンド等の詳細はLTR308ALSのデータシートなどを参考にしてください。

`WireWriteAndGet()`は`addr`のデバイスに対して`cmd`を１バイト送信してから、１バイト受信して値を返します。



### MY\_APP\_CHILD::STATE\_IDLE (0)

```cpp
MWX_STATE(MY_APP_CHILD::STATE_IDLE, uint32_t ev, uint32_t evarg) {
	if (PEV_is_coldboot(ev,evarg)) {
		Serial << "[STATE_IDLE:START_UP(" << int(evarg) << ")]" << mwx::crlf;
		// then perform the first sleep at on_begin().
	} else
	if (PEV_is_warmboot(ev,evarg)) {
		Serial << "[STATE_IDLE:START_UP(" << int(evarg) << ")]" << mwx::crlf;
		PEV_SetState(STATE_SENSOR);
	}
}
```

0番の状態は特別な意味を持ちます。起動直後またはスリープ復帰後の状態です。

起動直後`PEV_is_coldboot(ev,evarg)`判定が`true`になって呼び出されます。`on_begin()`から、そのままスリープしてしまうため、状態遷移するようなコードも含まれません。**この時点では主要な初期化がまだ終わっていませんので、無線パケットの送信など複雑な処理を行うことが出来ません。**そのような処理を行うための最初の状態遷移を行うためには`on_begin()`からイベントを送り、そのイベントに従って状態遷移を行います。

スリープ復帰後は`PEV_is_warmboot(ev,evarg)`が`true`になる呼び出しが最初にあります。`PEV_SetState()`を呼び出し`STATE_SENSOR`状態に遷移します。



### MY\_APP\_CHILD::STATE\_SENSOR

```cpp
MWX_STATE(MY_APP_CHILD::STATE_SENSOR, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_SENSOR:NEW] Start Sensor." << mwx::crlf;

		// start sensor capture
		shtc3_start();
		ltr308als_start();

		// take a nap waiting finish of capture.
		Serial << "..nap for 66ms" << mwx::crlf;
		Serial.flush();
		PEV_KeepStateOnWakeup(); // stay this state on waking up.
		the_twelite.sleep(66, false, false, TWENET::SLEEP_WAKETIMER_SECONDARY);
	} else
	if (PEV_is_warmboot(ev,evarg)) {
		// on wakeup, code starts here.
		Serial << "[STATE_SENSOR:START_UP] Wakeup." << mwx::crlf;

		PEV_SetState(STATE_TX);
	}
}
```

スリープ復帰後`STATE_IDLE`から遷移したとき、`STATE_SENSOR`の状態ハンドラが続けて呼び出されます。この時のイベント`ev`は`E_EVENT_NEW_STATE`です。

ここではSHTC3, LTR308ALSの２センサーの動作開始を行います。一定時間経過すれば、センサーはデータ取得可能な状態になります。この時間待ちを`66`ms設定のスリープで行います。スリープ前に`PEV_KeepStateOnWakeup()`が呼ばれている点に注意してください。この呼び出しを行うと、スリープ復帰後の状態は`STATE_IDLE`ではなく、スリープしたときの状態、つまり`STATE_SENSOR`となります。

短いスリープから復帰すると`PEV_is_warmboot(ev,evarg)`判定が`true`となる呼び出しが最初に発生します。この呼び出し時点で、無線パケットの送信などを行うことが出来ます。`STATE_TX`に遷移します。



### MY\_APP\_CHILD::STATE\_TX

```cpp
MWX_STATE(MY_APP_CHILD::STATE_TX, uint32_t ev, uint32_t evarg)
	static int u8txid;

	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_TX:NEW]" << mwx::crlf;
		u8txid = -1;

		auto&& r1 = shtc3_read();
		auto&& r2 = ltr308als_read();

		Serial << "..shtc3 t=" << int(i16Temp) << ", h=" << int(i16Humd) << mwx::crlf;
		Serial << "..ltr308als l=" << int(u32Lumi) << mwx::crlf;

		if (r1 && r2) {
			if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

ここでは`E_EVENT_NEW_STATE`イベントの時に、センサーデータ読み出し、無線パケットの送信手続きに入ります。送信手続きの詳細は他のアクトサンプル例を参考にしてください。

```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}

    // ↓　↓　↓ メッセージ送付

} else if (ev == E_ORDER_KICK && evarg == uint32_t(u8txid)) {
		Serial << "[STATE_TX] SUCCESS TX(" << int(evarg) << ')' << mwx::crlf;
		PEV_SetState(STATE_SLEEP);
}
```

送信完了まちの処理はループでのアクト記述と違い、`transmit_complete()`からの`PEV_Process()`によるメッセージを待つことで完了確認としています。メッセージを受け取った時点でスリープします。スリープ処理は`STATE_SLEEP`に遷移することで行っています。



```cpp
	if (PEV_u32Elaspsed_ms() > 100) {
		// does not finish TX!
		Serial << "[STATE_TX] FATAL, TX does not finish!" << mwx::crlf << mwx::flush;
		the_twelite.reset_system();
	}
```

最後にタイムアウト処理を行っています。万が一送信パケットの完了メッセージが戻ってこなかった場合を想定します。`PEV_u32Elaspsed_ms()`はその状態に遷移してからの経過時間を\[ms]で返します。時間経過した場合は、上記では（このタイムアウトは余程のことだとして）システムリセット`the_twelite.reset_system()`を行います。



### MY\_APP\_CHILD::STATE\_SLEEP

```cpp
MWX_STATE(MY_APP_CHILD::STATE_SLEEP, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "..sleep for 5000ms" << mwx::crlf;
		pinMode(PAL_AMB::PIN_BTN, PIN_MODE::WAKE_FALLING_PULLUP);
		digitalWrite(PAL_AMB::PIN_LED, PIN_STATE::HIGH);
		Serial.flush();

		the_twelite.sleep(5000); // regular sleep
	}
}
```

スリープを行います。前の状態から遷移した直後の`E_EVENT_NEW_STATE`に記述します。スリープ直前に他のイベントが呼ばれる可能性がありますので、必ず１回だけ実行される判定式の中で`the_twelite.sleep()`を実行してください。
