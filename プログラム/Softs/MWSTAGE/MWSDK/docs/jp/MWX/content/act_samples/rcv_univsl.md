---
title: "Rcv_Unvsl (ユニバーサル レシーバー)"
author: "Mono Wireless Inc."
description: "Rcv_Unvsl (ユニバーサル レシーバー)"
---

# Rcv_Unvsl (ユニバーサル レシーバー)

MWXライブラリの `twe_twelite.network` に `NWK_LAYERED`、 `twe_twelite.network2` に `NWK_SIMPLE`を動作させることで、レイヤーツリーネット (TWELITE PAL, ARIAなど)のパケットを含む、さまざまな種類のパケットを受信、解釈できます。

ただし無線パケットは、同一チャネルであることと、同一アプリケーションIDであることが条件です。



## main.cpp

`setup()`, `loop()`、受信パケットのコールバック関数 `on_rx_packet()`を記述しています。



### setup()

これらのオブジェクトは `pkt_handler.cpp` で宣言され `setup()`中で`pnew()`により初期化されます。主にパケットのペイロード（データ）を解釈します。

```cpp
	mwx::pnew(g_pkt_pal);
	mwx::pnew(g_pkt_apptwelite);
	mwx::pnew(g_pkt_actsamples);
	mwx::pnew(g_pkt_unknown);
```



２つのネットワークオブジェクトを生成しています。必ず `the_twelite.network`に`NWK_LAYERED`にします。

```cpp
	auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
	auto&& nwk_sm = the_twelite.network2.use<NWK_SIMPLE>();
```



### loop()

このサンプルでは`loop()`の処理で重要なのは約1秒おきに行っている `.refresh()` 処理です。`g_pkt_apptwelite().refresh()`のみ重複チェッカのタイムアウト処理を行っています。それ以外のオブジェクトは何もしません。

```cpp
	if (TickTimer.available()) {
		static unsigned t;
		if (!(++t & 0x3FF)) {
			g_pkt_pal.refresh();
			g_pkt_apptwelite.refresh();
			g_pkt_actsamples.refresh();
			g_pkt_unknown.refresh();
		}
	}
```



### on_rx_packet()

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	auto type = rx.get_network_type();
	bool b_handled = false;

	// PAL
	if (!b_handled
		&& type == mwx::NETWORK::LAYERED
		&& g_pkt_pal.analyze(rx, b_handled)
	) {
		g_pkt_pal.display(rx);
	}

	// Act samples
	if (!b_handled
		&& type == mwx::NETWORK::SIMPLE
		&& g_pkt_actsamples.analyze(rx, b_handled)
	) {
		g_pkt_actsamples.display(rx);
	}

	// Standard application (e.g. App_Twelite)
	if (!b_handled
		&& type == mwx::NETWORK::NONE
		&& g_pkt_apptwelite.analyze(rx, b_handled)
	) {
		g_pkt_apptwelite.display(rx);
	}

	// unknown
	if (!b_handled) {
		g_pkt_unknown.analyze(rx, b_handled);
		g_pkt_unknown.display(rx);
	}
}
```

このサンプルコードで最も重要な部分です。`auto type = rx.get_network_type();`によりパケットの種別を判定しています。

* `mwx::NETWORK::LAYERED` : `NWK_LAYERED` レイヤーツリーネットのケット
* `mwx::NETWORK::SIMPLE` : `NWK_SIMPLE`のパケット
* `mwx::NETWORK::NONE` : ネットワークなし（App\_Tweliteなど)
* その他 : エラーまたは未対応のパケット



`mwx::NETWORK::NONE` の場合は、再送などで複数送信されうる同一パケットの重複チェッカ等の処理は MWX ライブラリ内部で行われません。これらの対応を記述する必要があります。本サンプルでは `dup_checker.hpp`, `dup_checker.cpp` を用意しています。



パケットの解釈は`tsRxDataApp*`をラップした `packet_rx&` オブジェクトを参照します。`packet_rx`クラス自体は特別な機能はなく、`get_psRxDataApp()`を用いて`tsRxDataApp*`から得られる一部の情報へのアクセス手段を定義しているのみです。



## pkt_common.hpp

パケット解釈部分のインタフェースを統一する目的で定義しています。

```cpp
template <class D>
struct pkt_handler {
	D& self() { return static_cast<D&>(*this); }
	bool analyze(packet_rx& rx, bool &b_handled) {
		return self().pkt.analyze(rx, b_handled);
	}
	void display(packet_rx& rx) {
		Serial
			<< crlf
			<< format("!PKT_%s(%03d-%08x/S=%d/L=%03d/V=%04d)"
					, self().get_label_packet_type()
					, self().pkt.data.u8addr_src
					, self().pkt.data.u32addr_src
					, rx.get_psRxDataApp()->u8Seq
					, rx.get_lqi()
					, self().pkt.data.u16volt
					);

		self().disp_detail(rx);
	}
	void refresh() {
		self()._refresh();
	}
};

