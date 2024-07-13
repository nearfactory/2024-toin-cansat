---
title: "TWELITE STAGE SDK (Japanese)"
author: "Mono Wireless Inc."
description: "TWELITE STAGE SDK (Japanese)"
---

# TWELITE STAGE SDK

TWELITE STAGE SDK は TWELITE STAGE アプリを核として、TWELITE 無線モジュールの開発や動作確認などを行うための総合パッケージです。



## インストール

TWELITE STAGE SDK のアーカイブを適当なフォルダに展開します。

**TWELITE STAGE SDK の展開先のフォルダには、空白や日本語文字列や `.` `-` `_`以外の記号文字が含まれてはいけません。**





## ファイル一覧

```
TWELITE_Stage.xxx  : TWELITE STAGE APP 実行ファイル（このファイルを起動してください）
  TWELITE_stage.exe（Windows）
  TWELITE_stage.command（macOS）
  TWELITE_stage.run（Linux/Raspberry Pi 2+）
TWELITE_Stage.sav  : TWELITE STAGE APP 設定ファイル
TWELITE_Stage.ini  : MWSDKディレクトリの選択など、追加的な設定

TWELTIE_Stage_VSCode.xxx : VSCode利用の設定での TWELITE STAGE APP
  TWELITE_stage_VSCode.exe（Windows）
  TWELITE_stage_VSCode.command（macOS）
  TWELITE_stage_VSCode.run（Linux）
TWELTIE_Stage_VSCode.sav : VSCode利用の設定済みの設定ファイル
TWELITE_Stage_VSCode.ini : MWSDKディレクトリの選択など、追加的な設定
※設定ファイルや追加設定ファイルは実行形式のファイル名に合わせます。

以下のTWELITE STAGE APP はRasbperry Pi 版のみ含まれます。
TWELITE_Stage_nox.run      : X11未使用時のTWELITE STAGE APP 実行ファイル (Rasbperry Pi 2+)
TWELITE_Stage_nox.sav      : X11未使用時のTWELITE STAGE APP 設定ファイル (Rasbperry Pi 2+)
TWELITE_Stage_nox.ini      : MWSDKディレクトリの選択など、追加的な設定 (Rasbperry Pi2+)
TWELITE_Stage_Zero.run  : X11使用時のTWELITE STAGE APP 実行ファイル (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero.sav  : X11使用時のTWELITE STAGE APP 設定ファイル (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero.ini  : MWSDKディレクトリの選択など、追加的な設定 (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_nox.run   : X11未使用時のTWELITE STAGE APP 実行ファイル (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_nox.sav   : X11未使用時のTWELITE STAGE APP 設定ファイル (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_nox.ini   : MWSDKディレクトリの選択など、追加的な設定 (Raspberry Pi Zero/1A/1B)

flask_wsns_db/	         : Python, Flask, sqlite3 による簡易的なサーバ
  _readme/               : 本ドキュメントの画像など
  static/                : CSS など
  templates/             : html ひな型
  app.py                 : サーバー用のスクリプト
  README.md/.html        : ドキュメント

ftd2xx.dll         : 上記実行形式を実行するために必要なライブラリ (Windows版のみ)

BIN/               : [BINから選択]メニューで参照されるTWELITE無線マイコン向け.BINファイル

log/               : TWELITE STAGE APP のログ機能の保存先

MWSDK/             : MWSDK 環境ファイル、サンプルファイル
  000manifest      : MWSDKのバージョン情報
  LICENSE          : MWSDKのライセンス記述
  README.md        : README
  ReleaseNotes.md  : 更新履歴(トップページ)
  ReleaseNotes_en.md  : 更新履歴(英語)
  ReleaseNotes_jp.md  : 更新履歴(日本語)

  Act_samples/     : アクト(Act)のビルドプロジェクト
  Wks_TweApps/     : TWELITE APPS (レディメードソフトウェア) のビルドプロジェクト
  Act_extras/      : 追加的なアクトのビルドプロジェクト

  TWENET/          : TWENET C/C++ ライブラリ・ソースコード
  ChipLib/         : 半導体チップライブラリ
  MkFiles/         : Makefileの本体処理部

MWSDK{YYYY}_{MM}/  : 古いバージョン (TWELITE_Stage.iniで指定可能)

Tools/             : コンパイル用のツールチェインなど

TWELITE_Stage/     : TWELITE STAGE APP 関連ファイル
  readme-j.txt     : TWELITE STAGE APP ライセンス記述
  INSTALL/         : TWELITE STAGE APP インストール補足
  LICENSE/         : TWELITE STAGE APP ライセンス
  MANUAL/	         : TWELITE STAGE APP ドキュメント
```


