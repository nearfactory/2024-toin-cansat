---
title: "ビルド定義 Makefile"
author: "Mono Wireless Inc."
description: "ビルド定義 Makefile"
---

# ビルド定義 Makefile

Makefileはbuild/Makefileに格納されています。makeコマンドを実行することで、アクトをビルドするよう予め定義されています。

{% hint style="info" %}
**MWSDK 2020-04 では、プロジェクトフォルダ中の .cpp ファイルを自動で検出するため、通常は Makefile の修正は不要です。**

**ソースファイルをサブフォルダに格納するような場合は、編集が必要になります。**

**MWSDK 2019-12では、.cpp ファイルが複数ある場合は、Makefile の編集が必要です。**
{% endhint %}

{% hint style="warning" %}
プロジェクトフォルダを他の環境からコピーしたあとには、必ず`build/objs_???`フォルダを削除してください。他の環境での中間ファイルが残っているとmakeがエラーになります。

`(MWSDK 2020-04) `USE\_APPDEPS=0 を付加して clean してから、改めて make コマンドを実行することでエラーを回避できます。

`$ make USE_APPDEPS=0 TWELITE=BLUE clean`\
`...`\
`$ make TWELITE=BLUE`

``
{% endhint %}

## makeのパラメータ

### TWELITE=

ビルド対象をBLUEまたはREDで指定します。TWELITE BLUEなら`make TWELITE=BLUE`と指定します。

### all

ビルドを実行します。通常は省略して`make TWELITE=BLUE`のように実行します。



### clean

 ビルドの中間ファイルを削除します。`make TWELITE=BLUE clean`のように実行します。



### cleanall

すべての中間ファイルを削除します。`make cleanall`のように実行します。buildフォルダのobjs\_???フォルダをすべて削除するのと同じです。



### USE\_APPDEPS=0 または 1

1 (デフォルト) を設定すると、ファイルの依存関係をもとに、ビルドファイルを決定します。例えば、ヘッダファイルに変更があった場合に関連するソースファイルが再コンパイル対象となります。

0 では依存関係を評価しません。0 に設定した場合、矛盾ある中間ファイルが残っていても makefile がエラーになりません。



## Makefile 定義

アクトの規模に応じて、また、ビヘイビアの定義をする場合には、通常はソースファイルを分割してビルドします。

ビルドファイルの一つは{プロジェクトフォルダ名.cpp}です。

他にファイルを定義する場合は、プロジェクトフォルダの`build/Makefile`を編集します。

```perl
##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# USER PROJECT BUILD DEFINITION.
##############################################################################

#####################################################################
### set TWELITE model
TWELITE ?= BLUE
#TWELITE = RED

#####################################################################
### set application version (MUST SET THIS.)
VERSION_MAIN = 0
VERSION_SUB  = 1
VERSION_VAR  = 0

#####################################################################
### set an additional source file
###   the default file name is dirname.

### for C++ files compiled with g++ (must have .cpp suffix)
APPSRC_CXX += myAppBhvParent.cpp
APPSRC_CXX += myAppBhvParent-handlers.cpp
APPSRC_CXX += myAppBhvChild.cpp
APPSRC_CXX += myAppBhvChild-handlers.cpp

### for C files compiled with gcc (must have .c suffix)
#APPSRC += my_c_file.c

### Additional Src/Include Path
# if set, find source files from given dirs.
#
APP_COMMON_SRC_DIR_ADD1 = ../Parent
APP_COMMON_SRC_DIR_ADD2 = ../Child
#APP_COMMON_SRC_DIR_ADD3 = 
#APP_COMMON_SRC_DIR_ADD4 =

#####################################################################
### set misc option for compiler

### C++ flags passed to g++
# e.g. CXXFLAGS += -DMY_DEFS
#CXXFLAGS +=

### C++/C flags passed to g++/gcc
# e.g. CFLAGS += -DMY_DEFS
#CFLAGS +=

### include opts
# e.g. INCFLAGS += -I../my_common_src/
#INCFLAGS +=

### optimize flag (default is -Os, normally no need to change)
#OPTFLAG=-O2

#####################################################################
### must include mwx.mk (the makefile body part.)
MWSDK_PATH?=$(realpath $(MWSDK_ROOT))
include $(MWSDK_PATH)/MkFiles/mwx.mk
#####################################################################
```

上記は[サンプルPAL\_AMB-bihavior](../api-reference/behavior/pal\_amb-behavior.md)でのMakefileの例です。



### VERSION\_???

```perl
### set application version (MUST SET THIS.)
VERSION_MAIN = 0
VERSION_SUB  = 1
VERSION_VAR  = 0
```

バージョン番号を指定します。ビルド結果ファイル名に反映されます。

コンパイル中は `-DVERSION_MAIN=0` `-DVERSION_SUB=1` `-DVERSION_VAR=0`のように定義として渡されます。



### ソースファイルの追加

{% hint style="success" %}
`(MWSDK 2020-04)` サブフォルダにファイルを配置しない場合は、追加指定は不要になりました。プロジェクトファイルにある .c .cpp ファイルがすべて追加されます。&#x20;
{% endhint %}

ソースファイルを追加する際に必要なのは`APPSRC_CXX`と`APP_COMMON_SRC_DIR_ADD?`です。

{% hint style="info" %}
サブフォルダにソースファイルを配置する場合は必ずフォルダ APP\_COMMON\_SRC\_DIR\_ADD? の指定が必要です。
{% endhint %}



ソースファイル名をAPPSRC\_CXXに追記します。このファイル名には**フォルダ名が含まれてはいけません**。サブフォルダにあるものもフォルダなしで指定します（つまり同じファイル名がサブフォルダにある場合は、ビルドが失敗します）

```perl
APPSRC_CXX += myAppBhvParent.cpp
APPSRC_CXX += myAppBhvParent-handlers.cpp
APPSRC_CXX += myAppBhvChild.cpp
APPSRC_CXX += myAppBhvChild-handlers.cpp
```



次にソースファイルがプロジェクトフォルダ以外の場所に格納されている場合の検索パスを指定します。**最大４つまで**設定できます。

```perl
APP_COMMON_SRC_DIR_ADD1 = ../Parent
APP_COMMON_SRC_DIR_ADD2 = ../Child
```

フォルダの指定は**Makefileからの相対パス**になります。



### コンパイル・リンカオプション

その他にもいくつかのオプションをコンパイラ・リンカに渡すことができます。

| 指定         | 内容                                                 |
| ---------- | -------------------------------------------------- |
| `CXXFLAGS` | C++ソースファイルに対してコンパイルオプションを指定します。                    |
| `CFLAGS`   | C/C++ソースファイルに対してコンパイルオプションを指定します。                  |
| `INCFLAGS` | ヘッダファイルのインクルードファイル指定をします。                          |
| `OPTFLAGS` | 特別な理由があって-Os以外のコンパイルオプションを適用したい場合に定義します。           |
| `LDFLAGS`  | リンカオプションを指定します。(上記Makefileのコメントには記述はありませんが指定は可能です) |

