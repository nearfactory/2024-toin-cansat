---
title: "PulseCounter"
author: "Mono Wireless Inc."
---
# PulseCounter

[pulse counter](../api-reference/predefined\_objs/pulsecounter.md) is an example of an ACT.

The pulse counter counts the number of rising or falling pulses of a signal without intervening microcontroller. It can be used to count irregular pulses and send wireless packets when the count reaches a certain number of times.



## Function of ACT

* Counts the pulses connected to DIO8 on the Child Node side and transmits them wirelessly after a certain period of time or when a certain number of counts are detected.
* The Child Node side operates while sleeping.


## How to use ACT

### Required TWELITE

| Role | Example                                                                                                                                                                                                                                                                                                                            |
| -- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Parent Node | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUE or RED</a><br> ACT <a href="parent_monostick.md"> Parent_MONOSTICK</a> in action.。                                                                                                                                                              |
| Child Node | 1. <a href="https://mono-wireless.com/jp/products/TWE-Lite-DIP/index.html">TWELITE DIP</a><br>2. <a href="https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html">BLUE PAL or RED PAL</a> +<a href=" https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html">environmental sensor pal AMBIENT SENSE PAL</a> |



## Explanation of ACT

### setup()

```cpp
// Pulse Counter setup
PulseCounter.setup();
```

Initializes the pulse counter.



### begin()

```cpp
void begin() {
	// start the pulse counter capturing
	PulseCounter.begin(
		  100 // 100 count to wakeup
		, PIN_INT_MODE::FALLING // falling edge
		);

	sleepNow();
}
```

Starts the pulse counter operation and performs the first sleep. The first parameter of `PulseCounter.begin()` is the count number `100` to generate a wake-up interrupt, and the second is the falling detection `PIN_INT_MODE::FALLING`.



### wakeup()

```cpp
void wakeup() {
	Serial	<< mwx::crlf
			<< "--- Pulse Counter:" << FOURCHARS << " wake up ---"
			<< mwx::crlf;

	if (!PulseCounter.available()) {
		Serial << "..pulse counter does not reach the reference value." << mwx::crlf;
		sleepNow();
	}
}
```

Checks `PulseCounter.available()` when waking up. available, that is, `true`, indicates that the count is greater than or equal to the specified count. If it is `false`, it resleeps.

If the count is more than the specified number, `loop()` processes the transmission and waits for the completion of the transmission.



### loop()

```cpp
uint16_t u16ct = PulseCounter.read();
```

Reads the pulse count value. The counter is reset after the readout.

