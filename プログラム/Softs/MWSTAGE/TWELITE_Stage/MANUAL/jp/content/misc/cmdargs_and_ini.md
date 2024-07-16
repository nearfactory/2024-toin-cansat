---
title: "コマンドライン引数と ini ファイル"
author: "Mono Wireless Inc."
description: "コマンドライン引数と ini ファイル"
---

# コマンドライン引数と ini ファイル

## コマンドライン引数
コマンドライン引数は、TWELITE STAGE APPのいくつかの細かい設定を行います。

| コマンドライン引数 | 内容 |
| --- | ---- |
| `-E 0` | フェードアウトのようなグラフィカルな効果を無効にする。 |
| `-R {type}` | `{type}` 値でレンダリングタイプを設定します。<br>`0`: デフォルト<br>`1`: OpenGL<br> `2`: DirectX(Windows) Metal(macOS)<br> `3`: ソフトウェア |
| `-J` | ゲームコントローラーを有効にします。|
|`-x {x_pos}`, `-y {y_pos}` | 起動時のTWELITE STAGE Appのグラフィカルウィンドウの位置を設定します。`{x_pos}`と`{y_pos}`はウィンドウの左上のスクリーン座標です。|


## iniファイル

iniファイルはTWELITE STAGE APPの基本的な設定(MWSDKのフォルダを参照するなど)を行うために使用されます。

iniファイル名は{TWELITE STAGE APPの実行ファイルのベース名} + `.ini` です。
通常は `TWELITE_Stage.ini` となります。

```
;;; MWSDKの参照を変更します。
; MWSDK=MWSDK
mwsdk=mwsdk2020_10

;;; インターフェース言語
; LANG=en

;;; ウィンドウのジオメトリ
GEOM_X=200
GEOM_Y=100
```

### シンタックス
* `ini` ファイルはプレーンテキストファイルとして記述される。
* キーと値は `=` で区切られた1行に格納される (例: `KEY=value`)。
* キーと値の文字列は行頭から始まる(キーの前に空白や他の文字は許されない)。
* キーと値の間にスペースを入れてはならない。
* コメント行は `;` または `#` を行頭に追加する。

### 設定

| キー | 値 |
| --- | ---- |
| `MWSDK` | MWSDKのフォルダを変更する。デフォルトのフォルダは、TWELITE STAGE APPの実行ファイルが置かれているのと同じフォルダにある `MWSDK` です。古いMWSDKやカスタムMWSDKを使用する必要がある場合は、そのフォルダの名前を指定することができます。|
| `LANG` | `LANG=en` は、ユーザーインターフェースの言語をデフォルト(日本語)から英語に変更します。|
| `GEOM_X`, `GEOM_Y` | TWELITE STAGEアプリのウィンドウが表示される場所を変更する。

### 異なる設定の TWELITE STAGE APP を実行する

異なる設定の TWELITE STAGE APP が必要な場合は、TWELITE STAGE APPと同じフォルダに実行ファイルをコピーして、同じ名前の `.ini` ファイルを作成します。

例えば、英語のインターフェースを使用する場合、`TWELITE_Stage.exe`(注: .exe はWindowsの実行ファイルの拡張子)を `TWELITE_Stage_en.exe` にコピーして、 `LANG=en` の設定を `TWELITE_Stage_en.ini` に書き込むことで、英語のインターフェースを有効化した実行ファイルを作成できます。

```
  TWELITE_Stage.exe
  TWELITE_Stage.ini | 特別な設定なし

  TWELITE_Stage_ja.exe | TWELITE_Stage.exe のコピー
  TWELITE_Stage_en.ini | LANG=en が設定されている。
```
