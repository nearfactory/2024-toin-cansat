---
title: PAL_AMB-usenap
author: "Mono Wireless Inc."
---

# PAL\_AMB-usenap

[PAL\_AMB ](pal\_amb.md)のサンプルを少し改良して、センサーデータ取得中の待ち時間（約50ms)を、スリープで待つようにします。

{% hint style="success" %}
このアクトの解説の前に[PAL\_AMB](pal\_amb.md)のアクトの解説をご覧ください。
{% endhint %}

## アクトの解説

### begin()

`begin()`関数は`setup()`関数を終了し(そのあとTWENETの初期化が行われる)一番最初の`loop()`の直前で呼ばれます。

```cpp
void begin() {
	sleepNow(); // the first time is just sleeping.
}
```

`setup()`終了後に初回スリープを実行します。`setup()`中にセンサーデータ取得を開始していますが、この結果は評価せず、センサーを事前に一度は動かしておくという意味あいで、必ずしも必要な手続きではありません。



### wakeup()

起床後の手続きです。以下の処理を行います。

* まだセンサーデータの取得開始をしていない場合、センサーデータ取得を行い、短いスリープに入る。
* 直前にセンサーデータ取得開始を行ったので、データを確認して無線送信する。

```cpp
void wakeup() {
	if (!b_senser_started) {
		// delete/make shorter this message if power requirement is harder.	
		Serial	<< mwx::crlf
				<< "--- PAL_AMB:" << FOURCHARS << " wake up ---"
				<< mwx::crlf
				<< "..start sensor capture again."
				<< mwx::crlf;

		startSensorCapture();
		b_senser_started = true;

		napNow(); // short period sleep.
	} else {
		Serial << "..wake up from short nap.." << mwx::crlf;

		auto&& brd = the_twelite.board.use<PAL_AMB>();

		b_senser_started = false;

		// tell sensors waking up.
		brd.sns_LTR308ALS.process_ev(E_EVENT_START_UP);
		brd.sns_SHTC3.process_ev(E_EVENT_START_UP);
	}
}
```

上記の分岐をグローバル変数の`b_sensor_started`により制御しています。`!b_sensor_started`の場合はセンサー取得開始(`startSensorCapture()`)を行い、`napNow()`により短いスリープに入ります。時間は100msです。

`napNow()`によるスリープ復帰後、`b_sensor_started==true`の節が実行されます。ここでは、2つのセンサーに対して`E_EVENT_START_UP`イベントを通知しています。このイベントは、センサーの取得が終了するのに十分な時間が経過したことを意味します。この通知をもとに`sns_LTR308ALS`と`sns_SHTC3`はavailableになります。この後`loop()`に移行し、無線パケットが送信されます。

{% hint style="warning" %}
センサーに通知するイベントは必要な時間待ちが終わったかどうかを判定するために使われます。実際時間が経過しているかどうかは`napNow()`で正しい時間を設定したかどうかで決まります。短い時間で起床した場合は、必要とされる時間経過に足りないため、続く処理でセンサーデータが得られないなどのエラーが出ることが想定されます。
{% endhint %}



### napNow()

ごく短いスリープを実行する。

```cpp
void napNow() {
	uint32_t u32ct = 100;
	Serial << "..nap " << int(u32ct) << "ms." << mwx::crlf;
	the_twelite.sleep(u32ct, false, false, TWENET::SLEEP_WAKETIMER_SECONDARY);
}
```

sleepのパラメータの2番目をtrueにすると前回のスリープ復帰時刻をもとに次の復帰時間を調整します。常に5秒おきに起床したいような場合設定します。

3番目をtrueにするとメモリーを保持しないスリープになります。復帰後はwakup()は呼び出されじ、電源再投入と同じ処理になります。

4番目はウェイクアップタイマーの2番目を使う指定です。ここでは1番目は通常のスリープに使用して、2番目を短いスリープに用いています。このアクトでは2番目を使う強い理由はありませんが、例えば上述の5秒おきに起床したいような場合、短いスリープに1番目のタイマーを用いてしまうとカウンター値がリセットされてしまい、経過時間の補正計算が煩雑になるため2番目のタイマーを使用します。

{% hint style="info" %}
あまり短いスリープ時間を設定してもスリープ復帰後のシステムの再初期化などのエネルギーコストと釣り合いません。目安として最小時間を30-50ms程度とお考え下さい。
{% endhint %}



