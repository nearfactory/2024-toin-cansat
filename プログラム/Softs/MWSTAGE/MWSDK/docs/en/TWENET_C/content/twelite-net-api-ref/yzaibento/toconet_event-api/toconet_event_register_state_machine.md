# ToCoNet_Event_Register_State_Machine()

## Explanation

Register a user-defined event processing function with the system. This function is executed by [`cbAppColdStart(TRUE)`](../../krubakku/cbappcoldstart.md).

## Argument

| Type   | Name   | Remark                                                  |
| ------ | ------ | ------------------------------------------------------- |
| void\* | pvFunc | Specify a user-defined event processing function (\*1). |

```
*1: The function prototype for pvFunc is as follows:
void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg)
```

## Returns

| Type  | Remark                                                     |
| ----- | ---------------------------------------------------------- |
| uint8 | Handle number of the user defined event handling function. |

## Sample code

```c
void vProcessEvCore(
        tsEvent *pEv,
        teEvent eEvent,
        uint32 u32evarg);

void cbAppColdStart(bool_t bInit) {
    if (bInit == FALSE) {
 		...
    } else {
    	...
    	ToCoNet_Event_Register_State_Machine(vProcessEvCore);
    }
}
```
