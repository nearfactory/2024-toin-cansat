# コマンドラインでのビルド方法

## コマンドラインによるビルド方法

{% hint style="info" %}
VSCode を利用する場合、VSCode 内からコマンドラインが呼び出されていますので、以下の手順については意識する必要はありません。
{% endhint %}

### 事前準備

コマンドラインの make ユーティリティが実行できるようにしておいてください。

{% hint style="warning" %}
以下の解説では bash コマンドラインを前提に解説します。
{% endhint %}

#### Windows
SDK 添付の BASH (msysによる) を利用します。

{MWSDKインストールフォルダ} にある BASH.CMD をエクプローラから実行してください。Wks_TWELITE ワークスペースをカレントフォルダにした bash コマンドラインが表示されます。ツールチェインへの PATH の TWELITE SDK 向けに設定します。

```bash
user1@MACHINE_NAME   /c/MWSDK/Wks_TWELITE
$ pwd
/c/MWSDK/Wks_TWELITE

$ ls
App_IO  App_Tag  Samp_PER  Samp_Wayback
App_Melody  
...
```

#### macOS
コマンドライン (bash) にて、make を実行した時エラーが出る場合は XCode のインストールを行います。

```c
$ xcode-select --install
```

インストール完了後、make を入力して以下のメッセージが出れば OK です。

```c
$ make
make: *** No targets specified and no makefile found.  Stop.
```

### ビルド

例として Samp_PingPong をビルドしてみます。

最初にビルドプロジェクト Samp_PingPong のビルドフォルダに移動します。

```bash
$ pwd
/c/MWSDK/Wks_TWELITE
$ cd Samp_PingPong/PingPong/Build/
$ pwd
/c/MWSDK/Wks_TWELITE/Samp_PingPong/PingPong/Build/
```



次に clean を実行します。ここでは TWELITE BLUE のビルドで clean を実行する例を挙げます。

```
$ make TWELITE=BLUE clean
../../../../MkFiles/chipsel.mk:36: "WARNING: TWELITE BLUE"

```

{% hint style="info" %}
make を実行する際には TWELITE=BLUE または TWELITE=RED の指定が必ず必要です。

WARNING が出ますが、これは TWELITE BLUE のビルドであることを明示するためで、問題を指摘しているものではありません。

make を実行せずに Build フォルダをクリーン状態にするには、Build フォルダ中の .bin ファイルと objs_\* フォルダを消去します。
{% endhint %}



clean が終われば、ビルドを実行します。

```bash
$ make TWELITE=BLUE
rm -f objs_BLUE/PingPong.o
mkdir -p objs_BLUE
Compiling /c/MWSDK/Wks_TWELITE/Samp_PingPong/Pi..
...
/c/MWSDK/ChipLib/SW4063V1416/../../Tools/ba-elf-ba2-r..
y -S -O binary objs_BLUE/Samp_PingPong_PingPong_BLUE_..
Samp_PingPong_PingPong_BLUE_L1200_V0-1-4.bin

$ ls *.bin
Samp_PingPong_PingPong_BLUE_L1200_V0-1-4.bin
```

{% hint style="info" %}
ビルド実行後 [.bin ファイル](birudonitsuite/binfuiru.md)が出来上がっていれば成功です。ビルドが成功しない場合は、何らかのエラーメッセージが出ています。

中間ファイルは objs\_{ARCH} フォルダに格納されます。
{% endhint %}

