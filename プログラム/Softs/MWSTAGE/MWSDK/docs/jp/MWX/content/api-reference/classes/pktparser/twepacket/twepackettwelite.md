---
title: "TwePacketTwelite"
author: "Mono Wireless Inc."
description: "TwePacketTwelite"
---
# TwePacketTwelite

`TwePacketTwelite`クラスは、標準アプリ[App\_Tweliteの0x81コマンド](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_twelite)を解釈したものです。

```cpp
class TwePacketTwelite : public TwePacket, public DataTwelite { ... };
```

パケットデータ内の諸情報は`parse<TwePacketTwelite>()`実行後にパケット情報が`DataTwelite`に格納されます。



### DataTwelite構造体

```cpp
struct DataTwelite {
		//送信元のシリアル#
		uint32_t u32addr_src;
		
		// 送信元の論理ID
		uint8_t u8addr_src;

		// 宛先の論理ID
		uint8_t u8addr_dst;

		// 送信時のタイムスタンプ
		uint16_t u16timestamp;

		// 低レイテンシ送信時のフラグ
		bool b_lowlatency_tx;

		// リピート中継回数
		uint16_t u8rpt_cnt;

		// LQI値
		uint16_t u8lqi;

		// DIの状態 (true がアクティブ Lo,GND)
		bool DI1, DI2, DI3, DI4;
		// DIの状態ビットマップ (LSBから順にDI1,2,3,4)
		uint8_t DI_mask;

		// DIアクティブならtrue (過去にアクティブになったことがある)
		bool DI1_active, DI2_active, DI3_active, DI4_active;
		// DIのアクティブビットマップ(LSBから順にDI1,2,3,4)
		uint8_t DI_active_mask;

		// モジュールの電源電圧[mV]
		uint16_t u16Volt;

		// AD値 [mV]
		uint16_t u16Adc1, u16Adc2, u16Adc3, u16Adc4;
		// ADがアクティブ（有効）なら 1 になるビットマップ (LSBから順にAD1,2,3,4)
		uint8_t Adc_active_mask;
};
```

