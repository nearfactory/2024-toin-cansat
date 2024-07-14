---
title: "インストール"
author: "Mono Wireless Inc."
description: インストール
---

# インストール

動作環境によっては、本アプリケーションの動作に各種設定が必要です。問題が生じた場合には、本資料の記述を参考にして環境を整備してください。

{% hint style="warning" %}
開発環境を構築するためには、ソフトウェア群のインストール、またこれらの利用許諾に同意する必要があります。また、セキュリティ設定等が必要になる場合があります。

* 配布時に十分注意しておりますが、お客さまの側でもウィルスやマルウェアが含まれていないことを確認いただくようお願いします。
* セキュリティの運用（外部アプリケーションのインストールの可否など）については、お客さまの環境の管理者にご確認ください。

[「アプリケーションの配布と実行について」](https://twelite.gitbook.io/general/appinstall) も併せてご覧ください。以下の内容を含みます。

* 本来のファイルとダウンロードしたファイルの同一性の確認について
* macOS/Windowsにおけるコード署名の取り扱いについて
{% endhint %}


## TWELITE STAGE SDK のインストール手順

### ① アーカイブを取得

各プラットフォーム (Windows / macOS / Linux) 用の TWELITE STAGE SDK を [ダウンロード](https://mono-wireless.com/jp/products/stage/) します。

### ② アーカイブを展開

ダウンロードしたZipアーカイブを展開します。

{% hint style="warning" %}
展開先のファイルパスには、半角数字 `0..9`、半角アルファベット`a..zA..Z`、一部の記号 `-_.` 以外の空白や漢字・ひらがな等を含めないでください。

Windows の場合の例
- × `C:\work\作業\`
- × `C:\Users\user1\work dir\`
- 〇 `C:\Work\Work1`
{% endhint %}

### ③ ファイルを確認

展開先のフォルダを確認します。

{% hint style="warning" %}
通常 `C:\Work` 上に配置したアーカイブは `C:\Work\MWSTAGE` に展開されますが、展開ソフトによってはフォルダ名が異なる可能性があります。
必要に応じて変更してください。
{% endhint %}

{% hint style="info" %}
文中では展開したフォルダ \(例 `C:\Work\MWSTAGE` \) を `{MWSTAGE インストール}` のように表記する場合があります。
{% endhint %}

展開先のフォルダ `{MWSTAGE インストール}` には、以下が含まれます。

* TWELITE STAGE アプリ
  * Windows の場合：`TWELITE_Stage.exe`（通常版）、`TWELITE_Stage_VSCode.exe`（VSCode対応版）
  * macOS の場合：`TWELITE_Stage.command`（通常版）、`TWELITE_Stage_VSCode.command`（VSCode対応版）
  * Linux の場合：`TWELITE_Stage.run`（通常版）、`TWELITE_Stage_VSCode.run`（VSCode対応版）
* TWELITE STAGE - TWELITE STAGE APP の関連ファイル
* MWSDK - ライブラリ、ソースコードなど
* Tools - ビルドするためのツールチェインなど
* BIN - TWELITE STAGE APP の [BINから選択]メニューで参照されるTWELITE 向け.BINファイル
* log - TWELITE STAGE APP のログ機能やデータベースファイルの保存先
* flask_wsns_db - Python, Flask, sqlite3 による簡易的なサーバ

詳細は「[フォルダ構成](../misc/directories.md)」をご覧ください。

## プラットフォーム別の情報

以下を参照してください。

* [Windows](windows.md)
* [macOS](macos.md)
* [Linux](linux.md)
* [Raspberry Pi](raspberrypi.md)
