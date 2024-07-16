# TWELITE NET  ライブラリ構造

ユーザアプリケーションは TWELITE NET (TWENET) のライブラリおよびペリフェラルAPIを介して無線モジュールを制御します。TWELITE NET のライブラリは３つに分かれています。

![TWENET ライブラリ構造](<../.gitbook/assets/image (7).png>)

### libTWENET\_{ARCH}.a

アプリケーションループの定義、イベント処理、無線ネットワーク処理など

### libTWENETExt\_{ARCH}.a

始動処理、MAC層へのインタフェースなど

### libTWENETUtils\_{ARCH}.a

一般的なアルゴリズムやペリフェラルの手続きなど

_（ソース添付：MWSDK/TWENET/????/TWENETUtils/Source : ????にはライブラリバージョンに相当するフォルダ名が入ります）_



{% hint style="info" %}
{ARCH} は、TWELITE のモデル名が入ります。BLUE, REDなど。
{% endhint %}
