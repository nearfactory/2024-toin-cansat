---
title: "The MWX Library"
author: "Mono Wireless Inc."
description: Mono Wireless C++ Library for TWELITE.
---

# The MWX Library

{% hint style="success" %}
Please refer to the [Handling of Materials](https://twelite.gitbook.io/general/about\_documents) section. If you have any questions or concerns, please contact our support desk.
{% endhint %}

{% hint style="danger" %}
This page contains information that is still under development. The contents of this page may not yet be reflected in the public source code.
{% endhint %}



The MWX library is intended to simplify the code notation of the TWELITE radio module. a program created with MWX is called an act. There are two types of acts: descriptions by loops and descriptions by events (called **BEHAVIOR**).

This page introduces the main features of ACT.

* Description by loop (`setup()`, `loop()`). Suitable for writing small-scale functions.

```cpp
#include <TWELITE>
const uint8_t PIN_LED = 5;

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if (TickTimer.available()) {
    uint32 t_now = millis();
    
    // blink LED every 1024ms
    digitalWrite(PIN_LED, (t_now >> 10) & 1 ? HIGH : LOW);
  }
}	
```



* Event-driven application description. It is possible to define various event and interrupt handlers, as well as a state machine within a class that is suitable for describing complex behavior of the application, and write code with good visibility. This description is called **BEHAVIOR**.

```cpp
// myApp.hpp
...
class myApp : MWX_APPDEFS_CRTP(myApp) {
...
  void loop() {
    // main loop
  }
  
  void receive(mwx::packet_rx& rx) {
    // on receive
  }
};

// myApp.cpp
...
MWX_DIO_EVENT(12, uint32_t arg) {
		// on event from DIO12
}
```



* Simplify peripheral procedures. Defines class objects to handle commonly used UART, I2C, SPI, ADC, DIO, timer, and pulse counter.

```cpp
void loop() {
  while(Serial.available() { 
    auto x = Serial.read(); ... } // serial message
  if (Analogue.available() {
    auto x = Analogue.read(...); } // adc values
  if (Buttons.available() { 
    Buttons.read(...); } // DIO changes
  if (the_twelite.receiver.available()) { 
    auto&& rx = the_twelite.receiver.read(); } // on rx packet
}
```



* A simple relay network is defined. This relay network is implemented in the same way as the TWELITE standard application, and is characterized by the fact that individual addresses are managed by 8-bit logical IDs, and that wireless packets can be sent to the network immediately after power-on because there is no communication to build the network.

```cpp
#include <TWELITE>
#include <NWK_SIMPLE>

void setup() {
  ...
  auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) 
	           // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);
	           // can repeat a packet up to three times.
}

void loop() {
  ...
  vTransmit();
  ...
}

void vTransmit() {
  if (auto&& pkt =
    the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet(); 
  pkt << tx_addr(0x00)  // to parent 
	  	<< tx_retry(0x3); // set retry
	
	pack_bytes(pkt.get_payload() // prepare payload data
	    , uint8_t(0x01)
	    , uint16_t(analogRead(PIN_ANALOGUE::A1))
	    , uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)));
	
	pkt.transmit(); // transmit!
}
```

{% hint style="info" %}
Although it does not allow for packet intercommunication with TWELITE standard applications, it does allow for more flexibility in the following areas

* The logical ID is the same in that 0 is the parent device, but since 0x01..0xEF can be used as the child device address, the number of identifications can be 200 or more.
* As a rule, the maximum number of times a packet can be relayed has been set to 64 times. (\* If a packet returns after a certain period of time due to a detour, the duplicate packet management table will be cleared and the packet may have to be relayed again even if it has already been relayed. (Be careful when setting a large number of relays.
{% endhint %}



* Board definition for PAL and MONOSTICK. Easy handling of sensors etc. on the board.

```cpp
#include <TWELITE>
#include <PAL_AMB> // include the board support of PAL_AMB

void setup() {
	auto&& brd = the_twelite.board.use<PAL_AMB>(); // use PAL AMB
	uint8_t u8dip = brd.get_DIP_SW();   // check DIP s/w status
	brd.set_led(LED_TIMER::BLINK, 100); // LED switchs on/off every 100ms
	...
	
	// start capture of sensors
	brd.sns_SHTC3.begin();
}

void loop() {
	if (TickTime.available()) { // check every ms
		auto&& brd = the_twelite.board.use<PAL_AMB>();

		if (brd.sns_LTR308ALS.available()) {
		  Serial << brd.sns_SHTC3..get_temp();
		} else {
		  // notify sensor that 1ms passed.
			brd.sns_SHTC3.process_ev(E_EVENT_TICK_TIMER);
		}
	}
}
```

