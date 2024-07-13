---
title: PAL_AMB
author: "Mono Wireless Inc."
---
# PAL\_AMB

[環境センサーパル AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) を用い、センサー値の取得を行います。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの送受信
* インタラクティブモードによる設定 - [\<STG\_STD>](../settings/stg\_std.md)
* ステートマシンによる状態遷移制御 - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_AMB>](../boards/pal/pal\_amb.md)ボードビヘイビアによるボード操作
{% endhint %}



## アクトの機能

* 環境センサーパル AMPIENT SENSE PAL を用い、センサー値の取得を行います。
* コイン電池で動作させるための、スリープ機能を利用します。

## アクトの使い方

### 必要なTWELITE

| 役割 | 例                                                                                                                                                                                            |
| -- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUEまたはRED</a>アクト<a href="parent_monostick.md">Parent_MONOSTICK</a>を動作させる。                                |
| 子機 | [BLUE PAL または RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) +[環境センサーパル AMBIENT SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |



## アクトの解説

### インクルード

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>// ネットワークサポート
#include <PAL_AMB>   // PAL_AMB
#include <STG_STD>   // インタラクティブモード
#include <SM_SIMPLE> // 簡易ステートマシン
```

環境センサーパル [`<PAL_AMB>`](../boards/pal/pal\_amb.md) のボードビヘイビアをインクルードします。

### setup()

```cpp
void setup() {
	/*** SETUP section */
	step.setup(); // ステートマシンの初期化
		
	// PAL_AMBのボードビヘイビアを読み込む
	auto&& brd = the_twelite.board.use<PAL_AMB>();
	
	// インタラクティブモードを読み込む
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appname(FOURCHARS);
	set << SETTINGS::appid_default(APP_ID); // set default appID
	set.hide_items(E_STGSTD_SETID::POWER_N_RETRY, E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

	// SET ピンを検出した場合は、インタラクティブモードを起動する
	if (digitalRead(brd.PIN_BTN) == PIN_STATE::LOW) {
		set << SETTINGS::open_at_start();
		step.next(STATE::INTERACTIVE);
		return;
	}

	// インタラクティブモードのデータを読み出す
	set.reload();
	APP_ID = set.u32appid();
	CHANNEL = set.u8ch();
	OPT_BITS = set.u32opt1();

	// DIPスイッチとインタラクティブモードの設定からLIDを決める
	LID = (brd.get_DIPSW_BM() & 0x07); // 1st priority is DIP SW
	if (LID == 0) LID = set.u8devid(); // 2nd is setting.
	if (LID == 0) LID = 0xFE; // if still 0, set 0xFE (anonymous child)
	
	// LED初期化
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)

	// the twelite main object.
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID); // set Logical ID. (0xFE means a child device with no ID)

	/*** BEGIN section */
	Wire.begin(); // start two wire serial bus.
	Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC)); // _start continuous adc capture.

	the_twelite.begin(); // start twelite!

	startSensorCapture(); // start sensor capture!

	/*** INIT message */
	Serial << "--- PAL_AMB:" << FOURCHARS << " ---" << mwx::crlf;
}
```



最初に変数などの初期化を行います。ここではステートマシンstepの初期化を行っています。

最初にボードサポート [`<PAL_AMB>`](parent\_monostick.md) を登録します。ボードサポートの初期化時にセンサーやDIOの初期化が行われます。最初に行うのは、ボードのDIP SWなどの状態を確認してから、ネットワークの設定などを行うといった処理が一般的だからです。

```cpp
auto&& brd = the_twelite.board.use<PAL_AMB>();
```

つづいて、インタラクティブモード関連の初期化と読出しを行います。

```cpp
// インタラクティブモードを読み込む
auto&& set = the_twelite.settings.use<STG_STD>();
set << SETTINGS::appname(FOURCHARS);
set << SETTINGS::appid_default(APP_ID); // set default appID
set.hide_items(E_STGSTD_SETID::POWER_N_RETRY, E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);

// SET ピンを検出した場合は、インタラクティブモードを起動する
if (digitalRead(brd.PIN_BTN) == PIN_STATE::LOW) {
	set << SETTINGS::open_at_start();
	step.next(STATE::INTERACTIVE);
	return;
}

// インタラクティブモードのデータを読み出す
set.reload();
APP_ID = set.u32appid();
CHANNEL = set.u8ch();
OPT_BITS = set.u32opt1();

// DIPスイッチとインタラクティブモードの設定からLIDを決める
LID = (brd.get_DIPSW_BM() & 0x07); // 1st priority is DIP SW
if (LID == 0) LID = set.u8devid(); // 2nd is setting.
if (LID == 0) LID = 0xFE; // if still 0, set 0xFE (anonymous child)
```

ここではsetオブジェクトの取得、アプリ名の反映、デフォルトのアプリケーションIDの反映、設定メニューで不要項目の削除を行います。

次にSETピンの状態を読み出します。このサンプルはスリープによる間欠動作を行うため、+++入力によるインタラクティブモード遷移は出来ません。替わりに起動時のSETピン=LO状態でインタラクティブモードに遷移します。このとき`SETTINGS::open_at_start()`を指定していますが、これは`setup()`を終了後速やかにインタラクティブモード画面に遷移する指定です。

最後に`.reload()`を実行して設定値をEEPROMから読み出します。設定値を各変数にコピーしています。



続いてLEDの設定を行います。ここでは 10ms おきに ON/OFF の点滅の設定をします（スリープを行い起床時間が短いアプリケーションでは、起床中は点灯するという設定とほぼ同じ意味合いになります）。

```cpp
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



