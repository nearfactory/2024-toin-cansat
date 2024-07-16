# tsTimerContext

Timer ライブラリで用いる設定用の構造体。

* 0クリアすること。
* 静的に確保すること。

| 型      | 名前          | 解説                                        |
| ------ | ----------- | ----------------------------------------- |
| uint8  | u8Device    | タイマーデバイスを指定する (E_AHI_DEVICE_TIMER0 .. 4)。 |
| uint16 | u16Hz       | タイマー周波数を Hz で指定する。                        |
| uint8  | u8PreScale  | 16Mhz クロックに対するプリスケールを設定する。                |
| bool_t | bPWMOut     | TRUE なら PWM 出力を行う。                        |
| bool_t | bDisableInt | TRUE なら割り込みを禁止する。                         |

