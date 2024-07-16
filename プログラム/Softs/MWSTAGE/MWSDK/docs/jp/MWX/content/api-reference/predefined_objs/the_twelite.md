---
title: "the_twelite"
author: "Mono Wireless Inc."
description: "TWENET 利用の中核クラス (mwx::twenet)"
---

# the\_twelite

`the_twelite`オブジェクトは、TWENETの利用手続きをまとめたもので、無線の基本設定やスリープ等の手続きなど無線マイコンを操作するための手続きが含まれます。



## 概要

`the_twelite`は`setup()`関数内で設定と開始`the_twelite.begin()`を行います。`setup()`以外では設定は行えません。

```cpp
void setup() {
  ...
 	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();  
  ...
  the_twelite.begin();
}
```

上記の例では、アプリケーションIDの設定、通信チャネルの設定、受信回路の設定を行っています。



様々な手続きが含まれます。

```cpp
// シリアル番号を得る
uint32_t u32hwser = the_twelite.get_hw_serial();

// チャネルを 11 に設定する
the_twelite.change_channel(11);

// １秒のスリープを行う
the_twelite.sleep(1000);

// リセットを行う
the_twelite.reset_system();
```



また無線ネットワークを取り扱うクラスやボード対応をまとめたクラス、ユーザ記述のイベントドリブン処理を行うクラスを登録できるようになっています。このクラスを登録することにより、専用化した機能を手軽に利用できるようになります。これらのクラスを本解説中では「[**ビヘイビア**](../behavior/)」と呼称します。

```cpp
void setup() {
	/*** SETUP section */
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_AMB>();
	
	...
	
	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID);

```

上記の例では環境センサーパル`<PAL_AMB>`と、シンプル中継ネットワーク`<NWK_SIMPLE>`の２種類を登録しています。これらを登録することにより環境センサーパル上のセンサーなどハードウェアを簡易に取り扱うことが出来ます。また煩雑な無線パケットの取り扱いについて中継の処理や重複で届いたパケットの自動破棄などの機能を暗黙に持たせることが出来ます。



## メソッド

{% hint style="warning" %}
MWXライブラリには、ここで紹介したメソッド以外にも定義されています。

アクト記述には直接関係ないもの、設定しても有効に機能しないもの、内部的に使用されているものが含まれます。
{% endhint %}



### `<<`演算子 (設定)

オブジェクト`the_twelite`の初期設定を行うために`<<`演算子を用います。

以下に挙げる設定用のクラスオブジェクトを入力とし、設定をしなければデフォルト値が適用されます。

#### TWENET::appid(uint32\_t id)

パラメータ`id`に指定したアプリケーションIDを設定します。これは必須指定です。

設定の読み出しは `uint32_t the_twelite.get_appid()` で行います。

#### TWENET::channel(uint8\_t ch)

パラメータ`ch`に指定したチャネル番号（`11`..`26`)を設定します。

設定の読み出しは`uint8_t the_twelite.get_channel()`で行います。

#### TWENET::tx\_power(uint8\_t pw)

パラメータ`pw`に指定した出力設定を(`0`..`3`)を設定します。デフォルトは(3:出力減衰無し)です。

設定値の読み出しは`uint8_t the_twelite.get_tx_power()`で行います。

#### TWENET::rx\_when\_idle(uint8\_t bEnable)

パラメータ`bEnable`が`1`であれば常に受信回路を動作させ、他からの無線パケットを受信できるようになります。デフォルトは`0`で、もっぱら送信専用となります。

設定値の読み出しは`uint8_t the_twelite.get_rx_when_idle()`で行います。

#### TWENET::chmgr(uint8\_t ch1 = 18, uint8\_t ch2 = 0, uint8\_t ch3 = 0)

チャネルマネージャを有効にします。チャネルを複数指定すると複数チャネルでの送受信を行います。`ch2`,`ch3`に0を指定すると、その指定は無効になります。

#### STG\_STD