このアクトではもっぱら無線パケットを送信しますので、TWENET の設定では動作中に受信回路をオープンにする指定（`TWENET::rx_when_idle()`）は含めません。

```cpp
	the_twelite
		<< TWENET::appid(APP_ID)     // set application ID (identify network group)
		<< TWENET::channel(CHANNEL); // set channel (pysical channel)
```



ボード上のセンサーはI2Cバスを用いますので、バスを利用開始しておきます。

```cpp
Wire.begin(); // start two wire serial bus.
```



ボード上のセンサーの取得を開始します。[`startSensorCapture()`](pal\_amb.md#startsensorcapture)の解説を参照ください。

```
startSensorCapture();
```



### loop()

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_AMB>();

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
brd.sns_SHTC3.begin();
brd.sns_LTR308ALS.begin();

step.next(STATE::SENSOR);
```

センサーのデータ取得を開始します。



#### case STATE::SENSOR:

```cpp
	if (!brd.sns_LTR308ALS.available()) {
		brd.sns_LTR308ALS.process_ev(E_EVENT_TICK_TIMER);
	}

	if (!brd.sns_SHTC3.available()) {
		brd.sns_SHTC3.process_ev(E_EVENT_TICK_TIMER);
	}
```

ボード上のセンサーは `.sns_LTR308ALS` または `.sns_SHTC3` という名前でアクセスでき、このオブジェクトに操作を行います。センサーの完了待ちを行います。まだセンサーの取得が終わっていない場合(`.available()`が`false`)はセンサーに対して時間経過のイベント(`.process_ev(E_EVENT_TICK_TIMER)`)を送付します。

上記２つのセンサーがavailableになった時点で、センサー値を取得し、STATE\_TXに遷移します。

```cpp
// now sensor data is ready.
if (brd.sns_LTR308ALS.available() && brd.sns_SHTC3.available()) {
	sensor.u32luminance = brd.sns_LTR308ALS.get_luminance();
	sensor.i16temp = brd.sns_SHTC3.get_temp_cent();
	sensor.i16humid = brd.sns_SHTC3.get_humid_per_dmil();

	Serial << "..finish sensor capture." << mwx::crlf
		<< "  LTR308ALS: lumi=" << int(sensor.u32luminance) << mwx::crlf
		<< "  SHTC3    : temp=" << div100(sensor.i16temp) << 'C' << mwx::crlf
		<< "             humd=" << div100(sensor.i16humid) << '%' << mwx::crlf
		;
	Serial.flush();

	step.next(STATE::TX);
}
```

照度センサーは`.get_luminance() : uint32_t`で得られます。

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



パケットのペイロード部に識別子の`FOURCHARS`とセンサーデータを格納します。得られた値のうち温度値は `int16_t `ですが、送信パケットのデータ構造は符号なしで格納するため、`uint16_t`にキャストしています。

```cpp
pack_bytes(pkt.get_payload() 
	, make_pair(FOURCHARS, 4)  
	, uint32_t(sensor.u32luminance)
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

8,9行目、この例ではシリアルポートからの出力を待ってスリープに入ります。通常は消費エネルギーを最小化したいため、スリープ前のシリアルポートの出力は最小限（または無し）にします。

12行目、スリープに入るには `the_twelite.sleep()` を呼びます。この呼び出しの中で、ボード上のハードウェアのスリープ前の手続きなどが行われます。たとえばLEDは消灯します。

パラメータとしてスリープ時間をmsで指定しています。

{% hint style="danger" %}
TWELITE PAL では、必ず60秒以内に一度起床し、ウォッチドッグタイマーをリセットしなければなりません。スリープ時間は`60000`を超えないように指定してください。
{% endhint %}



### wakeup()

スリープから復帰し起床すると `wakeup()` が呼び出されます。そのあと`loop()` が都度呼び出されます。`wakeup()`の前に、UARTなどの各ペリフェラルやボード上のデバイスのウェイクアップ処理が行われます。例えばLEDの点灯制御を再始動します。

```cpp
void wakeup() {
	Serial	<< mwx::crlf
			<< "--- PAL_AMB:" << FOURCHARS << " wake up ---"
			<< mwx::crlf
			<< "..start sensor capture again."
			<< mwx::crlf;
```



## 応用編

### 消費エネルギーの削減

アクト [PAL\_AMB-UseNap ](pal\_amb-usenap.md)は、センサーのデータ取得待ちをスリープで行い、より低消費エネルギーで動作できます。
