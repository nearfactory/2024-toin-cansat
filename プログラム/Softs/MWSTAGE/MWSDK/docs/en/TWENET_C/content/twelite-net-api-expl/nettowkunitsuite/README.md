# About the network

TWELITE NET offers two methods for the delivery of wireless packets.

## Simple net

Simple net communicates only by application ID and address. Communication within the radio range is free. There is no concept of parent and child, the application decides the role.

For example, a network with a short address of 0 as the parent station and a maximum of 101 child stations from 1 to 100. A child station can always send to address 0 to communicate to the parent station.

Simple net carries out the following communications

* Communication with a [short address](../paketto/adoresuno.md#short-address)
* Communication with a[ long address](../paketto/adoresuno.md#long-address)
* Broadcast communications to all radio coverage areas

It also provides a way of knowing if your communication has been successful.

* Receive confirmation packet from destination MAC ACK communication.

{% hint style="info" %}
MAC ACKs are useful because they allow you to confirm the arrival of a message at the destination (in exceptional cases this may not be possible). However, the destination address must be known in advance.

If MAC ACK is used, a certain number of retransmissions are automatically performed, but if the signal condition is good, there will be no retransmissions, but if it is bad, the maximum number of retransmissions will be performed. In contrast, when not used, the same packet is always sent twice or three times. Which is better is a case by case basis.

One of the difficulties in using MAC ACK is when relaying is used. If a child station wants to communicate to the parent station through a relay, the child station must decide before transmission whether it will send directly to the parent station or to the relay, otherwise MAC ACK cannot be used. This is fine if the destination is known at the time of installation, but many applications require the installation to transmit to dynamically close stations, and at least one round trip is required to determine the most proximate and likely station to communicate with. This communication may fail, or it may choose an unreliable relay. It is also a cause of battery drain. As long as a stable repeater is determined and maintained, efficient communication is possible, but if not, the network will be unstable due to communication interruptions.

Applications that use simple nets, such as App_Twelite, do not dare to use MAC ACK and relay by means of broadcast communication. Two simple rules are used for relaying: once a packet has been relayed a certain number of times, it is not relayed any more, and the same packet is not relayed again. If the number of relay stages is not large, the efficiency does not deteriorate much, and above all, it is stable in that as long as the child station sends a packet, it is always likely to reach the parent station as long as there are repeaters around.
{% endhint %}

## Relay net

Relay nets automate the construction and delivery of relay networks within certain defined conditions, such as the configuration of the network.
