# ByteQueue

Provides a FIFO queue of type `uint8`.

```c
#include "ByteQueue.h"

// declare structure
tsQueue sQue;
uint8 au8FIFO[128];

// init que
QUEUE_vInit(sQue, au8FIFO, sizeof(au8FIFO));

// if que is empty
if (QUEUE_bEmpty(sQue)) { .. }

// if que is full
if (QUEUE_bFull(sQue)) { .. }

// remove item
if (!QUEUE_bEmpty(sQue)) {
    int16 i16Result = (int16)QUEUE_u8RemoveItem(
            sQue, // the queue 
            TRUE  // block interrupt while removing item.
            );
    uint16 u16ct = QUEUE_u16Count(sQue);
}

// add item
if (!QUEUE_bFull(sQue)) {
    QUEUE_vAddItem(sQue, 'a', FALSE);
}
```
