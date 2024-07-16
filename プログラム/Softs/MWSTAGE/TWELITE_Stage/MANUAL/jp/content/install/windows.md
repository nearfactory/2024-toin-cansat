---
title: "Windows"
author: "Mono Wireless Inc."
description: TWELITE_Stage インストール (Windows10)
---

# Windows

`Windows`

### 環境

以下の環境で開発・動作確認しています。

* Windows10 バージョン 1903
* VisualStudio 2019 \(32bit ビルド\)

### シリアルポートの取り扱い
MONOSTICK や TWELITE-R は、 FTDI社の USBシリアル変換IC FT230 / FT232 シリーズを搭載しています。これらを利用するために、デバイスドライバのインストールが必要となる場合があります。

PC がMONOSTICK や TWELITE-R を認識しない場合には、[https://www.ftdichip.com](https://www.ftdichip.com) より D2XX ドライバをインストールしてください。

### Visual C++ ランタイムライブラリの追加インストール

場合によっては、**Visual Studio 2019 の  Visual C++ 頒布可能コード**（ランタイムライブラリ）が必要です。

アプリケーションの起動時にエラーが出て起動しない場合は、本パッケージで再配布している **TWELITE\_Stage¥INSTALL¥VC\_redist.x86.exe** を実行するか、マイクロソフト社のウェブサイトから入手してください。なお、再配布バイナリは 32bit です。
