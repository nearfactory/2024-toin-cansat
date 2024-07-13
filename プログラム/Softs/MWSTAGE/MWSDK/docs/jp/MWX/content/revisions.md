---
title: "改版履歴"
author: "Mono Wireless Inc."
description: "改版履歴"
---

# 改版履歴

{% hint style="info" %}
軽微な修正にあたるものは、本改版履歴には記載を行わずGitHub上の改版のみとなります。必要に応じて修正を引用ください。
{% endhint %}



## 更新方法

TWELITE STAGE の配布パッケージリリース後の修正・追加分などはGitHubレポジトリに格納しております。必要に応じて配布パッケージの位置を差し替えて利用いただくようお願いいたします。

MWSDKの他の更新が必要になる場合があります。更新時のリリース記述を参照してください。MWSDKの更新については[こちら](https://sdk.twelite.info/latest)を参照ください。

### MWXライブラリコードの更新方法

ライブラリのソースコードは GitHub ([https://github.com/monowireless/mwx](https://github.com/monowireless/mwx))にて公開しています。ライブラリのソースコードの差し替えは、以下の手順で行ってください。

1. 各リリースのリンクよりGitのクローンを行うか zip 形式でソースコードをダウンロードします。
2. 以下のフォルダの内容を差し替えます。

```
.../MWSTAGE/              --- TWELITE STAGE 配布フォルダ
        .../MWSDK         --- MWSDKフォルダ
              .../TWENET/current/src/mwx <--　このフォルダを差し替える
```

## リリース前の更新

> [https://github.com/monowireless/mwx/wiki](https://github.com/monowireless/mwx/wiki)

リリース前の更新については上記に掲載する場合があります。



## 0.2.0 - 2022-03-01

| ライブラリ名 | 依存バージョン                                               |
| ------------ | ------------------------------------------------------------ |
| mwx          | [0.2.0](https://github.com/monowireless/mwx/releases/tag/0.2.0) |
| twesettings  | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C     | 1.3.5                                                        |

* ヒープ領域へのメモリ確保を行う Wire オブジェクトを変更した。
* `utils.h`での名前の衝突を避けるため、関数名を`G_OCTET()`から[`G_BYTE()`](api-reference/funcs/utility/byte-array-utils.md)に変更した。
* `attachIntDio()`において、`vAHI_DioInterruptEnable()`の順番を変更した。
* ユニバーサルレシーバ (NWK_LAYERED, NWK_SIMPLE またはネットワークレスパケットを同一実行コードで受信する) をサポートするために `the_twelite.network2` を追加した。
* [NWK_LAYERED](./networks/nwk_layered.md) を追加 (現時点では親機受信のみ対応)
* MWXの初期化時にアプリケーションのバージョンを設定する `MWX_Set_Usder_App_Ver()` 関数を導入した。
* [mwx::pnew()](./api-reference/funcs/utility/pnew.md) を追加し配置newの記述を簡素化した。
* [EASTL](api-reference/external_libraries/EASTL.md)のサポート追加
  * EASTL用の`new[]`演算子の追加
* MWXのソースコードのほとんどをプリコンパイルし、コンパイルの高速化を図った。
* 修正されました。DIOイベントが無関係なポートに引き渡されていたのを修正。

  

## 0.1.9 - 2021-12-15



| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.9](https://github.com/monowireless/mwx/releases/tag/0.1.9)         |
| twesettings | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C    | 1.3.5                                                                   |



#### 主な改定内容

* TWELITE ARIA向けのボードサポート [BRD\_ARIA](act\_samples/brd\_aria.md) とセンサー定義 [SHT4x](sensor\_object/sht4x.md) を追加
* インタラクティブモード中で Serialクラスオブジェクトを用いた出力を可能とする内部手続きを追加 (`Serial._force_Serial_out_during_intaractive_mode()`)



## 0.1.8 - 2021-09-09

| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.8](https://github.com/monowireless/mwx/releases/tag/0.1.8)         |
| twesettings | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C    | 1.3.5                                                                   |



#### 主な改定内容

* `Serial1`のポート、代替ポートの定義が適切でなかった
* `Serial`(UART0)のボーレートを変更できるようにした
* 受信パケット([`on_rx_packet()`](api-reference/sys_callbacks/on_rx_packet.md))、送信完了([`on_tx_comp()`](api-reference/sys_callbacks/on_tx_comp.md))を知らせるイベントコールバックを追加
  * コールバック関数の定義をしなければ従前の手続きも利用可能
* `<STG_STD>`インタラクティブモード設定の定義ID間違いや一部デフォルト値の変更など
* `<STG_STD>`インタラクティブモード設定でAppIDに加えて、チャネルと論理デバイスIDのデフォルト値を変更できるようにした
* `the_twelite` と `<NWK_SIMPLE>` オブジェクトの設定を、一部の設定についてインタラクティブモード`<STG_STD>` オブジェクトで行えるようにした
* `<NWK_SIMPLE>`で再送回数のデフォルト値を設定できるようにした
* `<STG_STD>` インタラクティブモードの画面が出ている間はアプリケーションからの`Serial`(UART0)の入出力を行わないようにした
* `CUE::PIN_SET`, `PAL???"":PIN_SET`を追加 (`PIN_BTN`はボタンのないCUEで`PIN_BTN`を用いるのは不自然であるため)
* `random()`の名前空間を`mwx::`に移動 (グローバル名にエリアス)
* MONOSTICKのウォッチドッグ設定を32ms単位で行うようにした
* `BRD_TWELITE`を用いスリープを行うと、復帰時にピンが正しく初期化されなかった



## 0.1.7 - 2020-12-03

| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.7](https://github.com/monowireless/mwx/releases/tag/0.1.7)         |
| twesettings | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C    | 1.3.4                                                                   |



#### 主な改定内容

* TWELITE CUE のボードビヘイビア([https://mwx.twelite.info/v/v0.1.7/boards/cue](https://mwx.twelite.info/v/v0.1.7/boards/cue))を追加。
* NWK\_SIMPLE 利用時に NWK\_SIMPLE 形式でない他のパケット（ネットワーク利用無し）を受信する方法を追加。`NWK_SIMPLE::receive_nwkless_pkt()`を追加してNWK\_SIMPLEを初期化する。 このパケット情報を用いる場合は `.get_psRxDataApp()` による TWENET C ライブラリ層の構造体、および `.get_payload()` により得られるデータ配列のみを利用してください。受信パケット(`auto&& rx = the_twelite.receiver.read()`)の他のメソッドから得られる情報は不定です。
* `get_stream_helper()` コードのリファインと読み書き位置のAPIの整備。
* EEPROMクラスオブジェクトを追加 ([https://mwx.twelite.info/v/v0.1.7/api-reference/predefined\_objs/eeprom](https://mwx.twelite.info/v/v0.1.7/api-reference/predefined\_objs/eeprom))
  * サンプル ([https://github.com/monowireless/Act\_samples/tree/master/Unit\_EEPROM](https://github.com/monowireless/Act\_samples/tree/master/Unit\_EEPROM))
* `smplbuf::get_stream_helper()`の不具合修正
* pktparserクラスを追加しました ([https://mwx.twelite.info/v/v0.1.7/api-reference/classes/pktparser](https://mwx.twelite.info/v/v0.1.7/api-reference/classes/pktparser))
  * サンプル ([https://github.com/monowireless/Act\_samples/tree/master/Unit\_PktParser](https://github.com/monowireless/Act\_samples/tree/master/Unit\_PktParser))
* `serparser/pktparser` を他のプラットフォームでビルドできるようサンプルを用意しました ([https://github.com/monowireless/mwx/tree/master/stdio](https://github.com/monowireless/mwx/tree/master/stdio))



## 0.1.6 - 2020-10-09

| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.6 ](https://github.com/monowireless/mwx/releases/tag/0.1.6)        |
| twesettings | [0.2.5](https://github.com/monowireless/twesettings/releases/tag/0.2.5) |
| TWENET C    | 1.3.4                                                                   |



#### 主な改定内容

* 商・余を計算する `div100()` をSerial等へ出力できるようにした
* `smplbuf<>` 配列クラスの実装変更。消費メモリの削減などを目的として`mwx::stream`の継承をやめ、別途継承クラスとヘルパークラス定義した
* `mwx_printf()` `mwx_snprintf()` の関数を追加した
* `the_twelite.stop_watchdog()`, `the_twelite.restart_watchdog()` を追加した
* `mwx::stream` のメンテナンス: `operator bool()` の廃止。読み出しタイムアウトの設定で 0xff を指定した場合(`.set_timeout(0xff)`)タイムアウトを無効に。その他 `<<` 演算子の定義を追加。
* NOTICE PAL / PCA9632 のサポートを追加 (解説 [https://mwx.twelite.info/v/latest/boards/pal/pal\_notice](https://mwx.twelite.info/v/latest/boards/pal/pal\_notice), サンプル [https://github.com/monowireless/Act\_samples/tree/master/Unit\_using\_PAL\_NOTICE](https://github.com/monowireless/Act\_samples/tree/master/Unit\_using\_PAL\_NOTICE))
* 除算を行わない 8bit と 0..1000 間のスケール関数を追加。
* 10,100,1000による除算（商と余を同時に計算） `div10()`, `div100()`, `div1000()` を追加。値域を制限し乗算とビットシフトを中心に構成。
* 暗号化パケットの対応メソッドを追加
  * `packet_rx::is_secure_pkt()` : 受信パケットが暗号化されているかどうかの判定
  * `STG_STD::u8encmode()` : インタラクティブモードでの暗号化設定を取得
  * `STG_STD::pu8enckeystr()` : インタラクティブモードでの暗号化鍵バイト列の取得
* Serial1: デフォルトのポートは半導体の仕様では I2C と重複する DIO14,15 だが、通常 I2C に割り当てられるため DIO11(TxD), DIO9(RxD) とした。
* Serial: ボーレートの指定で /100 が発生するが、主要なボーレートについてこの計算を省略するようにした。
* `Serial: available()`, `read()` を外部で実施するための代理関数の保持を `void*` のみとし、仕様メモリを 8bytes 削減。
* `typedef boolean` の追加
* ネットワーク: 暗号化の対応を追加。
  *   暗号化を有効にするには `NWK_SIMPLE::secure_pkt(const uint8_t*, bool = false)` を設定追加する。１番目のパラメータは暗号キー、２番目を `true` にすると、平文のパケットも受信する。

      ```
      auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
      nwk << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
          << NWK_SIMPLE::secure_pkt((const uint8_t*)"0123456789ABCDEF");
          ;
      ```
* SHT3xとBME280のセンサーサポート追加
* センサー: レガシーコード（Cライブラリのラッパクラス）で、設定パラメータや状態をやり取りするための仕掛けを追加した。
* センサー: SHT3x, BME280では I2C アドレスを指定可能とした。
* 設定: `hide_items()` を追加。不要な設定項目を削除可能。
* 設定: H/W UTIL メニューを追加。DIの状態表示、I2Cのプローブ、PAL EEPROM内容の表示。
* 設定: 暗号化関連のメニューの追加
* I2C関連の修正(TwoWireクラスを用いて実装されたコードとの親和性を向上するための修正）
  * `requestFrom(false)` の処理時に NO\_STOP メッセージの送信コードが無かったため処理が正常に行われなかった。
  * `TwoWire` のクラス名エリアスを追加した。
  * `begin()` 処理で、多重初期化しないようにした。
  * `setClock()` メソッドを追加（ただしダミー関数で何もしない）
  * `WIRE_CONF::WIRE_???KHZ` を追加。バスクロックの主要な設定値を追加した。



## 0.1.5 - 2020-08-05

| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.5](https://github.com/monowireless/mwx/releases/tag/0.1.5)         |
| twesettings | [0.2.5](https://github.com/monowireless/twesettings/releases/tag/0.2.5) |
| TWENET C    | 1.3.4                                                                   |



#### 一括ダウンロード

> [MWSDK2020\_08\_UNOFFICIAL](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/releases/tag/MWSDK2020\_08\_UNOFFICIAL) ([README.md](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/blob/master/README.md))



#### 主な改定内容

* [設定ビヘイビア（インタラクティブモード機能）](settings/)を追加
* チャネルマネージャ [`chmgr`](api-reference/predefined\_objs/the\_twelite.md#twenet-chmgr-uint-8-\_t-ch1-18-uint-8-\_t-ch2-0-uint-8-\_t-ch3-0) の実装



## 0.1.4 - 2020-07-29 (MWSDK2020\_07\_UNOFFICIAL)

| ライブラリ名      | 依存バージョン                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.4](https://github.com/monowireless/mwx/releases/tag/0.1.4)         |
| twesettings | [0.2.4](https://github.com/monowireless/twesettings/releases/tag/0.2.4) |
| TWENET C    | 1.3.3                                                                   |

####

#### 一括ダウンロード

> [MWSDK2020\_07\_UNOFFICIAL](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/releases/tag/MWSDK2020\_07\_UNOFFICIAL) ([README.md](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/blob/master/README.md))



#### 主な改定内容

* `delayMilliseconds()` の追加
* `digitalReadBitmap()` の追加
* `delay()` の精度向上
* `Serial1` インスタンスが定義されていない問題を修正
* `Analogue`の割り込みハンドラが呼び出されない問題を修正



## 0.1.3 - 2020-05-29

MWSDK2020\_05 に対応

* 重複チェッカ duplicate\_checker の初期化等に不備があり期待通りの除去を行っていなかった
* format() の実装を機種依存の少ないものとした。また、引数を最大８までとした。64bit引数が含まれる場合は引数の数は制限される。

> [https://github.com/monowireless/mwx/releases/tag/0.1.3](https://github.com/monowireless/mwx/releases/tag/0.1.3)

{% hint style="warning" %}
修正は MWSDK2020\_05 を前提としています。

本修正については、更新を推奨します。
{% endhint %}

## 0.1.2 - 2020-04-24

MWSDK2020\_04 に対応

* Timer0..4の初期化の問題を修正
* mwx::format() の内部処理を変更
* インタラクティブモード対応のための実験的なコードの追加

> [https://github.com/monowireless/mwx/releases/tag/0.1.2](https://github.com/monowireless/mwx/releases/tag/0.1.2)

{% hint style="warning" %}
本修正は MWSDK2020\_04 を前提としています。

本修正については、更新を推奨します。
{% endhint %}

## 0.1.1 - 2020-02-28

パケット内の中継フラグの扱いについての問題を修正

> [https://github.com/monowireless/mwx/releases/tag/0.1.1](https://github.com/monowireless/mwx/releases/tag/0.1.1)

{% hint style="warning" %}
本修正については、更新を推奨します。
{% endhint %}

## 0.1.0 - 2019-12-23

初版リリース (SDL 2019/12月号収録)

> [https://github.com/monowireless/mwx/releases/tag/0.1.0](https://github.com/monowireless/mwx/releases/tag/0.1.0)

