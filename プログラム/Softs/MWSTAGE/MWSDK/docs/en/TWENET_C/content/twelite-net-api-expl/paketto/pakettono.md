# Maximum packet length

| Network layer   | Encryption | Dest Addr Type | Src Addr Type | Maximum payload |
| --------------- | ---------- | -------------- | ------------- | -------- |
| None(simple)    | None       | Short          | Short         | 110      |
| None(simple)    | Yes        | Short          | Short         | 108      |
| None(simple)    | None       | Short          | Long          | 102      |
| None(simple)    | Yes        | Short          | Long          | 100      |
| None(simple)    | None       | Long           | Long          | 96       |
| None(simple)    | Yes        | Long           | ロLongング       | 94       |
| Yes(layer-tree) | None       | NA             | NA            | 88       |
| Yes(layer-tree) | Yes        | NA             | NA            | 86       |

※ Up to MWSDK2020\_12, the above packet size had a margin of 6 bytes from the IEEE802.15.4 packet size, but this has been changed to 2 bytes as a reserve for the future. This 2 bytes can be used at the user's discretion.