## TWELITE STAGE アプリの英語対応

TWELITE STAGEアプリは、英語版ユーザーインターフェースに切り替えることができます。設定ファイルを以下のように編集してください。

1. アプリが起動している場合は、アプリを終了します。
2. 使用する TWELITE_Stage の `.ini` ファイルを開く。
   例：`TWELITE_Stage.ini`。
3. 以下の行を追加します。

```
LANG=en
```

4. `.ini` ファイルを保存して閉じます。
5. アプリを（再）起動する。



※ 参考 - [コマンドライン引数と ini ファイル](TWELITE_Stage/MANUAL/jp/content/misc/cmdargs_and_ini.html)





## 参照資料

* [TWELITE STAGE アプリマニュアル](TWELITE_Stage/MANUAL/jp/index.html)
* [TWELITE STAGE SDK, MWX ライブラリマニュアル](MWSDK/docs/jp/MWX/index.html) - MWXライブラリのインストール、クイック開発ガイド、リファレンスマニュアルです。
* [(TWELITE NET C Library Manual)](MWSDK/docs/jp/TWENET_C/index.html) - (注意: 最初に参照する必要はありません。TWENTE C APIで開発する場合やSDK構造などの情報が必要な場合に参照ください)
* [Python, Flask, sqlite3 による簡易的なサーバ](flask_wsns_db/README.html) - Python, Flask, sqlite3 による簡易的なサーバのマニュアルです。

注： 上記のリンクは `MWSDK/docs/` を参照しています。このフォルダには対応するMWSDKライブラリ資料が格納されます。




## VSCode (Visual Studio Code)の利用

TWELITE STAGE SDK では VSCode を利用したときの利便性を向上するため、TWELITE STAGE アプリの機能（要設定）や、設定済みのプロジェクト設定があります。そのため VSCode の適切なインストールと設定が必要になります。特に code コマンドで VSCode を起動できるようにする設定が必要で、一部システムでは追加的な設定が必要になります。

[Installing VSCode](MWSDK/docs/jp/MWX/content/install_n_build/install_vscode.html)

詳しくは Installing VSCodeを参照ください。


## ライセンス

* TWELITE STAGE APP 実行ファイルには MW-SLA-1J,E が適用されます。
* TWELITE 無線マイコン用の実行形式(ファームウェア,アプリ)は MW-SLA-1J,Eが適用されます。
* 本パッケージに含まれるソースファイルは、ソースファイル中で記載されたライセンスに従います。
* toolchain などオープンソースプロジェクトの成果物は、各々のライセンスが適用されます。

特に記述がない場合は MW-SLA-1J,1E 記載の条項が適用されます。

## リリースノート

### MWSTAGE2022_08 (2022/8/1)

* TWELITE STAGE APPのドキュメントを一部修正

#### TWELITE APPS
##### キューアプリ(App_CUE/App_CUE_OTA) v1.1.1
* 動作センサーパルモード(シングルショット) 時にパルIDが意図しない値が出力される点を修正

##### アリアアプリ(App_ARIA/App_ARIA_OTA) v1.1.1
* 計測した温度が42℃以上になったときに、異常な値が出力される点をを修正

##### パルアプリ(App_PAL) v1.5.2
* 動作センサーパルのシングルショットモード時に意図しないパルIDが出る点を修正
* 計測した温度が一定以上になったときに、異常な値が出力される点を修正(TWELITE ARIA用の修正なので、環境センサーパルのコードの修正はありません。)

