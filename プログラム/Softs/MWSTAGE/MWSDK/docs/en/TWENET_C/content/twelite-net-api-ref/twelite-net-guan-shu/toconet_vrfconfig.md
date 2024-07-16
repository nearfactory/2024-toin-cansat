# ToCoNet_vRfConfig()

## Explanation

Modify parameters of MAC layer. The following members of [`sToCoNet_AppContext`](../gou-zao-ti/stoconet_appcontext.md) structure can be changed.

| `u8Channel`       | \*1 | Switches the channel. If the switching fails, the structure `u8Channel` will return to its pre-set value. |
| ----------------- | --- | --------------------------------------------------------------------------------------------------------- |
| `u32ChMask`       | \*1 |                                                                                                           |
| `u16ShortAddress` | \*1 | Change the short address of its own station.                                                              |
| `u8CPUClk`        |     | Switching the CPU clock takes a few tens of usec.                                                         |
| `u8TxPower`       | \*1 |                                                                                                           |
| `bRxOnIdle`       |     | If set to FALSE, no wireless packets will be received.                                                    |

{% hint style="warning" %}
\*1 When making changes to these settings, the results are indefinite during radio transmission and reception.

In order for the changes to take effect for new packets to be sent, they should only be made after the entire transmission queue has been processed.

It is also possible, in principle, for a change in timing from just before to just after a packet is received, for that packet to never be received. In many applications, the receiver does not know when the packet will be received, so the implementation does not adjust the timing of the switch.
{% endhint %}

## Argument

None

## Returns

None

## Sample code

```c
sToCoNet_AppContext.u8Channel = 25;
sToCoNet_u8TxPower = 2;
ToCoNet_vRfConfig();
```

