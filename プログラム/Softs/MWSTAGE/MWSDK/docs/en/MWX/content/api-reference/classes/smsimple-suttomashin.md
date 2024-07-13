---
title: "SM_SIMPLE state machine"
author: "Mono Wireless Inc."
---
# SM\_SIMPLE state machine

SM\_SIMPLE is provided to wait for processing such as state transitions, waiting for timeouts, and completion of transmission in the sample code.

The following is a basic code excerpt from SM\_SIMPLE.

```cpp
#include <SM_SIMPLE>

enum class STATE : uint8_t {
	INIT = 0,
	SENSOR,
	TX,
	TX_WAIT_COMP,
	GO_SLEEP
};

SM_SIMPLE<STATE> step;

begin() {
  ...
  step.init(); //initialize
}

loop() {
  do {
    switch(step.state()) {
    case STATE::INIT:
      ...
      step.next(STATE::SENSOR);
    break;
    
    case STATE::SENSOR:
      ...
      step.next(STATE::TX);
    break;
    
    case STATE::TX:
      if (/*success on tx request*/) {
        step.set_timeout(100); // set timeout as 100ms
        step.clear_flag();
          
        step.next(STATE::TX_WAIT_COMP);
      }
    break;
    
    case STATE::TX_WAIT_COMP:
      if (step.is_timeout()) the_twelite.reset_system(); // is timeout?
      if (step.is_flag_ready()) sleepNow(); // is set the flag?
    break;
    
    ...
    }
  } while(step.b_more_loop());
}

void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus);
}

void sleepNow() {
	step.on_sleep(false); // reset state machine.
  the_twelite.sleep(10000); // 10sec
}
```



### Explanation

To use SM\_SIMPLE, you need to define an `enum class` as a list of states. In the above, it is defined as `STATE`. The class object is generated as `SM_SIMPLE<STATE> step;` with this stage as a parameter. The generated class object should be initialized by `.setup()`.

```cpp
enum class STATE : uint8_t {
	INIT = 0,
	SENSOR,
	TX,
	TX_WAIT_COMP,
	GO_SLEEP
};

SM_SIMPLE<STATE> step;

void setup() {
  step.init();
}
```



The initial state of SM\_SIMPLE has the value 0, corresponding to `STATE::INIT` in the above example. To get the current state, use `.state()` and use it as a judgment expression in the _switch_ clause of the _do while_ statement as in the above example.

```cpp
loop() {
  do {
    switch(step.state()) {
    case STATE::INIT: // State with value 0
    ...
```



Call `.next()` for state transitions. If the state changes, `b_more_loop()` is set to `true` and the loop in the _do while_ clause is executed again. In the example, calling `.next(STATE::TX)` from the `STATE::SENSOR` state will cause the loop to be executed again and the `case STATE::TX:` clause will also be executed. If the state is not changed, the _do while_ loop is escaped and the `loop()` is terminated once. Wait until the next call to `loop()`.

```cpp
  do {
    switch(step.state()) {
    ...
    case STATE::SENSOR:
      ...
      step.next(STATE::TX); // (1)state transition
    break;
    
    case STATE::TX: // (3) Called in the second loop
      if (/*success on tx request*/) {
      ...
    }
  } while (b_more_loop()); // (2) loop continue check
```



If you want to wait for processing such as completion of transmission, call `.clear_flag()`, and then signal the completion of processing by `.set_flag(uint32_t)` in another callback function or the like. Parameters of type `uint32_t` specified here can be read from `.get_flag_value()`.

If you want to process a timeout, you can record the time when you call `.set_timeout(uint32_t)` and check if the timeout time has elapsed with `.is_timeout()`.

```cpp
    case STATE::TX:
      if (/*success on tx request*/) {
        step.set_timeout(100); // set timeout
        step.clear_flag();
          
        step.next(STATE::TX_WAIT_COMP);
      }
    break;
    
    case STATE::TX_WAIT_COMP:
      if (step.is_timeout()) ...; // timeout
      if (step.is_flag_ready()) ...; // is set the flag?
    break;
...

// an event of tx completion
void on_tx_comp(mwx::packet_ev_tx& ev, bool_t &b_handled) {
	step.set_flag(ev.bStatus); // set the flag
}
```



SM\_SIMPLE will be used again when returning from sleep, but it should always be called `.on_sleep(bool)` before sleep. If you put `false` in the parameter, it will start from the `0` state after recovery, and if you put `true`, it will resume from the state just before sleep.

```cpp
void sleepNow() {
	step.on_sleep(false); // reset state machine.
  the_twelite.sleep(10000); // 10sec
}
```



### Source Code

The following is the source code for SM\_SIMPLE.

```cpp
// very simple class to control state used in loop().
template <typename STATE>
class SM_SIMPLE {
	uint32_t _u32_flag_value;  // optional data when flag is set.
	uint32_t _ms_start;		// system time when start waiting.
	uint32_t _ms_timeout;	// timeout duration

	STATE _step;			  // current state
	STATE _step_prev;		// previous state
	bool_t _b_flag; 		// flag control.
public:
	// init
	void setup() { memset(this, 0, sizeof(SM_SIMPLE)); }
	// call befoer sleeping (save state machine status)
	void on_sleep(bool b_save_state = false) {
		STATE save = _step;
		setup();
		if(b_save_state) _step = _step_prev = save;
	}

	// state control
	void next(STATE next) { _step = next; } // set next state
	STATE state() { return _step; } // state number
	bool b_more_loop() { // if state is changed during the loop, set true
		if (_step != _step_prev) { _step_prev = _step; return true; }
		else return false;
	}

	// timeout control
	void set_timeout(uint32_t timeout) {
		_ms_start = millis();
		_ms_timeout = timeout;
	}
	bool is_timeout() { return (millis() - _ms_start) >= _ms_timeout; }

	// flag control
	void clear_flag() { _b_flag = false; _u32_flag_value = 0; }
	void set_flag(uint32_t u32_flag_value = 0) {
		_b_flag = true;
		_u32_flag_value = u32_flag_value; }
	uint32_t get_flag_value() { return _u32_flag_value; }
	bool is_flag_ready() { return _b_flag; }
};
```

* Contents may change depending on the version.
* The main body will be stored in SM\_SIMPLE.hpp in the mwx library resource folder.

