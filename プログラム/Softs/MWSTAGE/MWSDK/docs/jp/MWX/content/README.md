---
title: "The MWX Library"
author: "Mono Wireless Inc."
description: "Mono Wireless C++ Library for TWELITE."
---

# The MWX Library

{% hint style="success" %}
[資料の取り扱いについて](https://twelite.gitbook.io/general/about\_documents)をご参照ください。  お気付きの点がありましたら、当サポート窓口にご連絡いただければ幸いです。
{% endhint %}

{% hint style="danger" %}
このページには開発中の情報が含まれます。本ページの内容は、まだ公開ソースコードに反映されていない場合があります。
{% endhint %}



MWX ライブラリは、TWELITE 無線モジュールのコード表記を簡素化することを目的としています。MWXで作成されたプログラムを**アクト** act と呼びます。アクトにはループによる記述と、イベントによる記述（**ビヘイビア** behavior と呼びます）の二種類があります。

本ページではアクトの主な特徴を紹介します。



* ループによる記述 (`setup(), loop()`)。小規模な機能を記述するのに向いています。

```cpp
#include <TWELITE>
const uint8_t PIN_LED = 5;

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (TickTimer.available()) {
    uint32 t_now = millis();
    
    // blink LED every 1024ms
    digitalWrite(PIN_LED, (t_now >> 10) & 1 ? HIGH : LOW);
  }
}	
```



* イベントドリブンのアプリケーション記述。各種イベント・割り込みハンドラの定義、アプリケーションの複雑な振る舞いを記述するのに向いたステートマシンをクラス内に定義して見通しの良いコードを記述できます。この記述を**ビヘイビア**と呼びます。

```cpp
// myApp.hpp
...
class myApp : MWX_APPDEFS_CRTP(myApp) {
...
  void loop() {
    // main loop
  }
  
  void receive(mwx::packet_rx& rx) {
    // on receive
  }
};

// myApp.cpp
...
MWX_DIO_EVENT(12, uint32_t arg) {
		// on event from DIO12
}
```



* ペリフェラルの手続きを簡素化。よく使われる UART, I2C, SPI, ADC, DIO, タイマー, パルスカウンタを取り扱うクラスオブジェクトを定義しています。

```cpp
void loop() {
  while(Serial.available() { 
    auto x = Serial.read(); ... } // serial message
  if (Analogue.available() {
    auto x = Analogue.read(...); } // adc values
  if (Buttons.available() { 
    Buttons.read(...); } // DIO changes
  if (the_twelite.receiver.available()) { 
    auto&& rx = the_twelite.receiver.read(); } // on rx packet
}
```



* シンプルな中継ネットワークを定義。この中継ネットワークは TWELITE 標準アプリケーションと同等の実装で、個体アドレスの管理は 8bit の論理IDで行うこと、ネットワーク構築のための通信を行わないため電源投入後すぐにネットワーク宛に無線パケットを送ることができる点が特徴です。

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>

void setup() {
  ...
  auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) 
	           // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);
	           // can repeat a packet up to three times.
}

void loop() {
  ...
  vTransmit();
  ...
}

void vTransmit() {
  if (auto&& pkt =
    the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet(); 
  pkt << tx_addr(0x00)  // to parent 
	  	<< tx_retry(0x3); // set retry
	
	pack_bytes(pkt.get_payload() // prepare payload data
	    , uint8_t(0x01)
	    , uint16_t(analogRead(PIN_ANALOGUE::A1))
	    , uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)));
	
	pkt.transmit(); // transmit!
}
```

{% hint style="info" %}
TWELITE 標準アプリケーションとのパケットの相互通信はできませんが、以下の点で自由度が上がっています。

* 論理IDは 0 が親機である点は同様ですが、子機アドレスとして 0x01..0xEF を利用できるため、識別数を 200 以上とすることができます。
* 原則３回までとしていた中継回数について、最大数を64回までを設定できるようにしています。（※ パケットが遠回りして一定時間経過後に戻ってきた場合、重複パケットの管理テーブルがクリアされ中継済みのパケットであっても、再中継が発生する場合があります。中継回数を大きくする設定は注意してください）
{% endhint %}



* PAL や MONOSTICK 向けのボード定義。ボード上のセンサーなどを容易に取り扱えます。

```cpp
#include <TWELITE>
#include <PAL_AMB> // include the board support of PAL_AMB

void setup() {
	auto&& brd = the_twelite.board.use<PAL_AMB>(); // use PAL AMB
	uint8_t u8dip = brd.get_DIP_SW();   // check DIP s/w status
	brd.set_led(LED_TIMER::BLINK, 100); // LED switchs on/off every 100ms
	...
	
	// start capture of sensors
	brd.sns_SHTC3.begin();
}

void loop() {
	if (TickTime.available()) { // check every ms
		auto&& brd = the_twelite.board.use<PAL_AMB>();

		if (brd.sns_LTR308ALS.available()) {
		  Serial << brd.sns_SHTC3..get_temp();
		} else {
		  // notify sensor that 1ms passed.
			brd.sns_SHTC3.process_ev(E_EVENT_TICK_TIMER);
		}
	}
}
```

