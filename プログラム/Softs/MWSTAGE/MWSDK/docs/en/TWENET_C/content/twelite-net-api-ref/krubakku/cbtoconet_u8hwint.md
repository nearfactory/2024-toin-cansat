# cbToCoNet_u8HwInt()

## Explanation <a href="jie" id="jie"></a>

Interrupt handler for hardware interrupts.

{% hint style="info" %}
Due to the lower call delay, it can be used to control external hardware directly by controlling the DIO port.
{% endhint %}

{% hint style="warning" %}
Interrupt handler processing should be kept to a minimum, and the function should be returned as soon as possible.
{% endhint %}

## Arguments <a href="yin-shu" id="yin-shu"></a>

| Type     | Name            | Remark              |
| -------- | --------------- | ------------------- |
| `uint32` | `u32DeviceId`   | Interrupt source    |
| `uint32` | `u32ItemBitmap` | Interrupt parameter |

## Returns <a href="ri" id="ri"></a>

| Type    | Remark                                                                                                          |
| ------- | --------------------------------------------------------------------------------------------------------------- |
| `uint8` | `FALSE` will call [`cbToCoNet_vHwEvent()`](cbtoconet_vhwevent.md), but returning `TRUE` will not make the call. |

{% hint style="info" %}
Select `TRUE` if you do not want to overload the application loop with frequent timer interrupts, for example.
{% endhint %}

## Sample code <a href="sanpuru" id="sanpuru"></a>

```c
uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap) {
    uint8 u8handled = FALSE;
    switch (u32DeviceId) {
    case E_AHI_DEVICE_TIMER0: // increment countr on TIMER0
        u32counter++;
        u8handled = TRUE; // no vHwEvent()
        break;
    default:
        break;
    }
    return u8handled;
}
```
