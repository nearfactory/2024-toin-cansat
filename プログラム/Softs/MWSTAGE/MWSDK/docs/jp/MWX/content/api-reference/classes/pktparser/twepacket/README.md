---
title: "TwePacket"
author: "Mono Wireless Inc."
description: "TwePacket"
---
# TwePacket

パケット型の基底クラスですが、メンバー構造体`common`にはアドレス情報など共通情報が含まれます。

```cpp
class TwePacket {
	public:
		static const E_PKT _pkt_id = E_PKT::PKT_ERROR;
		
		struct {
			uint32_t tick;     // 解釈実行時のシステム時刻[ms]
			uint32_t src_addr; // 送信元アドレス(シリアル番号)
			uint8_t src_lid;   // 送信元アドレス(論理アドレス)
			uint8_t lqi;       // LQI
			uint16_t volt;     // 電圧[mV]
		} common;
};
```

{% hint style="info" %}
種別を混在してpktparser型として配列等に格納するような場合に、アドレス情報などを最小限の情報を取得したい場合に使用します。
{% endhint %}



