# ユーザ定義イベント処理関数

ユーザ定義イベント処理関数は、TWENET からコールバックとして呼び出される形でイベント処理を行う目的でユーザが独自に定義する関数です。関数と呼んでいますが、ステート（状態遷移）マシンとして振るいます。

```c
void cbAppColdStart(bool_t bInit) {
    if (bInit == FALSE) {
 		...
    } else {
    	...
    	ToCoNet_Event_Register_State_Machine(vProcessEvCore);
    }
}

void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg) {
    // check boot seq
	if (eEvent == E_EVENT_START_UP) {
		if (u32evarg & EVARG_START_UP_WAKEUP_RAMHOLD_MASK) {
			// woke from NORMAL SLEEP (RAMHOLD)
		}
	    if (u32evarg & EVARG_START_UP_WAKEUP_MASK) {
			// woke from DEEP SLEEP
	    } else {
			// COLD boot
	    }
	}
	
	switch (pEv->eState) {
	case E_STATE_IDLE:
		; // some task
		ToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
		break;
	case E_STATE_APP_NORMAL:
		; // some task (e.g. send s Tx packet)
		if (cond) {
			ToCoNet_Event_SetState(pEv, E_STATE_APP_WAIT_TX);
		}
		break;
	case E_STATE_APP_WAIT_TX:
		; // some task (e.g. wait until Tx finishes)
		if (cond) {
		    ToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
		}
		break;
	}
}
```



### 登録

ユーザ定義イベント処理関数は [ToCoNet_Event_Register_State_Machine](toconet_event-api/toconet_event_register_state_machine.md)() により最大２つまで登録します。

{% hint style="info" %}
[E_EVENT_START_UP](ibento.md)イベントを受け取るにはユーザ定義イベント処理関数の登録が必要です。
{% endhint %}



### 伝達されるイベント

伝達されるイベントは以下の３イベントです。これ以外のイベントは [ToCoNet_Event_Process](toconet_event-api/toconet_event_process.md)() 関数により関数のコールバック呼び出しとして伝達されます。

* [E_EVENT_START_UP](ibento.md) : 始動時
* [E_EVENT_TICK_TIMER](ibento.md) : 4ms 周期（デフォルト）
* [E_EVENT_TICK_SECOND](ibento.md) : １秒周期

以下のコードは送信完了をユーザ定義イベント処理関数 vProcessEvCore に伝達する例です。 

```c
cbToCoNet_vTxEvent(uint8 u8CbId, uint8 u8Stat) {
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, vProcessEvCore);
}
```

{% hint style="warning" %}
ToCoNet_Event_Process() を用いずに vProcessEvCore() を呼び出すことは避けるべきです。[フロー図](../../twelite-net-api-expl/twenet-fur/yzaibentofur.md)に示した状態遷移が行われず、また、ユーザ定義イベント処理関数の管理構造体に含まれる情報に矛盾が発生します。

割り込みハンドラから呼び出してはいけません。
{% endhint %}



### 状態遷移について

ユーザ定義イベント処理関数のステートマシンは、[E_STATE_IDLE](broken-reference)_ _(0) から開始します。

ユーザ定義イベント処理関数が呼び出される際に、ToCoNet_Event_SetState() 関数を呼び出すことで状態遷移を宣言します。状態遷移を宣言したのちユーザ定義イベント処理関数を脱出すると、[E_EVENT_NEW_STATE](ibento.md) イベントをパラメータにもう一度関数が呼び出されます。状態遷移が続く限りこの処理は連続します（[フロー図](../../twelite-net-api-expl/twenet-fur/yzaibentofur.md)）。

{% hint style="info" %}
スリープ(RAM保持)前に [ToCoNet_Event_vKeepStateOnRamHoldSleep](toconet_event-api/toconet_event_vkeepstateonramholdsleep.md)() を呼び出しておくことで直前の状態を保持でき、スリープ復帰後に [E_EVENT_START_UP](ibento.md) イベントを受け取ることができます。
{% endhint %}

