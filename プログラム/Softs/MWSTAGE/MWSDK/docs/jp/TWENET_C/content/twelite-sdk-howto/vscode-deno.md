# VSCode での利用

VSCode では、コード解釈 ([IntelliSense](https://code.visualstudio.com/docs/editor/intellisense)) 機能があるため、コード編集が効率的に行えます。またタスク機能により[ビルドコマンド](komandoraindenobirudo.md)の実行を VSCode から行うことができます。

## プロジェクトを開く

TWELITE のアプリをビルドするためにプロジェクトを Visual Studio Code で開く必要があります。\
プロジェクトを開く手順は下記の通りです。

![メニューバーの "ファイル" → "フォルダーを開く"を選択します。](<../.gitbook/assets/image (43).png>)

![開きたいプリジェクトのフォルダを選択します。](<../.gitbook/assets/image (44).png>)

![エクスプローラー上に開いたプロジェクトのフォルダが追加されます。](<../.gitbook/assets/image (45).png>)



## アプリをビルドする

アプリのビルドは下記の手順で行います。VSCode 上から make ユーティリティを実行しています。make の詳細については「[ビルド方法](komandoraindenobirudo.md)」や「[Makefileについて](birudonitsuite/makefile-nitsuite.md)」を参照してください。

例えば、超簡単！TWELITE標準アプリをビルドする場合、VSCode のタスク”Build App_Twelite_Master (BLUE)” を選択します。

![メニューバーの”タスク” → ”タスクの実行” を選択します。](<../.gitbook/assets/image (47).png>)

![右図の赤枠内よりビルドしたい定義を選択します。](<../.gitbook/assets/image (48).png>)

![ターミナルより右図の赤枠内のような出力がされればビルド成功です。](<../.gitbook/assets/image (49).png>)

{% hint style="info" %}
VSCode からはコマンドラインの make を実行しているため、エラー等もコマンドラインのエラーメッセージが出力されます。
{% endhint %}

## コード解釈を切り替える

TWELITE BLUE 用のコード解釈と TWELITE RED 用のコード解釈を切り替えることができます。 コード解釈を切り替えるには図の示す場所をクリックして開発対象にあった TWELITE のモデルを選択してください。

{% hint style="info" %}
TWELITE モジュールの種別ごとに特有の C 定義を切り替えます。

コード解釈とコンパイラ解釈が完全に一致することを保証するわけではありません。またヘッダファイルの記述などにより一部解釈できない定義も発生します。
{% endhint %}

![コード解釈を切り替えます。](<../.gitbook/assets/image (46).png>)

{% hint style="info" %}
TWELITE モジュールの種別ごとに特有の C 定義を切り替えます。
{% endhint %}

{% hint style="warning" %}
コード解釈とコンパイラ解釈が完全に一致することを保証するわけではありません。またヘッダファイルの記述などにより一部解釈できない定義も発生します。

読み込むヘッダファイルは、コンパイラやライブラリのヘッダファイルを直接参照するわけではなく {SDKフォルダ}/Tools/VSCode/include　以下のコピーを参照します。
{% endhint %}

## JSON ファイルについて

上記のビルドやコード解釈については JSON ファイルに定義があります。

### .vscode/c_cpp_properties.json

ビルド時の暗黙定義などの定義やインクルードフォルダの指定を行っています。

追加の定義が必要な場合は、すでにあるファイル中の `"name" : "TWELITE_BLUE"` または `"name" : "TWELITE_RED"`_ _のエントリを複製または編集して使用します。

編集が必要になるのは `"name"` `"includePath"` などです。 特有のビルド定義が必要な場合は  `"defines"` にエントリを追加します。

### .vscode/tasks.json

ビルドタスクの定義を行なっています。

新たにビルド定義を追加する場合は、すでにあるエントリをコピーして編集します。各ビルドタスクには windows, linux, osx 用のエントリーがあります。

編集が必要になるのは `"label"` `"options" の "cwd"` などです。

```bash
"tasks": [
{ 
  "label": "Build Samp_PingPong,PingPong (BLUE)",
  "type": "shell",
  ..
  "windows": {
     "command": "make",
     "args": [ "TWELITE=BLUE",  "clean", "all" ],
     "options": { "env": { "PATH": "${workspaceRoot}\\..\\..\\Tools\\MinGW\\msys\\1.0\\bin" } }
  },
  "problemMatcher": { "base": "$gcc", "fileLocation": [ "absolute" ] },
  "options": { "cwd": "${workspaceRoot}/PingPong/Build" }
},
..

```

{% hint style="info" %}
* `"options": { "cwd" ... } `の設定でビルドフォルダを指定しています。
* make 引数を追加したい場合は、_ _`"args"`_ _の `TWELITE=???"` の後にエントリを追加します。
{% endhint %}

#### 既存のビルドタスクに設定する

以下の例のように`group": { "kind": "build", "isDefault": true } `  を追加します。そのタスクがデフォルトのビルド定義になり、\[タスク]>\[ビルドタスクの実行] やショートカットキーによる実行ができるようになります。

```bash
"tasks": [
{ 
  "label": "Build Samp_PingPong,PingPong (BLUE)",
  "type": "shell",
  ..
  "options": { "cwd": "${workspaceRoot}/PingPong/Build"},
  "group": { "kind": "build", "isDefault": true }
},
..
```
