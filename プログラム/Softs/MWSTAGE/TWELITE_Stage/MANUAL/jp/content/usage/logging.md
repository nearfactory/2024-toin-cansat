---
title: "ログ機能"
author: "Mono Wireless Inc."
description: "ログ機能"
---
# ログ機能

`Windows` `macOS` `Linux` `RasPi`

TWELITE と PC 間のシリアル通信のログを記録できます。

### ログの開始

`Windows` `macOS` `Linux` `RasPi`

TWELITE と PC 間のシリアル通信のログを記録できます。

Alt(Cmd)+Lキーを押します。

![](<../.gitbook/assets/img_help_ovrly.png>)



### ログ記録の終了

ログ記録中にもう一度 Alt(Cmd) + L キーを押します。

![](<../.gitbook/assets/img_logging_fin.png>)

ログの記録が終了し、その時点のログファイルをOS標準の方法(Windowsならメモ帳、macOS ならコンソール)で開きます。

{% hint style="warning" %}
Raspberry Pi では、ログファイルの保存だけを行います。ファイルを開く機能はありません。
{% endhint %}

### ログの記録

TWELITE から受信した文字列は、そのまま記録されます。

TWELITE に送信した文字列は、１文字ずつ記録されます。Windowsの場合は `｢ ｣`、 macOS/Linux/RaspBerryPiは `« »` で囲われます。

例えば`«t»`とある場合は、キーボードから`t`を入力したことを意味します。


### ログ記録のフォルダとファイル名

`Windows` `macOS` `Linux` `RasPi`

`{TWELITE STAGE APP の実行形式のあるフォルダ}/log` にログ開始時の日時を元にしたファイル名で保存されます。

`Alt(Cmd)`+`Shift`+`L` を押すことで、そのフォルダを開きます。

![](<../.gitbook/assets/img_logging_folder.png>)
