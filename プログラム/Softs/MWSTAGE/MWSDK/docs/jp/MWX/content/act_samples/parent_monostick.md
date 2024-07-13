---
title: Parent_MONOSTICK
author: "Mono Wireless Inc."
description: 親機アプリケーション(MONOSTICK用)
---

# Parent\_MONOSTICK

MONOSTICKを親機として使用するアクトです。子機からのパケットのデータペイロードをシリアルポートに出力します。サンプルアクトの多くのサンプルでのパケットを表示することが出来ます。

{% hint style="success" %}
このアクトには以下が含まれます。

* 無線パケットの受信
* 受信したパケットのデータ解釈
* インタラクティブモードの設定 - [\<STG\_STD>](../settings/stg\_std.md)
* バイト列のアスキー形式への変換 - [serparser](../api-reference/classes/ser\_parser.md)
{% endhint %}

## アクトの機能

* サンプルアクトの子機からのパケットを受信して、シリアルポートへ出力する。

## アクトの使い方

### 必要なTWELITEと配線

| 役割 | 例                                                                        |
| -- | ------------------------------------------------------------------------ |
| 親機 | [MONOSTICK BLUEまたはRED](https://mono-wireless.com/jp/products/MoNoStick/) |
| 子機 | サンプルアクトの子機設定 (例: `Slp_Wk_and_Tx`, `PAL_AMB`, `PAL_MAG`, `PAL_MOT???`など)  |

最初は以下のデフォルトの設定にて確認してください。

* アプリケーションID: `0x1234abcd`
* チャネル: `13`



## アクトの解説

### 宣言部

#### インクルード

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <MONOSTICK>
#include <NWK_SIMPLE>
#include <STG_STD>
```

MONOSTICK用のボードビヘイビア[`<MONOSTICK>`](../boards/monostick.md)をインクルードしています。このボードサポートには、LEDの制御、ウォッチドッグ対応が含まれます。

* \<NWK\_SIMPLE> 簡易中継ネットの定義を読み込みます
* \<STG\_STD> インタラクティブモードの定義を読み込みます。



#### その他

```cpp
// application ID
const uint32_t DEFAULT_APP_ID = 0x1234abcd;
// channel
const uint8_t DEFAULT_CHANNEL = 13;
// option bits
uint32_t OPT_BITS = 0;

/*** function prototype */
bool analyze_payload(packet_rx& rx);
```

デフォルト値や関数プロトタイプなどの宣言をしています。



### setup()

```cpp
auto&& brd = the_twelite.board.use<MONOSTICK>();
auto&& set = the_twelite.settings.use<STG_STD>();
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
```

`setup()`では、まず`<MONOSTICK>`ボードビヘイビア、`<STG_STD>`インタラクティブモード ビヘイビア、`<NWK_SIMPLE>`ビヘイビアを`use<>`を用い読み込みます。この手続きは必ずsetup()内で行います。



```cpp
set << SETTINGS::appname("PARENT"); // 設定画面中のタイトル
set << SETTINGS::appid_default(DEFAULT_APP_ID); // アプリケーションIDデフォルト
set << SETTINGS::ch_default(DEFAULT_CHANNEL); // チャネルデフォルト
set << SETTINGS::lid_default(0x00); // LIDデフォルト
set.hide_items(E_STGSTD_SETID::OPT_DWORD2, E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4, E_STGSTD_SETID::ENC_KEY_STRING, E_STGSTD_SETID::ENC_MODE);
set.reload(); // 設定を不揮発性メモリから読み出す
OPT_BITS = set.u32opt1(); // 読み込み例（オプションビット）
```

続いてインタラクティブモードの設定と設定値の読み出しを行います。[`<STG_STD>`インタラクティブモード](../settings/stg\_std.md)では、標準的な項目が用意されていますが、作成するアクトごとにいくつかのカスタマイズを行えるようになっています。

* `appname`→ 設定画面中のタイトル行にでるアクト名称
* `appid_default`→ デフォルトのアプリケーションID
* `ch_default`→ デフォルトのチャネル
* `lid_default`→ デバイスID(LID)のデフォルト値
* .`hide_items()`→ 項目の非表示設定

設定値を読み出す前には必ず`.reload()`を呼び出します。設定値は`.u32opt1()`のように設定値ごとに読み出し用のメソッドが用意されています。



```cpp
the_twelite
	<< set                    // インタラクティブモードの設定を反映
	<< TWENET::rx_when_idle() // 受信するように指定
	;

// Register Network
nwk << set;							// インタラクティブモードの設定を反映
nwk << NWK_SIMPLE::logical_id(0x00) // LIDだけは再設定
	;
```

いくつかの設定値は[`<STG_STD>`オブジェクトを用いて直接反映](../settings/stg\_std.md#no-1)することが出来ます。また、DIPスイッチの設定などにより特定の値を書き換えたいような場合などは、反映されたあとに別個に値を書き換えることも出来ます。上記の例では[`the_twelite`](../api-reference/predefined\_objs/the\_twelite.md#yan-suan-zi-she-ding)オブジェクトにアプリケーションID、チャネル、無線出力などを設定し、[`nwk`](../networks/nwk\_simple.md#yan-suan-zi-she-ding)オブジェクトに対してLIDと再送回数の設定をしてから、再度LIDを0に設定し直しています。



```cpp
brd.set_led_red(LED_TIMER::ON_RX, 200); // RED (on receiving)
brd.set_led_yellow(LED_TIMER::BLINK, 500); // YELLOW (blinking)
```

`<MONOSTICK>`ボードビヘイビアではLED点灯制御のための手続きを利用できます。

1行目では赤色のLEDを無線パケットを受信したら200ms点灯する設定をしています。最初のパラメータは`LED_TIMER::ON_RX`が無線パケット受信時を意味します。２番目は点灯時間をmsで指定します。

2行目はLEDの点滅指定です。１番目のパラメータは`LED_TIMER::BLINK`が点滅の指定で、２番目のパラメータは点滅のON/OFF切り替え時間です。500msごとにLEDが点灯、消灯(つまり１秒周期の点滅)を繰り返します。



```cpp
the_twelite.begin(); // start twelite!
```

`the_twelite`を開始するための手続きです。act0..4では出てきませんでしたが`the_twelite`の設定や各種ビヘイビアの登録を行った場合は、必ず呼び出すようにしてください。



### loop()

このサンプルでは`loop()`中の処理はありません。

```cpp
void loop() {
}
```



### on\_rx\_packet()

パケットを受信したときに呼び出されるコールバック関数です。この例では受信したパケットデータに対していくつかの出力を行っています。

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {  
	Serial << ".. coming packet (" << int(millis()&0xffff) << ')' << mwx::crlf;

  ...
  
	// packet analyze
	analyze_payload(rx);
}
```



#### analyze\_payload()

関数の末尾で呼び出されるanalyze\_payload()は、いくつかのサンプルアクトのパケットを解釈するコードが含まれています。サンプルアクト中のパケット生成部分と対応させてコードを参照してください。

```cpp
bool b_handled = false;

uint8_t fourchars[4]{};
auto&& np = expand_bytes(
	    rx.get_payload().begin(), rx.get_payload().end()
		, fourchars
    );
    
if (np == nullptr) return;

// display fourchars at first
Serial
	<< fourchars 
	<< format("(ID=%d/LQ=%d)", rx.get_addr_src_lid(), rx.get_lqi())
	<< "-> ";
	
```

この関数では最初に４文字識別データを`fourchars[5]`配列に読み込みます。

読み込みは[`expand_bytes()`](../api-reference/funcs/utility/expand\_bytes.md)関数を用います。この関数の第１・第２パラメータはC++の標準ライブラリの作法に倣い、受信パケットのペイロード部の先頭ポインタ`.begin()`と末尾ポインタの次`.end()`を与えます。続くパラメータは可変引数として、読み込むデータ変数を与えます。戻り値はエラー時は_`nullptr`_、それ以外は次の解釈ポインタとなります。末尾まで解釈した場合は`.end()`が戻ります。ここでのパラメータは`uint8_t fourchars[4]`です。



{% hint style="info" %}
この記述で対応するのは配列長さ`N`が指定される`uint8_t[N]`型のみで、`uint8*`型、`char*`型、`char[]`型などを用いる場合は、`make_pair(char*,int)`を用いた指定が必要になります。

```cpp
char fourchars[5]{}; // 終端文字\0も含め5バイト確保する
auto&& np = expand_bytes(
	    rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((char *)fourchars, 4)
	);
```
{% endhint %}



つづいて４バイトヘッダに対応した処理を行います。ここではサンプルアクトSlp\_Wk\_and\_Txのパケットを解釈し内容を表示します。

```cpp
// Slp_Wk_and_Tx
if (!b_handled && !strncmp(fourchars, "TXSP", 4)) {
	b_handled = true;
	uint32_t tick_ms;
	uint16_t u16work_ct;

	np = expand_bytes(np, rx.get_payload().end()
		, tick_ms
		, u16work_ct
	);

	if (np != nullptr) {
		Serial << format("Tick=%d WkCt=%d", tick_ms, u16work_ct);
	} else {
		Serial << ".. error ..";
	}
}
```

他の解釈部の判定をスキップするように`b_handled`を_`true`_に設定します。

`"TXSP"`のパケットでは`uint32_t`型のシステムタイマーカウントと、`uint16_t`型のダミーカウンタの値が格納されています。各々変数を宣言して[`expand_bytes()`](../api-reference/funcs/utility/expand\_bytes.md)関数を用い読み込みます。上述と違うのは、読み出しの最初のポインタとして第一パラメータが`np`となっている点です。`tick_ms`と`u16work_ct`をパラメータとして与え、ビッグエンディアン形式のバイト列としてペイロードに格納された値を読み出します。

読み出しに成功すれば内容を出力して終了です。



#### 独自のアスキー書式を定義して出力する

ユーザが定義した並び順で[アスキー形式](../api-reference/classes/ser\_parser.md#asuk)により構成します。

```cpp
smplbuf_u8<128> buf;
mwx::pack_bytes(buf
	, uint8_t(rx.get_addr_src_lid())		   // 送信元の論理ID
	, uint8_t(0xCC)											   // 0xCC
	, uint8_t(rx.get_psRxDataApp()->u8Seq) // パケットのシーケンス番号
	, uint32_t(rx.get_addr_src_long())		 // 送信元のシリアル番号
	, uint32_t(rx.get_addr_dst())			     // 宛先アドレス
	, uint8_t(rx.get_lqi())					       // LQI:受信品質
	, uint16_t(rx.get_length())				     // 以降のバイト数
	, rx.get_payload() 						         // データペイロード
);

serparser_attach pout;
pout.begin(PARSER::ASCII, buf.begin(), buf.size(), buf.size());

Serial << "FMT PACKET -> ";
pout >> Serial;
Serial << mwx::flush;
```

1行目はアスキー書式に変換する前のデータ列を格納するバッファをローカルオブジェクトとして宣言しています。

2行目は`pack_bytes()`を用いてデータ列を先ほどの`buf`に格納します。データ構造はソースコードのコメントを参照ください。`pack_bytes()`のパラメータには`smplbuf_u8 (smplbuf<uint8_t, ???>)`形式のコンテナを指定することもできます。

{% hint style="info" %}
パケットのシーケンス番号は、`<NWK_SIMPLE>`で自動設定され、送信パケット順に割り振られます。この値はパケットの重複検出に用いられます。

LQI (Link Quality Indicator)は受信時の電波強度に相当する値で、値が大きければ大きいほどより強い電界強度で受信できていることになります。ただしこの値と物理量との厳格な関連は定義されていませんし、環境のノイズと相対的なものでLQIがより大きな値であってもノイズも多ければ通信の成功率も低下することになります。
{% endhint %}

13,14,17行目は、シリアルパーサーの宣言と設定、出力です。



#### NWK\_SIMPLEのヘッダを含めてダンプ出力する

最初の出力（`if(0)`により実行されないようになっています)は`<NWK_SIMPLE>`の制御データを含めたデータをすべて表示します。制御データは11バイトあります。通常は制御情報を直接参照することはありませんが、あくまでも参考です。

```cpp
serparser_attach pout;
pout.begin(PARSER::ASCII, rx.get_psRxDataApp()->auData, 
    rx.get_psRxDataApp()->u8Len, rx.get_psRxDataApp()->u8Len);

Serial << "RAW PACKET -> ";
pout >> Serial;
Serial << mwx::flush;

// 参考：制御部のパケット構造
// uint8_t  : 0x01 固定
// uint8_t  : 送信元のLID
// uint32_t : 送信元のロングアドレス(シリアル番号)
// uint32_t : 宛先アドレス
// uint8_t  : 中継回数
```

1行目は出力用のシリアルパーサをローカルオブジェクトとして宣言しています。内部にバッファを持たず、外部のバッファを流用し、パーサーの出力機能を用いて、バッファ内のバイト列を[アスキー形式](../api-reference/classes/ser\_parser.md#asuk)出力します。

２行目はシリアルパーサーのバッファを設定します。すでにあるデータ配列、つまり受信パケットのペイロード部を指定します。`serparser_attach pout`は、既にあるバッファを用いたシリアルパーサーの宣言です。`pout.begin()`の１番目のパラメータは、パーサーの対応書式を`PARSER::ASCII`つまりアスキー形式として指定しています。２番目はバッファの先頭アドレス。３番目はバッファ中の有効なデータ長、４番目はバッファの最大長を指定します。出力用で書式解釈に使わないため４番目のパラメータは３番目と同じ値を入れています。

6行目でシリアルポートへ`>>`演算子を用いて出力しています。

7行目の`Serial << mwx::flush`は、ここで出力が終わっていないデータの出力が終わるまで待ち処理を行う指定です。(`Serial.flush()`も同じ処理です)