// packet analyzer for App_Twelite
class pkt_handler_apptwelite : public pkt_handler<pkt_handler_apptwelite> {
	friend class pkt_handler<pkt_handler_apptwelite>;
	pkt_apptwelite pkt;
	void disp_detail(packet_rx& rx);
	const char* get_label_packet_type() { return "AppTwelite"; }
	void _refresh() { pkt.refresh(); }
public:
	pkt_handler_apptwelite() : pkt() {}
};
```



* `analyze()` : パケットのペイロードを解釈する。
* `display()` : パケット情報を表示する。
* `refresh()` : 1秒おきの処理を記述します。
* `self()` : 派生クラス`D`にキャストします。



さらにパケット解釈クラス(上記例 `pkt_handler_apptwelite`)には、メンバーオブジェクトの `pkt` が含まれます。実際のパケットの解釈部分は、`pkt_???.cpp` にある各々の `analyze()` の実装を参照してください。



## pkt\_???.hpp, pkt\_???.cpp

パケット種別ごとのパケット解釈部 `analyze()` と、データ構造 `data` が定義されています。メンバー`data`は、構造体ですが`PktDataCommon`の共通構造体を継承しています。この共通部を用いてパケットのデータのシリアル出力のコードを簡潔に記述しています。



### pkt_pal

PAL関連のパケットに対応します。PALのパケット構造は複雑なデータ構造を持っています。ここでは  EASTL のコンテナを用いた実装を行っています。

* `_vect_pal_sensors` : `_pal_sensor`オブジェクトのプール。このオブジェクトは instusive map で使用するための専用クラスです。
* `_map_pal_sensors` : センサーデータを効率よく検索するための intrusive map 構造。

１パケット中の複数データに対して各々が追加されるたびに`_vect_pal_sensors` にエントリを確保して値を格納します。パケット中のすべてのデータを解釈した時点でセンサータイプをキーとした`_map_pal_sensors`を構築します。



## dup_checker

重複チェッカーを実装します。チェッカーの動作はテンプレート引数によってカスタマイズできます。



### テンプレート引数

* `MODE` : `MODE_REJECT_SAME_SEQ`を指定すると、同じシーケンス番号のパケットを除外します。パケット順が並び変わるような場合に使用します。`MODE_REJECT_OLDER_SEQ`はより新しい番号を採用します。
* `TIMEOUT_ms` : 重複データベースの初期化を行う間隔です。`1000`と指定すると1秒経過したデータは抹消されます。直前では除外されていたパケットも、重複データベースの初期化されると再び採用されることになります。
* `N_ENTRIES` : データ構造に最大確保される要素数です。
* `N_BUCKET_HASH` : ハッシュ値の最大数です。素数を指定します。受信される無線ノードの種類をもとに決めます。



### コンテナ

* `_mmap_entries` : intrusive ハッシュ マルチ マップ構造です。検索キーは無線ノードのシリアル番号です。
* `_vect_pool` : マップ構造で用いられる要素を固定数(`N_ENTRIES`)を確保します。
* `_ring_vecant_idx` : `_mmap_entries` に利用されていない`_vect_pool` の要素を配列インデックス番号で管理します。リングバッファの構造で、要素を追加するときはリングバッファから値を一つ取り出し、削除するときはリングバッファに値を返します。



### 重複チェック

```cpp
	bool check_dup(uint32_t u32ser, uint16_t u16val, uint32_t u32_timestamp) {
		// find entry by key:u32ser.
		auto r = _mmap_entries.equal_range(u32ser);

        ...
    }
```

マルチマップ構造からデータを検索するには `.equal_range()` を呼び出します。得られた `r` はイテレータで、同一のシリアル番号の要素を列挙します。

各要素(`_dup_checker_entry`)にはタイムスタンプやシーケンス番号が記録されています。この値に従い重複を確認します。







