---
title: "TwePacketIO"
author: "Mono Wireless Inc."
description: "TwePacketIO"
---
# TwePacketIO

`TwePacketAppIO`クラスは、標準アプリ[App\_IOのシリアルメッセージ(0x81)](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_io)を解釈したものです。

```cpp
class TwePacketAppIO : public TwePacket, public DataAppIO { ... };
```

パケットデータ内の諸情報は`parse<TwePacketIO>()`実行後に`DataTwelite`に格納されます。



### DataAppIO構造体

```cpp
struct DataAppIO {
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

		// DIの状態ビットマップ (LSBから順にDI1,2,3,4,...)
		uint8_t DI_mask;

		// DIのアクティブ(使用なら1)ビットマップ(LSBから順にDI1,2,3,4,...)
		uint8_t DI_active_mask;
		
		// DIが割り込み由来かどうかのビットマップ(LSBから順にDI1,2,3,4,...)
		uint16_t DI_int_mask;
};
```
