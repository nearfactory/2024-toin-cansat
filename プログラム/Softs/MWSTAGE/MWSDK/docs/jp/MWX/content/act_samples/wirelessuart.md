---
title: WirelessUART 
author: "Mono Wireless Inc."
description: "WirelessUART"
---

# WirelessUART

WirelessUARTはシリアル通信を行います。



## アクトの機能

* ２台のUART接続のTWELITE同士をアスキー書式で通信する。



## アクトの使い方

### 必要なTWELITE

PCにシリアル接続されている以下のデバイスを2台。

* [MONOSTICK BLUE または RED](https://mono-wireless.com/jp/products/MoNoStick/index.html)
* [TWELITE R](https://mono-wireless.com/jp/products/TWE-LITE-R/index.html) でUART接続されている[TWELITE DIP](https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html)など

{% hint style="info" %}
親機宛のパケットであれば[`Parent_MONOSTICK`](parent\_monostick.md)でも受信できます。
{% endhint %}



## アクトの解説

### setup()

```cpp
void setup() {
	auto&& set = the_twelite.settings.use<STG_STD>();
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

	/*** INTERACTIE MODE */
	// settings: configure items
	set << SETTINGS::appname("WirelessUART");
	set << SETTINGS::appid_default(DEFAULT_APP_ID); // set default appID
	set << SETTINGS::ch_default(DEFAULT_CHANNEL); // set default channel
	set << SETTINGS::lid_default(DEFAULT_LID); // set default lid
	set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
	set.reload(); // load from EEPROM.
	
	/*** SETUP section */
	// the twelite main class
	the_twelite
		<< set                      // from iteractive mode (APPID/CH/POWER)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	nwk	<< set;						// from interactive mode (LID/REPEAT)

	/*** BEGIN section */
	SerialParser.begin(PARSER::ASCII, 128); // Initialize the serial parser
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- WirelessUart (id=" << int(nwk.get_config().u8Lid) << ") ---" << mwx::crlf;
}
```

インタラクティブモードを初期化しています。このサンプルでは互いに論理デバイスID(LID)が異なるデバイスを2台以上用意します。



```cpp
SerialParser.begin(PARSER::ASCII, 128); 
```

[シリアルパーサー](../api-reference/predefined\_objs/serialparser.md)を初期化します。



### loop()

```cpp
while(Serial.available())  {
	if (SerialParser.parse(Serial.read())) {
		Serial << ".." << SerialParser;
		const uint8_t* b = SerialParser.get_buf().begin();
		uint8_t addr = *b; ++b; // the first byte is destination address.
		transmit(addr, b, SerialParser.get_buf().end());
	}
}
```

シリアルからのデータ入力があった時点で、シリアルパーサーに１バイト入力します。アスキー形式が最後まで受け付けられた時点で`SerialParser.parse()`は`true`を戻します。

`SerialParser`は内部バッファに対して`smplbuf`でアクセスできます。上の例ではバッファの１バイト目を送信先のアドレスとして取り出し、２バイト目から末尾までを`transmit()`関数に渡します。



### on\_rx\_packet()

パケットを受信したときには、送信元を先頭バイトにし続くペイロードを格納したバッファ`smplbuf_u8<128> buf`を生成し、出力用のシリアルパーサー`serparser_attach pout`からシリアルに出力しています。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	// check the packet header.
	const uint8_t* p = rx.get_payload().begin();
	if (rx.get_length() > 4 && !strncmp((const char*)p, (const char*)FOURCHARS, 4)) {
		Serial << format("..rx from %08x/%d", rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;

		smplbuf_u8<128> buf;
		mwx::pack_bytes(buf			
				, uint8_t(rx.get_addr_src_lid())            // src addr (LID)
				, make_pair(p+4, rx.get_payload().end()) );	// data body

		serparser_attach pout;
		pout.begin(PARSER::ASCII, buf.begin(), buf.size(), buf.size());
		Serial << pout;
	}
}
```





## テスト用のコマンド

{% hint style="warning" %}
テストデータは必ず**ペースト機能**を用いてターミナルに入力してください。入力にはタイムアウトがあるためです。

参考： TWE ProgrammerやTeraTermでのペーストはAlt+Vを用います。
{% endhint %}

{% hint style="info" %}
入力の末尾にCR LFが必要です。

最初はCR LFが省略できるXで終わる系列を試してください。終端文字列が入力されない場合は、その系列は無視されます。
{% endhint %}



### 例

```
:FE00112233X

:FE001122339C

```

任意の子機宛に`00112233`を送付します。



### 例

```
:03AABBCC00112233X

:03AABBCC0011223366

```

子機３番に対して`AABBCC00112233`を送付します。



### 例

```
:FF00112233X

:00112233X

```

任意の親機または子機宛(`0xFF`)、親機宛(`0x00`)に送付します。

