---
title: packet_rx
author: "Mono Wireless Inc."
---

# packet\_rx

This class is a wrapper class for TWENET's `tsRxDataApp` structure.

This class object is a wrapper class for [behavior](../behavior/README.md) callback function or by [`on_rx_packets()`](../sys_callbacks/on_rx_packet.md).

In `packet_rx`, in particular, the data payload of the packet can be handled by the `smplbuf` container, and utility functions such as `expand_bytes()` simplify the payload interpretation description.

{% hint style="info" %}
At this time, we have implemented methods and other interfaces, mainly those required for the simple relay network `<NWK_SIMPLE>`.
{% endhint %}


## Methods

### get\_payload()

```cpp
smplbuf_u8_attach& get_payload()
```

Get the data payload of the packet.

{% hint style="info" %}
If `<NWK_SIMPLE>` is used, there is header data for `<NWK_SIMPLE>` at the beginning. The container referred to in the return will be a sub-array excluding the header. If you want to refer to the header part, refer to the `tsRxDataApp` structure by `get_psRxDataApp()`.
{% endhint %}



### get\_psRxDataApp()

```cpp
const tsRxDataApp* get_psRxDataApp() 
```

Obtain the receiving structure of the TWENET C library.



### get\_length()

```cpp
uint8_t get_length()
```

Returns the data length of the payload. The value is the same as `.get_payload().size()`.



### get\_lqi()

```cpp
uint8_t get_lqi()
```

Obtain the LQI value (Link Quality Indicator).

{% hint style="info" %}
LQI is a value that indicates the quality of radio communication, expressed as a number from 0 to 255.

Incidentally, if you want to evaluate it in several levels, you can classify it as follows: less than 50 (bad - less than 80 dbm), 50 to 100 (somewhat bad), 100 to 150 (good), 150 or more (near the antenna), and so on. Please note that these are only guidelines.
{% endhint %}



### get\_addr\_src\_long(), get\_addr\_src\_lid()

```cpp
uint32_t get_addr_src_long()
uint8_t get_addr_src_lid()
```

Get the address of the sender.

`get_addr_src_long()` is the serial number of the sender and MSB(bit31) is always 1.

`get_addr_src_lid()` is the logical ID of the sender and takes values from `0x00`-`0xFE` (the logical ID specified by `<NWK_SIMPLE>`).



### get\_addr\_dst()

```cpp
uint32_t get_addr_dst()
```

Gets the destination address.

The destination address is specified by the source, and the range of values varies depending on the type of destination.

| Value | Explanation                        |
| ------------------- | ------------------------- |
| MSB (bit31) is set.      | `0x00`-`0xFF` | logical ID (8bit) is specified as destination.
| Logical ID (8bit) is specified as the destination. | `0x00`-`0xFF` | A logical ID (8bit) is specified as destination.



### is\_secure\_pkt()

```cpp
bool is_secure_pkt()
```

Returns `true` for encrypted packets and `false` for plaintext.



### get_network_type()
```cpp
uint8_t get_network_type() 
```

Returns network type of the packet identified by Network BEHAVIOR.

| Value | Explanation                        |
| ----------------------- | ------------------------- |
| `mwx::NETWORK::LAYERED` | packets from `<NWK_LAYERED>` |
| `mwx::NETWORK::SIMPLE`  | packets from `<NWK_SIMPLE>` |
| `mwx::NETWORK::NONE`    | neworkless packets |
| others                  | error or unknow packet type |
