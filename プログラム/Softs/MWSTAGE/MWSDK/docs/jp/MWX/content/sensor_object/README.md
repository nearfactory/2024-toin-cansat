---
title: "センサー・デバイス (SNS)"
author: "Mono Wireless Inc."
description: "センサー・デバイス (SNS) ビヘイビア"
---

# センサー・デバイス (SNS)

センサーや各種デバイスの手続きを定型化したクラスを用意しています。

{% hint style="info" %}
TWELITE PAL 搭載のセンサーやデバイスを利用する場合は、[ボード(BRD)](../boards/)の記載も参照してください。TWELITE PAL 特有の手続きが必要な場合があり、ボード定義に含まれています。
{% endhint %}

## センサー取り扱いのための手続き

温度センサーなど、センサー稼働開始→待ち時間→センサー値の読み出しといった手続きが共通のものもあります。

I2Cセンサーの取り扱い前に`Wire.begin()`を実施しておいてください。スリープ復帰後は、Wireの再初期化は自動で行われるため特別な記述は必要ありません（注：ユーザコード上から明示的に `Wire.end()` を呼び出した場合は、再初期化を `wakeup()` に記述します）

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_AMB>();
  ..
  Wire.begin();
	brd.sns_SHTC3.begin();
	brd.sns_LTR308ALS.begin();
}
```



読み出し開始後の手続きはセンサーの種類ごとに違いますが例えば`<PAL_AMB>`のセンサーは２つとも時間経過を管理します。時間経過をセンサーオブジェクトに伝えるには `process_ev()` メソッドを用います。

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_AMB>();

	// mostly process every ms.
	if (TickTimer.available()) {
		//  wait until sensor capture finish
		if (!brd.sns_LTR308ALS.available()) {
			brd.sns_LTR308ALS.process_ev(E_EVENT_TICK_TIMER);
		}

		if (!brd.sns_SHTC3.available()) {
			brd.sns_SHTC3.process_ev(E_EVENT_TICK_TIMER);
		}
..
```

上記の例では1msおきのTickTimerを起点にして時間経過を伝えています。`E_EVENT_TICK_TIMER`はセンサーオブジェクトに1msの経過を伝えるものです。

スリープ復帰などで十分な時間が経過したときは替わりに`E_EVENT_START_UP`を渡します。センサーオブジェクトは速やかに読み出し可能として処理されます。

{% hint style="warning" %}
いずれの処理も現実世界の時間の経過と一致することを保証しているわけではありません。実際の経過時間が不足している場合は、センサーがエラーを返したり、期待しない値を返すことになります。
{% endhint %}



## センサー共通メソッド

### setup()

```cpp
void setup(uint32_t arg1 = 0, uint32_t arg2 = 0) 
```

センサーの初期化を行います。



### begin(), end()

```cpp
void begin(uint32_t arg1 = 0, uint32_t arg2 = 0)
void end()
```

センサーの取得を開始, 終了する。



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

待ち時間処理のセンサーの場合は`arg1`に`E_EVENT_TICK_TIMER`または`E_EVENT_START_UP`を与え時間の経過を知らせます。このメソッド呼出し後に、必要な時間が経過していればavailableになり、センサー値の読み出しが可能になります。



### available()

```cpp
bool available()
```

センサーが読み出し条件を満足したときに`true`を返します。



### probe()

```cpp
bool probe()
```

（対応しているセンサーのみ）センサーが接続されているときに`true`を返します。

{% hint style="warning" %}
`probe()`直後の初回の通信が失敗することがある。
{% endhint %}
