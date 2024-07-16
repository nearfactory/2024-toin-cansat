---
title: サンプルアクト
author: "Mono Wireless Inc."
description: Sample Acts
---

# サンプルアクト

アクトの動作を理解するため、いくつかのサンプルを用意しています。

{% hint style="info" %}
**サンプルは MWSDK をインストールしたフォルダにある `Act_samples` にあります。**
{% endhint %}

## サンプルアクトの紹介

以下には、目的別のアクトを紹介します。

### 無線通信を行わなず、マイコン機能のみの短いアクト

* [act0..4](act\_opening.md)は、無線機能などを使わないごくシンプルな例です。Actの基本構造が理解できます。

### I2C センサーを用いたアクトの記述例

I2Cセンサーを接続し、スリープによる簡潔動作を行いながら無線パケットを送信する、無線センサーの実装例です。比較的簡潔、かつ、代表的な構造ですので、`act0`～`act4`を確認後に参照することを推奨します。

* [BRD\_I2C_TEMPHUMID](brd\_i2c\_temphumid.md)

TWELITE で無線センサーを実装するための代表的な要素（[シンプル中継ネット `<NWK_SIMPLE>`](../networks/nwk_simple.md) の利用・[インタラクティブモード `<STG_STD>`](../settings/stg_std.md)、[I2Cセンサーの取り扱い `Wire`](../api-reference/predefined_objs/wire/README.md)、スリープによる間欠動作など）が含まれます。


### 無線通信を行う基本的なアクト

無線パケットを送信、または送受信するサンプルですが、各々少し違った視点で実装されています。

* [Scratch](scratch.md) は、UARTから１バイトコマンドを受けて、送信などを行うシンプルなコードです。
* [Slp\_Wk\_and\_Tx](slp\_wk\_and\_tx.md)は、ステートマシンを用い、スリープを用いた間欠動作で、スリープ復帰→無線送信→スリープを繰り返します。
* [PingPong](pingpong.md)は、一方から他方にパケットを送信し、受信した他方がパケットを送り返すサンプルです。送信と受信についての基本的な手続きが含まれます。
* [WirelessUART](wirelessuart.md)は、UART入力を[serparser](../api-reference/classes/ser\_parser.md)を用いアスキー形式を解釈し、これを送信します。


### 親機側のアクト

