# コールバック関数

コールバック関数は、TWENET の様々なイベントが発生したときに処理するための関数です（[フロー図](../../twelite-net-api-expl/twenet-fur/)）。TWENET ライブラリでアプリケーションをビルドする際に、すべて実装が必要です。特に重要な処理を記述するのが [cbAppColdStart](cbappcoldstart.md)() で、一般的な main() 関数に相当する関数です。

* [cbAppColdStart](cbappcoldstart.md)() – 電源投入時に呼び出される関数
* [cbAppWarmStart](cbappwarmstart.md)() – スリープ復帰時に呼び出される関数
* [cbToCoNet_vMain](cbtoconet_vmain.md)() – メインループ無限ループではなく割り込みなどの発生を起点として呼び出されます
* [cbToCoNet_vRxEvent](cbtoconet_vrxevent.md)() – 無線パケット受信時に呼び出されます
* [cbToCoNet_vTxEvent](cbtoconet_vtxevent.md)() – 無線パケット送信完了時に呼び出されます
* [cbToCoNet_vNwkEvent](cbtoconet_vnwkevent.md)() – MAC層やネットワーク層の各種イベント通知
* [cbToCoNet_vHwEvent](cbtoconet_vhwevent.md) () – ペリフェラルの割り込み処理の遅延実行部。割り込み処理後に呼び出されます
* [cbToCoNet_u8HwInt](cbtoconet_u8hwint.md)() – ペリフェラルの割り込みハンドラ
* ユーザ定義イベント処理関数 – 状態遷移によるタスク処理を記述します

いずれのコールバック関数も、呼び出された後に制御を返す必要があり、非常に長い処理を記述すると全体の振る舞いに影響します。特に割り込みハンドラの記述には気を配ります。

