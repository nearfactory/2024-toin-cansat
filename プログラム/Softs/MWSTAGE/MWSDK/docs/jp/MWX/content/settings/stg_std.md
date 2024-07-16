---
title: "STG_STD"
author: "Mono Wireless Inc."
description: "STG_STD"
---
# \<STG\_STD>

\<STG\_STD> は、最小限の設定項目を有した設定ビヘイビアです。

```
　　　　　　　　　　　　　　　設定画面例
[CONFIG/MY_APP:0/SID=8102ECE3]
a: (0x1234ABCD) Application ID [HEX:32bit]
i: (         1) Device ID [1-100,etc]
c: (        13) Channel [11-26]
o: (0x00000000) Option Bits [HEX:32bit]

 [ESC]:Back [!]:Reset System [M]:Extr Menu                      
```



{% hint style="info" %}
このビヘイビアはSerialオブジェクトの入出力をフックし、インタラクティブモードでの画面入出力を行います。アプリケーション中で入力処理を明示的に記述する必要はありません。インタラクティブモード画面中のアプリケーションからの画面出力は抑制されます。
{% endhint %}

{% hint style="info" %}
M キーを入力すると、補助機能にアクセスできます。機能は末尾の Extra Menu 項目を参照ください。
{% endhint %}



## 使用法

### 登録

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <STG_STD> // interactive mode
```

上記のように `#include <STG_STD> `を追加します。



### setup() での読み出し

```cpp
uint32_t APP_ID;
uint8_t CHANNEL;
uint8_t LID;

void setup() {
   ...
   auto&& set = the_twelite.settings.use<STG_STD>();

   // call reload() before reading values.   
   set.reload();
   
   // read value
   APP_ID = set.u32appid();
   CHANNEL = set.u8ch();
   LID = set.u8devid();
   
   // apply them
	 the_twelite
		<< TWENET::appid(APP_ID)
		<< TWENET::channel(CHANNEL);
		
   auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
   nwk	<< NWK_SIMPLE::logical_id(LID);
}
```

多くの場合、設定の読み出しは`setup()` 中の早い段階で行います。

上記の例はでは、まず`the_twelite.settings.use<STG_STD>()`により設定ビヘイビアを登録します。

{% hint style="info" %}
登録直後に、デフォルトのアプリケーションIDを変更するなど設定ビヘイビアの変更を行うことができますが、これは後述します。
{% endhint %}

つぎに`set.reload()`を呼び出し、実際にEEPROMからデータを読み出し、これを解釈します。自動で読み出さないことに注意してください。

`set.u32appid()`, `set.u8ch()`, `set.u8devid()`は各々アプリケーションIDの設定値、チャネルの設定値、論理デバイスIDの設定値を取得しています。ここでは変数に各設定値を格納しています。

{% hint style="info" %}
設定値を格納した内部データ構造は比較的複雑であるため、求める設定を得るまで、設計配列のデータ探索が必要になります。計算量を気にする場合は一旦設定値を別の変数に格納するようにしてください。
{% endhint %}

あとは、設定値を利用してアプリケーションIDやチャネルなどの値を反映します。



## 設定一覧

以下が設定IDの一覧(`enum class E_STGSTD_SETID`)定義です。

| 設定ID             | 内容              |
| ---------------- | --------------- |
| `APPID`          | アプリケーションID      |
| `LOGICALID`      | 論理デバイスID (8bit) |
| `CHANNEL`        | チャネル            |
| `CHANNELS_3`     | チャネル（３ｃｈまで）     |
| `POWER_N_RETRY`  | 出力とリトライ回数       |
| `OPTBITS`        | Option 1        |
| `OPT_DWORD2`     | Option 2        |
| `OPT_DWORD3`     | Option 3        |
| `OPT_DWORD4`     | Option 4        |
| `ENC_MODE`       | 暗号化モード          |
| `ENC_KEY_STRING` | 暗号化キー（文字列入力）    |

`<STG_STD>`では、代表的な設定と自由に使える32bit値を４つ定義されています。これらは、ユーザが自由に利用できます。

{% hint style="info" %}
* STG\_STD>から読み出すだけでは設定は反映されません。
* 不要な項目を非表示にできます。
* 項目名や項目詳細を変更できます。
{% endhint %}



## 設定ビヘイビアのカスタマイズ

設定ビヘイビアのカスタマイズは`.reload()`を行う前に全項目を行っておきます。

