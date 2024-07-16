---
title: "VSCodeのインストール"
author: "Mono Wireless Inc."
description: "VSCodeのインストール"
---

# VSCodeのインストール

TWELTIE STAGE SDK では、アクト（ソースコード）記述をより容易に行うため、VisualStudio Code(VSCode)を紹介しています。添付のアクトには、VSCodeで適切にコード解釈が行われるように設定したファイルが含まれます。

{% hint style="warning" %}
VSCodeはソースファイルやヘッダファイルを読み込み、ソースコードを解釈し、これによりソースコードの記述の助けとなる関数定義情報や、関数・メソッド名の補完などを行います。C言語の開発に比べて、MWXライブラリでは読み込まれるヘッダファイルの分量が多くなります。環境によってはエディタの動作が重く感じる場合があります。
{% endhint %}

ソースコードの解析や VSCode からのビルドを行うために、ライブラリソースコードが格納されるフォルダの情報などの情報が必要になります。これらの情報は TWELITE STAGE アプリから VSCode を呼び出すことによって反映されます。（具体的には適切な環境変数を設定して VSCode を始動します。プロジェクトの設定は環境変数を参照しています）

## VSCode のインストール

{% hint style="danger" %}
当サポートでは VSCode のインストール方法、使用方法のお問い合わせについては対応いたしません。一般で得られる情報を参照ください。

環境によっては、インストールのためにセキュリティ設定などが必要になる場合があります。インストールの可否はシステム管理者にご確認の上、手順は配布元や一般の情報を参考にしてください。
{% endhint %}

VSCode では、以下のことができます。

* ソースコードの編集
* ソースコード解釈に基づく intellisense（＊全ての定義が正確に解釈されることを保証するわけではありません）


VSCode はリンク先より入手してください。

{% embed url="https://code.visualstudio.com" %}

### プラグインの導入

Visual Studio Code が C/C++ 言語の記述を解釈できるようにするために、プラグインをインストールします。

* C/C++ for Visual Studio Code



### 各OSでの特記事項

TWELITE STAGE から VSCode を呼び出すために、`code` コマンドが有効になっている必要があります。



以下は code.visualstudio.com の情報です。

* [macOS](https://code.visualstudio.com/docs/setup/mac) - code コマンドを実行できるように PATH 設定が必要です。
* [Windows](https://code.visualstudio.com/docs/setup/windows)
* [Linux](https://code.visualstudio.com/docs/setup/linux)



## 注記事項

{% hint style="warning" %}
MWXライブラリのサンプルには .vscode の定義を含めています。この定義は MWSDK\_ROOT 環境変数を用い、ライブラリのソースコード(`{MWSDK_ROOT}/TWENET/current`以下)を特定しています。

TWELITE STAGEからVSCodeを始動した場合、上記の環境変数などを設定します。既に始動済みであるような場合など、設定が反映されない場合もあります。
{% endhint %}

{% hint style="warning" %}
VSCode のソースコードの解釈はコンパイラでの解釈とは完全には一致しません。またソースコードの編集状況によっては解釈がより不完全な状態になる場合もあります。
{% endhint %}
