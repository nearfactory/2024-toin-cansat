---
title: SM_SIMPLE ステートマシン
author: "Mono Wireless Inc."
description: "SM_SIMPLE ステートマシン"
---

# SM\_SIMPLE ステートマシン

SM\_SIMPLEは、サンプルコード中の状態遷移、タイムアウト待ち、送信完了などの処理待ちを行うために用意しています。

SM\_SIMPLEの基本的なコード抜粋を示します。

```cpp
#include <SM_SIMPLE>

enum class STATE : uint8_t {
	INIT = 0,
	SENSOR,
	TX,
	TX_WAIT_COMP,
	GO_SLEEP
};

SM_SIMPLE<STATE> step;

begin() {
  ...
  step.init(); //初期化
}

loop() {
  do {
    switch(step.state()) {
    case STATE::INIT:
      ...
      step.next(STATE::SENSOR);
    break;
    
    case STATE::SENSOR:
      ...
      step.next(STATE::TX);
    break;
    
    case STATE::TX:
      if (/*送信要求成功*/) {
        step.set_timeout(100); // タイムアウトの設定
        step.clear_flag(); //処理待ち
          
        step.next(STATE::TX_WAIT_COMP);
      }
    break;
    
    case STATE::TX_WAIT_COMP:
      if (step.is_timeout()) the_twelite.reset_system(); // タイムアウト
      if (step.is_flag_ready()) sleepNow(); // flagがセットされた
    break;
    
    ...
    }
  } while(step.b_more_loop());
}

void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}

void sleepNow() {
	step.on_sleep(false); // reset state machine.
  the_twelite.sleep(10000); // 10sec
}
```



### 解説

SM\_SIMPLEを利用するには状態一覧としての`enum class`定義が必要です。上記では`STATE`として定義しています。このステージをパラメータとして`SM_SIMPLE<STATE> step;`のようにクラスオブエクトを生成します。生成したクラスオブジェクトは`.setup()`により初期化しておきます。

```cpp
enum class STATE : uint8_t {
	INIT = 0,
	SENSOR,
	TX,
	TX_WAIT_COMP,
	GO_SLEEP
};

SM_SIMPLE<STATE> step;

void setup() {
  step.init();
}
```



SM\_SIMPLEの初期状態は値が0で、上記の例では`STATE::INIT`が対応します。現在の状態を取得するには`.state()`を用、上記例のように_do while_文中の_switch_節の判定式に用います。

```cpp
loop() {
  do {
    switch(step.state()) {
    case STATE::INIT: // 値0の状態
    ...
```



状態の遷移には`.next()`を呼び出します。状態が変更された場合、`b_more_loop()`が`true`になり_do while_節のループがもう一度実行されます。例では`STATE::SENSOR`状態から`.next(STATE::TX)`を呼び出すことで、ループがもう一度実行され`case STATE::TX:`節も実行されることになります。状態を変更しない場合は_do while_ループを脱出し`loop()`を一旦終了します。次の`loop()`の呼び出しまで一旦待ちます。

```cpp
  do {
    switch(step.state()) {
    ...
    case STATE::SENSOR:
      ...
      step.next(STATE::TX); // (1)状態遷移
    break;
    
    case STATE::TX: // (3) ２回めのループで呼び出される
      if (/*送信要求成功*/) {
      ...
    }
  } while (b_more_loop()); // (2) ループ継続判定 true
```



送信完了などの処理待ちをしたい場合は`.clear_flag()`を呼び出し、別のコールバック関数などで`.set_flag(uint32_t)`により処理完了を知らせます。ここで指定した`uint32_t`型のパラメータをは`.get_flag_value()`から読み出せます。

またタイムアウトの処理を行いたい場合は`.set_timeout(uint32_t)`を呼び出した時刻を記録し、`.is_timeout()`によりタイムアウト時間が経過したかを調べることができます。

```cpp
    case STATE::TX:
      if (/*送信要求成功*/) {
        step.set_timeout(100); // タイムアウトの設定
        step.clear_flag(); //処理待ち
          
        step.next(STATE::TX_WAIT_COMP);
      }
    break;
    
    case STATE::TX_WAIT_COMP:
      if (step.is_timeout()) ...; // タイムアウト
      if (step.is_flag_ready()) ...; // flagがセットされた
    break;
...

// 送信完了イベント
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus); // flag を設定する
}
```



スリープからの復帰で再びSM\_SIMPLEを利用することになりますが、スリープ前に必ず`.on_sleep(bool)`を呼び出すようにします。パラメータに`false`を入れると復帰後に`0`状態から開始し、`true`を入れるとスリープ直前の状態から再開します。

```cpp
void sleepNow() {
	step.on_sleep(false); // reset state machine.
  the_twelite.sleep(10000); // 10sec
}
```



### ソースコード

以下にSM\_SIMPLEのソースコードを示します。

```cpp
// very simple class to control state used in loop().
template <typename STATE>
class SM_SIMPLE {
	uint32_t _u32_flag_value;  // optional data when flag is set.
	uint32_t _ms_start;		// system time when start waiting.
	uint32_t _ms_timeout;	// timeout duration

	STATE _step;			  // current state
	STATE _step_prev;		// previous state
	bool_t _b_flag; 		// flag control.
public:
	// init
	void setup() { memset(this, 0, sizeof(SM_SIMPLE)); }
	// call befoer sleeping (save state machine status)
	void on_sleep(bool b_save_state = false) {
		STATE save = _step;
		setup();
		if(b_save_state) _step = _step_prev = save;
	}

	// state control
	void next(STATE next) { _step = next; } // set next state
	STATE state() { return _step; } // state number
	bool b_more_loop() { // if state is changed during the loop, set true
		if (_step != _step_prev) { _step_prev = _step; return true; }
		else return false;
	}

	// timeout control
	void set_timeout(uint32_t timeout) {
		_ms_start = millis();
		_ms_timeout = timeout;
	}
	bool is_timeout() { return (millis() - _ms_start) >= _ms_timeout; }

	// flag control
	void clear_flag() { _b_flag = false; _u32_flag_value = 0; }
	void set_flag(uint32_t u32_flag_value = 0) {
		_b_flag = true;
		_u32_flag_value = u32_flag_value; }
	uint32_t get_flag_value() { return _u32_flag_value; }
	bool is_flag_ready() { return _b_flag; }
};
```

* バージョンによって内容が変化する場合があり。
* 本体はmwxライブラリソースフォルダのSM\_SIMPLE.hppに格納されます。

