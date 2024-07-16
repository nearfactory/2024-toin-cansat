---
title: "000desc.txt"
author: "Mono Wireless Inc."
description: "000desc.txt"
---
# 000desc.txt

プロジェクトフォルダに`000desc.txt`が作成した場合には、TWELITE STAGE APP が、プロジェクトフォルダの一覧にその内容を表示します。

![000desc.txtの表示例](../.gitbook/assets/img_fimprog_act_sel.png)

ファイルは UTF-8 形式のプレーンテキストで記述します。書式は以下の２種類があります。



### 書式1

```text
スイッチを押した時にLEDを点灯
act4はTWELITE DIPに接続されたスイッチを押した時にLEDを点灯させるactを動作させます。
https://mono-wireless.com/jp/products/act/index.html
```

* 1行目はタイトル行です。
* 2行目以降は詳細の記述です。
* 最終行が http で始まる場合は、ウェブサイトへのリンクになります。



### 書式２

```text
[JAPANESE]
TITLE=Actのテンプレート
DESC=中身が何もない setup(), loop() のみのファイルです。
新しく Act を記述するのに利用してください。
URL=jp/MWX/content/act_samples/README.html
[ENGLISH]
TITLE=Act empty template
DESC=This act file only contains empty setup() and loop(),
which is intended to write a new Act.
URL=en/MWX/content/act_samples/README.html
```

iniファイルのような書式です。行頭から始まる項目名と`=`文字までを項目の定義として`=`以降が項目の内容です。

| 項目定義 | 詳細 |
| :--- | :--- |
| `[JAPANESE]`, `[ENGLISH]` | ブロックの区切り |
| `TITLE=` | タイトル行 |
| `DESC=` | 詳細の記述。改行を含めて複数行にすることもできます。 |
| `URL=` | ウェブサイトまたはファイルへのリンク |

#### URL 指定について

| `URL=` | 詳細 |
| :--- | :--- |
| `https:`, `http:` で始まる | そのアドレスを開きます |
| それ以外 | `{MWSDK_ROOT}/docs/` を起点とした相対フォルダを指定します。`a/b/c.html` とした場合は `{MWSDK_ROOT}/docs/a/b/c.html` に変換されます。 |
