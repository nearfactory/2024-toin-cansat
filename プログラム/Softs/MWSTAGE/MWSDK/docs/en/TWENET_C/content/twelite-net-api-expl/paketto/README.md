# Wireless packets

## Destination

Destination information is required for the delivery of wireless packets. The destination needs to be aware of three elements

* Channel
* Application ID
* Destination address

The channel is the physical communication channel. 16 channels can be specified in total, from ch11 to 26. If the communication channel is different, it cannot be physically received. In order to communicate, the communication channels must first match.

It is necessary to be able to run more than one radio system on the same communication channel; if more than one system using TWENET is in the same location and may communicate on the same channel, they will be treated as separate networks according to their application ID.

Finally, if both the channel and the application ID match, communication is possible by setting the destination address appropriately. There are three types of destination address: short address (which can be set arbitrarily in the range of 12-bit values), long address (communication by individual identification address) and broadcast communication.

## Sender's address

TWELITE NET allows the sender's address to be sent as a short or long address.

## Maximum packet length

Wireless packets are based on the packet structure of the IEEE802.15.4 standard, but the maximum data size that can be transmitted is the amount of data that is not directly used by the user, such as destination data and headers used by TWENET.

By reducing the data size (saving payload, making active use of short addresses), the current-consuming radio transmission time can be reduced. Smaller packets are also relatively less likely to fail.

