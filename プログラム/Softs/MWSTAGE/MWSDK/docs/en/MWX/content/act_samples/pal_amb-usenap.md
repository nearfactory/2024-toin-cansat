---
title: "PAL_AMB-usenap"
author: "Mono Wireless Inc."
---
# PAL\_AMB-usenap

The sample in [PAL\_AMB ](pal\_amb.md) is slightly modified so that the waiting time (approx. 50 ms) during sensor data acquisition is replaced by a sleep wait.

{% hint style="success" %}
Please see the explanation of the ACT in [PAL\_AMB](pal\_amb.md) before the explanation of this ACT.
{% endhint %}

## Explanation of ACT

### begin()

The `begin()` function exits the `setup()` function (after which TWENET is initialized) and is called just before the first `loop()`.

```cpp
void begin() {
	sleepNow(); // the first time is just sleeping.
}
```

The first sleep is performed after `setup()` ends. Although sensor data acquisition is started during `setup()`, this result is not evaluated and is not necessarily necessary, since it means that the sensor is run once in advance.


### wakeup()

Procedures after waking up. The following process is performed.

* If the sensor data acquisition start has not yet been performed, the sensor data acquisition is performed and a short sleep is entered.
* Since the start of sensor data acquisition was performed immediately before, the data is checked and sent wirelessly.

```cpp
void wakeup() {
	if (!b_senser_started) {
		// delete/make shorter this message if power requirement is harder.	
		Serial	<< mwx::crlf
				<< "--- PAL_AMB:" << FOURCHARS << " wake up ---"
				<< mwx::crlf
				<< "..start sensor capture again."
				<< mwx::crlf;

		startSensorCapture();
		b_senser_started = true;

		napNow(); // short period sleep.
	} else {
		Serial << "..wake up from short nap.." << mwx::crlf;

		auto&& brd = the_twelite.board.use<PAL_AMB>();

		b_senser_started = false;

		// tell sensors waking up.
		brd.sns_LTR308ALS.process_ev(E_EVENT_START_UP);
		brd.sns_SHTC3.process_ev(E_EVENT_START_UP);
	}
}
```

The above branch is controlled by the global variable `b_sensor_started`. If `!b_sensor_started`, it starts acquiring a sensor (`startSensorCapture()`) and goes into a short sleep by `napNow()`. The time is 100ms.

After returning from sleep by `napNow()`, the clause `b_sensor_started==true` is executed. Here, the `E_EVENT_START_UP` event is notified to the two sensors. This event means that enough time has passed for the sensors to finish acquiring. Based on this notification, `sns_LTR308ALS` and `sns_SHTC3` are made available. After this, it will go to `loop()` and wireless packets will be sent.

{% hint style="warning" %}
The event that notifies the sensor is used to determine if the required time wait is over. Whether or not the actual time has elapsed depends on whether or not the correct time was set with `napNow()`. If the wake-up time is short, it is expected that the time elapsed is not enough to meet the required time elapsed and that subsequent processing will result in errors such as sensor data not being available.
{% endhint %}



### napNow()

Perform a very short sleep.

```cpp
void napNow() {
	uint32_t u32ct = 100;
	Serial << "..nap " << int(u32ct) << "ms." << mwx::crlf;
	the_twelite.sleep(u32ct, false, false, TWENET::SLEEP_WAKETIMER_SECONDARY);
}
```

If the second parameter of sleep is set to true, the next wake-up time is adjusted based on the previous sleep wake-up time. This is useful if you always want to wake up every 5 seconds.

If the third parameter is set to true, the sleep mode is set without memory retention. After waking up, wakup() will not be called and the process will be the same as power-on.

The fourth specifies the use of the second wake-up timer. Here, the first is used for normal sleep and the second is used for short sleep. There is no strong reason to use the second timer in this ACT, but if, for example, the user wants to wake up every 5 seconds as described above, using the first timer for a short sleep would reset the counter value, which would complicate the elapsed time correction calculation, so the second timer is used.

{% hint style="info" %}
Setting too short a sleep time does not balance the energy cost of reinitializing the system after returning from sleep. As a rule of thumb, the minimum time should be 30-50 ms.
{% endhint %}



