---
title: "macOS"
author: "Mono Wireless Inc."
description: TWELITE_Stage インストール (macOS)
---

# macOS

`macOS`

### 環境

以下の環境で開発・動作確認しています。

* macOS 10.14 (Mojave, Intel)
* macOS 12.4 (Monterey, Apple Silicon)

### 依存するソフトウェアや警告ダイアログについて

下記の事象が発生した場合には、 TWELITE\_Stage の動作のために、実行の許可やインストールが必要です。

* ツールチェインにはコード署名がなされていますが、コード署名が正しく認証されない場合は、ビルドツールチェイン \(ba-elf-gcc など\) の実行形式一つずつについて、動作許可を求められる場合があります。
* ダウンロードアーカイブには署名しておりません。実行時には、インターネットからダウンロードされたアプリケーションとしてセキュリティ警告が出る場合があります。
* TWELITE\_Stage をインストールしたパスからの実行許可を要求される場合があります。
* ビルド実行時に make ユーティリティのインストールダイアログが出る場合があります。

#### make ユーティリティの追加インストール

場合によっては、make ユーティリティをインストールしなくてはなりません。

{% hint style="warning" %}
macOSバージョンの違いなどにより、別の手順で行う必要があるかもしれません。
{% endhint %}

コマンドライン \(zsh\) から make を実行したときに、エラーが出る場合には Command Line Tools をインストールします。

```bash
$ xcode-select --install
```

インストールが完了したら、make を入力して以下のメッセージの出力を確認します。

```bash
$ make
make: *** No targets specified and no makefile found.  Stop.
```


### シリアルポートの取り扱い

MONOSTICK や TWELITE R には、 FTDI社 \(https://www.ftdichip.com\) の USBシリアル変換IC FT232 シリーズを、TWELITE R2 には FT230 シリーズを搭載しています。これらを利用するために、デバイスドライバのインストールが必要となる場合があります。

TWELITE\_Stage を起動してもシリアルポートが表示されない場合は、FTDI社のドライバをアンロード（無効化）してください。

{% hint style="danger" %}
このユーティリティは MONOSTICK や TWELITE-R を挿入した際にOS標準のデバイスドライバのロードを抑制するだけでなく、同じ USB の ID を持つそれ以外のデバイスについてもデバイスドライバのロードを抑制します。
{% endhint %}

[https://www.ftdichip.com/Drivers/D2XX.htm](https://www.ftdichip.com/Drivers/D2XX.htm) より D2xxHelper をダウンロードできます。
なお、TWELITE STAGE SDKの TWELITE\_Stage/INSTALL フォルダにも同じものを収録しています。

#### 参考：FTDI社デバイスドライバの手動アンロード

FTDI 関連のドライバをアンロードするには、以下のコマンドを実行します。

```bash
$ sudo kextunload -b com.apple.driver.AppleUSBFTDI
```
