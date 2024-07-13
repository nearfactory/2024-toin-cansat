# tsTimerContext

A configuration structure used by the Timer library.

* Must be cleared to zero before setting.
* Allocate memory statically.

| Type   | Name        | Remark                                                   |
| ------ | ----------- | -------------------------------------------------------- |
| uint8  | u8Device    | Specify the timer device (`E_AHI_DEVICE_TIMER0 .... 4`). |
| uint16 | u16Hz       | Specifies the timer frequency in Hz.                     |
| uint8  | u8PreScale  | Sets the prescale for the 16Mhz clock.                   |
| bool_t | bPWMOut     | If TRUE, PWM output is performed.                        |
| bool_t | bDisableInt | If TRUE, disables the interrupt.                         |

