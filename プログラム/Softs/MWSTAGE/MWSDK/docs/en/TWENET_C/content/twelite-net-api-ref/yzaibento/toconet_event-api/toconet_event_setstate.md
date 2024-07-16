# ToCoNet_Event_SetState()

## Explanation

State transitions are performed within user-defined event processing functions.

{% hint style="info" %}
After the user-defined event processing function has finished, the `E_EVENT_NEW_STATE` event is called in a new state.
{% endhint %}

## Argumtents

| Type       | Name      | Remark                                                    |
| ---------- | --------- | --------------------------------------------------------- |
| tsEvent \* | pEv       | Management structure for user event definition functions. |
| teState    | eNewState | New state.                                                |

## Returns

None

## Sample code

```c
sToCoNet_Event_SetState(pEv, E_STATE_APP_NORMAL);
```

