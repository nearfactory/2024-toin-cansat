---
title: "ビヘイビア"
author: "Mono Wireless Inc."
description: "ビヘイビア"
---
# ビヘイビア

ビヘイビアは、指定の方法でクラスを定義することで、[`the_twelite`](../predefined\_objs/the\_twelite.md)クラスオブジェクトに登録できるようになります。登録したビヘイビアはTWENETに組み込まれて動作し、ユーザコードではアプリケーションの振る舞いを記述できるようになります。ループでの記述ではできなかったTWENETからの割り込みやイベントのコールバック関数を定義することが出来ます。ループでの記述に比べ、定義が多くなりますが、より複雑なアプリケーションを記述するのに向いています。

{% hint style="success" %}
ビヘイビアのサンプル[PAL\_AMB-behavior](pal\_amb-behavior.md)を参照してください。
{% endhint %}

## クラス定義 (.hpp)

ビヘイビアの定義には下記に示すようなクラス定義が必要です。

```cpp
class MY_APP_CLASS: MWX_APPDEFS_CRTP(MY_APP_CLASS)
{
public:
    static const uint8_t TYPE_ID = 0x01;

    // load common definition for handlers
    #define __MWX_APP_CLASS_NAME MY_APP_CLASS
    #include "_mwx_cbs_hpphead.hpp"
    #undef __MWX_APP_CLASS_NAME

public:
    // constructor
    MY_APP_CLASS() {}

    void _setup() {}    
    void _begin() {}

public:
    // TWENET callback handler (mandate)
    void loop() {}
    void on_sleep(uint32_t & val) {}
    void warmboot(uint32_t & val) {}
    void wakeup(uint32_t & val) {}

    void on_create(uint32_t& val) { _setup();  }
    void on_begin(uint32_t& val) { _begin(); }
    void on_message(uint32_t& val) { }

public:
    void network_event(mwx::packet_ev_nwk& pEvNwk) {}
    void receive(mwx::packet_rx& rx) {}
    void transmit_complete(mwx::packet_ev_tx& evTx) {}
};
```

上記の例ではMY\_APP\_CLASSという名前でビヘイビアクラスを定義しています。いくつかの箇所にMY\_APP\_CLASSの記述が必要です。



```cpp
class MY_APP_CLASS: MWX_APPDEFS_CRTP(MY_APP_CLASS)
```

クラス名の定義と、ベース（親）クラスの定義をします。`MWX_APPDEFS_CRTP()`はマクロです。



```cpp
    #define __MWX_APP_CLASS_NAME MY_APP_CLASS
    #include "_mwx_cbs_hpphead.hpp"
    #undef __MWX_APP_CLASS_NAME
```

ここでは必要な定義を `#include` により取り込んでいます。



```cpp
MY_APP_CLASS() {}
```

コンストラクタの定義です。



## メソッド

### loop()

メインループで、グローバル定義の`loop()`と同じ役割の関数です。



### on\_create()

`on_create()`はオブジェクト生成時（`use<>()`メソッド）に呼び出されます。`val`は将来の拡張のためのパラメータです。



### on\_begin()

`on_begin()`は`setup()`終了後に呼び出されます。`val`は将来の拡張のためのパラメータです。



### on\_sleep()

スリープ前に呼び出されます。`val`は将来の拡張のためのパラメータです。



### warmboot()

スリープ復帰時の初期段階で呼び出されます。`val`は将来の拡張のためのパラメータです。

この時点でまだペリフェラルが初期化されていません。スリープ起床要因の確認ができます。



### wakeup()

スリープ復帰時に呼び出されます。`val`は将来の拡張のためのパラメータです。

{% hint style="info" %}
ここでスリープ呼び出しも可能です。
{% endhint %}



### receive()

```cpp
void receive(mwx::packet_rx& rx)
```

パケットが受信されたとき、受信したパケット情報を`rx`として呼び出されます。



### transmit\_complete()

```cpp
void transmit_complete(mwx::packet_ev_tx& evTx)
```

パケット送信完了時に送信情報を`evTx`として呼び出されます。`evTx.u8CbId`が送信時のIDで`evTx.bStatus`が送信の成功(`1`)失敗(`0`)を示すフラグです。





## ハンドラの定義 (.cpp)

ビヘイビアのハンドラ（割り込み、イベント、状態定義）はcppファイルに定義します。ファイルは分割できず、全てのハンドラ定義を一つのファイル中に記述します。

{% hint style="warning" %}
ハンドラの定義をしないビヘイビアの場合でも、必ず、下記のcppファイルを作成します。
{% endhint %}



cppファイルの冒頭と末尾にはMWXライブラリの必要な定義(`#include "_mwx_cbs_cpphead.hpp"`)をインクルードする必要があります。

