# ToCoNet_vMacStart()

## 解説

MAC 層の初期化処理を行います。

{% hint style="info" %}
スリープ起床したとき、無線の送受信を行わず、マイコン処理のみを行う場合は、本関数を呼び出さずに実行します。
{% endhint %}

起動時に[`cbAppColdStart()`](../krubakku/cbappcoldstart.md) , [`cbAppWarmStart()`](../krubakku/cbappwarmstart.md) で、`TRUE`を引数とした２回目の呼び出し処理部の末尾に通常は記述します。

{% hint style="warning" %}
cbAppColdStart(), cbAppWarmStart() 以外での呼び出しについては、問題が発生しうる内部設計ではありませんが、動作については未定義とします。必要に応じて、一旦スリープを実行し、スリープ復帰後に無線送信を実施するようにしてください。
{% endhint %}

{% hint style="info" %}
[`sToCoNet_AppContext.u8MacInitPending`](../gou-zao-ti/stoconet_appcontext.md) を設定した場合は、本関数は呼び出しません。
{% endhint %}

## 引数

なし。

## 戻り値

なし。

## サンプル

```c
void cbAppWarmStart(bool_t bStart) {
    if (!bStart) {
        // very initial call
    } else {
        // initializing...
        
        // finally start the MAC layer
        ToCoNet_vMacStart();
    }
}
```

