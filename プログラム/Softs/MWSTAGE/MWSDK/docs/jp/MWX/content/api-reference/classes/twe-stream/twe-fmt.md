---
title: "format (mwx::mwx_format)"
author: "Mono Wireless Inc."
description: "mwx::stream に printf の書式を入力"
---

# format (mwx::mwx\_format)

`mwx::stream` の << 演算子に対してフォーマット書式を書き出すヘルパークラスです。ライブラリ内では `Using format=mwx::mwx_format;` として別名定義しています。

```cpp
Serial << format("formatted print: %.2f", (double)3123 / 100.) << mwx::crlf;

// formatted print: 31.23[改行]
```

{% hint style="warning" %}
可変数引数リストに登録できる引数は最大８つです。doubleやuint64\_t型など64bitのパラメータが含まれる場合は引数の数が制限されます。制限を超えた場合はstatic\_assertによるコンパイルエラーになります。
{% endhint %}

* コンストラクタで受け取った引数リストを、パラメータパックの展開機能を用いてクラス内部変数に格納する
* `operator <<` が呼び出された時点で、`fctprintf()` を呼び出し、ストリームにデータを書き出す



### コンストラクタ

```cpp
format(const char *fmt, ...)
```

コンストラクタでは、書式のポインタとパラメータを保存します。続く `<<`演算子による呼び出しでフォーマットを解釈して出力処理を行います。

| パラメータ | 解説                                                                                                          |
| ----- | ----------------------------------------------------------------------------------------------------------- |
| `fmt` | フォーマット書式。TWESDK/TWENET/current/src/printf/README.md 参照                                        |
| `...` | フォーマット書式に応じたパラメータ。<br>※ 最大数は４で、５つ以上のパラメータではコンパイルエラーとなる。※ 書式との整合性はチェックしないため、不整合な入力に対しては安全ではない。 |

{% hint style="warning" %}
`fmt`は本オブジェクトが破棄されるまで、アクセス可能であることが必要です。
{% endhint %}



