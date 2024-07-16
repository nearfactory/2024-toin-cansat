# ToCoNet_vChConfig()

## 解説

チャネルを変更します。

{% hint style="info" %}
`ToCoNet_vRfConfig()` はチャネル以外の設定も適用しますが、本関数はチャネルのみの変更となります。
{% endhint %}

{% hint style="warning" %}
送信中や受信中などチャネル変更できないタイミングでは、変更が行われません。変更が行われない場合は [`sToCoNet_AppContext.u8Channel`](../gou-zao-ti/stoconet_appcontext.md) の値が変更前のチャネルに戻されます。
{% endhint %}

{% hint style="warning" %}
[モジュール](../../twelite-net-api-expl/mojru.md) `ToCoNet_USE_MOD_CHANNEL_MGR` を利用する場合は、本関数によるチャネル変更は行われません。
{% endhint %}

## 引数

なし

## 戻り値

なし

## サンプル

```c
sToCoNet_AppContext.u8Channel = 25;
ToCoNet_vChConfig();
if (sToCoNet_AppContext.u8Channel != 25) {
  // Oops! Channel is not changed to 25.
}
```

