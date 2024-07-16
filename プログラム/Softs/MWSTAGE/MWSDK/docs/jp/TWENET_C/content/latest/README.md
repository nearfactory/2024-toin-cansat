# 最新版の取得

## TWELITE STAGE

{% hint style="info" %}
TWELITE NET 1.3.0 (2020/04) 以降は TWELITE STAGE として配布されます。

```
*** TWELITE STAGE フォルダ構成 ***
{STAGE TO FOLDER}/
  TWELITE_Stage.exe            -- STAGE 実行形式(Windowsの場合)
  dll/                         -- STAGE 実行用(Windowsの場合)
  TWELITE_Stage/               -- STAGE 関連ファイル
  BIN/                         -- .binファイル格納フォルダ
  
  MWSDK/                       -- TWELITE SDK(MWSDK)トップ
```
{% endhint %}



### リリース版

リリース版は、以下よりダウンロードしてください。MWSDK はTWELITE STAGEの配布物の一部となります。

{% embed url="https://mono-wireless.com/jp/products/stage/index.html" %}



## 正式リリース

{% hint style="info" %}
正式リリースには名前が付きます。名前の書式は `MWSTAGE2020_05` のように MWSTAGEに続いて {年}\_{月} を表記します。また TWELITE Stage ツールを含まない TWELITE SDK (MWSDK) のバージョンは、同様に `MWSDK2020_05` のように MWSDK に続いて  {年}\_{月} です。

リリース名に `UNOFFICIAL `が付随する場合は「リリース間の更新」に対しての命名です。
{% endhint %}

以下より配布します。

> [https://mono-wireless.com/stage](https://mono-wireless.com/stage)



## リリース間の更新

リリース後に発見された問題や、サンプル等の更新などは、以下のGitHubリポジトリ（ソースコードやファイルを履歴を含めて格納したデータベース）に反映されます。

* [MWSDK_COMMON](https://github.com/monowireless/MWSDK_COMMON) - MWSDK の共通部をまとめたレポジトリ。サブモジュールを用いているためGitの取り扱いに習熟している必要があります。
* [MWSDK_COMMON_SNAP](https://github.com/monowireless/MWSDK_COMMON_SNAP) - MWSDK の共通部を一括ダウンロードできるようにしたレポジトリ。zip形式でダウンロードできます。

{% hint style="warning" %}
これらの修正は中間的な内容、所定の動作確認などが省略されたものも含まれます。確認されていない副作用があったり、構成間の整合性が取れていない場合もあります。利用の可否はお客様のほうで判断いただくようお願い申し上げます。

原則としてリリース後の修正に対してのサポート対応は行っておりません。技術サポートは、リリース版をもとに依頼いただくようお願いいたします。

また、リリース後の修正に対して、お客様のほうで不都合など発見された場合は、お手数ですが技術サポートにご連絡頂けますと幸いです。

当サポートでは Git ならびに GitHub の利用方法についてのご質問はお受けできません。一般の解説などを参考いただくようお願い申し上げます。また、GitHubのサービスに対しては当社では対応できない場合もございます。
{% endhint %}



### MWSDK_COMMON_SNAP レポジトリ

> [https://github.com/monowireless/MWSDK_COMMON_SNAP/releases](https://github.com/monowireless/MWSDK_COMMON_SNAP/releases)

リリースごとのMWSDKファイル（共通部分）を一括格納しています。MWSDKの更新方法は、このリポジトリから zip ファイルをダウンロードして、フォルダやファイルを更新前のものと差し替えます。



リンク先には各リリース（暫定版はUNOFFICIAL）でのファイル一式へのリンクが含まれます。

![GitHub画面例](<../.gitbook/assets/image (55).png>)

レポジトリには以下のフォルダが含まれます。

* ChipLib
* LICENSE
* MkFiles
* TWENET

この４つのフォルダをTWELITE STAGEの配布フォルダ内で**差し替え**ます。

```
.../MWSTAGE/              --- TWELITE STAGE 配布フォルダ
        .../MWSDK         --- MWSDKフォルダ
              .../ChipLib <-- 以下の４フォルダを差し替える
              .../LICENSE
              .../MkFiles
              .../TWENET
```



### MWSDK_COMMON レポジトリ

> [https://github.com/monowireless/MWSDK_COMMON](https://github.com/monowireless/MWSDK_COMMON)

このリポジトリにはMWSDKの共通部分のファイルを管理していますが、上述の MWSDK_COMMON_SNAP ではリリースごとのファイル差分のみに注目しているのに対して、各ライブラリやサンプルなどのレポジトリをサブモジュールとして、個別に更新履歴が参照できるようになっています。

{% hint style="warning" %}
zip ファイルによるダウンロードを行った場合、サブモジュールに対尾するフォルダにはファイルが格納されません。 (GitHubの仕様です)
{% endhint %}



#### リポジトリ利用について

* GITのsubmoduleを用いています。GitHubのサイトから.zip形式などでアーカイブをダウンロードした場合、submoduleの含まれるファイルは含まれません。(submodule付きのクローンを行うgitコマンド例は後述)
* MWSDK_COMMON リポジトリは、配布の MWSDK フォルダから Tools フォルダと一部ファイル(Windows版の環境変数登録用の cmd ファイル)を除外したものです。Tools フォルダは配布済みのものを利用してください。



#### リポジトリからクローンする方法 (MWSDK 2020/04 以降)

ここでは、コマンドラインにて MWSDK の内容を GitHub からクローンする方法を記述します。あらかじめ git コマンドが利用できる環境を用意しておいてください。Windowsを利用の場合は bash を用いて作業することを想定します。



まず最初に配布のMWSDK(TWELITE STAGE)を展開しておきます。以下のような構成になっていると思います。

```
=== フォルダ構成 ===
.../MWSTAGE +- MWSDK +- TWENET
                      - ChipLib
                      - ...
                      - Tools               
```



配布時の MWSDK フォルダを別の名前に変更します。

```
$ cd .../MWSTAGE    ...はお客さまのインストールフォルダ
$ mv MWSDK MWSDK.orig

=== フォルダ構成 ===
.../MWSTAGE +- MWSDK.ORIG
                  +- TWENET
                   - ...
                   - Tools 
```



MWSTAGE フォルダ上で GitHubより MWSDK_COMMON をクローンします。submodulesのファイルを取得するため`--recurse-submodules`オプションを追加します。

```
$ pwd
.../MWSTAGE
$ git clone --recurse-submodules https://github.com/monowireless/MWSDK_COMMON MWSDK
...(少し時間がかかります)

=== フォルダ構成 ===
.../MWSTAGE +- MWSDK
                  +- TWENET
                   - ... (Tools は含まれない)
             - MWSDK.ORIG
                  +- TWENET
                   - ...
                   - Tools 
```



最後に MWSDK.ORIG/Tools フォルダを MWSDK に移植します。

```
$ mv MWSDK.ORIG/Tools MWSDK/

=== フォルダ構成 ===
.../MWSTAGE +- MWSDK
                  +- TWENET
                   - ...
                   - Tools 
             - MWSDK.ORIG
                  +- TWENET
                   - ... (mv したのでTools は含まれない)
```



## TWELITE MWX ライブラリ SDK (2019/12)

MWXライブラリ専用の初版SDKです。

MWXによる開発は、最新版の TWELITE STAGE を利用ください。



## TWELITE NET 1.2.0 (2018/05) 以前

以下のリンクを参照ください。

{% embed url="https://mono-wireless.com/jp/products/TWE-NET/index.html" %}

