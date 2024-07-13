# Application ID

TWELIET NET uses 32 bit application IDs to configure different networks for communication on the same physical channel. This means that different application IDs cannot communicate with each other.

{% hint style="info" %}
IEEE802.15.4 uses a 16-bit PAN ID to identify the network, and TWELITE NET also uses the PAN ID mechanism to implement an application ID.
{% endhint %}

## Application ID rules

For the sake of explanation, 32 bits are written as `HHHLLLL`. That is, upper 16 bits are `HHHH` and lower 16 bits are `LLLLL`.

### User configurable values

The addresses specified in this range may be set by other users. In case of duplication, please operate on the premise of coordination between installers.

* HHHH : 0x0001 - 0x7FFF
* LLLL: 0x0001 - 0x7FFF

### Values that can be set by the TWELITE module owner

You can use the ID of your TWELITE module serial number plus 0x80000000 as a non-duplicate address.

* HHHH : 0x8000 - 0x8FFF
* LLLL: 0x0001 - 0xFFFE