### アプリケーション名

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appname("MY_APP");
	...
	set.reload();
```

アプリケーション名はインタラクティブモードの先頭行に表示されます。

```
[CONFIG/MY_APP:0/SID=8102ECE3]
```

文字列ポインタを指定してください。内部でコピーを作らないようにしているため、ローカル変数を指定できません。



### デフォルト値

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appid_default(0x13579be);
	set << SETTINGS::ch_default(18);
	set << SETTINGS::lid_default(7);
		...
	set.reload();
```

アプリケーションID、チャネル、論理ID(LID)については、デフォルト値を変更できます。



### 複数チャネル設定メニュー

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::ch_multi();
	...
	set.reload();
```

`SETTINGS::ch_multi() `を指定すると、チャネル設定が複数指定になります。複数設定を行う場合、設定値の読み出しは`.u32chmask()`を用います。



### すぐに設定画面を表示する

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::open_at_start();
	...
	set.reload();
```

アプリケーションID、チャネル、論理IDについては、デフォルト値を変更できます。



### 項目名、詳細の記述内容の変更

```cpp
const TWESTG_tsMsgReplace SET_MSGS[] = {
	{ int(E_STGSTD_SETID::OPTBITS),    "オプション1", 
			"オプション１を設定してください" },
	{ int(E_STGSTD_SETID::OPT_DWORD2), "オプション2",
			"オプション２を設定してください\r\nオプション２は云々" },
	{ 0xFF } // terminator
};

setup() {
  auto&& set = the_twelite.settings.use<STG_STD>();
	set.replace_item_name_desc(SET_MSGS);
	...
```

項目名を別のモノに変更することが出来ます。上記の例ではUTF-8による日本語にしていますが、ターミナルの表示など条件がそろわないと適切には表示されません。

{% hint style="info" %}
TWELITE STAGEの場合、登録フォントの文字数を減らしている場合は、表示されない文字が出てくるかもしれません。必要な文字列が含まれるよう、再ビルドしてください。
{% endhint %}

この配列は最後に `{ 0xFF }` で終端します。

１番目のエントリは設定ID、２番目が項目名、３番目が設定入力時に表示される解説になります。`\r\n`により改行できます。



### 現在設定画面かどうか判定

```cpp
		auto&& set = the_twelite.settings.use<STG_STD>();
		if (!set.is_screen_opened()) {
		   // 設定画面が出ていないときの処理
		}
```

設定画面出力中にシリアルへの出力を行うと画面が崩れたりする原因になります。設定画面でないことを確認するには`.is_screen_opened()`で確認します。



### 項目の削除

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
set.hide_items(E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4);

