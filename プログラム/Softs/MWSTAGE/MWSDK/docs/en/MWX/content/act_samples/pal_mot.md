---
title: "PAL_MOT-fifo"
author: "Mono Wireless Inc."
---
# PAL\_MOT-fifo

[MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) is used to acquire sensor values.

{% hint style="success" %}
The ACT of the ACT includes the following.

* Sending and receiving wireless packets
* Interactive settings mode configuration - [\<STG\_STD>](../settings/stg\_std.md)
* State transition control by state machine - [\<SM\_SIMPLE>](../api-reference/classes/smsimple-suttomashin.md)
* [\<PAL\_MOT>](../boards/pal/pal\_mot.md) or [\<CUE>](../boards/cue.md)Board operation with board BEHAVIOR
{% endhint %}



## ACT Features

* Uses the motion sensor PAL MOTION SENSE PAL to continuously measure the acceleration of the accelerometer and transmit it wirelessly.
* Use the sleep function, to operate on coin cell batteries.



## how to use act

### Required TWELITE

| Role | Example                                                                                                                                                                                           |
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Parent Node | <a href="https://mono-wireless.com/jp/products/MoNoStick/">MONOSTICK BLUE or RED</a><br>Act <a href="parent_monostick.md">Parent\_MONOSTICK</a> to work.                                 |
| Child Node | [BLUE PAL or RED PAL](https://mono-wireless.com/jp/products/twelite-pal/BnR/index.html) + [Motion Sensor PAL MOTION SENSE PAL](https://mono-wireless.com/jp/products/twelite-pal/sense/amb-pal.html) |



## Explanation of ACT

### Include


```cpp
#include <TWELITE>
#include <NWK_SIMPLE>
#include <PAL_>
```

 Board BEHAVIOR [`<PAL_MOT>`](../boards/pal/pal\_mot.md) is included.



### setup()

```cpp
void setup() {
	/*** SETUP section */
	// board
	auto&& brd = the_twelite.board.use<PAL_MOT>();
	brd.set_led(LED_TIMER::BLINK, 100);

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)     
		<< TWENET::channel(CHANNEL);

	// Register Network
	auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
	nwk	<< NWK_SIMPLE::logical_id(0xFE); 
	
	/*** BEGIN section */
	the_twelite.begin(); // start twelite!
	brd.sns_MC3630.begin(SnsMC3630::Settings(
		SnsMC3630::MODE_LP_14HZ, SnsMC3630::RANGE_PLUS_MINUS_4G));

	/*** INIT message */
	Serial << "--- PAL_MOT(Cont):" << FOURCHARS 
				 << " ---" << mwx::crlf;
}

```



First, register the board BEHAVIOR `<PAL_MOT>`. When the board BEHAVIOR is initialized, the sensors and DIOs are initialized. The reason for doing this first is that it is common to check the status of the board's DIP SW, etc., and then configure the network settings, etc.

```cpp
auto&& brd = the_twelite.board.use<PAL_MOT>();

u8ID = (brd.get_DIPSW_BM() & 0x07) + 1;
if (u8ID == 0) u8ID = 0xFE; // 0 is to 0xFE
```

Here, 3 bits of the 4-bit DIP SW on the board are read and set as the Child Node ID. 0 means no ID (`0xFE`).

Set the LED settings. Here the ON/OFF blinking is set to blink every 10ms (in an application that sleeps and has a short wake-up time, this is almost equivalent to setting the LED to turn on during wake-up).

```cpp
	brd.set_led(LED_TIMER::BLINK, 10); // blink (on 10ms/ off 10ms)
```



#### Accelerometer initialization

```cpp
	brd.sns_MC3630.begin(SnsMC3630::Settings(
		SnsMC3630::MODE_LP_14HZ, SnsMC3630::RANGE_PLUS_MINUS_4G));
```

Starts accelerometer measurement. The accelerometer settings (`SnsMC3630::Settings`) specify the measurement frequency and measurement range. Here we use 14HZ measurement (`SnsMC3630::MODE_LP_14HZ`) with ±4G range (`SnsMC3630::RANGE_PLUS_MINUS_4G`).

Once started, the accelerometer takes 14 measurements per second and the values are stored in a FIFO queue inside the sensor. The sensor is notified when 28 measurements have been taken.



### begin()

The `begin()` function exits the `setup()` function (after which TWENET is initialized) and is called just before the first `loop()`.

```cpp
void begin() {
	sleepNow(); // the first time is just sleeping.
}
```

Call `sleepNow()` after `setup()` ends to perform the first sleep.



### sleepNow()

```cpp
void sleepNow() {
	pinMode(PAL_MOT::PIN_SNS_INT, WAKE_FALLING);
	the_twelite.sleep(60000, false);
}
```

Before going to sleep, set up an interrupt for the accelerometer's DIO pin, which is generated when the FIFO queue reaches a certain number. The second parameter is `PIN_MODE::WAKE_FALLING`. This is a setting to wake up when the pin state changes from HIGH to LOW.

The third line executes sleep with `the_twelite.sleep()`. The parameter 60000 is the wake-up setting required to reset the watchdog on the TWELITE PAL board. If not reset, a hard reset will be performed after 60 seconds.


### wakeup()

When the accelerometer wakes up from sleep due to a FIFO interrupt from the accelerometer, `wakeup()` is called. After that, `loop()` is called each time. Before `wakeup()`, each peripheral such as UART and devices on the board are woken up (e.g. resetting the watchdog timer). For example, it restarts the LED lighting control.

```cpp
void wakeup() {
	Serial << "--- PAL_MOT(Cont):" << FOURCHARS
	       << " wake up ---" << mwx::crlf;

	b_transmit = false;
	txid[0] = 0xFFFF;
	txid[1] = 0xFFFF;
}
```

Here we are initializing variables to be used in `loop()`.



### loop()

Here, the acceleration information stored in the FIFO queue in the accelerometer is retrieved and packet transmission is performed based on this information. After the packet transmission is completed, sleep is executed again.

```cpp
void loop() {
	auto&& brd = the_twelite.board.use<PAL_MOT>();

	if (!b_transmit) {
		if (!brd.sns_MC3630.available()) {
			Serial << "..sensor is not available." 
					<< mwx::crlf << mwx::flush;
			sleepNow();
		}

		// send a packet
		Serial << "..finish sensor capture." << mwx::crlf
			<< "  seq=" << int(brd.sns_MC3630.get_que().back().t) 
			<< "/ct=" << int(brd.sns_MC3630.get_que().size());

		// calc average in the queue.
		{
			int32_t x = 0, y = 0, z = 0;
			for (auto&& v: brd.sns_MC3630.get_que()) {
				x += v.x;
				y += v.y;
				z += v.z;
			}
			x /= brd.sns_MC3630.get_que().size();
			y /= brd.sns_MC3630.get_que().size();
			z /= brd.sns_MC3630.get_que().size();

			Serial << format("/ave=%d,%d,%d", x, y, z) << mwx::crlf;
		}

		for (int ip = 0; ip < 2; ip++) {
			if(auto&& pkt = 
				the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet())
				
				// set tx packet behavior
				pkt << tx_addr(0x00)
					<< tx_retry(0x1)
					<< tx_packet_delay(0, 0, 2);
			
				// prepare packet (first)
				uint8_t siz = (brd.sns_MC3630.get_que().size() >= MAX_SAMP_IN_PKT)
									? MAX_SAMP_IN_PKT : brd.sns_MC3630.get_que().size();
				uint16_t seq = brd.sns_MC3630.get_que().front().t;
			
				pack_bytes(pkt.get_payload()
					, make_pair(FOURCHARS, 4)
					, seq 
					, siz
				);

				// store sensor data (36bits into 5byts, alas 4bits are not used...)
				for (int i = 0; i < siz; i++) {
					auto&& v = brd.sns_MC3630.get_que().front();
					uint32_t v1;

					v1  = ((uint16_t(v.x/2) & 4095) << 20)  // X:12bits
						| ((uint16_t(v.y/2) & 4095) <<  8)  // Y:12bits
						| ((uint16_t(v.z/2) & 4095) >>  4); // Z:8bits from MSB
					uint8_t v2 = (uint16_t(v.z/2) & 255);   // Z:4bits from LSB
					pack_bytes(pkt.get_payload(), v1, v2); // add into pacekt entry.
					brd.sns_MC3630.get_que().pop(); // pop an entry from queue.
				}

				// perform transmit
				MWX_APIRET ret = pkt.transmit();

				if (ret) {
					Serial << "..txreq(" << int(ret.get_value()) << ')';
					txid[ip] = ret.get_value() & 0xFF;
				} else {
					sleepNow();
				}
			}
		}

		// finished tx request
		b_transmit = true;
	} else {
		if(		the_twelite.tx_status.is_complete(txid[0])
			 && the_twelite.tx_status.is_complete(txid[1]) ) {

			sleepNow();
		}
	}
}

```



The `b_transmit` variable controls the behavior within `loop()`. After a successful transmission request, this value is set to 1, and the program waits for the packet transmission to complete.

```cpp
	if (!b_transmit) {
```



First check to see if the sensor is available. Since it is after an interrupted wake-up, it is not normal for it not to be AVAILABLE, and it will go straight to sleep.

```cpp
if (!brd.sns_MC3630.available()) {
	Serial << "..sensor is not available." 
			<< mwx::crlf << mwx::flush;
	sleepNow();
}
```



It is not used in the wireless transmission packet, but we will check the information on the acceleration taken out.

```cpp
Serial << "..finish sensor capture." << mwx::crlf
	<< "  seq=" << int(brd.sns_MC3630.get_que().front().t) 
	<< "/ct=" << int(brd.sns_MC3630.get_que().size());

// calc average in the queue.
{
	int32_t x = 0, y = 0, z = 0;
	for (auto&& v: brd.sns_MC3630.get_que()) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	x /= brd.sns_MC3630.get_que().size();
	y /= brd.sns_MC3630.get_que().size();
	z /= brd.sns_MC3630.get_que().size();

	Serial << format("/ave=%d,%d,%d", x, y, z) << mwx::crlf;
}
```

The measurement results of the accelerometer are stored in a FIFO queue obtained by `brd.sns_MC3630.get_que()`.

The structure `axis_xyzt` that stores the measurement results of the accelerometer contains the information of three axes (x, y, z) and the sequence number t. The number of samples stored is the size of the queue.

The number of samples stored can be checked by reading the queue size (`brd.sns_MC3630.get_que().size()`). Normally 28 samples, but it may go a little further due to processing delays, etc. The first sample is taken from `front()'. The first sample can be obtained by `front()`. Its sequential number is `front().t`.

Here we will take the average of the samples before taking them out of the queue. Each element of the queue can be accessed with a for statement (`for (auto&& v: brd.sns_MC3630.get_que()) { ... }`), where `v.x, v.y, v.z` in the for statement is each element. Here, the sum of each element is calculated, and after the for statement, the average is calculated by dividing by the number of elements.



Next, a packet is generated and a request for transmission is made, but because the data volume is large, the packet is divided into two and transmitted twice. Therefore, the sending process is performed twice in the for statement.

```cpp
		for (int ip = 0; ip < 2; ip++) {
```



The number of samples to be included in the packet to be sent and the sample first sequence number are stored in the first part of the packet payload.

```cpp
// prepare packet (first)
uint8_t siz = (brd.sns_MC3630.get_que().size() >= MAX_SAMP_IN_PKT)
? MAX_SAMP_IN_PKT : brd.sns_MC3630.get_que().size();
uint16_t seq = brd.sns_MC3630.get_que().front().t;

pack_bytes(pkt.get_payload()
	, make_pair(FOURCHARS, 4)
	, seq 
	, siz
);
```



Finally, the acceleration data is stored. While earlier we only referenced each element of the queue to calculate the average value, here we read one sample at a time from the queue and store it in the payload of the packet.

```cpp
for (int i = 0; i < siz; i++) {
	auto&& v = brd.sns_MC3630.get_que().front();
	uint32_t v1;

	v1  = ((uint16_t(v.x/2) & 4095) << 20)  // X:12bits
		| ((uint16_t(v.y/2) & 4095) <<  8)  // Y:12bits
		| ((uint16_t(v.z/2) & 4095) >>  4); // Z:8bits from MSB
	uint8_t v2 = (uint16_t(v.z/2) & 255);   // Z:4bits from LSB
	pack_bytes(pkt.get_payload(), v1, v2); // add into pacekt entry.
	brd.sns_MC3630.get_que().pop(); // pop an entry from queue.
}
```

Use `.front()` to read the head of the data queue from the accelerometer. After reading, `.pop()` is used to release the top of the queue.

The data acquired from the accelerometer is in milli-G units, where 1G is 1000. Since the range is set to ±4G, the data is divided by 2 to be stored within a 12-bit range. To save the number of data, the first 4 bytes store the upper 8 bits of the X, Y and Z axes, and the next 1 byte stores the lower 4 bits of the Z axis, generating a total of 5 bytes.



The transmission ID is stored in the `txid[]` array to wait for two transmissions.

```cpp
MWX_APIRET ret = pkt.transmit();

if (ret) {
	Serial << "..txreq(" << int(ret.get_value()) << ')';
	txid[ip] = ret.get_value() & 0xFF;
} else {
	sleepNow();
}
```



Then, if `b_transmit` is `true` during `loop()`, a completion check is performed, and if completed, `sleepNow()` puts the program to sleep.

```cpp
} else {
	if(		the_twelite.tx_status.is_complete(txid[0])
		 && the_twelite.tx_status.is_complete(txid[1]) ) {

		sleepNow();
	}
}
```

The completion of transmission is confirmed by `the_twelite.tx_status.is_complete()` The `.txid[]` is the ID value returned on transmission.

