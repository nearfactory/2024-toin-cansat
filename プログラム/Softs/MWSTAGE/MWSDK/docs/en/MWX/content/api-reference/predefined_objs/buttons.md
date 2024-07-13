---
title: Buttons
author: "Mono Wireless Inc."
description: Digital input management class (mwx::periph_buttons)
---

# Buttons

Detects changes in digital inputs. This class detects changes when the same detected value is obtained multiple times. This is useful for reducing the effect of chattering on mechanical buttons.



## Methods

### setup()

```cpp
void setup(uint8_t max_history);
```

The parameter `max_history` is the maximum number of references that can be set with `begin()`. Memory is allocated and initialized here.



### begin()

```cpp
void begin(uint32_t bmPortMask,
				   uint8_t u8HistoryCount,
				   uint16_t tick_delta);
```

Starts the `Buttons` operation, the first parameter `bmPortMask` specifies the bitmap of digital inputs to be monitored. bit 0 corresponds to DIO 0, ... , bit N corresponds to DIO N. More than one can be specified: the second `u8HistoryCount` is the number of times required to confirm the value; the third `tick_delta` specifies the interval in ms between value checks; the fourth `u8HistoryCount` is the number of times to confirm the value.

It will take `u8HistoryCount*tick_delta`\[ms] to confirm the value. For example, if `u8HistoryCount`=5 and `tick_delta`=4, it will take at least 20ms to confirm the state.

The confirmation is done in the `TickTimer` event handler. Since it is not an interrupt handler, it is affected by delays in processing, etc., but it is sufficient to suppress chattering of mechanical buttons, etc.



### end()

```cpp
void end()
```

Terminates the `Buttons` operation.



### available()

```cpp
inline bool available()
```

Returns `true` when a change is detected. It is cleared when `read()` is executed.



### read()

```cpp
bool read(uint32_t& u32port, uint32_t& u32changed)
```

Called when `u32port` becomes available. u32port` is the bitmap of the current input DIO and `u32changed` is the bitmap of the DIO where the change was detected.

Returns `false` if Buttons is not working.



## About operation

### Initial value determination

When Buttons starts to operate, the input status of DIO is not yet determined. When the value is determined, it becomes available. At this time, the MSB (bit31) of the bitmap read by `read()` is set to 1.

Since this function requires the operation to be determined, it cannot be used to monitor ports whose input values change constantly.



### Sleep

If Buttons is in operation before Sleep, it will resume after it is restored. After resumption, initial determination is performed.





