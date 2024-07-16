---
title: "環境変数について"
author: "Mono Wireless Inc."
description: "環境変数について"
---

# 環境変数について

TWELITE STAGE APP では、内部的に環境変数を設定して make などのビルドプログラムの動作を行います。事前に環境変数の設定は不要です。



### 内部的に設定される環境変数

|  環境変数   |  解説   |
| --- | --- |
| `MWSDK_ROOT`          | 標準では TWELITE STAGE APP の実行形式が格納されるフォルダにある `MWSDK` フォルダ(つまり`../MWSTAGE/MWSDK`)が指定されます。 `MWSDK.ini`が指定される場合は、指定されたフォルダ名を採用します。                                                                                                                                                                     |
| `MWSDK_TWENET_LIBSRC` | サンプルコードやTWELITE APPSのソースコードフォルダには、Microsoft社の Visual Studio Code (VS Code) 用の定義ファイルを予め作成しています。この定義ファイル中にVS Codeエディタ中でコード解釈を行う目的でライブラリソースコードの参照先を指定しますが、この環境変数を用いています。<br />`MWSDK_TWENET_LIBSRC`環境変数が適切に指定されると、MWSDK以下ではないプロジェクトフォルダでもコード解釈が行われ、ライブラリ関数名の補完などが機能します。（参考１） |
| `LANG=C`              | ツールチェインのメッセージを規定の言語(英語)にするため、明示的に設定しています。                                                                                                                                                                                                                                                        |
| `PATH`                | Windowsでは、SDK添付のmsysユーティリティへのPATHを追加します                                                                                                                                                                                                                                                          |
| `MWSDK_MAKE_JOBS` `MWSDK_MAKE_DISABLE_LTO`      | VS Codeの設定定義で利用します。<br>`JOBS` : STAGE APPで設定された並列ビルド数を渡します<br>`DISABLE_LTO` : LTOを無効化します(`Windows`)|


#### 参考１

`.vscode/settings.json` の設定例（抜粋）

```
    "C_Cpp.default.includePath": [
        "${env:MWSDK_TWENET_LIBSRC}/include/**",
        "${env:MWSDK_TWENET_LIBSRC}/src/**"
    ],
    "C_Cpp.default.browse.path": [
        "${env:MWSDK_TWENET_LIBSRC}/include/**",
        "${env:MWSDK_TWENET_LIBSRC}/src/**"
    ],
```

"`../../`"で始まる定義は、TWELITE STAGEアプリからプロジェクトを開く場合は不要です。環境変数`MWSDK_TWENET_LIBSRC`を設定しない場合に、既定のフォルダ構成の時にソース参照先を指定しています。