...
if(set.is_hidden(E_STGSTD_SETID::OPT_DWORD3) {
  ; // OPT_DWORD3は非表示
}
```

不要な項目の削除を行います。`.hide_items`は項目IDをパラメータとして（可変引数で複数指定可能）不要な項目を非表示にします。非表示項目かどうかは`.is_hidden()`により確認できます。

{% hint style="warning" %}
アプリケーションID,チャネル,論理ID(LID)の３種類は削除できません。
{% endhint %}

{% hint style="warning" %}
非表示設定は内部のワークメモリを使用します。最小限のメモリ容量(32バイト)としているため、非表示項目を多数設定する場合、メモリ不足で非表示にならない場合があります。ワークメモリの容量はコンパイル引数として`-DSIZE_SETSTD_CUST_COMMON=48`のように指定できます。
{% endhint %}



## メソッド

### reload()

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appname(APP_NAME)
		  << SETTINGS::appid_default(DEF_APP_ID)
   		<< SETTINGS::open_at_start();
   
	set.reload();
```

設定を読み込みます。すべてのカスタマイズが終わってから実行します。



## メソッド （データ読み出し）

データの読み出しは以下のメソッドを呼び出します。

{% hint style="warning" %}
読み出し前に必ず`.reload()`を呼び出してください。
{% endhint %}

| メソッド                            | 内容                                              |
| ------------------------------- | ----------------------------------------------- |
| `uint32_t u32appid()`           | アプリケーションID                                      |
| `uint8_t u8devid()`             | 論理デバイスID                                        |
| `uint8_t u8ch()`                | 設定チャネル (11..26)                                 |
| `uint32_t u32chmask()`          | チャネル設定マスク (ビットマスクで指定、13 なら 1UL << 13 にビットを設定する) |
| `uint8_t u8power()`             | 出力設定 (0..3)                                     |
| `uint8_t u8retry()`             | リトライ数                                           |
| `uint32_t u32opt1()`            | オプション1                                          |
| `uint32_t u32opt2()`            | オプション2                                          |
| `uint32_t u32opt3()`            | オプション3                                          |
| `uint32_t u32opt4()`            | オプション4                                          |
| `uint8_t u8encmode()`           | 暗号化モード (0: なし 1: 有効 2: 有効、平文パケットも表示する)          |
| `const uint8_t * u8enckeystr()` | 暗号化キーの取得                                        |

{% hint style="info" %}
内部では、構造体配列を線形探索して設定IDに合致する設定構造体を探索しており、オーバーヘッドがあります。頻繁に参照される設定値は一旦別の変数にコピーして使用してください。
{% endhint %}



### 設定の反映

`the_twelite`や`<NWK_SIMPLE>`オブジェクトに対して、本オブジェクトを用いて直接設定を反映できます。

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
set.reload(); //ここで実際に設定がEEPROMより読み出される

the_twelite << set; // 設定値の反映 (APPIDなど)

auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

nwk << set; // 設定値の反映 (LIDなど)

```

反映される設定値は以下となります。`.hide_items()`により非表示になっている項目は反映しません。

| 対象             | 項目ID            | 内容                                                                                                                                                  |
| -------------- | --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| `the_twelite`  | `APPID`         | `TWENET::appid(値)`に反映されます。                                                                                                                          |
|                | `CHANNEL`       | `TWENET::channel(値)`に反映されます。`※ SETTINGS::ch_multi() `を指定したときは反映されません                                            |
|                | `CHANNELS_3`    | `TWENET::chmask(値)`による設定を行います。`※ SETTINGS::ch_multi() `を指定したときのみ、チャネルマスクとして反映されます。                              |
|                | `POWER_N_RETRY` | `TWENET::tx_power(値)`と`TWENET::mac_retry_count(値)`による設定を行います。注： `&#x3C;NWK_SIMPLE>`での再送設定も同じ値を参照します。 |
| `<NWK_SIMPLE>` | `LOGICALID`     | `NWK_SIMPLE::logical_id(LID)`による設定を行います。                                                                                                            |
|                | `POWER_N_RETRY` | `NWK_SIMPLE::repeat_max(LID)`による設定を行います。                                                                                                            |



## 項目ID

`.hide_items()`などで項目IDを指定する場合があります。この項目IDは`enum class E_STGSTD_SETID`で定義されてます。

| E\_STGSTD\_SETID:: | 内容           |
| ------------------ | ------------ |
| `APPID`            | アプリケーションID   |
| `LOGICALID`        | 論理ID(0..100) |
| `CHANNEL`          | チャネル         |
| `CHANNELS_3`       | チャネル（複数指定）   |
| `POWER_N_RETRY`    | 出力とリトライ設定    |
| `OPTBITS`          | オプションビット     |
| `UARTBAUD`         | UARTのボーレート設定 |
| `OPT_DWORD2`       | オプションビット２    |
| `OPT_DWORD3`       | オプションビット３    |
| `OPT_DWORD4`       | オプションビット４    |
| `ENC_MODE`         | 暗号化モード       |
| `ENC_KEY_STRING`   | 暗号化キー        |



## Extra Menu

```
[ROOT MENU/BAT1/SID=810B645E]
0: CONFIG
1: EEPROM UTIL
2: H/W UTIL

 [ESC]:Back [!]:Reset System                                    
```

Mキーを入力すると追加メニューにアクセスできます。

* CONFIG : 設定画面に戻ります
* EEPROM UTIL : EEPROMのメンテナンスを行うためのメニューです
* H/W UTIL : ハードウェアの状態を調べるためのメニューです



### EEPROM UTIL

```
[EEPROM UTIL/BAT1/SID=810B645E]
r: Read sector.
R: Read ALL sectors.
e: Erase sector.
E: Erase ALL sectors.

 [ESC]:Back [!]:Reset System [M]:Extr Menu
```

セクターの読み出し、削除を行います。全読み出し、全消去を行うときは大文字で "YES" の３文字を入力します。



### H/W UTIL

```
[H/W UTIL/BAT1/SID=810B645E]                   
```

現バージョンでは機能は提供されません。
