# NB beacon system connection

In this method, a repeater or a parent sends out NB beacons periodically, and the router receives the beacons and decides which node is the superior node based on the received NB beacons.

{% hint style="info" %}
It is also possible for each node to perform [a neighbourhood search (NBSCAN) ](../../../twelite-net-api-ref/mojru-raiburari/nb-scan.md)at startup to determine the top node. This has the following advantages

* Avoid confusion when the network starts all at once
* Reduced risk of search failure in very congested networks

The downside is that the upper node cannot be determined until the NB beacon is received, which can take extra time in good radio conditions.
{% endhint %}

{% hint style="warning" %}
In TWELITE NET 1.2, the previously provided method using Neighbourhood Search (NBSCAN) can also work, but is not recommended for new implementations.
{% endhint %}

## About programminプログラミングについて

`tsToCoNet_NwkLyTr_Config` 構造体の設定パラメータにより NB ビーコンモードを設定する。本モードを利用する場合 Parent ならびに Router は必ず設定が必要である。

コード例

```c
tsToCoNet_NwkLyTr_Config sNwkLayerTreeConfig;
  
// set NB beacon mode
sNwkLayerTreeConfig.u8Second_To_Beacon = 3; // set NB beacon interval
sNwkLayerTreeConfig.u8StartOpt = 0x02; // set NB beacon mode
  
// set as Router
sAppData.sNwkLayerTreeConfig.u8Role = TOCONET_NWK_ROLE_ROUTER;
    /* For Parent Node Set TOCONET_NWK_ROLE_PARENT */;
pContextNwk = ToCoNet_NwkLyTr_psConfig(&ssNwkLayerTreeConfig);
```

## Method of determining the upper node

In principle, a high-level node transmits an NB beacon approximately once every 10 seconds. The upper-ranking node is determined by evaluating the number of arrival times and LQI with the packets that have arrived within the last 120 seconds.

Up to 6 nodes and 8 packets per node are evaluated as arrival history.

## Limitations

* A combination of LAYERTREE_MININODE for the child and NB beacons for the repeater and parent is assumed. (For App_Tag)
* SUBTREE, which relays within the same layer, is not supported.
