# cbToCoNet_vHwEvent()

## 解説

ハードウェアの割り込みなどのイベントです。[`cbToCoNet_u8HwInt()`](cbtoconet_u8hwint.md) 割り込みハンドラが終了した後に呼び出されます。割り込みに対応する通常のアプリケーション処理は、本コールバック関数内に記述します。

## 引数

| 型         | 名前              | 詳細        |
| --------- | --------------- | --------- |
| `uint32 ` | `u32DeviceId`   | 割り込み源     |
| `uint32`  | `u32ItemBitmap` | 割り込みパラメータ |

## 戻り値

なし。

## サンプル

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

