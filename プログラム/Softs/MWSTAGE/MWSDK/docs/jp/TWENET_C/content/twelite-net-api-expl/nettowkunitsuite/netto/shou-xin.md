# 受信

パケットの受信は常にコールバック関数 [cbToCoNet_vRxEvent](../../../twelite-net-api-ref/krubakku/cbtoconet_vrxevent.md)() の呼び出しによって行われます。パラメータとして [tsRxDataApp](../../../twelite-net-api-ref/gou-zao-ti/tsrxdataapp.md) 構造体へのポインタが渡されます。この構造体の内容を参照し、アプリケーションの各処理を実施します。

{% hint style="danger" %}
本関数の終了(return)を遅延させるべきではありません。通常無線パケットは一般でも最短で 1ms 程度の間隔をあけて伝達されますが、これを超えた長時間の遅延により、無線パケットの取りこぼしが発生するかもしれません。
{% endhint %}
