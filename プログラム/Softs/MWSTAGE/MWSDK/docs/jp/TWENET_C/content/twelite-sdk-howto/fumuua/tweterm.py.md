# tweterm.py

本プログラムは pyftdi ([https://github.com/eblot/pyftdi](https://github.com/eblot/pyftdi)) ライブラリサンプルスクリプト pyterm.py に TWELITE 用のファームウェア書き込みスクリプトを組み込んだものです。以下の機能があります。

* TWELITE 用ファームウェアの書き込み　(TWELITE R/MONOSTICK)
* シリアルポートでの動作振る舞いの確認

{% hint style="warning" %}
本スクリプトの OS X での実行には Python3 インタプリタが必要です。コマンドライン環境ならびに Python インタプリタの取り扱いに慣れた方を対象とします。
{% endhint %}

{% hint style="warning" %}
本スクリプトを Linux で動作させるには同等のパッケージ (libusb-dev, pyserial, pyftdi) を用意します。コマンドライン環境ならびに Python インタプリタの取り扱いに慣れた方を対象とします。

* 参考環境：Ubuntu 16.04 (i386 32bit), Python3.5.2 / Ubuntu 18.04 (x86-64 64bit), Python 3.6.5 
{% endhint %}

## 保証・ライセンス

本パッケージ内で、ライセンス上特別な記述のないものは、[モノワイヤレスソフトウェア使用許諾契約書](../../overview/mwsla.md)を適用します。

本ソフトウェアについては、モノワイヤレス株式会社が正式にサポートを行うものではありません。お問い合わせにはご回答できない場合もございます。予めご了承ください。

不具合などのご報告に対してモノワイヤレス株式会社は、修正や改善をお約束するものではありません。

また導入パッケージなどお客様の環境に依存して動作しない場合もございます。

```python
# Copyright (C) 2017 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT)
```

## 動作環境・必要パッケージ

* Mac OS X または Linux
* python3.5 以降
* libusb
* pyserial
* pyftdi

### 開発環境

以下の環境で開発、動作確認を実施しました。ただし、これら環境で動作を保証するものではありません。

| 環境                                      |
| --------------------------------------- |
| Mac OS X 10.11.6, Python3.5.1 (2018/05) |
| Ubuntu 18.04, Python3.6.7 (2018/05)     |
| Mac OS X 10.14.2, Python3.7.2 (2019/01) |

*

## インストール

### パッケージのインストール

#### macOS
以下では、Homebrew を用いた新しく全パッケージをインストールする例をご紹介します。

#### Homebrew

Homebrew をインストールします。

```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

####   Python3

```bash
$ brew install python3
```

####  libusb

```bash
$ brew install libusb
```

#### pyserial

```bash
$ pip3 install pyserial
```

#### pyftdi

```bash
$ pip3 install pyftdi
```

#### Linux
お使いのディストリビューションのパッケージ導入法を調べてください。以下のパッケージが必要です。

* python3.5 以降 (多くの場合導入されています)
* libusb-dev
* pyserial
* pyftdi

パッケージ導入コマンド例。

```bash
$ sudo apt-get install libusb-dev
$ sudo apt-get install python3-pip
$ pip3 install pyserial
$ pip3 install pyftdi
```

### 実行

TWELITE SDK をインストールしたフォルダを `${TWELITESDK}` と記載します。

スクリプトは以下になります。

```bash
${TWELITESDK}/Tools/tweterm/tweterm.py
```

スクリプトに実行許可を与えます。

```bash
$ chmod +x ${TWELITESDK}/Tools/tweterm/tweterm.py
```

必要に応じて環境変数 PATH に追加しておきます。

```bash
$ PATH=${TWELITESDK}/Tools/tweterm:$PATH
```

## 利用方法

### USBドライバのアンロード

{% hint style="warning" %}
libusb と OS のドライバが競合するため、ドライバをアンロードしておきます。
{% endhint %}

#### macOS
FTDI 関連のドライバをアンロードします。

```bash
$ sudo kextunload -b com.apple.driver.AppleUSBFTDI
```

#### Linux
ドライバのアンロードは不要です。

{% hint style="warning" %}
エラーが出る場合は、ドライバをアンロードを試してみてください。

```bash
$ sudo rmmod ftdi_sio
$ sudo rmmod usbserial
```
{% endhint %}

###   コマンドパラメータ

| パラメータ                      | 解説                                                  |
| -------------------------- | --------------------------------------------------- |
| `-p ftdi:///?`   または `-p`  |  デバイス一覧を表示します。                                      |
| `-p [デバイス名]`               | デバイスを指定します。                                         |
|                            | `ftdi:///1`  他にデバイスがない時。                            |
|                            | `ftdi://::MW19ZZUB/1 `シリアル番号による指定。\`                |
| `-b [ボーレート]`               | ボーレートを指定します。                                        |
|                            | `-b 115200`  115200bps を指定。                         |
| `-F [ファームウェア]`             | ファームウェアを書き込みます。                                     |
|                            | `-F App_Twelite.bin` ファイル名が App_Twelite.binを書き込みます。 |
| `--no-color`               | 文字のカラー出力を抑制します。                                     |
| `--no-term`                | ファームウェアの書き込みのみを実施しターミナルを開きません。                      |

### キーボード操作

`Ctrl+C` キーを入力することで制御プロンプトを表示し、いくつかの特別な操作が可能です。それ以外の場合は、直接 TWELITE 無線モジュールに入力文字列が送付されます。 

| 入力キー                              | 解説                                                                  |
| --------------------------------- | ------------------------------------------------------------------- |
| `Ctrl+C Ctrl+C`                   | ターミナルを終了します。                                                        |
| `Ctrl+C Ctrl+R` または `Ctrl+C r`    | TWELITE 無線マイコンをリセットします。                                             |
| `Ctrl+C Ctrl+I` または `Ctrl+C i`    | インタラクティブモードへ遷移する + + + コマンドを入力します。インタラクティブモード対応ファームウェアのみ有効です。       |
| `Ctrl+C A`                        | 書式の解釈を開始します。TWELITE無線マイコンからの出力とキー入力に対して、アスキー形式の解釈を行います。             |
| `Ctrl+C B`                        | 書式の解釈を開始します。TWELITE無線マイコンからの出力に対してバイナリ形式の解釈を行います。キー入力はアスキー形式で解釈します。 |
| `Ctrl+C N`                        | 書式の解釈を停止します。                                                        |

{% hint style="info" %}
書式解釈中は TWELITE からの電文は解釈できた電文のみ表示し、キーボードの入力はエコーバックされますが、アスキー形式の電文が完成した時に TWELITE    に送付されます。
{% endhint %}

## \[実行例]

実行例中では、適宜改行を挟んでいます。

{% hint style="warning" %}
エラーが発生する場合は、シリアルポートの権限の問題かもしれません。root権限(sudo等)で実行します。
{% endhint %}

#### ＜最初にデバイスがリストされるかを確認します＞

```bash
$ tweterm.py -p ftdi:///?
Available interfaces:
  ftdi://ftdi:232:MW19ZZUB/1   (MONOSTICK)
  Please specify the USB device
```

#### ＜ファームウェアを書き込んでターミナルを起動します＞

以下の例では App_UART (UART 通信アプリ) を書き込み、起動メッセージを確認しています。

```bash
$ tweterm.py -p ftdi://ftdi:232:MW19ZZUB/1 -b 115200 -F ../App_Uart_Master_RED_L1101_V1-2-15.bin 
*** TWE Wrting firmware ... ../App_Uart_Master_RED_L1101_V1-2-15.bin
MODEL: TWEModel.TWELite
SER: 102eebd

 file info: 0f 03 000b
erasing sect #0..#1..#2..
0%..10%..20%..30%..40%..50%..60%..70%..80%..90%..done - 10.24 kb/s
Entering minicom mode

!INF TWE UART APP V1-02-15, SID=0x8102EEBD, LID=0x78
8102EEBD:0> 
```



#### ＜Ctrl+Cを入力すると制御プロンプトが表示されます＞

```
*** r:reset i:+++ A:ASCFMT B:BINFMT x:exit>
```



#### ＜続けて i を入力します。インタラクティブモードに入ります＞

```
*** r:reset i:+++ A:ASCFMT B:BINFMT x:exit>[+ + +]
--- CONFIG/TWE UART APP V1-02-15/SID=0x8102eebd/LID=0x00 -E ---
 a: set Application ID (0x67720103) 
 i: set Device ID (121=0x79) 
... ＜省略＞ 
```

通常通り + + + と３回入力しても同じ結果になります。   



#### ＜書式の解釈の例：App_UART をバイナリ形式に設定しておきます＞

インタラクティブモードで  `m` `B` `Enter`  `S`  と順に入力します。   

```
--- CONFIG/TWE UART APP V1-02-15/SID=0x8102eebd/LID=0x00 -E ---
 a: set Application ID (0x67720103) 
 i: set Device ID (121=0x79) 
 c: set Channels (18) 
 x: set RF Conf (3) 
 r: set Role (0x0) 
 l: set Layer (0x1) 
 b: set UART baud (38400) 
 B: set UART option (8N1) 
 m: set UART mode (B)*
 h: set handle name [sadkasldja] 
 C: set crypt mode (0) 
 o: set option bits (0x00000000) 
---
 S: save Configuration
 R: reset to Defaults
 
!INF Write config Success
!INF RESET SYSTEM...
```



#### ＜書式の解釈の例＞

以下の例ではApp_UARTがバイナリ形式で入出力を行ないます。`Ctrl+C B` を入力します。 

```
*** r:reset i:+++ A:ASCFMT B:BINFMT x:exit>
[FMT: console ASCII, serial BINARY]
```

この状態では入出力は書式形式となります。キーボードの入力はアスキー形式、 TWELITE からの電文はバイナリ形式として解釈します。



TWELITE からの電文を受け取る例を示します。一番簡単な方法は TWELITE をリセットします。`Ctrl+C r`  を入力します。 

```
*** r:reset i:+++ A:ASCFMT B:BINFMT x:exit>[RESET TWE]
[dbf1677201030001020f008102eebd0000]
```

出力された `[dbf...]` がTWELITEからの電文で実際は`0xdb 0xf1 0x67...` と続くバイナリ列になります。  



反対に TWELITE に電文を送る場合はアスキー書式で入力します。`:7800112233AABBCCDDX` と入力します。 ここでは ペイロードが 0x7800112233AABBCCDD のデータをバイナリ形式で TWELITE に送付しています。直後に応答として `[dba18001]` が戻ってきています。

```
:7800112233AABBCCDDX[dba18001]
```



#### ＜終了します＞

`Ctrl+C Ctrl+C` を入力します。 

```
*** r:reset i:+++ A:ASCFMT B:BINFMT x:exit>[
[EXIT]
Bye.
```
