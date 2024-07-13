# cbToCoNet_u8HwInt()

## 解説 <a href="jie" id="jie"></a>

ハードウェアの割り込みの割り込みハンドラです。

{% hint style="info" %}
呼び出し遅延が少ないため、[DIO](../../hw-api-ref/perifuraru/dio.md) ポートの制御により直接外部ハードウェアを制御するといった目的に利用できます。
{% endhint %}

{% hint style="warning" %}
割り込みハンドラの処理は最小限にし、速やかに関数から `return` してください。
{% endhint %}

## 引数 <a href="yin-shu" id="yin-shu"></a>

| 型        | 名前              | 詳細        |
| -------- | --------------- | --------- |
| `uint32` | `u32DeviceId`   | 割り込み源     |
| `uint32` | `u32ItemBitmap` | 割り込みパラメータ |

## 戻り値 <a href="ri" id="ri"></a>

| 型       | 解説                                                                                          |
| ------- | ------------------------------------------------------------------------------------------- |
| `uint8` | `FALSE` では [`cbToCoNet_vHwEvent()`](cbtoconet_vhwevent.md)  を呼び出しますが、`TRUE` を返すと呼び出しを行いません。 |

{% hint style="info" %}
頻繁なタイマー割り込みなどでアプリケーションループに負担をかけたくない場合は `TRUE` を選択します。
{% endhint %}

## サンプル <a href="sanpuru" id="sanpuru"></a>

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
