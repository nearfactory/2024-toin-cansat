# Structure of the source code

The components of an application in TWELITE NET are as follows

* Definitions of [callback functions](../twelite-net-api-ref/krubakku/) and [modules](mojru.md): mandatory definitions, to be written in the main file.
* [User-defined event processing functions](../twelite-net-api-ref/yzaibento/): these are written as required. These functions are designed to work with state machines.
* User-defined functions: These are other processing functions that are application-specific code.

The main files are described below.

## Main file

In the TWENET API, the main file contains the following definition.

* Module declaration (mandate in the Main file)
* Required header file includes
* Callback functions

You can also put other procedure in files other than the main file.

{% hint style="info" %}
The TWENTE API does not have a main() function as is common in C language processors. The first function to be called is the [`cbAppColdStart()`](../twelite-net-api-ref/krubakku/cbappcoldstart.md) function.
{% endhint %}



### Module definition and inclusion of required header files

Define functions and memory to be determined statically at compile time in order to minimise build binary and runtime memory.

Write the definition of the module usage declaration before `cbAppColdStart()` and initialize it by calling the `ToCoNet_REG_MOD_ALL()` macro in `cbAppColdStart()`.

For header files, `"ToCoNet.h"` and `"ToCoNet_mod_prototype.h"` are required.

```c
// Select Modules (define befor include "ToCoNet.h")
#define ToCoNet_USE_MOD_RXQUEUE_BIG
#define ToCoNet_USE_MOD_CHANNEL_MGR

// Includes
#include "ToCoNet.h"
#include "ToCoNet_mod_prototype.h"

…
void cbAppColdStart(bool_t bStart) {
  if (!bStart) {
    // Module Registration
    ToCoNet_REG_MOD_ALL();
  } else {
…
```

### Callback functions

The [callback functions](../twelite-net-api-ref/krubakku/) should always be defined in the source code.

