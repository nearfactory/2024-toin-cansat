# Address of the relay network

This is an addressing scheme specific to relay nets.

| Definition Name                      | Value        | Remark                                                           |
| ------------------------------------ | ------------ | ---------------------------------------------------------------- |
| `TOCONET_NWK_ADDR_PARENT`            | `0x80000000` | 親機                                                               |
| `TOCONET_NWK_ADDR_\ NEIGHBOUR_ABOVE` | `0x80000001` | Network address, directly connected relay/parent unit            |
| `TOCONET_NWK_ADDR_BROADCAST`         | `0x8000FFFF` | Network address, broadcast (only specified from the parent unit) |
