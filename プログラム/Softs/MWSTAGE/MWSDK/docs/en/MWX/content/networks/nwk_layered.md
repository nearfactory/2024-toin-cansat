---
title: "Layered Tree Net NWK_LAYERED"
author: "Mono Wireless Inc."
description: "Layered Tree Net NWK_LAYERED"
---

# layered tree network \<NWK\_LAYERED>

{% hint style="warning" %}
As currently implemented, this BEHAVIOR is intended for reception by the Parent Node of the Layer TreeNet. It is not implemented to transmit from the Parent Node or to use a repeater or Child Node.
{% endhint %}



Initialize it in `setup()` as follows. Assign a role as Parent Node as `NWK_LAYERED::ROLE_PARENT`.

```cpp
#include <NWK_LAYERED>
void setup() {
  ...
  auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
  nwk_ly << NWK_LAYERED::network_role(NWK_LAYERED::ROLE_PARENT);
            // set a role as parent.
}
```



When a packet is received, `on_rx_packet()` is called as in NWK_SIMPLE.

```cpp
void on_rx_packet(packet_rx& rx, bool_t &handled) {
  auto type = rx.get_network_type();
    
  if (type == mwx::NETWORK::LAYERED) {
    ; // Layer tree net packet
    handled = true; // mark as processed
  }
}
```

`rx` is a class that wraps packet information. Internally, no processing of packet information etc. is done except for setting internal flags for processing `.get_network_type()`.

In other words, if you refer to `rx.get_psRxDataApp()` which returns `tsRxDataApp* `, you will get the same packet information as the TWENET C library. `packet_rx` defines some procedures to access this information, but the information obtained remains the same.



## Use with NWK\_SIMPLE

When used with `NWK_SIMPLE`, assign `NWK_LAYERED` to `the_twelite.network` and `NWK_SIMPLE` to `the_twelite.newwork2`.

```cpp
#include <NWK_LAYERED>
#include <NWK_SIMPLE>

void setup() {
    ...
	auto&& nwk_ly = the_twelite.network.use<NWK_LAYERED>();
	auto&& nwk_sm = the_twelite.network2.use<NWK_SIMPLE>();
}

void on_rx_packet(packet_rx& rx, bool_t &handled) {
  auto type = rx.get_network_type();
    
  if (type == mwx::NETWORK::LAYERED) {
      ; // Layer tree net packet
  }
  else if (type == mwx::NETWORK::SIMPLE) {
      ; // Packets of NWK_SIMPLE
  }
  else if (type == mwx::NETWORK::NONE) {
      ; // Normal apps (e.g. App_Twelite)
  }
  else {
      ; // What could not be interpreted
  }
  
  // The packet is processed and no further MWX library intervention is made.
  handled = true;
}
```

Each packet type is determined by `.get_network_type()` as shown above.

* `mwx::NETWORK::LAYERED` : refer to packet information as is.
* `mwx::NETWORK::SIMPLE` : follow `NWK_SIMPLE` processing.
* `mwx::NETWORK::NONE` : No network processing or duplicate packet processing is done. In the App_Twelite standard application, for example, 3 packets are sent out including retransmissions for each transmission. In this case, if all packets are successfully received, `on_rx_packt()` is called three times. Normally, there is no need for the second and third data just even it has been received three times. We need to ignore these duplicate packets.



Refer to `Rcv_Univsl` in Act\_Samples for an example: TWELITE PAL, Act\_samples, and App\_Twelite wireless packets with the same CHANNEL and Application ID but different types are received and processed. In addition, a duplicate checker process is also provided for App\_Twelite.
