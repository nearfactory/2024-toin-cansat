# Static relay with fixed host address

This is a method of static relaying with a fixed upper address in a LayerTree network.

Set the members of the `tsNwkLayerTreeConfig` structure as follows.

```c
sNwkLayerTreeConfig.u8StartOpt = 
    TOCONET_MOD_LAYERTREE_STARTOPT_FIXED_PARENT;
            // Do not scan at the start

sNwkLayerTreeConfig.u8ResumeOpt = 
    TOCONET_MOD_LAYERTREE_RESUMEOPT_NO_RELOCATE_ON_RESUME;			
            // Operate a fixed parent unit even when resuming

sNwkLayerTreeConfig.u32AddrHigherLayer = 0x80012345;
            // The node at 0x80012345 is the superior node.

```

{% hint style="warning" %}
If [NB BEACON](nbbkonno.md) (`TOCONET_MOD_LAYERTREE_STARTOPT_NB_BEACON`) is specified, the fixed configuration address is enabled only when the NB beacon cannot determine the upper level.
{% endhint %}
