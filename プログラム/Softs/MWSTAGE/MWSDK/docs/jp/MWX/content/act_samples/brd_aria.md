---
title: BRD_ARIA
author: "Mono Wireless Inc."
---
# BRD\_ARIA

TWELITE ARIA - トワイライトアリア を用い、センサー値の取得を行います。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの送信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ステートマシンによる状態遷移制御 - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<ARIA>](../boards/aria.md)ボードビヘイビアによるボード操作
{% endhint %}



## アクトの機能

* TWELITE ARIA - トワイライトアリア を用い、センサー値の取得を行います。
* コイン電池で動作させるための、スリープ機能を利用します。

## アクトの使い方

### 必要なTWELITE

| 役割 | 例                                                                                                                                                             |
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUEまたはRED</a>アクト<a href="parent_monostick.md">Parent_MONOSTICK</a>を動作させる。 |
| 子機 | [TWELITE ARIA BLUE または RED](https://mono-wireless.com/jp/products/twelite-aria/index.html)                                                                    |



## アクトの解説

### インクルード

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>// ネットワークサポート
#include <ARIA>   // TWELITE ARIA
#include <STG_STD>   // インタラクティブモード
#include <SM_SIMPLE> // 簡易ステートマシン
```

TWELITE ARIA[`<ARIA>`](../boards/aria.md)のボードビヘイビアをインクルードします。

### setup()

```cpp
void setup(){
	/*** SETUP section */
	/// init vars or objects
	step.setup(); // initialize state machine
	
	/// load board and settings objects
	auto&& brd = the_twelite.board.use<ARIA>(); // load board support
	auto&& set = the_twelite.settings.use<STG_STD>(); // load save/load settings(interactive mode) support
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>(); // load network support

	/// configure settings
	// configure settings
	set << SETTINGS::appname("ARIA");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

	// if SET=LOW is detected, start with intaractive mode.
	if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
		set << SETTINGS::open_at_start();
		step.next(STATE::INTERACTIVE);
		return;
	}

	// load values
	set.reload(); // load from EEPROM.
	OPT_BITS = set.u32opt1(); // this value is not used in this example.
	
	LID = set.u8devid(); // set logical ID

	/// configure system basics
	the_twelite << set; // apply settings (from interactive mode)

	/// configure network
	nwk << set; // apply settings (from interactive mode)
	nwk << NWK_SIMPLE::logical_id(LID); // set LID again (LID can also be configured by DIP-SW.)	

	/// configure hardware
	// LED setup (use periph_led_timer, which will re-start on wakeup() automatically)
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
	
	// let the TWELITE begin!
	the_twelite.begin();

	/*** INIT message */
	Serial << "--- ARIA:" << FOURCHARS << " ---" << mwx::crlf;
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



最初に変数などの初期化を行います。ここではステートマシンstepの初期化を行っています。

最初にボードサポート [`<ARIA>`](../boards/aria.md) を登録します。ボードサポートの初期化時にセンサーやDIOの初期化が行われます。

```cpp
auto&& brd = the_twelite.board.use<ARIA>();
```

つづいて、インタラクティブモード関連の初期化と読出しを行います。

```cpp
// configure settings
set << SETTINGS::appname("ARIA");
set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

// if SET=LOW is detected, start with intaractive mode.
if (digitalRead(brd.PIN_SET) == PIN_STATE::LOW) {
	set << SETTINGS::open_at_start();
	step.next(STATE::INTERACTIVE);
	return;
}

// load values
set.reload(); // load from EEPROM.
OPT_BITS = set.u32opt1(); // this value is not used in this example.

LID = set.u8devid(); // set logical ID
```

ここではsetオブジェクトの取得、アプリ名の反映、デフォルトのアプリケーションIDと通信チャネルの反映、設定メニューで不要項目の削除を行います。

次にSETピンの状態を読み出します。このサンプルはスリープによる間欠動作を行うため、+++入力によるインタラクティブモード遷移は出来ません。替わりに起動時のSETピン=LO状態でインタラクティブモードに遷移します。このとき`SETTINGS::open_at_start()`を指定していますが、これは`setup()`を終了後速やかにインタラクティブモード画面に遷移する指定です。

最後に`.reload()`を実行して設定値をEEPROMから読み出します。設定値を各変数にコピーしています。



このアクトではもっぱら無線パケットを送信しますので、TWENET の設定では動作中に受信回路をオープンにする指定（`TWENET::rx_when_idle()`）は含めません。

```
the_twelite << set; // apply settings (from interactive mode)
```



続いてLEDの設定を行います。ここでは 10ms おきに ON/OFF の点滅の設定をします（スリープを行い起床時間が短いアプリケーションでは、起床中は点灯するという設定とほぼ同じ意味合いになります）。

```cpp
brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



### loop()

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<ARIA>();

	do {
		switch (step.state()) {
		 // 各状態の振る舞い
		case STATE::INIT:
		...
		break;
		...
		}
	while(step.b_more_loop());
}	
```

`loop()`は、[SM\_SIMPLEステートマシン](../api-reference/classes/smsimple-suttomashin.md)`step`を用いた制御を行っています。スリープ復帰からセンサー値取得、無線パケット送信、送信完了待ち、スリープといった一連の流れを簡潔に表現するためです。ループの戦闘では`brd`オブジェクトを取得しています。



#### case STATE::INTERACTIVE:

インタラクティブモード中にメインループが動作するのは都合が悪いため、この状態に固定します。



#### case STATE::INIT:

```cpp
brd.sns_SHT4x.begin();

step.next(STATE::SENSOR);
```

センサーのデータ取得を開始します。



#### case STATE::SENSOR:

```cpp
//  wait until sensor capture finish
if (!brd.sns_SHT4x.available()) {
	brd.sns_SHT4x.process_ev(E_EVENT_TICK_TIMER);
}else{ // now sensor data is ready.
	sensor.i16temp = brd.sns_SHT4x.get_temp_cent();
	sensor.i16humid = brd.sns_SHT4x.get_humid_per_dmil();

	// read magnet sensor
	sensor.b_north = digitalRead(ARIA::PIN_SNS_NORTH);
	sensor.b_south = digitalRead(ARIA::PIN_SNS_SOUTH);

	Serial << "..finish sensor capture." << mwx::crlf
		<< "  MAGnet   : north=" << int(sensor.b_north) << mwx::crlf
		<< "             south=" << int(sensor.b_south) << mwx::crlf
		<< "  SHT4x    : temp=" << div100(sensor.i16temp) << 'C' << mwx::crlf
		<< "             humd=" << div100(sensor.i16humid) << '%' << mwx::crlf
		;
	Serial.flush();

	step.next(STATE::TX);
}
```

ボード上のセンサーは `.sns_SHT4x` という名前でアクセスでき、このオブジェクトに操作を行います。センサーの完了待ちを行います。まだセンサーの取得が終わっていない場合(`.available()`が`false`)はセンサーに対して時間経過のイベント(`.process_ev(E_EVENT_TICK_TIMER)`)を送付します。

センサーがavailableになった時点で、センサー値を取得し、STATE\_TXに遷移します。

温湿度センサーは以下のように取得できます。

* `.get_temp_cent()` : `int16_t` : 1℃を100とした温度 (25.6 ℃なら 2560)
* `.get_temp()` : `float` : float値 (25.6 ℃なら 25.6)
* `.get_humid_dmil()` : `int16_t` : 1%を100とした湿度 (56.8%なら 5680)
* `.get_temp()` : `float` : float値 (56.8%なら 56.8)



#### case STATE::TX:

送信手続きについては他のアクトのサンプルと同様です。ここでは、再送１回、再送遅延を最小にする設定になっています。

```cpp
pkt << tx_addr(0x00)  // 親機0x00宛
	<< tx_retry(0x1)    // リトライ1回
	<< tx_packet_delay(0, 0, 2); // 遅延は最小限
```



パケットのペイロード部に識別子の`FOURCHARS`とセンサーデータを格納します。得られた値のうち温度値は `int16_t` ですが、送信パケットのデータ構造は符号なしで格納するため、`uint16_t`にキャストしています。

```cpp
pack_bytes(pkt.get_payload() // set payload data objects.
	, make_pair(FOURCHARS, 4)  // just to see packet identification, you can design in any.
	, uint8_t(sensor.b_north)
	, uint8_t(sensor.b_south)
	, uint16_t(sensor.i16temp)
	, uint16_t(sensor.i16humid)
);
```



送信要求を行います。送信要求が成功したら送信完了街の準備を行います。完了イベントを待つために`.clear_flag()`、万が一のときのタイムアウトを`set_timeout(100)`を指定します。パラメータの100の単位はミリ秒\[ms]です。

```cpp
// do transmit
MWX_APIRET ret = pkt.transmit();

if (ret) {
	step.clear_flag(); // waiting for flag is set.
	step.set_timeout(100); // set timeout
	step.next(STATE::TX_WAIT_COMP);
}
```



#### case STATE::TX\_WAIT\_COMP:

ここではタイムアウトの判定、送信完了イベントの判定を行います。

```cpp
if (step.is_timeout()) { // maybe fatal error.
	the_twelite.reset_system();
}
if (step.is_flag_ready()) { // when tx is performed
	Serial << "..transmit complete." << mwx::crlf;
	Serial.flush();
	step.next(STATE::GO_SLEEP);
}
```



#### STATE::GO\_SLEEP:

`sleepNow()`の処理を行います。



### on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}
```

送信完了時に呼び出されるシステムイベントです。ここでは`.set_flag()`により完了としています。



### sleepNow()

スリープに入る手続きをまとめています。

```cpp
void sleepNow() {
	step.on_sleep(false); // reset state machine.

	// randomize sleep duration.
	uint32_t u32ct = 1750 + random(0,500);

	// set an interrupt for MAGnet sensor.
	pinMode(ARIA::PIN_SNS_OUT1, PIN_MODE::WAKE_FALLING);
	pinMode(ARIA::PIN_SNS_OUT2, PIN_MODE::WAKE_FALLING);

	// output message
	Serial << "..sleeping " << int(u32ct) << "ms." << mwx::crlf;
	Serial.flush(); // wait until all message printed.
	
	// do sleep.
	the_twelite.sleep(u32ct);
}
```

スリープ前に`.on_sleep(false)`によりステートマシンの状態を初期化します。パラメータのfalseはスリープ復帰後`STATE::INIT(=0)`から始めます。

ここでは、起床までの時間を乱数により 1750ms から 2250ms の間に設定しています。これにより他の同じような周期で送信するデバイスのパケットとの連続的な衝突を避けます。

{% hint style="info" %}
周期が完全に一致すると、互いのパケットで衝突が起き通信が困難になります。通常は時間の経過とともにタイマー周期が互いにずれるため、しばらくすると通信が回復し、また時間がたつと衝突が起きるという繰り返しになります。
{% endhint %}

8,9行目では、スリープに入るまえに磁気センサーのDIOピンの割り込み設定をします。`pinMode()`を用います。２番めのパラメータは`PIN_MODE::WAKE_FALLING`を指定しています。これはHIGHからLOWへピンの状態が変化したときに起床する設定です。

11,12行目、この例ではシリアルポートからの出力を待ってスリープに入ります。通常は消費エネルギーを最小化したいため、スリープ前のシリアルポートの出力は最小限（または無し）にします。

12行目、スリープに入るには `the_twelite.sleep()` を呼びます。この呼び出しの中で、ボード上のハードウェアのスリープ前の手続きなどが行われます。たとえばLEDは消灯します。

パラメータとしてスリープ時間をmsで指定しています。

{% hint style="danger" %}
TWELITE ARIA では、必ず60秒以内に一度起床し、ウォッチドッグタイマーをリセットしなければなりません。スリープ時間は`60000`を超えないように指定してください。
{% endhint %}



### wakeup()

スリープから復帰し起床すると `wakeup()` が呼び出されます。そのあと`loop()` が都度呼び出されます。`wakeup()`の前に、UARTなどの各ペリフェラルやボード上のデバイスのウェイクアップ処理が行われます。例えばLEDの点灯制御を再始動します。

```cpp
void wakeup() {
    Serial	<< mwx::crlf
        	<< "--- ARIA:" << FOURCHARS << " wake up ";

    if (the_twelite.is_wokeup_by_wktimer()) {
        Serial << "(WakeTimer) ---";
    } else 
    if (the_twelite.is_wokeup_by_dio(ARIA::PIN_SNS_NORTH)) {
        Serial << "(MAGnet INT [N]) ---";
    } else 
    if (the_twelite.is_wokeup_by_dio(ARIA::PIN_SNS_SOUTH)) {
        Serial << "(MAGnet INT [S]) ---";
    } else {
        Serial << "(unknown source) ---";
    }

	Serial  << mwx::crlf
			<< "..start sensor capture again."
			<< mwx::crlf;
}
```



