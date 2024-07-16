---
title: テンプレートコード
author: "Mono Wireless Inc."
description: "テンプレートコード"
---
# Scratch

テンプレートコードです。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの送信 ('`t`' キー)
* スリープ ('`s`'キー)
* シリアルポートからの入力 - [Serial](../api-reference/classes/twe-stream/#read)
* ディジタル（ボタン）入力 - [Buttons](../api-reference/predefined\_objs/buttons.md)
{% endhint %}

## setup()

```cpp
void setup() {
	/*** SETUP section */
	tx_busy = false;

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(0xFE); // set Logical ID. (0xFE means a child device with no ID)

	/*** BEGIN section */
	Buttons.begin(pack_bits(PIN_BTN), 5, 10); // check every 10ms, a change is reported by 5 consequent values.

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- Scratch act ---" << mwx::crlf;
}
```

`the_twelite`を設定してアプリケーションID `APP_ID`, 無線チャネル`CHANNEL`、受信有を設定します。

また`nwk`を生成し、子機アドレス`0xFE`を指定しています。このアドレスは子機でアドレスを指定していない名無しの子機という意味です。

{% hint style="info" %}
設定できるアドレスは0x00: 親機,0x01\~0xEF: 子機, 0xFE:子機アドレス未指定の範囲です。

送信先として指定するアドレスは0x00は親機宛、0x01\~0xEFは指定の親機アドレス、0xFEは任意の子機アドレス、0xFFは親機を含む任意のアドレスです。
{% endhint %}

また`Buttons`オブジェクトを初期化します。連続参照によるチャタリング抑制アルゴリズムです。10msごとに5回連続同じ値になれば対象のポート(`PIN_BTN`のみ)の`HI`または`LOW`を確定します。[`pack_bits(N1, N2, ..)`](../api-reference/funcs/utility/pack\_bits.md)は`1UL<<N1 | 1UL << N2 | ...`を行いビットマップを生成します。



```cpp
the_twelite.begin(); // start twelite!
```

`the_twelite`を開始するための手続きです。act0..4では出てきませんでしたが`the_twelite`の設定や各種ビヘイビアの登録を行った場合は、必ず呼び出すようにしてください。



## begin()

```cpp
void begin() {
	Serial << "..begin (run once at boot)" << mwx::crlf;
}
```

始動時`setup()`の後に１回だけ呼び出されます。メッセージの表示のみ。



## loop()

### ボタン（スイッチ）の入力検出

```cpp
if (Buttons.available()) {
	uint32_t bm, cm;
	Buttons.read(bm, cm);

	if (cm & 0x80000000) {
		// the first capture.
	}

	Serial << int(millis()) << ":BTN" << format("%b") << mwx::crlf;
}
```

[Buttons](../api-reference/predefined\_objs/buttons.md)による連続参照により状態を確定します。ボタン状態が変化したらシリアルに出力します。



### シリアルからの入力

```cpp
while(Serial.available()) {
  int c = Serial.read();

	Serial << '[' << char(c) << ']';

  switch(c) {
  case 'p': ... // millis() を表示
  case 't': ... // 無線パケットを送信 (vTransmit)
        if (!tx_busy) {
					tx_busy = Transmit();
					if (tx_busy) {
						Serial  << int(millis()) << ":tx request success! (" 
										<< int(tx_busy.get_value()) << ')' << mwx::crlf;
 					} else {
						Serial << int(millis()) << ":tx request failed" << mwx::crlf;;
					}
				}
  case 's': ... // スリープする
				Serial << int(millis()) << ":sleeping for " << 5000 << "ms" << mwx::crlf << mwx::flush;
				the_twelite.sleep(5000);
				break;
  }
}
```

`Serial.available()`が_`true`_の場合は、シリアルポートからの入力が保存されています。シリアルから１文字読み込んで、入力文字に応じた処理をします。



#### `t`を入力して無線送信

'`t`'を入力したときは送信を行います。このサンプルでは`tx_busy`フラグを用い連続的に入力は行わないようにしています。

{% hint style="info" %}
送信要求は一定数までキューに保存されるため、キューの範囲(3パケット)で要求を積むことは可能です。

以下は`if(!tx_busy)`の判定をしないようにして '`tttt`'と連続的に入力した場合の処理例です。4つ目の要求でキューが一杯になって要求は失敗しています。`Transmit()`の`.prepare_tx_packet()`で得られた`pkt`オブジェクトが_`false`_になります。

送信タイミングはランダム化されるため、送信完了は送信要求順にはなりません。

```
--- Scratch act ---
..begin (run once at boot)
[t]11591:Transmit()
11592:tx request success! (1)
[t]11593:Transmit()
11593:tx request success! (2)
[t]11594:Transmit()
11595:tx request success! (3)
[t]11595:Transmit()
TX QUEUE is FULL
11596:tx request failed
11654:tx completed!(id=2, stat=1)
11719:tx completed!(id=3, stat=1)
11745:tx completed!(id=1, stat=1)
```
{% endhint %}



#### `s`を入力してスリープ

```cpp
the_twelite.sleep(5000);
```

5000ms=5秒のスリープを実施します。復帰後は`wakeup()`が実行されます。



## wakeup()

```cpp
void wakeup() {
	Serial << int(millis()) << ":wake up!" << mwx::crlf;
}
```

スリープ起床時に最初に呼び出されます。メッセージの表示のみ。



## Transmit()

```cpp
MWX_APIRET Transmit() {
	Serial << int(millis()) << ":Transmit()" << mwx::crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0xFF)  // 同報通信＝ブロードキャスト
			<< tx_retry(0x1)    // 再送１回
			<< tx_packet_delay(100,200,20); // 送信時遅延100-200msの間に送信、再送間隔20ms

		// 送信データの指定（アプリケーションごとに決める）
		pack_bytes(pkt.get_payload()
			, make_pair("SCRT", 4) // ４文字識別子
			, uint32_t(millis())   // タイムスタンプ
		);
		
		// 送信要求を行う
		return pkt.transmit(); 
	} else {
		// .prepare_tx_packet() 時点で失敗している(送信キューが一杯)
		Serial << "TX QUEUE is FULL" << mwx::crlf;
	  return MWX_APIRET(false, 0);
	}
}
```

送信要求を行う最小限の手続きです。

この関数を抜けた時点では、まだ要求は実行されていません。しばらく待つ必要があります。この例では100-200msの送信開始の遅延を設定しているため、送信が開始されるのは早くて100ms後です。



## on\_tx\_comp()

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	Serial 	<< int(millis()) << ":tx completed!"
			<< format("(id=%d, stat=%d)", ev.u8CbId, ev.bStatus) << mwx::crlf;
	tx_busy = false; // clear tx busy flag.
}
```

送信完了時に呼び出されます。evには送信IDと完了ステータスが含まれます。



## on\_rx\_packet()

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	Serial << format("rx from %08x/%d", 
					rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;
}
```

パケットを受信したら、送信元のアドレス情報を表示します。







