---
title: PulseCounter
author: "Mono Wireless Inc."
description: "PulseCounter"
---
# PulseCounter

[パルスカウンター](../api-reference/predefined\_objs/pulsecounter.md)を用いたアクト例です。

パルスカウンターは、マイコンを介在せず信号の立ち上がりまたは立ち下りの回数を計数するものです。不定期のパルスを計数し一定回数までカウントが進んだ時点で無線パケットで回数を送信するといった使用方法が考えられます。



## アクトの機能

* 子機側のDIO8に接続したパルスを計数し、一定時間経過後または一定数のカウントを検出した時点で無線送信する。
* 子機側はスリープしながら動作する。\


## アクトの使い方

### 必要なTWELITE

| 役割 | 例                                                                                                                                                                                                                                                                                                                           |
| -- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 親機 | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUEまたはRED</a><br>アクト<a href="parent_monostick.md">Parent_MONOSTICK</a>を動作させる。                                                                                                                                                               |
| 子機 | １．<a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>２．<a href="https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html">BLUE PAL または RED PAL</a> +<a href="https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html">環境センサーパル AMBIENT SENSE PAL</a> |



## アクトの解説

### setup()

```cpp
// Pulse Counter setup
PulseCounter.setup();
```

パルスカウンターの初期化を行います。



### begin()

```cpp
void begin() {
	// start the pulse counter capturing
	PulseCounter.begin(
		  100 // 100 count to wakeup
		, PIN_INT_MODE::FALLING // falling edge
		);

	sleepNow();
}
```

パルスカウンターの動作を開始し、初回スリープを実行します。`PulseCounter.begin()`の最初のパラメータは、起床割り込みを発生させるためのカウント数`100`で、２番目は立ち下がり検出`PIN_INT_MODE::FALLING`を指定しています。



### wakeup()

```cpp
void wakeup() {
	Serial	<< mwx::crlf
			<< "--- Pulse Counter:" << FOURCHARS << " wake up ---"
			<< mwx::crlf;

	if (!PulseCounter.available()) {
		Serial << "..pulse counter does not reach the reference value." << mwx::crlf;
		sleepNow();
	}
}
```

起床時に`PulseCounter.available()`を確認しています。availableつまり`true`になっていると、指定したカウント数以上のカウントになっていることを示します。ここでは`false`の場合再スリープしています。

カウント数が指定以上の場合は`loop()`で送信処理と送信完了待ちを行います。



### loop()

```cpp
uint16_t u16ct = PulseCounter.read();
```

パルスカウント値の読み出しを行います。読み出した後カウンタはリセットされます。