[インタラクティブモードの設定値](../../settings/stg\_std.md#no-1)を反映します。

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
set.reload();       // 設定値をロード
the_twelite << set; // インタラクティブモードの設定値を反映
```

反映される項目は以下です。

* **app\_id**
* **channel**
* **tx\_power**
* MAC ack 使用時の再送回数



{% hint style="warning" %}
MWXライブラリコード中には他にも設定項目がありますが、現時点ではライブラリの機能に無関係な設定であったり、設定を行うと矛盾を起こす可能性があるものです。
{% endhint %}



### begin()

```cpp
void begin()
```

事前に設定（`<<`演算子参照)や、ビヘイビアの登録を済ませた後に実行します。通常は`setup()`関数内の一番最後に記述します。

* `the_twelite` 設定完了
* ビヘイビアの初期化

{% hint style="warning" %}
TWENETの初期化は `setup()` 関数が終了した後にも実行されます。多くの処理は`setup()`が終了した後に実行するようになっているため、ここでは初期化以外の処理を行わないようにしてください。
{% endhint %}

#### 例

```cpp
void setup() {
	// use PAL_AMB board support.
	auto&& brd = the_twelite.board.use<PAL_AMB>();
	
	// settings
 	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();  
	
	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk << NWK_SIMPLE::logical_id(u8ID);
	
	// somo others
	
	// begin the TWENET!
	the_twelite.begin();
}
```



### change\_channel()

```cpp
inline bool change_channel(uint8_t u8Channel)
```

チャネル設定を変更します。失敗時にはチャネルは変更されず`false`を戻します。



### get\_channel\_phys()

```cpp
uint8_t get_channel_phys()
```

現在設定中のチャネル番号(11..26)を取得する。MAC層のAPIより取得します。



### get\_hw\_serial()

```cpp
inline uint32_t get_hw_serial()
```

モジュールのシリアル番号を取得します。



### sleep()

```cpp
inline void sleep(
        uint32_t u32Periodms, 
        bool_t bPeriodic = true, 
        bool_t bRamOff = false, 
        uint8_t u8Device = TWENET::SLEEP_WAKETIMER_PRIMARY)
```

モジュールをスリープさせる。

| パラメータ         | 解説                                                                                                                                         |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------ |
| `u32Periodms` | スリープ時間\[ms]                                                                                                                                |
| `bPeriodic`   | 前回の起床時間をもとに次の起床時間を再計算する。<br>※次の起床タイミングが迫っているなどの理由で、現在のタイミングからになる場合もあります。                                                            |
| `bRamoff`     | `true`に設定すると、RAMを保持しないスリープになる（起床後は`wakeup()`ではなく`setup()`から再初期化する必要がある)                                                                    |
| `u8Device`    | スリープに用いるウェイクアップタイマーの指定。`TWENET::SLEEP_WAKETIMER_PRIMARY`または `TWENET::SLEEP_WAKETIMER_SECONDARY`を指定する。  |

{% hint style="info" %}
スリープ前に組み込みオブジェクトやビヘイビアの `on_sleep()` メソッドが呼び出され、スリープ前の手続きを行います。スリープ復帰後は反対に `on_wakeup()` メソッドにより復帰処理が行われます。
{% endhint %}



### is\_wokeup\_by\_dio()

```cpp
bool is_wokeup_by_dio(uint8_t port)
```

スリープからの復帰要因が指定したディジタルピンである場合に`true`を返します。



### is\_wokeup\_by\_wktimer()

```cpp
bool is_wokeup_by_wktimer()
```

スリープからの復帰要因がウェイクアップタイマーである場合に`true`を返します。



### reset\_system()

```cpp
inline void reset_system()
```

システムをリセットします。リセット後は`setup()`からの処理となります。



### stop\_watchdog()

```cpp
inline void stop_watchdog()
```

ウォッチドッグタイマーを停止します。長時間のポーリング待ちを行うような場合はタイマーを停止します。

{% hint style="info" %}
ウォッチドッグタイマーはライブラリ内部のメインループで都度再開（restart）しています。タイマーが切れリセットがかかるまで約４秒です。
{% endhint %}



### restart\_watchdog()

```cpp
inline void restart_watchdog()
```

ウォッチドッグタイマーを再開します。



## ビヘイビア

`twe_twelite`には３つのビヘイビアを登録でき、これらを格納する以下のクラスオブジェクトを定義されています。

* `network` :  ネットワークを実装するビヘイビアです。通常は[`<NWK_SIMPLE>`](../../networks/nwk\_simple.md)を登録します。
* `network2` :  ネットワークを実装するビヘイビアです。最初に `network`でペイロードのデータ構造などの判定により受理しなかったパケットを、別のネットワーク ビヘイビアで処理させたい場合に使用します。([参考: NWK\_LAYERED と NWK\_SIMPLEの併用](../../networks/nwk_layered.md))
* `board`: ボード対応のビヘイビアです。ボード上の各デバイス利用手続きが付加されます。
* `app`: ユーザアプリケーションを記述したビヘイビアです。割り込みやイベント記述、ステートマシンによる状態遷移によるふるまいの記述が可能です。また複数のアプリケーション記述を定義しておいて、起動時に全く振る舞いの違うアプリケーションを選択する記述が容易に行えます。
* `settings`: 設定（インタラクティブモード）を実行するためのビヘイビアです。`<SET_STD>`を登録します。



### use\<B>()

```cpp
// 例
auto&& brd = the_twelite.board.use<PAL_AMB>();
```

ビヘイビア\<B>を登録します。登録は[`setup()`](../sys_callbacks/setup.md)内で行います。戻り値は登録したビヘイビアに対応するオブジェクトの参照です。

登録後は登録時と同じ書式でオブジェクトの取得を行います。

{% hint style="danger" %}
誤ったビヘイビアを指定した場合は、パニック動作（無限ループ）となりプログラムの動作が停止します。
{% endhint %}

{% hint style="info" %}
グローバル変数としてビヘイビアのオブジェクトを宣言することを想定していません。利用都度`use<B>()`を用いてください。

```cpp
void loop() {
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
}
```

ただし、グローバル変数にオブジェクトのポインタを定義して以下のように記述することは可能です。（MWXライブラリでは原則としてポインタ型の利用を最小限にとどめ参照型を利用する方針ですので、下記のような記述は推奨しません）

```cpp
NWK_SIMPLE *pNwk = nullptr;

setup() {
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	pNwk = &nwk;
}

void transmit() {
  if (auto&& pkt = pNwk->prepare_tx_packet()) {
    ...
  }
}
```
{% endhint %}



## クラスオブジェクト

`the_twelite`には上述の`board`, `network`, `app`の３つのクラスオブジェクトが定義されていますが他に以下が定義されています。

### tx\_status

送信完了状態を通知する。

{% hint style="info" %}
イベントドリブンのビヘイビアの記述では[transmit\_complete()コールバック](../behavior/#transmit\_complete)で管理します。
{% endhint %}



#### is\_complete()

```cpp
bool is_complete(uint8_t cbid)
```

指定したIDのパケットが送信完了したときに`true`を返す。



#### is\_success()

```cpp
bool is_success(uint8_t cbid)
```

指定したIDのパケットが送信完了し、かつ送信成功したときに`true`を返す。



### receiver

受信パケットを取得する。

{% hint style="warning" %}
`the_twelite.receiver`は推奨されません。

従来`loop()`内での記述を意図して `the_twelite.receiver` による処理を行っていましたが、キューによる遅延処理である点で原理的に取りこぼしが発生し、また記述も煩雑になりがちである点から `on_rx_packet()` を追加しました。
{% endhint %}

{% hint style="info" %}
* イベントドリブンのビヘイビアの記述では[receive()コールバック](../behavior/#receive)で取得します。
{% endhint %}

{% hint style="warning" %}
`read()`メソッドで得られる受信パケットデータは、続くパケットが受信処理時に上書きされる設計となっています。`available`直後に読み出してなにか短い処理をする場合は問題になることはありませんが、原則として読み出し→アプリケーションが使うため必要なデータのコピー→`loop()`の終了を速やかに行います。例えば`loop()`中で長い`delay()`を行うと受信パケットの取りこぼしなどが発生します。
{% endhint %}


#### available()

```cpp
bool available()
```

まだ読み出していない受信パケットが存在する場合に`true`を返す。



#### read()

```cpp
packet_rx& read()
```

パケットを読み出します。

