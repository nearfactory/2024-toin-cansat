# cbToCoNet_vTxEvent()

## Explanation

It is called as a completion notification when a radio packet has been transmitted.

Transmission of radio packets is requested by the API, but there is an indeterminate delay until the transmission is completed. TWENET also has the ability to set delays in transmissions and to perform retransmissions, and in some cases the order of multiple transmission requests and the actual completion of transmissions may be back and forth.

{% hint style="info" %}
In the case of a transmission with Ack, success is achieved when an Ack is received.
{% endhint %}

{% hint style="info" %}
If you want to send as many packets as possible in as short a time as possible, you should send another transmission request as soon as possible after confirming the completion of the transmission.
{% endhint %}

{% hint style="info" %}
Send requests are stored in the send queue. Requests that exceed the send queue are ignored.

If multiple requests are made for a transmission with a set delay, the default number of requests processed at the same time is 3. ([Module](../../twelite-net-api-expl/mojru.md) ToCoNet_USE_MOD_TXRXQUEUE_MID)
{% endhint %}

## Arguments

| Type    | Name       | Remark                                                                                   |
| ------- | ---------- | ---------------------------------------------------------------------------------------- |
| `uint8` | `u8CbId`   | An ID set by the application for the purpose of associating it with the sending request. |
| `uint8` | `u8Status` | Fails if `(u8Status & 0x01) == 0`. Success if `1`.                                       |

### u8Status

| Type of transmission       | Conditions for success                                                                                                                                                                                        |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| with MAC Ack               | Success if a MAC Ack is received.                                                                                                                                                                             |
| w/o MAC Ack                | If a packet is sent even once, it succeeds.                                                                                                                                                                   |
| Network layer transmission | Success if delivery to the neighbouring node is carried out (multiple hops are used to communicate to the final destination, but this status does not confirm whether the final destination has been reached) |

{% hint style="warning" %}
The MAC Ack is a simple telegram response. In some cases it is reported as a failure when in fact it could have been sent to the other end, and in very rare cases it is reported as a success but the other end has not received it.
{% endhint %}

## Returns

None

## Sample code

```c
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus) {
	if(bStatus & 0x01) {
		// success
	} else {
		// fail
	}
}
```

