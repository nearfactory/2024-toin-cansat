---
title: "Simple Relay Net NWK_SIMPLE"
author: "Mono Wireless Inc."
description: "Simple Relay Net NWK_SIMPLE"
---

# simple relay network \<NWK_SIMPLE>

A network BEHAVIOR that implements a simple relay network.

```cpp
auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
nwksmpl << NWK_SIMPLE::logical_id(0xFE)
        << NWK_SIMPLE::repeat_max(3);
```

Above is an example of network usage declaration and configuration. The details will be explained later, but we will first explain the basics, such as the concept of network addresses.

{% hint style="info" %}
For actual usage examples, please refer to [Explanation of sample acts](../act\_samples/) for actual usage examples.
{% endhint %}



Each radio station in this network is identified by an 8-bit logical ID. This value is set independently by each radio station at startup. Duplicate logical IDs are allowed, but communication must be based on the assumption that duplicate IDs are used.

Set the ID for each radio station. Normally, a network will have a radio station in the role of Parent Node and a radio station in the role of Child Node. A network with only Child Nodes can also be operated.

Child Nodes can also play the role of a repeater.

| Radio station identification ID | Role |
| -------------- | ----------- |
| `0x00` | Parent station |
| `0x01`. `0xEF` | Child station |
| `0xFE` | child station that does not assign an ID |

Logical IDs can be specified as destinations, but `0xFE` and `0xFF` have special meanings. The table below summarizes the destination designation.

| Destination ID | Meaning
| -------------- | --------------------------- |
| `0x00` | Designation of a parent station from a child station. Designation from a parent station is invalid.     |
| `0x01`. `0xEF` | Specify a specific child station.                 | `0xFE` | all.
| `0xFE` | Broadcast communication (broadcast) specifying all child stations.  | `0xFF`...
| `0xFF` | broadcast broadcast specifying all radio stations. | `0xFF` | broadcast

A serial number, specified in 32 bits, can also be used to identify a radio station.



Packet delivery uses IEEE802.15.4 broadcast, which does not use ACK, so the success of delivery cannot be determined by the sender.

This may seem inefficient for large-scale, frequent communication, but it can be more efficient for networks that only collect data and have a relatively small number of relay stages.

In addition, since no communication is required to build the network, in principle it is less likely that communication will stop completely even in exceptional situations such as failures. As long as the Parent Node is receiving and within wireless range of the Child Node, and the Child Node transmits a packet, the Parent Node can receive it in most cases. In a network where communication is required to establish the network, once configuration information has been lost, data cannot be sent until communication between the Parent Node and Child Node is completed again to establish communication. This is the reason why the network behavior `<NWK_SIMPLE>` is simple in its naming.



For this simple network to work, it is often necessary to ignore retransmitted packets (identical packets) that arrive multiple times. Identification of identical packets in `<NWK_SIMPLE>` is based on the serial number of the sender and the sequence number of the packet when sent (called a duplicate checker). The sequential numbers are assigned in order as 0...63, and the assumption is made that the sequential numbers of packets arriving at close time are close numbers. The duplicate checker will time out packets that have been received more than a certain amount of time and are far away in terms of number (if packet 10 is received now, packets in the 40s are considered to have been sent some time ago) and exclude them from duplicate checker.

Considerations for the duplicate checker are as follows

{% hint style="info" %}
Under general usage conditions (e.g., less than 3 relay stages and packets per second arriving from less than 10 stations), no special considerations are required.
{% endhint %}

* Number of elements that can be checked (increasing the number increases memory consumption and processing time for checking)
* Timeout time setting.

By default, the timeout is 1 second and the number of radio stations to check is 16. This means that if a relay packet is passed around for more than 1 second, it will no longer be considered a duplicate packet. If packets from more than 16 wireless packets arrive in the short term, the duplicate packets will not be checked.

If the number of relay stages or at least the number of relay stations is large, or if retransmissions are made at very long intervals, you may need to consider this setting.



## Declaration and registration

Here is an example of using the network behavior `<NWK_SIMPLE>`.

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>