##### 親機・中継機アプリ(App_PAL) v1.2.1
* パルアプリ(キューアプリ、パルアプリ)のデータの出力が意図しない値が出力される場合がある点を修正

##### シリアル通信アプリ(App_Uart) v1.4.7
* デフォルトの再送回数を2回に変更

#### Act_extras
##### ActEx_AQM0802
  * MWXストリーム出力に対応
  * AQM1602(16x2行の液晶)に対応
  * コントラスト調整機能の追加

### MWSTAGE2022_07 (2022/7/12)
* flask_wsns_db の追加
  * TWELITE STAGE APPのセンサーグラフビューアで作成したデータベースにアクセスするためのPythonのサンプルスクリプト
    * 本サンプルではデータを表やグラフに変換し、Webブラウザで閲覧可能
* TWELITE STAGE APP、MWXライブラリ、TWELITE NET Cライブラリのドキュメントを本SDKに含めるように変更

#### TWELITE STAGE APP v1.3.8
* TWELITE CUEのリアルタイムグラフを描画する機能の追加
* TWELITE ARIA、TWELITE PAL、TWELITE CUEから受信したセンサーのデータをSQLiteに保存してからグラフ化する機能を追加
* ウインドウサイズを 640x480 に拡大
* 起動時にウィンドウの位置を指定する機能を追加
* ヘルプ選択時にローカルのHTMLを表示するように変更
* 英語に一部対応
* メニューの構成を変更

#### MWX ライブラリ v0.2.0
* ヒープ領域へのメモリ確保を行う Wire オブジェクトを変更
* utils.hでの名前の衝突を避けるため、関数名をG_OCTET()からG_BYTE()に変更
* attachIntDio()において、vAHI_DioInterruptEnable()の順番を変更
* ユニバーサルレシーバ (NWK_LAYERED, NWK_SIMPLE またはネットワークレスパケットを同一実行コードで受信する) をサポートするために the_twelite.network2 を追加
* NWK_LAYERED を追加 (現時点では親機受信のみ対応)
* MWXの初期化時にアプリケーションのバージョンを設定する MWX_Set_Usder_App_Ver() 関数を導入
* mwx::pnew() を追加し配置newの記述の簡素化
* EASTLのサポート追加
* EASTL用のnew[]演算子の追加
* MWXのソースコードのほとんどをプリコンパイルし、コンパイルの高速化を図った
* DIOイベントが無関係なポートに引き渡されていたのを修正

#### TWELITE APPS
* 000desc.txtに英訳を追加
* Samp_PingPong、Samp_I2Cを新規採用非推奨に変更

##### キューアプリ(App_CUE/App_CUE_OTA) v1.1.0
* 指定した間隔ごとに加速度を1サンプルずつ送信する機能の追加
* 一定期間計測した加速度の平均、最小最大値を送信する機能の追加

##### アリアアプリ(App_ARIA/App_ARIA_OTA) v1.1.0
* 温湿度の計測値にオフセットと係数を掛ける機能を追加

##### パルアプリ(App_PAL) v1.5.1
* 動作センサーパルで加速度を1サンプルずつ送信する機能の追加
* 動作センサーパルで 一定期間計測した加速度の平均、最小最大値を送信する機能の追加
* 環境センサーパルで温湿度の計測値にオフセットと係数を掛ける機能を追加 

##### 親機・中継機アプリ(App_PAL) v1.2.0
* 加速度の平均、最大最小値を出力する機能の追加

#### Act_samples
* 000desc.txtに英訳を追加
* BRD_I2C_TEMPHUMID の追加
  * I2C センサーデバイスを用いて、定期起床からの計測および送信を行うサンプル

#### Act_extras
* 000desc.txtに英訳を追加
* ActEx_AQM0802 の追加
  * Sitronix社のI2C接続LCDコントローラ ST7032 を実装したLCDモジュール AQM0802 を使用するサンプル

