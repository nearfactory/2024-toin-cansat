---
title: "SPI"
author: "Mono Wireless Inc."
description: "SPIバス (Controller) の読み書き"
---
# SPI

SPIバス (Controller) の読み書きを行います。

## 注意事項



## 定数

| 定数                                                                    | 意味               |
| --------------------------------------------------------------------- | ---------------- |
| `const uint8_t`<br>`SPI_CONF::MSBFIRST`  | MSB を先頭ビットにする    |
| `const uint8_t`<br>`SPI_CONF::LSBFIRST`  | LSB を先頭ビットにする    |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE0` | SPI MODE 0 に設定する |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE1` | SPI MODE 1 に設定する |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE2` | SPI MODE 2 に設定する |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE3` | SPI MODE 3 に設定する |

## 初期化と終了

SPIバスの利用手続きは`begin()`メソッドによります。

### begin()

```cpp
void begin(uint8_t slave_select, SPISettings settings)
SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
```

ハードウェアの初期化を行います。

{% hint style="warning" %}
スリープ復帰後にも本処理が必須です。
{% endhint %}

| パラメータ          | 解説                                                                                                                                                                                                                                                                                                       |
| -------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `slave_select` | 使用するSPIスレーブのセレクトピンを指定する。`0 : DIO19``1 : DIO0 (DIO 19 は予約されます)``2 : DIO1 (DIO 0,19 は予約されます)`                                                                                                                                                 |
| `settings`     | SPIのバス設定を指定します。<br><br>`clock`[hz]でSPIバスの周波数を指定します。指定した周波数に近いディバイザが選択されます。16Mhzまたは16Mhzを偶数で割った値になります。<br>`bitOrder`は`SPI_CONF::MSBFIRST`か`SPI_CONF::LSBFIRST`を指定します。<br>`dataMode`は`SPI_CONF::SPIMODE0..3`を指定します。 |

###

#### 例

```cpp
void setup() {
  ...
  SPI.begin(0, SPISettings(2000000, SPI_CONF::MSBFIRST, SPI_CONF::SPI_MODE3));
  ...
}

void wakeip() {
  ...
  SPI.begin(0, SPISettings(2000000, SPI_CONF::MSBFIRST, SPI_CONF::SPI_MODE3));
  ...
}
```



### end()

```cpp
void end()
```

SPIのハードウェアの利用を終了します。



## 読み書き

読み書きの手続きは、以下の2種類あります。いずれかを選択して利用します。

* [メンバ関数版 (以下のメンバ関数を用いた入出力)](spi-member.md)\
  `beginTransaction(), endTransaction(), transfer(), transfer16(), transfer32()`
* [ヘルパークラス版(stream機能が使用可能)](spi-helperclass.md)\
  `transceiver`

####

