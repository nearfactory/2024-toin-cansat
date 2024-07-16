# State

## Built-in state

State names other than `E_STATE_IDLE`(0) can be used freely in user-defined event handling functions. It is also possible to user-define state names.

| State name       | Remark                         |
| ---------------- | ------------------------------ |
| E_STATE_IDLE     | Initial state. The value is 0. |
| E_STATE_RUNNING  | Running                        |
| E_STATE_FINISHED | Completed                      |

## User's own state

Use the values after the `ToCoNet_EVENT_APP_BASE` definition.

```c
#include "ToCoNet_event.h"

typedef enum
{
    E_STATE_APP_BASE = ToCoNet_STATE_APP_BASE,
    E_STATE_USER_INIT,
    E_STATE_USER_FINISH,
    E_STATE_USER_MEASURING
} teStateApp;
```

{% hint style="warning" %}
Do not apply the compile option to reduce the enum to 16 or 8 bits.
{% endhint %}
