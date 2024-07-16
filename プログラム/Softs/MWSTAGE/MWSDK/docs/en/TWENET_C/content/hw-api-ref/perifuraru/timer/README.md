# Timer

Hardware timers that use 16-bit counters (TIMER0 ... 4) are available.

This timer can be used as a peripheral, such as a PWM output, or it can be used as a hardware timer for software processing.

## Pin assignment

| Timer Name | Primary pin assgin | Sub   | Sub2  | explanation           |
| ---------- | ------------------ | ----- | ----- | ------------ |
| TIMER0     | 8, 9, 10           | 2,3,4 |       | General-purpose timer function available |
| TIMER1     | 11                 | 5     |       | PWM output only     |
| TIMER2     | 12                 | 6     | DO0\* | PWM output only      |
| TIMER3     | 13                 | 7     | DO1\* | PWM output only      |
| TIMER4     | 17                 | 8     |       | PWM output only      |

{% hint style="info" %}
It can be changed to a sub-allocation with the peripheral `API vAHI_TimerSetLocation()`.

* TIMER2 and 3 can be reassigned to DO0 and 1. However, for these ports, the TWELITE wireless microcontroller may not start normally unless they are at the Vcc level when power is turned on.
{% endhint %}

{% hint style="warning" %}
In TWELITE RED, there are more PWM output systems, but the standard response in TWELITE NET is TIMER0..4.
{% endhint %}



## Initialize and start, end

Declare the ports that will not be used by the Timer by referring to the peripheral manual in advance. If this declaration is not made, the corresponding port will not be available as a general-purpose IO port after the Timer is initialized.

In the following example, bit0, 1, and 2 are set. This releases all PWM pins used of TIMER0 for general-purpose IO.

```c
vAHI_TimerFineGrainDIOControl(0x7);
```

> #### the parameter of vAHI_TimerFineGrainDIOControl()

| <br>Bit | Timer Input/Output       |
| -------------- | ------------------------ |
| 0              | TIMER 0 gate/event input |
| 1              | TIMER 0 capture input    |
| 2              | TIMER 0 PWM output       |
| 3              | TIMER 1 PWM output       |
| 4              | TIMER 2 PWM output       |
| 5              | TIMER 3 PWM output       |
| 6              | TIMER 4 PWM output       |
| 7              | reserved                 |



Next, the initialization structure [`tsTimerContext` ](timerraiburari/tstimercontext.md)for the Timer library is statically allocated and configured.必要なタイマーの周波数に対して適切な PreScale を選択するようにします。

{% hint style="info" %}
The prescale (`u8PreScale`) selects a smaller value in most cases for the required period (frequency).

The Timer prescales the counter value at 16Mhz, and the count value (Ct) for one cycle is given by the following equation

```
Ct = 16,000,000 / 2^p / hz
    p  : prescale value
    hz : Timer cycle
```

Set this `Ct` value so that it does not exceed the maximum number of 16 bits (65535). For example, if you want a 64Hz timer, `p=2` gives `Ct = 62500`, `p=4` gives `Ct = 15625`. If you need a PWM output with a more accurate duty ratio, choose a slightly smaller `p=2`, but if you simply need a timer for software, `p=4` makes no difference.
{% endhint %}

```c
tsTimerContext sTimerApp; // global or static allocation

// set 64ticks/sec
memset(&sTimerApp, 0, sizeof(tsTimerContext));
sTimerApp.u8Device = E_AHI_DEVICE_TIMER0;
sTimerApp.u16Hz = 64;
sTimerApp.u8PreScale = 4; // 15625ct@2^4
```

Finally, start the Timer.

```c
vTimerConfig(&sTimerApp); // initialize
vTimerStart(&sTimerApp); // start timer
```

To stop the timer temporarily, call `vTimerStop()`. To disable it completely, call `vTimerDisable()`.

```c
// just stop the timer
vTimerStop(&sTimerApp);
...
// restart
vTimerStart(&sTimerApp);
...
// now, disable timer completely
vTimerStop(&sTimerApp);
vTimerDisable(&sTimerApp);
```

## Timer interrupt

The `u8DeviceID` parameter of `ToCoNet_u8HwInt()` and `ToCoNet_vHwEvent()` takes `E_AHI_DEVICE_TIMER0` to `E_AHI_DEVICE_TIMER4`.

{% hint style="warning" %}
As the interrupt frequency increases, the overhead of processing hard interrupts and events becomes non-negligible.

* Disable unnecessary timer interrupts.
  * Set `bDisableInt` of `tsTimerContext` to `TRUE`.
* Handled by interrupt processing only, no hard events are generated.
  * To `return` an interrupt handler with `TRUE`.
{% endhint %}

```c
// hardware interrupt (return quickly!)
uint8 cbToCoNet_u8HwInt(
		uint32 u32DeviceId, 
		uint32 u32ItemBitmap) {
	uint8 u8handled = FALSE;

	switch (u32DeviceId) {
	case E_AHI_DEVICE_TIMER0:
		_C {
			static bool_t b;
			b = !b;
			vPortSet_TrueAsLo(PORT_DO1, b);
			
			u8handled = FALSE; // if TRUE, no HwEvent follows.
		}
		break;
	}
}

// hardware event (in the application loop)
//   where we can make a bigger job, like RF transmit.
void cbToCoNet_vHwEvent(
		uint32 u32DeviceId,
		uint32 u32ItemBitmap) {
		
	switch (u32DeviceId) {
	case E_AHI_DEVICE_TIMER0:
		vPutChar(&sSerStream, 'x'); // put x every tick
		if (...) {
			...
			ToCoNet_bMacTxReq(...); // send a packt
		}
		break;
	}	
	
}
```

## PWM output

The PWM output can be set as the output of the Timer.

{% hint style="info" %}
In order to improve the accuracy of the Duty ratio, it is recommended that the prescale value described above be kept small enough.
{% endhint %}

The following settings are required to have PWM output.

* Set `bPWMOut` of `tsTimerContext` to `TRUE`.
* Set `u16duty` of `tsTimerContext` to a value of 0..1024.
* Output is not disabled by `vAHI_TimerFineGrainDIOControl()`.

```c
tsTimerContext sTimerPWM; // global or static allocation

// initialize
memset(&sTimerPWM, 0, sizeof(tsTimerContext));
sTimerPWM.u8Device = E_AHI_DEVICE_TIMER1;
sTimerPWM.u16Hz = 1000; // 1Khz
sTimerPWM.u8PreScale = 0;
sTimerPWM.u16duty = 512; // 50%
sTimerPWM.bPWMout = TRUE; // PWM out
sTimerPWM.bDisableInt = TRUE; // no interrupt (for CPU save)
// start
vTimerConfig(&sTimerPWM);
vTimerStart(&sTimerPWM);

...
// change duty
sTimerPWM.u16Duty = 256; // set new duty ratio
vTimerStart(&sTimerPWM); // just start again to change duty
```