void setup() {
  ...
  
  auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
}
```

Include the definition information of `<NWK_SIMPLE>` in line 2. register `<NWK_SIMPLE>` in `the_twelite` in line 7.



## Configuration.

Configure after registering `<NWK_SIMPLE>`.

```cpp
void setup() {
  auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
  nwksmpl << NWK_SIMPLE::logical_id(0xFE);
}
```

Settings are made with the `<<` operator.

### `<<` operator (configuration)

The `<<` operator is used to configure the initial settings of the object `the_twelite`.

The following configuration class objects are used as input, and default values are applied if no configuration is made.

#### NWK\_SIMPLE::logical\_id(uint8\_t id)

Set the logical device ID specified in parameter `id`. Default is `0xFE` (ID not set Child Node).



#### NWK\_SIMPLE::retry\_default(uint8\_t val)

The number of times specified in parameter `val` is the default number of retransmissions when sending.



#### NWK\_SIMPLE::retry_max(uint8\_t val)

The number of times specified in parameter `val` is set to the maximum number of relays. Default is `2`.

If you do not want to relay, specify `0`.

{% hint style="info" %}
[Set not to run receiving circuit](../api-reference/predefined\_objs/the\_twelite.md#twenet-rx\_when\_idle-uint-8-\_t-benable), the TWELITE wireless microcontroller cannot receive wireless packets. relay operation will not be performed.
{% endhint %}



#### NWK\_SIMPLE::dup\_check(uint8\_t maxnodes, uint16\_t timeout\_ms, uint8\_t tickscale)

Parameters of the duplicate packet detection algorithm.

{% hint style="warning" %}
Consideration should be given if there are many relay stages or if the packet retransmission interval is long, but **no configuration change is required for general use. **
{% endhint %}

** `maxnodes` is the number of radio stations (nodes) to keep the history. If the number of nodes is set to a small number, some nodes will not be deduplicated if more packets from more than the set number of nodes arrive in a short period of time. If the node cannot be deduplicated, problems will occur such as data being displayed more than once when received, or more packets being relayed than necessary. The default is `16`, which consumes 21 bytes of memory per node.
* `timeout_ms` is the timeout time in milliseconds before the history is erased. The timeout is managed in blocks of continuation numbers, and timeout processing is done in blocks. The default is `1000`\[ms].
* `tickscale` is a unit of time to manage timeouts and is `2^tickscale`\[ms]. Since time is managed in 7 bits, it should be set so that `127*(2^tickscale) > timeout_ms`. The default is `5`(32ms).



#### NWK\_SIMPLE::secure\_pkt(const uint8\_t \*pukey, bool b\_recv\_plain\_pkt = false)

Enables encrypted packets.

* `pukey` specifies the encryption key in 16 bytes (128bit).
* `b_recv_plain_pkt`, if set to `true`, will receive plaintext packets with the same Application ID and channel.

{% hint style="info" %}
Whether a packet is plaintext or not can be determined by `packet_rx::is_secure_pkt()` with `true` (encrypted) or `false` (plaintext).
{% endhint %}



#### STG\_STD

Reflects the values set in [Interactive settings mode](../settings/stg\_std.md#no-1). The following values are reflected.

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
...
set.reload(); // Load configuration values
nwk << set;   // Reflects Interactive settings mode
```

* **logical\_id**
* **retry\_default**

## メソッド

### prepare\_tx\_packet()

```cpp
// The type name is packet_tx_nwk_simple<NWK_SIMPLE>, but it is written auto&&.
auto&&  preare_tx_packet()

//Example
if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  ...
  pkt.transmit();
}
```

Gets the send object. The object will be a class derived from [`packet_tx`](../api-reference/classes/packet_tx.md). This object contains the transmission address and payload, and the `.transmit()` method is used to send the packet.

The `bool` operator is defined in this object. If TWENET cannot accept the send request when the object is created, it returns `false`.



## send_object

The method of the send object obtained by `.prepare_tx_packet()` method.

### bool operator

```cpp
operator bool()

//Example
if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
```

Returns `false` if TWENET cannot accept the request for transmission at the time of object creation.



### transmit()

```cpp
MWX_APIRET transmit()

//Example
uint8_t txid;

if (auto&& pkt = 
  the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
  
  ...
  
  MWX_APIRET ret = pkt.transmit();
  if (ret) {
    txid = pkt.get_value();
  }
}
```

Processes the packet for transmission. If `MWX_APIRET` is `true`, the request is successful, but the sending process does not start at this request.

The transmission ID of the packet is stored in the value part obtained by [`MWX_APIRET::get_value()`](../api-reference/classes/mwx\_apiret.md). the_twelite.x_status.is_complete() or [`transmit_complete()`](../api-reference/behavior/#transmit\_complete) can be used to confirm completion of the transmission.


## Maximum packet length and structure

The maximum packet length is shown below. When the destination is LID (logical device ID), it is the destination short column and can contain up to 87 bytes without encryption.

| Network Layer | Encryption | Maximum Payload |
| ----------- | --- | -------- |
| NWK\_SIMPLE | None | 91 |
| NWK\_SIMPLE | Yes | 89 |

Two bytes are set aside as a reserve for future use. This reserve byte can be used at the user's discretion.



The structure of the packet is as follows.

````
|MacH:17[........................................] MacF:2|
         TWENET:3[.....................] TwenetF:1
                  NWK_SIMPLE:11|PAYLOAD
                         　　             (:n is the number of bytes)

1: MacH is IEEE802.15.4 MAC header information
2: TwenetH is information for TWENET identification
3: NWK_SIMPLE is control information for NWK_SIMPLE network
  |Type:1|Src LID:1|Src Addr:4|Dest LID:1|Dst Addr:4|Repeat:1|
4: PAYLOAD is payload
5: TwenetF is a CRC8 checksum (to discriminate TWENET packets)
6: MacF is a CRC16 MAC layer checksum
````
