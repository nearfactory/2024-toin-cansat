---
title: "Linux"
author: "Mono Wireless Inc."
description: TWELITE_Stage インストール (Linux)
---

# Linux

`Linux`

{% hint style="warning" %}
TWELITE R2 のUSBデバイスIDが従来の0403:6001から0403:6015に変わっています。udevの設定追加が必要です。
{% endhint %}

{% hint style="danger" %}
32bit版は用意しておりません。
{% endhint %}

{% hint style="warning" %}
Linux環境はディストリビューションやバージョンによって、用意されているパッケージの種類が異なります。個別にパッケージ等のインストールや設定が必要になる場合があります。

エラーメッセージ等を手がかりに一般の情報を参照いただくようお願いいたします。
{% endhint %}

### 環境

TWELITE STAGE は以下の環境で開発・動作確認しています。

* Ubuntu 16.04, 18.04, 20.04
* NNLinux Beta8 64bit
* Centos7


### シリアルポートの取り扱い

TWELITE STAGE から MONOSTICK や TWELITE-R を認識するには、ftdi\_sioモジュールをアンロードし、USBデバイスに対して読み書き権限を与える必要があります。

{% hint style="info" %}
USBデバイスのIDを以下に示します。

* ベンダーID 0x0403
* プロダクトID 0x6001\(MONOSTICK,TWELITE R\) または 0x6015 \(TWELITE R2\)
{% endhint %}

なお、この設定を自動化するための udev の設定スクリプト\(Ubuntu, CentOS\) を用意しています。

`/etc/udev/rules.d` に定義をコピーして、設定をリロードします。
設定後は USB デバイスを抜き差ししてから `TWELITE_Stage.run` を実行してください。起動直後の画面で USB デバイスが表示されたなら、設定が反映されています。


#### Ubuntu 16.04, 18.04. 20.04

```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/ubuntu/
$ sudo ./set_udev_sudo.sh
```

定義ファイル（読みやすいように改行しています）

```text
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001",
   MODE="0666",
   RUN+="/bin/sh -c 'rmmod ftdi_sio && rmmod usbserial'"
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015",
   MODE="0666",
   RUN+="/bin/sh -c 'rmmod ftdi_sio && rmmod usbserial'"
```



#### Centos 7

```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/centos/
$ sudo ./set_udev_sudo.sh
```

定義ファイル（読みやすいように改行しています）

```text
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001",
   MODE="0666",
   RUN+="/bin/sh -c '/usr/sbin/rmmod ftdi_sio'"
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015",
   MODE="0666",
   RUN+="/bin/sh -c '/usr/sbin/rmmod ftdi_sio'"
```



### アプリケーションの登録

必要に応じて、お使いのデスクトップ環境に合った方法でアプリケーションを登録してください。

{% hint style="info" %}
デスクトップ上のファイルアイコンから、`TWELITE_Stage.run` を実行できない場合があります。
これは、システムが `TWELITE_Stage.run` を実行形式として認識できない場合があるためです。
{% endhint %}


#### Ubuntu 16.04, 18.04, 20.04

Ubuntu用の定義ファイル生成スクリプトを用意しています。

```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/ubuntu/
$ ./make_launch_icon.sh
```

このスクリプトは `.desktop` ファイル（アプリ定義）を`$HOME/.local/share/applications` に作成します。

スクリプト実行後に、アプリケーション一覧に TWELITE STAGE のアイコンが追加されます。
