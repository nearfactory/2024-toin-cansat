# ToCoNet_u32GetSerial()

## Explanation

Getting the module serial number. This serial number is the value of the individual module and is also used for the purpose of specifying the communication partner.

{% hint style="info" %}
The value is the hexadecimal serial number displayed on the TWE module plus `0x80000000`.
{% endhint %}

## Argument

None

## Returns

| Type     | Remark                      |
| -------- | --------------------------- |
| `uint32` | 32 bit module serial value. |

## Sample code

```c
uint32 u32ser;
u32ser = ToCoNet_u32GetSerial();
```

