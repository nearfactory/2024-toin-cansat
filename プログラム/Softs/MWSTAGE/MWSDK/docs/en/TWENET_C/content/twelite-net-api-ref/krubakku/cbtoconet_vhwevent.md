# cbToCoNet_vHwEvent()

## 解説

These are events such as hardware interrupts. This function is called after the interrupt handler  `cbToCoNet_u8HwInt()` exits. Normal application processing in response to an interrupt is described within this callback function.

## Arguments

| Type      | Name            | Remark              |
| --------- | --------------- | ------------------- |
| `uint32 ` | `u32DeviceId`   | Interrupt source    |
| `uint32`  | `u32ItemBitmap` | Interrupt parameter |

## Returns

None

## Sample code

```c
void cbToCoNet_vHwEvent(Uint32 u32DeviceId, uint32 u32ItemBitmap) {
    Switch (u32DeviceId) {
    case E_AHI_DEVICE_SYSCTRL: // DIO etc
        if (u32ItemBitmap & (1UL << 12) {
            ; // interrupt on DIO12
        }
        break;
    case E_AHI_DEVICE_TICK_TIMER: // TickTimer
        vPortSet_TrueAsLo(18, u32TickCount_ms & 0x400); // blink LED every sec.
        break;
    default:
        break;
}
```

