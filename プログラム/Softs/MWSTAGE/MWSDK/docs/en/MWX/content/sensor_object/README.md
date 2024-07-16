---
title: "Sensor Devices (SNS)"
author: "Mono Wireless Inc."
description: Sensor behaviors
---

# Sensor Devices (SNS)

We have classes that formalize procedures for sensors and various devices.

{% hint style="info" %}
When using TWELITE PAL equipped sensors and devices, please also refer to the description in [board(BRD)](.../boards/). /TWELITE PAL specific procedures may be required and are included in the board definition.
{% endhint %}

## Procedures for sensor handling

Some procedures, such as temperature sensors, are common, such as starting sensor operation, waiting time, and reading the sensor value.

Please perform `Wire.begin()` before handling I2C sensors. After returning from sleep, reinitialization of Wire is performed automatically, so no special description is necessary (Note: If `Wire.end()` is explicitly called from the user code, reinitialization is described in `wakeup()`).

```cpp
void setup() {
  auto&& brd = the_twelite.board.use<PAL_AMB>();
  ..
  Wire.begin();
	brd.sns_SHTC3.begin();
	brd.sns_LTR308ALS.begin();
}
```



The procedure after the start of the readout depends on the type of sensor, for example, both sensors of `<PAL_AMB>` manage the elapsed time. For example, the `<PAL_AMB>` sensor uses the `process_ev()` method to tell the sensor object how much time has elapsed.

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_AMB>();

	// mostly process every ms.
	if (TickTimer.available()) {
		//  wait until sensor capture finish
		if (!brd.sns_LTR308ALS.available()) {
			brd.sns_LTR308ALS.process_ev(E_EVENT_TICK_TIMER);
		}

		if (!brd.sns_SHTC3.available()) {
			brd.sns_SHTC3.process_ev(E_EVENT_TICK_TIMER);
		}
..
```

In the above example, a TickTimer is used as a starting point every 1 ms to tell the passage of time. The `E_EVENT_TICK_TIMER` tells the sensor object that 1ms has passed.

When enough time has elapsed, for example to return from sleep, `E_EVENT_START_UP` is passed instead. The sensor object will be processed as ready to read immediately.

{% hint style="warning" %}
Neither of these processes is guaranteed to match the passage of time in the real world. If the actual elapsed time is insufficient, the sensor will return an error or an unexpected value.
{% endhint %}



## Sensor common methods

### setup()

```cpp
void setup(uint32_t arg1 = 0, uint32_t arg2 = 0) 
```

Initialize the sensor.



### begin(), end()

```cpp
void begin(uint32_t arg1 = 0, uint32_t arg2 = 0)
void end()
```

Start and end sensor acquisition.



### process\_ev()

```cpp
void process_ev(uint32_t arg1, uint32_t arg2 = 0)
```

In the case of a sensor with wait processing, `arg1` is given `E_EVENT_TICK_TIMER` or `E_EVENT_START_UP` to indicate the elapse of time. After calling this method, if the required time has elapsed, it will be available and the sensor value can be read.



### available()

```cpp
bool available()
```

Returns `true` when the sensor satisfies the readout condition.



### probe()

```cpp
bool probe()
```

(Only supported sensors) Returns `true` when the sensor is connected.

{% hint style="warning" %}
Initial communication immediately after `probe()` may fail.
{% endhint %}
