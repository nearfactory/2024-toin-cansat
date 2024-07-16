# SimpleNet

In simple nets, simple node-to-node communication is used without the intervention of relay delivery procedures within TWENET. Two methods of communication are used, one by addressing and the other by broadcast communication. Many applications, such as the standard application App_Twelite, use this simple net.

### About the relay

The communication supported by SimpleNet is itself a star network with no relaying, but there are ways to implement relaying that are simple and practical.

{% hint style="info" %}
There are a number of ways in which you can make a relay delivery. Here are some of the most common relay methods.

This is a method of relaying by means of broadcast communication. The received packets contain lifetime data, which is the number of times they are relayed, and each time they are relayed, the lifetime count is reduced and the packets are relayed a certain number of times unconditionally.
{% endhint %}

{% hint style="info" %}
It is also possible to implement networks based on specific requirements. For example, a linear network with a predetermined short address.

The parent station has an address of 0x0000, and the short addresses are assigned in order from the parent station to 0x0001 0x0002 ... and so on. It is assumed that the addresses before and after the own station are always available for communication.

This way, deliveries from the child station to the parent station will be made to an address that is one less than its own.
{% endhint %}

{% hint style="danger" %}
#### About the automatic configuration of the network

Searching for repeaters in the surrounding area and determining the relay station is not recommended in many applications because of the very high overhead in radio communication.

* A number of radio transmissions are required to determine the destination of the communication.
  * Battery drain
  * It takes a long time to be able to communicate after start-up
* A relay station which is not highly stable may be selected.
  * If the search conditions are set to the best communication at the time, a distant station with good conditions will be chosen by chance.
* It also takes time to learn of a communication breakdown at the other end.
  * It is not possible to assume that a communication partner has ceased to exist until conditions such as a series of communication failures are met, so a long period of time is required before a communication partner can be found again (or a prescription such as dynamically switching communication partners is necessary).
* A large number of radio transmissions are required to maintain the relay network.
  * This results in a squeeze on bandwidth and consequently a reduction in the number of nodes that can be accommodated.

There are principle issues involved in determining and maintaining relay stations as described above. Any approach has its advantages and disadvantages and must be considered in accordance with the requirements.

In general, this type of communication is not practical for battery-powered nodes.
{% endhint %}

### About the role and destination

Even a SimpleNet Network is usually a network with defined roles, such as parent and child. There are a number of ways in which this role can be implemented in wireless packet communications.

{% hint style="info" %}
#### By address

This is a method of realising a role in an application using the same addressing scheme supported by radio packets. Specifically, we use short addresses, where the address of the parent station is 0x0000 and the address of the child station is 0x0001 ...

When delivering to the parent station, the child station only needs to deliver to 0x0000 at all times, and when no relay is required, efficient radio delivery can be achieved using ACK communication.
{% endhint %}

{% hint style="info" %}
#### By data in the packet.

If you want to achieve relay, you store the equivalent of an address or role (here called a logical address) as data in the radio packet.

The parent station is assigned the logical address 0x00. The child station is allocated the logical address 0x01... The logical address is stored as data in the packet for wireless transmission. This logical address is stored as the data in the packet during wireless transmission as sender and destination.

We would like to use the short address mechanism, but it is not convenient for relaying (because we send relay packets with the short address of the relay station as the source address), so we store the data in the packet.

Therefore, all radio packets are sent as broadcast. Because they are broadcast packets, all nodes within radio range will receive them. (Note: Even if a packet has a specific destination, its contents will be received by all nodes within radio range. They just ignore the incoming data because it has a different destination).

The data in the received packet is used by the application to decide whether the packet should be processed at its own station or relayed.
{% endhint %}

