# Receive

Receiving packets is always done by calling the callback function [`cbToCoNet_vRxEvent()`](../../../twelite-net-api-ref/krubakku/cbtoconet_vrxevent.md). As a parameter, a pointer to a [`tsRxDataApp`](../../../twelite-net-api-ref/gou-zao-ti/tsrxdataapp.md) structure is passed. The contents of this structure are referenced to carry out the respective operations of the application.

{% hint style="danger" %}
The return of this function should not be delayed. Normally radio packets are transmitted with a minimum interval of 1ms, but longer delays may cause radio packets to be missed.
{% endhint %}