```cpp
#include <TWELITE>
#include "myAppClass.hpp" // ビヘイビア定義ファイル

/*****************************************************************/
// MUST DEFINE CLASS NAME HERE
#define __MWX_APP_CLASS_NAME MY_APP_CLASS
#include "_mwx_cbs_cpphead.hpp" // 冒頭の定義
/*****************************************************************/
```

ファイルの冒頭には、上記のようにビヘイビア定義の.hppファイルをインクルードします。`__MWX_APP_CLASS_NAME`にはビヘイビアのクラス名を指定します。上記では`MY_APP_CLASS`です。



```cpp
/*****************************************************************/
// common procedure (DO NOT REMOVE)
#include "_mwx_cbs_cpptail.cpp"
// MUST UNDEF CLASS NAME HERE
#undef __MWX_APP_CLASS_NAME
/*****************************************************************/
```

ファイルの末尾では必要な定義(`#include "_mwx_cbs_cpptail.cpp"`)をインクルードします。



ハンドラ定義は以下の例のように記述します。定義の種別については後述します。定義用のマクロを用いて利用したいハンドラの定義を記述します。利用しないハンドラは記述しないようにしてください。

`MWX_???_INT()`は割り込みハンドラの定義、`MWX_???_EVENT()`はイベントハンドラの定義、`MWX_STATE()`はステートマシンの状態定義です。

```cpp
// TickTimer割り込み
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled) {
	// blink LED
	digitalWrite(PAL_AMB::PIN_LED, 
	      ((millis() >> 9) & 1) ? PIN_STATE::HIGH : PIN_STATE::LOW);
}

// PAL_AMB::PIN_BIN(12)のイベント
MWX_DIO_EVENT(PAL_AMB::PIN_BTN, uint32_t arg) {
	Serial << "Button Pressed" << mwx::crlf;
	
	static uint32_t u32tick_last;
	uint32_t tick = millis();

	if (tick - u32tick_last > 100) {
		PEV_Process(E_ORDER_KICK, 0UL);
	}

	u32tick_last = tick;
}

// 状態 STATE_0 の動作定義
MWX_STATE(E_MWX::STATE_0, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_START_UP) {
		Serial << "[STATE_0:START_UP]" << mwx::crlf;	
	} else
	if (ev == E_ORDER_KICK) {
		PEV_SetState(E_MWX::STATE_1);
	}
}

// 状態 STATE_1 の動作定義	
MWX_STATE(E_MWX::STATE_1, uint32_t ev, uint32_t evarg) {
	if (ev == E_EVENT_NEW_STATE) {
		Serial << "[STATE_1]" << mwx::crlf;	
	} else
	if (ev == E_ORDER_KICK) {
		PEV_SetState(E_MWX::STATE_2);
	} else
	if (ev == E_EVENT_TICK_SECOND) {
		Serial << "<1>";
	}	
}
```



## 割り込み・イベントハンドラ

割り込みハンドラは、マイコンの割り込みが発生したときに現在実行中のコードを中断して実行されます。このため、可能な限り短い処理を記述することが望ましく、また、変数等の操作に対しても細心の注意が必要です。

割り込みハンドラのパラメータには`uint8_t& handled`があり、この値を`true`にセットすることで、続くイベント呼び出しを行いません。

`handled`が`false`のまま割り込みハンドラを終了した場合、アプリケーションループ（通常コード）の中でイベントハンドラが呼び出されます。イベントハンドラには`handled`のパラメータはありません。イベントハンドラは通常コードですので、比較的大きな処理を行うことが出来ます。イベントハンドラのオーバーヘッドも発生するため、頻繁な割り込み都度呼び出されるような処理の場合、処理しきれなくなる可能性があります。また、イベントの発生はシステム内部のFIFOキューにより処理されるため、一定時間内に処理できない場合はイベントが消失する場合もあります。

以下にハンドラ関数定義用のマクロの解説を行います。

### DIO

```cpp
MWX_DIO_INT(N, uint32_t arg, uint8_t& handled)
MWX_DIO_EVENT(N, arg)
```

DIO(ディジタルIO)割り込み・イベントです。`N`は対象DIOの番号を指定します。`arg`は将来の拡張のための定義です。

割り込みを発生させるためには[`pinMode()`](../funcs/dio/pinmode.md)による適切な入力設定, [`attachDioInt()`](../funcs/dio/attachintdio.md)による割り込み開始の設定が必要です。



### TICKTIMER

```cpp
MWX_TICKTIMER_INT(uint32_t arg, uint8_t& handled)
MWX_TICKTIMER_EVENT(uint32_t arg)
```

TickTimer割り込み・イベントです。`arg`は将来の拡張のための定義です。

{% hint style="danger" %}
TickTimerの`handled`フラグを`true`にセットしてはいけません。TWENETが動作しなくなります。
{% endhint %}



### TIMER

```cpp
MWX_TIMER_INT(N, uint32_t arg, uint8_t& handled)
MWX_TIMER_EVENT(N, uint32_t arg)
```

