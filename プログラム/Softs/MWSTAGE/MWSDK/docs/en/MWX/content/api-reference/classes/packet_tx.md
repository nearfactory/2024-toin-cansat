---
title: packet_tx
author: "Mono Wireless Inc."
---

# packet\_tx

This class is a wrapper class for the `tsTxDataApp` structure of the TWENET C library, and objects of derived classes based on this class are obtained from network behaviors or [`on_tx_comp()`](../sys_callbacks/on_tx_comp.md).

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
	pkt << tx_addr(0x00)
		<< tx_retry(0x1)
		<< tx_packet_delay(0,50,10);
		
	pack_bytes(pkt.get_payload()
		, make_pair("APP1", 4)
		, uint8_t(u8DI_BM)
	);
  
  pkt.transmit();
}
```



## Generating objects

This is done by the network behavior `.prepare_tx_packet()`.

```cpp
if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  ...
}
```

In the above example, `the_twelite.network.use<NWK_SIMPLE>()` retrieves an object of the network behavior. The object `pkt` is created by `.prepare_tx_packet()` of this object. It is a derived class of `packet_tx`, although the type name is inferred by auto&&.

This `pkt` object first returns `true` or `false` in the `if()`. A `false` return occurs when the queue for sending is full and no more requests can be added.



## Transmission Settings

Various settings are configured in the radio packet to deliver the packet to the destination, including destination information. To configure the settings, an object containing the settings is given as the right-hand side value of the << operator.

```cpp
pkt << tx_addr(0x00)
	  << tx_retry(0x1)
  	<< tx_packet_delay(0,50,10);
```



The following is a description of the objects used for configuration.

{% hint style="warning" %}
The availability and implications of each setting depend on the network behavior specification.
{% endhint %}

### tx\_addr

```cpp
tx_addr(uint32_t addr)
```

Specify the destination address `addr`. See the Network Behavior specification for the destination address value.

* `<NWK_SIMPLE>`
An address set with an MSB (bit31=`0x80000000`) means that it is addressed to the serial number of the radio module.
`0x00`. 0xFE means that the address is addressed to the child module (`0x01`...`0xEF`). 0xFE` means broadcast to the child (`0x01`...`0xEF`), and `0xFF` means broadcast to both the parent and the child.


### tx\_retry

```cpp
tx_retry(uint8_t u8count, bool force_retry = false)
```

Specifies the number of retransmissions. The number of retransmissions is specified by u8count. force\_retry is a setting to retransmit a specified number of times regardless of whether the transmission is successful or not.

* `<NWK_SIMPLE>`
ネットワークビヘイビア`<NWK_SIMPLE>`では、同じ内容のパケットを`u8count+1`回送信します。
`force_retry`の設定は無視されます。



### tx\_packet\_delay

```cpp
tx_packet_delay(uint16_t u16DelayMin,
                uint16_t u16DelayMax,
                uint16_t u16RetryDur)
```

Sets the delay between sending packets and the retransmission interval. Specify two values, `u16DelayMin` and `u16DelayMax`, in milliseconds [ms]. Transmission will be started at some point during this interval after a transmission request is made. Specify the retransmission interval as the value of `u16RetryDur` in milliseconds\[ms]. The retransmission interval is constant.

{% hint style="warning" %}
The transmission process may not start at the specified timing due to internal processing. In addition, IEEE802.15.4 processing also causes time blurring before packets are created. These timing blurs are an effective means of avoiding packet collisions in many systems.

Strict timing of packet transmission should be considered an exception due to the nature of the IEEE802.15.4 standard.
{% endhint %}

* `<NWK_SIMPLE>`
This specification is valid.
{% hint style="warning" %}
If the same packet is retransmitted and arrives more than 1 second after the first transmission, it is not deduplicated because a new packet has arrived. The same packet may be received after 1 second due to a longer retransmission interval or packet arrival delay in relay.
You can configure the handling of duplicate packets by initializing the `<NWK_SIMPLE>` behavior.
{% endhint %}



### tx\_process\_immediate

```cpp
tx_process_immediate()
```

This setting requests that packet transmission be performed "as quickly as possible." Packet transmission processing in TWENET is performed from the TickTimer, which runs every 1 ms. By setting this parameter, the packet transmission request is processed as soon as possible after the request is made. Of course, any setting other than `tx_packet_delay(0,0,0)` is a meaningless specification.

If other packet transmission processing is in progress, it will be processed as usual.

* `<NWK_SIMPLE>`
This designation is valid.



### tx\_ack\_required

```cpp
tx_ack_required()
```

In wireless packet communications, there is a transmission method in which a short radio message called ACK (ACK) is obtained from the destination after the transmission is completed to indicate that the transmission was successful. By setting this option, transmission with ACK is performed.

* `<NWK_SIMPLE>`
{% hint style="warning" %}
This specification is **VALID** for `<NWK_SIMPLE>`. It will result in a compile error.
`<NWK_SIMPLE>` is intended to implement a simple working relay network and does not communicate with ACK.
{% endhint %}



### tx\_addr\_broadcast

```
tx_addr_broadcast()
```

Specify broadcast.

* `<NWK_SIMPLE>`
{% hint style="warning" %}
This specification is **VALID** for `<NWK_SIMPLE>`. It will result in a compile error.
{% endhint %}
Specify the destination address `tx_addr(0xFF)` (broadcast) or `tx_addr(0xFE)` (broadcast to the child) instead.



### tx\_packet\_type\_id

```cpp
tx_packet_type_id(uint8_t)
```

Specify the type ID of the TWENET radio packet that can be 0..7.

* `<NWK_SIMPLE>`
{% hint style="warning" %}
This specification is **VALID** for `<NWK_SIMPLE>`. It will result in a compile error.
{% endhint %}
`<NWK_SIMPLE>` uses type ID internally. Users cannot use it.

