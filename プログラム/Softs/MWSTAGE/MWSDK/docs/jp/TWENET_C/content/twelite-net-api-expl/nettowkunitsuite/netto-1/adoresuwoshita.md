# 上位アドレスを固定した静的中継

LayerTree ネットワークで、上位アドレスを固定して静的に中継する方法です。

`tsNwkLayerTreeConfig` 構造体にメンバーを以下のように設定します。

```c
sNwkLayerTreeConfig.u8StartOpt = 
    TOCONET_MOD_LAYERTREE_STARTOPT_FIXED_PARENT;
            // 開始時にスキャンしない

sNwkLayerTreeConfig.u8ResumeOpt = 
    TOCONET_MOD_LAYERTREE_RESUMEOPT_NO_RELOCATE_ON_RESUME;			
            // レジューム時にも固定親機運用する

sNwkLayerTreeConfig.u32AddrHigherLayer = 0x80012345;
            // 0x80012345 のノードを上位とする

```

{% hint style="warning" %}
[NBビーコン](nbbkonno.md)(`TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON`)を指定した場合は、NBビーコンによる上位決定ができない時に限り固定設定アドレスが有効になります。
{% endhint %}
