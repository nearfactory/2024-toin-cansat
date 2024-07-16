---
title: "BMx280 - 環境センサー"
author: "Mono Wireless Inc."
description: "BMx280 - 環境センサー"
---

# BMx280 - 環境センサー

I2Cバスを利用した気圧・温度・湿度（BME280のみ）センサーです。

{% hint style="info" %}
本センサーはTWELITE PALシリーズでは使用されていません。利用例は以下を参照ください。\
[https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30](https://github.com/monowireless/ActEx\_Sns\_BME280\_SHT30)
{% endhint %}



## 処理の流れ

1. `Wire.begin()`: バスの初期化
2. `.setup()`: センサーの初期化
3. `.begin()`: センサーの動作開始
4. 時間待ち数ms
5. `.available()`が`true`になる
6. `.get_press(), .get_temp(), .get_humid()`: 値の読み出し



## 動作に必要な手続き

### Wireバス

`setup()`メソッド呼び出し前に`Wire.begin()`によりWireが動作状態にしておきます。



### スリープ復帰時の手続き

スリープ直前もWireバスが動作状態にしておきます（スリープ復帰後自動でWireを回復します）。



### コード例

```cpp
#include <TWELITE>
#include <SNS_BME280>

SNS_BME280 sns_bme280; // オブジェクトの宣言
```

`#include <SNS_SHT3X>`と`SNS_SHT3X`クラスオブジェクトの宣言が必要です。



#### 初期化

```cpp
void setup() {
    Wire.begin();
    sns_bme280.setup();
}
```



#### センサー値の取得開始

```cpp
void loop() {

  if(eState == E_STATE::INIT) {
    sns_bme280.begin();
    eState = E_STATE::CAPTURE;
  }

}
```

センサー値の取得開始には`.begin()`を呼び出します。完了まで数msかかります。

※ 上記 `loop()`内は状態変数eStateにより処理が分岐する設計とします。（[参考](../act\_samples/slp\_wk\_and\_tx.md)）



#### センサー値の取得待ち

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_bme280.available()) {
      // センサー値読み出し可能
    }
  }

}
```

センサー値が準備できたかどうかは`.available()`により判定できます。



#### センサー値の読み出し

```cpp
void loop() {

  if(eState == E_STATE::CAPTURE) {
    if (sns_bme280.available()) {
      Serial << crlf << "BMx280:"
            << " P=" << int(sns_bme280.get_press()) << "hP";
            << " T=" << sns_bme280.get_temp() << 'C'
						<< " H=" << sns_bme280.get_humid() << '%';
    }
  }

}
```

センサー値が準備出来次第、値を読み出すことが出来ます。



`.get_temp(), get_humid()`は浮動小数点演算が含まれます。100倍整数値を取得することもできます。

```cpp
auto temp = div100(sns_bme280.get_temp_cent());
auto humd = div100(sns_bme280.get_humid_per_dmil);

Serial << crlf << "BMx280:"
  << " P=" << int(sns_bme280.get_press()) << "hP";
  << format(" T=%c%d.%02d", temp.neg ? '-' : ' ', temp.quo, temp.rem)
  << format(" T=%c%d.%02d", humd.neg ? '-' : ' ', humd.quo, humd.rem);
```

ここでは[`div100()`](../api-reference/funcs/utility/div100.md)を用いて100倍値を整数部と小数部に分解しています。



## メソッド

### get\_press()

```cpp
int16_t get_press()
```

気圧を読み出します。単位はヘクトパスカル(_hectopascal_ )で、通常は1000前後の値を示します。



### get\_temp(), get\_temp\_cent()

```cpp
double get_temp()
int16_t get_temp_cent()
```

温度を読み出す。`get_temp()`は℃で、`get_temp_cent()`は℃の100倍の値を整数値で返します。

エラー時は-32760～-32768の値が返ります。



### get\_humid(), get\_humid\_per\_dmil()

```cpp
double get_humid()
int16_t get_humid_per_dmil()
```

湿度を読み出す。`get_humid()`は%で、`get_humid_per_dmil()`は%の100倍の値を整数値で返します。

エラー時は-32760～-32768の値が返ります。



## 共通メソッド

### setup()

```cpp
void setup(uint32_t arg1 = 0UL) 
```

センサー用のメモリ領域の確保や初期化を行います。

`arg1`のLSBから8bitには、I2Cアドレスを格納することが出来ます。指定しない場合は0としておきます。

```cpp
#include <SNS_BME280>
SNS_BME280 sns_bme280;
bool b_found_bme280 = false;

void setup() {
  ...
  sns_bme280.setup();
	if (!sns_bme280.probe()) {
			delayMicroseconds(100); // device needs small time for further I2C comm.
			sns_bme280.setup(0x77); // alternative ID
			if (sns_bme280.probe()) b_found_bme280 = true;
	} else {
			b_found_bme280 = true;
	}
	...
```

上記のコードではまずデフォルトのI2C IDでデバイスが応答するかを試し、応答が無ければ `0x77`で試みます。



### begin(), end()

```cpp
void begin()
void end()
```

センサーの取得を開始します。センサーの値を読み出すまで数ms必要で`available()`が`true`になるまで待つ必要があります。

`end()`には対応しません。



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

待ち時間処理のセンサーの場合は`arg1`に`E_EVENT_TICK_TIMER`または`E_EVENT_START_UP`を与え時間の経過を知らせます。このメソッド呼出し後に、必要な時間が経過していれば`available()`が`true`になり、センサー値の読み出しが可能になります。



### available()

```cpp
bool available()
```

センサーが読み出し条件を満足したときに`true`を返します。



### probe()

```cpp
bool probe()
```

センサーが接続されているときに`true`を返します。



### sns\_stat()

```cpp
uint32_t sns_stat()
```

センサーデバイスの諸情報が格納されます。

* 下位8bitにはBME280/BMP280のチップモデルが格納されます。`0x60`ならBME280, `0x58`ならBMP280となります。



### sns\_opt()

```cpp
uint32_t& sns_opt()
```

`setup(uint32_t arg1)`で渡した値が格納されています。

* 下位8bitには指定したI2Cデバイスのアドレスが格納されます。





