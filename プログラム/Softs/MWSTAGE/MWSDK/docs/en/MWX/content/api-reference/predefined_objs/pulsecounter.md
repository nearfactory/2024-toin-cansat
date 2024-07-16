---
title: PulseCounter
author: "Mono Wireless Inc."
description: PulseCounter (mwx::periph_pulse_counter)
---

# PulseCounter

The pulse counter is a circuit that reads and counts pulses even when the microcontroller CPU is not running. There are two systems of pulse counters: PC0 is assigned to `PulseCounter0` and PC1 to `PulseCounter1`.

PC0 is assigned to `PulseCounter0` and PC1 is assigned to `PulseCounter1`.



## method

### begin()

```cpp
void begin(uint16_t refct = 0, 
           E_PIN_INT_MODE edge = PIN_INT_MODE::FALLING,
           uint8_t debounce = 0)
```

Initializes the object and starts counting. the first parameter `refct` is the count number on which interrupts and available decisions are based. When this number is exceeded, it is reported to the application. You can also set `refct` to 0. In this case, it is not a sleep wake-up factor.

The second parameter `edge` specifies whether the interrupt is rising (`PIN_INT_MODE::RISING`) or falling (`PIN_INT_MODE::FALLING`).

The third `debounce` takes the values 0, 1, 2, or 3. 1, 2, or 3 settings require the same consecutive value to detect a change in value to reduce the effect of noise.

| setting | number of consecutive samples | maximum detection frequency |
| :-: | :-----: | :-----: |
| `0` |    -    |  100Khz |
| `1` |    2    |  3.7Khz |
| `2` |    4    |  2.2Khz |
| `3` |    8    |  1.2Khz |



### end()

```cpp
void end()
```

Discontinue detection.



### available()

```cpp
inline bool available()
```

If the specified count (`refct` in `begin()`) is 0, `true` is returned when the count exceeds 1.

If the specified count (`refct` in `begin()`) is 1 or more, `true` is returned when the number of detections exceeds the specified count.



### read()

```cpp
uint16_t read()
```

Reads the count value. Resets the count value to 0 after reading.



