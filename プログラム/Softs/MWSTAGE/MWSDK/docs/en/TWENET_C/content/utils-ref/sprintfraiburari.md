# SPRINTF library

A simple implementation of sprintf using the `fprintf` library. Allocates an internal buffer and writes a string according to the format to the buffer. If the string is written in succession, it is appended to the previous one.

### Initialisation

`SPRINTF_vInit128()` allocates a 128-byte region and initializes it.After the initialization, `SPRINTF_Stream` becomes available.

### Print some data by SPRINTF.

1. `SPRINTF_vRewind()` rewinds the buffer to the beginning.
2. Write to the internal buffer by `vfPrintf(SPRINTF_Stream, ...)` 
3. Continue with `vfPrintf(SPRINTF_Stream, ...) `will append to the previous output.

### Referencing the internal buffer

The internal buffer is terminated with `0x00 '\0'`. The first address can be obtained with `SPRINTF_pu8GetBuff()`. And `SPRINTF_u16Length()` is used in order to get the number of characters stored in the buffer.

### Sample code

```c
#include <sprintf.h>

    ...
    // initialisation
    SPRINTF_vInit128(); // allocate the internal buffer of 128 bytes
    
    ...
    static int iCt = 0; // some counter
    
    // write into the internal buffer
    SPRINTF_vRewind(); // rewind!
    vfPrintf(SPRINTF_Stream, "HELLO %d", 1123);
    vfPrintf(SPRINTF_Stream, ",GOODBYE", 1123);
    uint16 u16len = SPRINTF_u16Length();
    
    // Output (NUL-terminated, so it can be treated as a string)
    vfPrintf(&sSerStream, "%003d: %s"LB, 
        iCt++,
        SPRINTF_pu8GetBuff());

```