タイマー割り込み・イベントです。`N`は対象タイマーの番号を指定します。`arg`は将来の拡張のための定義です。

割り込みを発生させるためには、[Timerオブジェクト](../predefined\_objs/timers.md)をソフトウェア割り込みを有効にして開始します。



### その他

{% hint style="warning" %}
MWXライブラリで標準的に定義しない、その他の割り込み・イベントの定義です。AHIペリフェラルマニュアルの理解が必要です。
{% endhint %}

その他の割り込み・イベントは以下のハンドラ関数で受けることが出来ます。これらは将来的に専用のハンドラが定義された場合、利用できなくなります。

```cpp
MWX_MISC_INT(uint32_t arg, uint32_t arg2, handled)
MWX_MISC_EVENT(auint32_t rg, uint32_t arg2)
```

ペリフェラル (AHI) の割り込みハンドラの`u32DeviceId`が`arg`、`u32ItemBitmap`が`arg2`に対応します。



## 状態マシン

状態マシン（ステートマシン）は、メッセージを受け取り、そのメッセージに応じて状態を遷移させながら動作させるアプリケーションの記述方法です。

[PAL\_AMB-behavior](pal\_amb-behavior.md)サンプルでは、センサーの動作開始からセンサーの値取得、無線パケット送信から送信完了まで、スリープ遷移といったアプリケーションの動作の流れを記述しています。実例として参考にしてください。

受け取るイベントは以下となります。

| イベント名                 | 内容                                                                                                                                                                                                                                                                                 |
| --------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `E_EVENT_START_UP`    | システム始動時に呼び出される。電源投入直後はパラメータが`0`で呼び出されます。<strong>実行初期であるため、通常処理を行う状態に遷移する場合は一旦begin()メソッドから`PEV_Process()`を呼び出し動作を開始させます。</strong><br><strong></strong><br><strong></strong>スリープ復帰後も呼び出されるがパラメータは`0`以外です。<strong>この状態からは通常処理を行えます。</strong> |
| `E_EVENT_NEW_STATE`   | 状態遷移直後に新しい状態で呼び出されます。ある状態に遷移したときに最初に実行される処理を記述します。                                                                                                                                                                                                                                 |
| `E_EVENT_TICK_TIMER`  | 1msごとのTickTimerで呼び出されます。                                                                                                                                                                                                                                                           |
| `E_EVENT_TICK_SECOND` | １秒毎に呼び出されます。                                                                                                                                                                                                                                                                       |



### PEV\_SetState()

```cpp
void PEV_SetState(uint32_t s)
```

状態を`s`に設定します。

状態ハンドラを抜けると次の状態に遷移し、続けて`E_EVENTS_NEW_STATE`イベントで状態ハンドラが呼び出されます。



### PEV\_u32Elaspsed\_ms()

```cpp
uint32_t PEV_u32Elaspsed_ms()
```

状態遷移してからの経過時間\[ms]を返します。タイムアウトを管理するような目的で使用します。



```cpp
MWX_STATE(MY_APP_CHILD::STATE_TX, uint32_t ev, uint32_t evarg) {
  ...
  
	if (PEV_u32Elaspsed_ms() > 100) {
		// does not finish TX!
		Serial << "[STATE_TX] FATAL, TX does not finish!" << mwx::crlf << mwx::flush;
		the_twelite.reset_system();
	}
}
```

上記の例では100ms経過した時点でシステムリセットを行います。



### PEV\_Process()

```cpp
void PEV_Process(uint32_t ev, uint32_t u32evarg) {
```

状態ハンドラ外から呼び出します。状態ハンドラをイベント`ev`パラメータ`u32evarg`で実行します。



```cpp
void transmit_complete(mwx::packet_ev_tx& txev) {
    Serial << "..txcomp=" << int(txev.u8CbId) << mwx::crlf;
    PEV_Process(E_ORDER_KICK, txev.u8CbId); // pass the event to state machine
}
```

送信完了イベントを状態マシンに伝えます。つまり状態ハンドラの呼び出しを行います。

{% hint style="danger" %}
直接状態ハンドラを呼び出すことは行わないでください。`E_EVENT_NEW_STATE`が実行されないなどの問題が発生します。
{% endhint %}



### PEV\_KeepStateOnWakeup()

```cpp
void PEV_KeepStateOnWakeup()
```

スリープ直前に設定します。スリープ復帰後に、直前の状態を維持します。つまり、スリープを開始した状態で`E_EVENT_START_UP`で状態ハンドラが呼び出されます。



### PEV\_is\_coldboot()

```cpp
bool PEV_is_coldboot(uint32_t ev, uint32_t u32evarg)
```

イベントが起床時の`E_EVENT_START_UP`かどうか判定します。



### PEV\_is\_warmboot()

```cpp
bool PEV_is_warmboot(uint32_t ev, uint32_t u32evarg)
```

イベントがスリープ復帰時の`E_EVENT_START_UP`かどうか判定します。

