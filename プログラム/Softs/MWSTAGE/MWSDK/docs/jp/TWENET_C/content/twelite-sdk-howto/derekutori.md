# フォルダ構成

TWELITE SDK は、インストール先のフォルダを基準にして、所定の場所にソースコードなどプロジェクトファイルを格納するように用意されています。

{% hint style="danger" %}
SDKインストールフォルダを含め、フォルダ名・ファイル名は英数文字 **`a-z A-Z 0-9 _ -`**と拡張子を区切る **`.`** ピリオドとし、空白・その他特殊記号や多バイト圏文字列（日本語など）が含まないようにしてください。
{% endhint %}

| フォルダ                   | 内容                      |
| ------------------------ | ----------------------- |
| `.../MWSDK`              | TWELITE SDK の最上位フォルダ。 |
| `.../MWSDK/ChipLib`      | 半導体ライブラリ                |
| `.../MWSDK/License`      | モノワイヤレスソフトウェア仕様許諾書      |
| `.../MWSDK/MkFiles`      | makefile ライブラリ          |
| `.../MWSDK/Tools`        | toolchain、書き込みツールなど     |
| `.../MWSDK/TWENET`       | TWELITE NET ライブラリ       |
| `    .../????_libTWENET` | libTWENETUtils ソースコード   |
| `.../MWSDL/Wks_TWELITE`  | アプリ・サンプルのワークスペース        |

### Wks_TWELITE ワークスペースについて

{% hint style="danger" %}
ワークスペースを格納するフォルダはMWSDK 直下で、かつ、後述する Makefile の相対フォルダも変更してはいけません。

`.../MWSDK/{ワークススペースフォルダ}/{プロジェクトフォルダ}/{構成フォルダ}/Build/Makefile `
{% endhint %}

プログラムをビルドするためのワークスペースには、複数のプロジェクトが格納されています。Wks_TWELITE 内に格納されるプロジェクトは各々個別のアプリケーションをビルドするものです。

```
.../MWSDK/Wks_TWELITE/ProjectName1/...
.../MWSDK/Wks_TWELITE/ProjectName2/...
...
```

プロジェクト内は以下のような構成になっています。

```
.../ProjectNameX/
    Variant1/          : 構成１(var1)のビルド
       Source/         : var1 特有のソースコード
       Build/          : 生成バイナリの格納場所
           Makefile    : var1 の生成ルール
           Version.mk  : var1 のバージョン情報
    Variant2/...       : 構成2
    Common/            : 構成間共通のソースなど
```

* 同じプロジェクトでも、構成が一つのみの場合もあれば、親機・子機・中継機と構成が別れたソースコードになる場合もあります。



