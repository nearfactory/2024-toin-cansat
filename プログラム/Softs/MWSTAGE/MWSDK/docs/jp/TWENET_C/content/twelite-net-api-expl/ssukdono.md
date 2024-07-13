# ソースコードの構造

TWELITE NET でのアプリケーションの構成要素は以下となります。

* [コールバック関数](../twelite-net-api-ref/krubakku/)や[モジュール](mojru.md)の定義：必須定義でメインファイルに記述します。
* [ユーザ定義イベント処理関数](twenet-fur/yzaibentofur.md)：必要に応じて記述します。ステートマシン（状態遷移マシン）によるアプリケーションの動作を想定した関数です。
* ユーザ定義諸関数：その他処理関数で、アプリケーション特有のコードです。

以下にメインファイルについて補足します。

## メインファイル

TWENET API では、メインファイルには以下の定義を記述します。

* モジュール宣言
* 必須ヘッダファイルインクルード
* コールバック関数

これ以外の処理を、メインファイル以外のファイルに記述しても構いません。

{% hint style="info" %}
TWENTE API では C 言語の処理系で一般的な main() 関数がありません。最初に呼び出されるのは [cbAppColdStart](../twelite-net-api-ref/krubakku/cbappcoldstart.md)() 関数です。
{% endhint %}



### モジュールの定義と必須ヘッダファイルのインクルード

ビルドバイナリや実行時のメモリを最小化するため、コンパイル時に静的に決定する関数やメモリの定義を行います。

モジュール利用宣言の定義を cbAppColdStart() の前に記述し、cbAppColdStart() で ToCoNet_REG_MOD_ALL() マクロを呼び出して初期化します。

ヘッダファイルについては "ToCoNet.h" と "ToCoNet_mod_prototype.h" が必要になります。

```c
// Select Modules (define befor include "ToCoNet.h")
#define ToCoNet_USE_MOD_RXQUEUE_BIG
#define ToCoNet_USE_MOD_CHANNEL_MGR

// Includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

…
void cbAppColdStart(bool_t bStart) {
  if (!bStart) {
    // Module Registration
    ToCoNet_REG_MOD_ALL();
  } else {
…
```

### コールバック関数

ソースコードには必ず以下の[コールバック](../twelite-net-api-ref/krubakku/)関数を定義します。

