# NBビーコン方式の接続

中継器や親機は定期的にNBビーコンを送出し、このビーコンを受信したルータは、受信した NBビーコンにもとづき上位ノードを決定する方法です。

{% hint style="info" %}
各ノードが始動時などに[近隣探索](../../../twelite-net-api-ref/mojru-raiburari/nb-scan.md)(NBSCAN)を実行し、上位ノードを決定する方法もあります。これに比べ以下のメリットがあります。

* ネットワークが一斉に始動した時の混乱を避けられる
* 非常に混雑したネットワークでの探索失敗リスクが減少する

反面NBビーコンを受信できるまで上位ノードが決定できないため、電波条件が良い場合は余分に時間を必要とすることです。
{% endhint %}

{% hint style="warning" %}
TWELITE NET 1.2 では、従来提供していた[近隣探索](../../../twelite-net-api-ref/mojru-raiburari/nb-scan.md)(NBSCAN)を用いた方式も動作可能ですが、新規実装には推奨しません。
{% endhint %}

## プログラミングについて

`tsToCoNet_NwkLyTr_Config` 構造体の設定パラメータにより NB ビーコンモードを設定する。本モードを利用する場合 Parent ならびに Router は必ず設定が必要である。

コード例

```c
tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
  
// set NB beacon mode
sNwkLayerTreeConfig.u8Second_To_Beacon = 3; // set NB beacon interval
sNwkLayerTreeConfig.u8StartOpt = 0x02; // set NB beacon mode
  
// set as Router
sAppData.sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ROUTER;
    /* 親機の場合 TOCONET_NWK_ROLE_PARENT を設定する */;
pContextNwk = ToCoNet_NwkLyTr_psConfig(&ssNwkLayerTreeConfig);
```

## 上位ノード決定方法について

上位ノードは原則として約10秒に１回NBビーコン送信を行います。過去 120 秒以内に到達したパケットで、到達回数や[LQI](../../yong-yu.md#lqi)を評価して上位ノードを決定します。

到達履歴として最大６ノード、各ノードにつき８パケット分を評価します。

## 制限事項

* 子機は LAYERTREE_MININODE 方式、中継器、親機は NBビーコン方式という組み合わせを想定します 。（App_Tag向け）
* 同一レイヤ内を中継する SUBTREE には対応しません。
