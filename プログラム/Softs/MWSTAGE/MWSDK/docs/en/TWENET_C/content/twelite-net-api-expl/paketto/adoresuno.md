# Addressing conventions

## For simple nets

There are two ways of specifying the sender and destination address: long address and short address.

### Long address

Specifies the 32bit module serial number (32bit) of the TWELITE. The address can be obtained by ToCoNet_u32GetSerial() API.

It is also the 7-digit hexadecimal S/N (serial number) displayed on the shield can of the TWELITE module plus 0x80000000. For example, a serial number of 1012ABC would be 0x81012ABC.

{% hint style="warning" %}
When the destination is specified as long, the payload size of the packet becomes small. ([Maximum length of a packet](pakettono.md))
{% endhint %}

{% hint style="info" %}
IEEE802.15.4 standard uses 64bit MAC address. 64bit MAC address is specified based on 32bit module address.
{% endhint %}

### Short address

Addresses between 0x0000 and 0x0FFF can be specified. The address is set to [`sToCoNet_AppContext.u16ShortAddress`](../../twelite-net-api-ref/gou-zao-ti/stoconet_appcontext.md) in the [`cbAppColdStart()`](../../twelite-net-api-ref/krubakku/cbappcoldstart.md) function.

### broadcast communication（Short address of 0xFFFF）

This address is used for broadcast communication.

## For simple nets

The long address is mostly used for LayerTree Relay net, where some [special addresses](../nettowkunitsuite/netto-1/nettonoadoresu.md) is defined.

see [Relay net](../nettowkunitsuite/netto-1/) for details.



