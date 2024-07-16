# ToCoNet_Event_Process()

## Explanation

Event processing with the specified user defined event processing function. If it is specified by a handle, `ToCoNet_Event_ProcessH()` is called.

{% hint style="info" %}
This process is executed synchronously as a function call.
{% endhint %}

{% hint style="warning" %}
User-defined event handling functions must not be called directly. Proper state transitions may not be carried out and may conflict with internal management information.
{% endhint %}

## Arguments

### ToCoNet_Event_Process()

| Type                | Name     | Remark                               |
| ------------------- | -------- | ------------------------------------ |
| teEvent (or uint32) | eEvent   | Event ID                             |
| uint32              | u32evarg | Event Arg                            |
| void\*              | pvFunc   | User-defined event handling function |

### ToCoNet_Event_ProcessH()

| Type                 | Name     | Remark                                             |
| -------------------- | -------- | -------------------------------------------------- |
| teEvent (または uint32) | eEvent   | Event ID                                           |
| uint32               | u32evarg | Event Arg                                          |
| uint8                | u8Handle | Handle(ID) of User-defined event handling function |

## Returns

None.

## Sample code

```c
static u8hnd; // handle
...
    // register
    u8hnd = ToCoNet_Event_Register_State_Machine(vProcessEvCore);
... 
// pass the completion of tx to vProcessEvCore
cbToCoNet_vTxEvent(uint8 u8CbId, uint8 u8Stat) {
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, vProcessEvCore);
    // OR
    ToCoNet_Event_Process(E_EVENT_APP_TX_COMPLETE, 
        u8CbId, u8hnd);
}
```