{% hint style="info" %}
注：このサンプルに含まれるアクトの無線パケットの受信には [App_Wings](https://wings.twelite.info/) を利用することもできます。
{% endhint %}

独自の受信側親機アプリケーションを実装するときに参照してください。

*  [Parent\_MONOSTICK](parent\_monostick.md)は、専ら受信のみを行い、シリアルポートへ受信結果を出力します。このサンプルの無線パケットで、親機向け(0x00)や子機ブロードキャスト(0xFE)とアドレス設定しているものは受信できます。またインタラクティブモード[\<STG\_STD>](../settings/stg\_std.md)をActに追加するための手続きが含まれます。
* [Rcv_Univsl](rcv_univsl.md) はユニバーサルパケットレシーバ (TWENETレイヤーツリーネットワーク, App_Twelite, Act, ... など) のサンプルコードです。また、コンテナやアルゴリズムにEASTLライブラリを使用しています。


### インタラクティブモードを追加するためのアクト

インタラクティブモードを使用するアクトの解説には大まかな流れを記しています（ここでは上述の BRD\_I2C_TEMPHUMID を引用します）。どのサンプルの解説も大きな差はありません。

* [BRD\_I2C_TEMPHUMID](brd\_i2c\_temphumid.md)は、I2Cセンサーデバイスの読み書きコマンドを実行し I2C センサーから得られた計測値を無線送信します。またインタラクティブモード[\<STG\_STD>](../settings/stg\_std.md)をActに追加するための手続きが含まれます。
* Settings は、インタラクティブモード[\<STG\_STD>](../settings/stg\_std.md)のより高度なカスタマイズを行います。詳細はコードを参照ください。

### センサーなどのデバイスを動作させるためのアクト

内蔵ペリフェラルや外部センサーデバイスからセンサー情報を得るサンプルです。

* [BRD\_APPTWELITE](brd\_apptwelite.md)はディジタル入力、アナログ入力、ディジタル出力、アナログ出力を用いた双方向通信を行っています。またインタラクティブモード[\<STG\_STD>](../settings/stg\_std.md)をActに追加するための手続きが含まれます。
* [BRD\_I2C_TEMPHUMID](brd\_i2c\_temphumid.md)は、I2Cセンサーデバイスの読み書きコマンドを実行し I2C センサーから得られた計測値を無線送信します。またインタラクティブモード[\<STG\_STD>](../settings/stg\_std.md)をActに追加するための手続きが含まれます。
* [PulseCounter](pulsecounter.md) は、パルスカウンター機能を用い、スリープ中も含め入力ポートで検出したパルス数を計数し、これを無線送信します。
* [PAL\_AMB\_behavior](pal\_amb-behavior.md) は、ビヘイビアを用いた例です。PAL\_AMBでは温湿度センサーはライブラリ内部のコードが呼ばれますが、このサンプルでは温湿度センサーのアクセスのための独自の手続きも含まれます。


### TWELITE PAL を使用するためのアクト

TWELITE PAL には標準的なPALアプリが書き込まれていますが、PALアプリを用いずアクトによる記述を行うことができます。MWXライブラリには、PALで使用するセンサーを動作させるための標準的な手続きが用意されています。

各種PAL基板用のサンプルです。PAL基板上のセンサー値を取得し、送信し、スリープします。

* [PAL\_AMB](pal\_amb.md) 
* [PAL\_MOT-single](pal\_mot-oneshot.md)
* [PAL\_MAG](pal\_mag.md)

以下は応用例で、上記のアクトより少し複雑な記述になっています。
* [PAL\_AMB\_usenap](pal\_amb-usenap.md) は、数十msかかるセンサーの動作時間にTWELITEマイコンを短くスリープさせ、より省電力を目指すサンプルです。
* [PAL\_AMB\_behavior](pal\_amb-behavior.md) は、ビヘイビアを用いた例です。PAL\_AMBでは温湿度センサーはライブラリ内部のコードが呼ばれますが、このサンプルでは温湿度センサーのアクセスのための独自の手続きも含まれます。
* [PAL\_MOT\_fifo](pal\_mot.md) は、加速度センサーのFIFOおよびFIFOの割り込みを用いて、サンプルを中断することなく、連続的に取得し無線送信するためのサンプルです。

### TWELITE CUE を使用するためのアクト

PAL\_MOT 用のアクトが利用できます。小修整が必要な場合があります。

* [PAL\_MOT-single](pal\_mot-oneshot.md)
* [PAL\_MOT\_fifo](pal\_mot.md) は、加速度センサーのFIFOおよびFIFOの割り込みを用いて、サンプルを中断することなく、連続的に取得し無線送信するためのサンプルです。

### TWELITE ARIA を使用するためのアクト

* [BRD\_ARIA] は、TWELITE ARIA を動作させるためのアクトです。
* [BRD\_I2C_TEMPHUMID](brd\_i2c\_temphumid.md) は、I2C センサー利用のためのテンプレートですが、実装例として TWELITE ARIA で利用する SHT40 センサー用のコードが含まれます。
* PAL\_AMB 用のアクトを小修整することで利用可能です。

### 単体機能を紹介したAct

[Unitから始まる名前のAct](unit\_acts.md)は機能やAPIの紹介を目的としています。



## 最新版の入手

{% hint style="info" %}
最新版のコードや MWSDK バージョン間の修正履歴を確認する目的で Github にソース一式を置いています。以下のリンク先を参照してください。

[https://github.com/monowireless/Act\_samples](https://github.com/monowireless/Act\_samples)
{% endhint %}



## 共通の記述

アクトのサンプル中で以下の項目は共通の設定項目になり、以下で解説します。

```cpp
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;
const char APP_FOURCHAR[] = "BAT1";
```

{% hint style="info" %}
サンプルアクト共通として以下の設定をしています。

* アプリケーションID ` 0x1234abcd`
* チャネル `13`

アプリケーションIDとチャネルはともに他のネットワークと混在しないようにする仕組みです。

**アプリケーションIDが異なる者同士は、チャネルが同じであっても混信することはありません**。ただし、別のアプリケーションIDのシステムが頻繁に無線送信しているような場合はその無線送信が妨害となりますので影響は出ます。

チャネルは通信に使う周波数を決めます。TWELITE無線モジュールでは原則として**１６個のチャネル**が利用でき、通常のシステムでは実施しないような極めて例外的な場合を除き、**違うチャネルとは通信できません**。

サンプルアクト共通の仕様として、パケットのペイロード(データ部)の先頭には4バイトの文字列(`APP_FOURCHAR[]`)を格納しています。種別の識別性には1バイトで十分ですが、解説のための記述です。こういった**システム特有の識別子やデータ構造**を含めるのも混信対策の一つです。
{% endhint %}

