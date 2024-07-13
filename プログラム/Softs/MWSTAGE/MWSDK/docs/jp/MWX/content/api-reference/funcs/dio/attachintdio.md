---
title: "attachIntDio()"
author: "Mono Wireless Inc."
description: "attachIntDio()"
---

# attachIntDio()

DIO割り込みを有効にします。

```cpp
void attachIntDio(uint8_t u8pin, E_PIN_INT_MODE mode)
```

事前に入力設定したピンに対して、１番目のパラメータは割り込みを有効にしたいピン番号で、２番目は割り込み方向（[立ち上がり、立ち下がり](./#pinnochigariri)）を指定します。

{% hint style="info" %}
割り込みハンドラ、イベントハンドラの記述は[アプリケーションビヘイビア](../../behavior/)で行います。
{% endhint %}



## 例

DIO5のピンがHIGHからLOWに変化したときに割り込みが発生する設定を行う。

```cpp
void setup() {
  the_twelite.app.use<myAppClass>();
  
  pinMode(PIN_DIGITAL::DIO5, PIN_MODE::INPUT_PULLUP);
  attachIntDio(PIN_DIGITAL::DIO5, PIN_INT_MODE::FALLING);
}

void loop() {
  ;
}
```



#### myAppClass.hpp

```cpp
class myAppClass: public mwx::BrdPal, MWX_APPDEFS_CRTP(myAppClasslMot)
{

};
```

アプリケーションビヘイビア`myAppClass`の基本定義。詳細は省略している。

#### myAppClass.cpp

```cpp
/*****************************************************************/
// MUST DEFINE CLASS NAME HERE
#define __MWX_APP_CLASS_NAME myAppClass
#include "_mwx_cbs_cpphead.hpp"
/*****************************************************************/

MWX_DIO_INT(PIN_DIGITAL::DIO5, uint32_t arg, uint8_t& handled) {
  static uint8_t ct;
  digitalWrite(PIN_DIGITAL::DIO12, (++ct & 1) ? HIGH : LOW);
	handled = false; // if true, no further event.
}

MWX_DIO_EVENT(PIN_DIGITAL::DIO5, uint32_t arg) {
  Serial << '*';	
}

/*****************************************************************/
// common procedure (DO NOT REMOVE)
#include "_mwx_cbs_cpptail.cpp"
// MUST UNDEF CLASS NAME HERE
#undef __MWX_APP_CLASS_NAME
} // mwx
/*****************************************************************/

```

アプリケーションビヘイビア`myAppClass`の割り込みハンドラの記述。DIO5の割り込み発生時にDIO12の出力設定を反転させ、割り込みハンドラが終了してから発生するイベントではシリアルポート`Serial`に`*`を表示する。
