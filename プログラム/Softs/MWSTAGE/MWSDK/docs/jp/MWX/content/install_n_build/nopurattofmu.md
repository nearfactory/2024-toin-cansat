---
title: "他のプラットフォーム"
author: "Mono Wireless Inc."
description: "他のプラットフォーム"
---

# 他のプラットフォーム

他のプラットフォームでも一部の機能([serparser](../api-reference/classes/ser\_parser.md), [pktparser](../api-reference/classes/pktparser/), コンソール用Serialオブジェクト)をビルドできるように、ビルド定義を用意しています。必要なファイルのみを切り出しています。

ビルド定義は`{mwxライブラリ格納}/stdio`フォルダに格納しています。ビルド方法は[README.md](https://github.com/monowireless/mwx/tree/master/stdio)(リンクはGitHub上)を参照してください。

* C++11でのコンパイルが出来ること。
* C++11の標準ライブラリヘッダが利用できること (utility, algorithm, functional, iteratorなど)
* new/delete/virtualは使用しません。
* newによるメモリ確保は例外的に使用する場合があります。
  * serparser/pktparserでnew演算子を利用する[`alloc_heap`](../api-reference/classes/alloc.md)では`delete`による処理を行っています。
  * （参考） ただしmwxライブラリとしては`delete`については考慮しない前提で設計されている部分もあります。

