---
title: "フォルダ構成について"
author: "Mono Wireless Inc."
description: "フォルダ構成について"
---

# フォルダ構成について

TWELITE STAGE APP は、TWELITE STAGE SDKパッケージ内のフロントエンドアプリケーションとして動作します。ここでは、そのフォルダ構成について解説します。

```text
MWSTAGE/            : TWELITE STAGE SDK インストール
  TWELITE_Stage.??? : 実行形式 (Windwows .exe, macOS .command, Linux .run)
  TWELITE_Stage.sav : 設定ファイル
  TWELITE_Stage.ini : その他設定
  TWELITE_Stage/    : TWELITE STAGE APP の関連ファイル

  MWSDK/            : MWSDKのライブラリなど
  BIN/              : [BINファイル選択]時の格納先
  log/              : ログ・データベース格納先

  Tools/            : gcc コンパイラなどのツール一式

  flask_wsns_db/    : Python, Flask, sqlite3 による簡易的なサーバ
```



### MWSDK フォルダ

```text
MWSDK/
  Act_samples/        : mwx ライブラリによるサンプルコード
  Wks_TweApps/        : TWELITE APPS のソースコード
  Act_extras/         : mwx ライブラリによるより専門的なサンプル、他のライブラリを引用したもの
  TWENET/             : TWENET ライブラリ (mwx ライブラリなど)
  ChipLib/            : 半導体ライブラリ
  MkFiles/            : Makefile の本体処理部分
  docs/               : ライブラリマニュアルなど
  LICENSE             : MWSDKのライセンス記述
  000manifest         : MWSDKのバージョン情報
  ReleaseNotes.md     : 更新履歴(トップページ)
  ReleaseNotes_en.md  : 更新履歴(英語)
  ReleaseNotes_jp.md  : 更新履歴(日本語)
```

MWSDK フォルダには、TWELITE のソフトウェアを構築するためのライブラリや、サンプル、TWELITE APPS のソースコードが含まれます。


### TWELITE_Stage.sav

TWELITE Stage アプリ内での設定情報を記録します。

ファイル名は TWELITE_Stage アプリのファイル名 + `.sav` です。

### TWELITE_Stage.ini

iniファイルの詳細は[こちら](./cmdargs_and_ini.md)。

* `MWSDK=`
`MWSDK/` フォルダの替わりに別のフォルダを指定したいときに編集します。複数のライブラリバージョンを混在させる場合に便利です。上記の例では `MWSDK2020_10`フォルダを利用します。

* `LANG=`
TWELITE Stage の表示言語を英語にする場合は `LANG=en` を指定します。


### 設定の異なる TWELITE Stage を実行する
`TWELITE_Stage.exe` (Windows の場合) を別のファイル名でコピーします。
例えば `TWS1.exe` と変更した場合は、`TRS1.sav`, `TRS1.ini` という設定ファイルを参照します。

### BIN フォルダ

TWELITE STAGE APP の [\[BINから選択\]](../usage/screens/main\_menu/firm\_prog/bin.md) メニューを選択したときには、このフォルダにある ファームウェアファイル (.BIN) を利用します。


### log フォルダ

TWELITE STAGE APP でシリアルポートの[ログ機能](../usage/logging.md)を実行したときには、このフォルダにログファイルを格納します。

グラフ機能を用いた場合のデータベースファイルの格納先や、csvファイルの出力先もこのフォルダです。


### Tools フォルダ

gcc, g++ など、クロスコンパイラの toolchain 等が含まれます。

プラットフォーム特有のユーティリティもこのフォルダに格納されます。詳しくは `Tools/readme.txt` を参照してください。

### flask_wsns_db フォルダ
TWELITE STAGE APP のセンサーグラフビューアで作成したデータベースにアクセスするためのPythonのサンプルスクリプトです。<br>
本サンプルでは表やグラフでデータをWebブラウザで閲覧することができます。

詳しくは `flask_wsns_db/README.html` を参照してください。

## ビルドプロジェクトフォルダについて

{% hint style="warning" %}
本機能は標準的に提供する機能ではないものとして紹介します。記載の内容と異なった動作になる場合もあります。
{% endhint %}

### フォルダの検索順

TWELITE STAGE APP は、以下の順でビルドプロジェクトフォルダ (Act\_samples など) を検索します。

1. TWELITE STAGE アプリが起動したときのフォルダ
2. TWELITE STAGE アプリの実行形式があるフォルダ
3. `{MWSDKフォルダ}/..`
4. `{MWSDKフォルダ}`

{% hint style="info" %}
MWSDK は TWELITE STAGE APP の実行形式があるフォルダを起点に検索します。
{% endhint %}

### Wks\_Acts
`Wks_Acts` フォルダを作成した場合には、`Act_samples` フォルダの替わりに、このフォルダをメニューの\[Actビルド＆書換]メニューから参照します。

{% hint style="info" %}
自身で作成するビルドプロジェクトを Wks\_Acts に格納したいといった用途を想定しています。
{% endhint %}
