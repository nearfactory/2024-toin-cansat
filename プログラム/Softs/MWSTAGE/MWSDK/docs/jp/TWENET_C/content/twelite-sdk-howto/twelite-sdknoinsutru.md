# TWELITE SDKのインストール

## TWELITE STAGE (MWSDK2020-04以降)

{% hint style="success" %}
TWELITE STAGE (MWSDK2020-04以降) では、配布zipアーカイブを、日本語や空白文字が含まれないフォルダに展開します。

ビルドに TWELITE STAGEを用いず、makeコマンドを用いてビルドする場合は[こちら](https://mwx.twelite.info/install_n_build/install_sdk)を参照の上、`MWSDK_ROOT`環境変数を設定してください。
{% endhint %}



## MWSDK2018-05

{% hint style="warning" %}
TWELITE SDK は Windows, Linux, OS X にて実行することができますが、環境によっては本書の記載に違いがある場合があります。また取り扱いにはコマンドラインなどの知識が必要になります。

参考環境：

* Windows7, Windows10
* Linux Ubuntu 16.04 TLS(32bit), 18.04 TLS(64bit)
* macOS High Sierra 10.13.4
{% endhint %}



TWELITE SDK のインストールは以下の手順になります。

1. ZIP アーカイブの展開
2. IDE (VSCode のインストール)

## ZIPアーカイブの展開

TWELITE SDK のアーカイブを展開します。

{% hint style="danger" %}
以下のフォルダは避けてください。

* 深いフォルダ
* 日本語名やスペースが含まれるフォルダ
{% endhint %}

{% hint style="info" %}
本書では Windows でのインストールを C:\MWSDK として記述します。
{% endhint %}



ZIP アーカイブを展開すれば、ビルドに必要なコマンドラインツールは利用可能になりますが、ここでは、開発環境として VSCode について紹介します。

## 必要なインストール

#### Windows
Windows は SDK アーカイブ中に必要なツールが含まれます。

#### macOS
コマンドライン (bash) にて、make を実行した時エラーが出る場合は XCode のインストールを行います。

```bash
$ xcode-select --install
```

インストール完了後、make を入力して以下のメッセージが出れば OK です。

```bash
$ make
make: *** No targets specified and no makefile found.  Stop.
```

#### Linux
64bit の Linux では 32bit 実行環境をインストールする必要があります。

```bash
$ sudo apt-get install lib32z1
```

## VSCode のインストール

ソースコードの編集と make の呼び出しに [VSCode](https://code.visualstudio.com) を利用できます。TWELITE SDK に付属するビルドプロジェクトには VSCode で作業するための設定定義ファイルが含まれています。

{% hint style="danger" %}
当サポートでは VSCode のインストール方法、使用方法のお問い合わせについては対応いたしません。一般で得られる情報を参照ください。
{% endhint %}

VSCode では、以下のことができます。

* ソースコードの編集
* GIT への接続（お客様が独自にソース管理を GIT 上で行う場合）
* ソースコード解釈に基づく intellisense（＊全ての定義が正確に解釈されることを保証するわけではありません）
* [ビルドを行う make を呼び出す](komandoraindenobirudo.md#komandorainniyorubirudo)ためのタスク定義

VSCode はリンク先より入手してください。

{% embed url="https://code.visualstudio.com" %}

### プラグインの導入

Visual Studio Code が C言語の記述を解釈できるようにするために、プラグインをインストールします。

* C/C++ for Visual Studio Code

インストール手順は下記の通りです。

![Visual Studio Code のメニューバーの "表示" → "拡張機能"を選択します。](<../.gitbook/assets/image (38).png>)

![検索バーに "C/C++" と入力し "C/C++" をクリックします。](<../.gitbook/assets/image (39).png>)

![右図のインストールボタンをクリックします。](<../.gitbook/assets/image (40).png>)

![再読み込みをクリックすれば、プラグインのインストールが完了します。](<../.gitbook/assets/image (42).png>)

