---
title: "TwePacketPAL"
author: "Mono Wireless Inc."
description: "TwePacketPAL"
---
# TwePacketPAL

`TwePacketPal`クラスは、TWELITE PALのパケットデータを解釈したものです。このクラスはTWELITE PAL（センサーデータなど上り方向）共通に取り扱います。

```cpp
class TwePacketPal : public TwePacket, public DataPal { ... };
```



PAL共通データは`DataPal`に定義されています。

PALの各センサー基板特有のデータを取り出すためのジェネレータ関数を用意しています。



### DataPal構造体

PALは接続されるセンサーなどによってパケットデータ構造が異なりますが、`DataPal`では共通部のデータ構造を保持します。

```cpp
struct DataPal {
	uint8_t u8lqi;        // LQI値

	uint32_t u32addr_rpt; // 中継器のアドレス

	uint32_t u32addr_src; // 送信元のアドレス
	uint8_t u8addr_src;   // 送信元の論理アドレス

	uint16_t u16seq;      // シーケンス番号

	E_PAL_PCB u8palpcb;		// PAL基板の種別
	uint8_t u8palpcb_rev;	// PAL基板のレビジョン
	uint8_t u8sensors;		// データに含まれるセンサーデータの数 (MSB=1はエラー)
	uint8_t u8snsdatalen; // センサーデータ長(バイト数)

	union {
		const uint8_t *au8snsdata; // センサーデータ部への参照
		uint8_t _pobj[MWX_PARSER_PKT_APPPAL_FIXED_BUF]; // 各センサーオブジェクト
	};
};
```

PALのパケットデータ構造は大まかに２つのブロックからなり、全てのPAL共通部と個別のデータ部になります。個別のデータ部は、パケットの解釈を行わずそのまま格納しています。取り扱いを単純化するため32バイトを超えるデータは動的に確保する`uptr_snsdata`に格納します。

個別のデータ部は、`PalBase`をベースクラスに持つ構造体に格納されます。この構造体は、`TwePacketPal`に定義されるジェネレータ関数により生成されます。

`parse<TwePacketPAL>()`実行時に`MWX_PARSER_PKT_APPPAL_FIXED_BUF`に収まるサイズであれば、センサー個別のオブジェクトを生成します。

収まらない場合は`au8snsdata`に解析時のバイト列の参照が保存されます。この場合、解析に用いたバイト列のデータが書き換えられた場合は、センサー個別のオブジェクトは生成できなくなります。



### PalBase

PALの各センサーのデータ構造体はすべて`PalBase`を継承します。センサーデータの格納状況`u32StoredMask`が含まれます。

```cpp
	struct PalBase {
		uint32_t u32StoredMask; // 内部的に利用されるデータ取得フラグ
	};
```

###

### PalEvent

PALイベントは、センサーなどの情報を直接送るのではなく、センサー情報を加工し一定の条件が成立したときに送信される情報です。例えば加速度センサーの静止状態から一定以上の加速度が検出された場合などです。

```cpp
	struct PalEvent {
		uint8_t b_stored;       // 格納されていたら true
		uint8_t u8event_source; // 予備
		uint8_t u8event_id;     // イベントID
		uint32_t u32event_param;// イベントパラメータ
	};
```

イベントデータが存在する場合は`TwePacketPal`の`.is_PalEvent()`が`true`になることで判定でき、`.get_PalEvent()`により`PalEvent`データ構造を得られます。



## ジェネレータ関数

センサーPALの各種データを取り出すためのジェネレータ関数です。

```cpp
void print_pal(pktparser& pkt) {
	auto&& pal = pkt.use<TwePacketPal>();
	if (pal.is_PalEvent()) {
		PalEvent obj = pal.get_PalEvent();
	} else
	switch(pal.u8palpcb) {
	case E_PAL_PCB::MAG:
	  {
		  // generate pal board specific data structure.
		  PalMag obj = pal.get_PalMag();
	  } break;
  case E_PAL_PCB::AMB:
	  {
		  // generate pal board specific data structure.
		  PalAmb obj = pal.get_PalAmb();
	  } break;
	  ...
	default: ;
	}
}
```

ジェネレータ関数を利用するには、まず`pkt`がイベントかどうか判定(`.is_PalEvent()`)します。イベントの場合は`get_PalEvent()`を持ちます。それ以外は`u8palpcb`に応じてオブジェクトを生成します。



### get\_PalMag()

```cpp
PalMag get_PalMag()

// MAG
struct PalMag : public PalBase {
    uint16_t u16Volt;         // モジュール電圧[mV]
    uint8_t u8MagStat;        // 磁気スイッチの状態 [0:磁石なし,1,2]
    uint8_t bRegularTransmit; // MSB flag of u8MagStat
};
```

`.u8palpcb==E_PAL_PCB::MAG`の場合、開閉センサーパルのデータ`PalMag`を取り出します。



### get\_PalAmb()

```cpp
PalAmb get_PalAmb()

// AMB
struct PalAmb : public PalBase {
    uint16_t u16Volt;       // モジュール電圧[mV]
    int16_t i16Temp;        // 温度(100倍値)
    uint16_t u16Humd;       // 湿度(100倍値)
    uint32_t u32Lumi;       // 照度(Lux相当)
};
```

`.u8palpcb==E_PAL_PCB::AMB`の場合、環境センサーパルのデータ`PalAmb`を取り出します。



### get\_PalMot()

```cpp
PalMot get_PalMot()

// MOT
struct PalMot : public PalBase {
    uint16_t u16Volt;  // モジュール電圧[mV]
    uint8_t u8samples; // サンプル数
    uint8_t u8sample_rate_code; // サンプルレート (0: 25Hz, 4:100Hz)
    int16_t i16X[16]; // X 軸
    int16_t i16Y[16]; // Y 軸
    int16_t i16Z[16]; // Z 軸
};
```

`.u8palpcb==E_PAL_PCB::MOT`の場合、動作センサーパルのデータ`PalMot`を取り出します。



### get\_PalEvent()

```cpp
PalEvent get_PalEvent()

// PAL event
struct PalEvent {
    uint8_t b_stored;        // trueならイベント情報あり
    uint8_t u8event_source;  // イベント源
    uint8_t u8event_id;      // イベントID
    uint32_t u32event_param; // 24bit、イベントパラメータ
};
```

`.is_PalEvent()`が`true`の場合`PalEvent`(PALイベント)を取り出します。

