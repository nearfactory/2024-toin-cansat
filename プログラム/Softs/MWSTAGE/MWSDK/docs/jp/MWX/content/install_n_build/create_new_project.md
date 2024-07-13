---
title: "新しいプロジェクトの作成"
author: "Mono Wireless Inc."
description: "新しいプロジェクトの作成"
---

# 新しいプロジェクトの作成

新しいプロジェクトの作成は、すでにあるサンプルアクトのフォルダを別の名前でコピーし、ファイル名の編集を行います。

{% hint style="warning" %}
コピー先のフォルダは MWSDK 配下のフォルダでなくても構いません。ただし、フォルダ名に**空白文字や日本語名が含まれてはいけません**。
{% endhint %}

{% hint style="info" %}
{% endhint %}


プロジェクトのファイル構造は以下のようになっています（ここでは `PingPong `を例に挙げます）。

```
Act_samples
  +-PingPong
    +-PingPong.cpp   : アクトファイル
    +-build          : ビルドフォルダ
    +-.vscode        : VSCode 用の設定ファイル    
```

この `PingPong `フォルダを別の場所（ただしフォルダ名に日本語や空白が含まない）にコピーします。

```
SomeDir
  +-AlphaBravo
    +-PingPong.cpp -> AplhaBravo.cpp ※ファイル名を変更
    +-build          : ビルドフォルダ
    +-.vscode        : VSCode 用の設定ファイル
```

編集の必要があるのは、`PingPong.cpp` のファイル名です。これをフォルダ名と同じ`AlphaBravo.cpp`に変更します。

{% hint style="success" %}
`build\build-BLUE.cmd` を実行してBINファイルが生成されれば完了です（Windows10）。

Linux/WSL/macOS では`make TWELITE=BLUE`を実行して、ビルドが成功するか確認します。
{% endhint %}


## ビルド定義の編集
ビルド対象のファイルを追加する場合は build/Makefile を編集します。プロジェクト直下にある `.c` `.cpp` ファイルは自動で追加されますが、それ以外のファイルについては編集が必要です。

編集方法は [Makefile の解説](makefile.md)をご覧ください。

## VSCode用の設定
VSCode を利用する場合は、必要に応じて .vscode 以下の定義を編集してください。

TWELITE STAGE SDK に含まれるサンプルの多くは、以下のようになっています。
* TWELITE STAGE SDK ライブラリのソースコードは `${env:MWSDK_TWENET_LIBSRC}/include/**` `${env:MWSDK_TWENET_LIBSRC}/src/**` を引用する。この環境変数 `MWSDK_TWENET_LIBSRC` は TWELITE STAGE アプリから VSCode でプロジェクトを開いた場合には自動で設定されます。
* ビルドタスクについては、デフォルトで `-D` などの追加的なオプション等は設定されていない。
