# Version.mk について

Version.mk ファイルは、各ターゲットの Source フォルダに格納し、Makefile から参照され、出力ファイル名のバージョンなどの決定に使用されます。

以下のように指定します。

```
VERSION_MAIN = 1 	← 主バージョン(0-255)
VERSION_SUB  = 3	← 副バージョン(0-255)
VERSION_VAR  = 10	← ビルドなどの派生(0-255)
```

Makefile は Version.mk から以下のコンパイルオプションを追加します。

上記の例ではコンパイル時に -DVERSION_MAIN=1 -DVERSION_SUB=3 -DVERSION_VAR=10 を追加します。

{% hint style="warning" %}
SDK 2013/11版までは Version.h が生成されましたが省略されます。ソース中で Version.h をインクルードしている場合は、Version.h を空ファイルにする、またはインクルードしないようにしてください。
{% endhint %}

