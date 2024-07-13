---
title: "on_tx_comp()"
author: "Mono Wireless Inc."
description: "on_tx_comp()"
---
# on_tx_comp()

Called when transmission is complete.

```cpp
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) 
```

This function is called from within the MWX library with data stored in `ev` as `packet_ev_tx` when the wireless packet transmission is finished. If this function is not defined in the application, a weak function that does nothing is linked.

`ev.u8CbId` is the ID at the time of transmission and `ev.bStatus` is a flag indicating success (1) or failure (0) of the transmission.

Setting `b_handled` to *true* in this function tells the MWX library that the incoming packet has been processed in the application. If set to *processed*, it suppresses unnecessary processing. (Do not call event callback functions for `the_twelite.app`, `.board`, or `.settings`)
