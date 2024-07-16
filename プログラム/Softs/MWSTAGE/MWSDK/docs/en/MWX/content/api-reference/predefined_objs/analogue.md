---
description: ADC (mwx::periph_analogue.hpp)
---

# Analogue

Analogue performs ADC and acquires values. It can continuously acquire multiple channels at a time, and can do so sequentially according to a timer or other cycle.

## Constant.

### Pin Definition.

| constant                                                                                         | classification      | Pin name in standard apps |
| ------------------------------------------------------------------------------------------------ | ------------------- | :-----------------------: |
| `uint8_t PIN_ANALOGUE::A1 = 0`                                                                   | ADC1 pin            |            AI1            |
| `uint8_t PIN_ANALOGUE::A2 = 1`                                                                   | ADC2 pin            |            AI3            |
| <p><code>uint8_t PIN_ANALOGUE::A3 = 2</code></p><p><code>uint8_t PIN_ANALOGUE::D0 = 2</code></p> | ADC3 pin (DIO0) \*1 |            AI2            |
| <p><code>uint8_t PIN_ANALOGUE::A4 = 3</code></p><p><code>uint8_t PIN_ANALOGUE::D1 = 3</code></p> | ADC4 pin (DIO1) \*1 |            AI4            |
| `uint8_t PIN_ANALOGUE::VCC = 4`                                                                  | Vcc Supply voltage  |                           |

{% hint style="warning" %}
In the standard application (App\_Twelite), the pin names ADC2/ADC3 in the semiconductor data sheet are AI3/AI2 to match the TWELITE DIP. Please note this.
{% endhint %}

{% hint style="danger" %}
\*1 ADC2/ADC3 pins for both digital and analog are subject to usage procedures and restrictions.

Assume no pull-up on the pin to be used before the ADC starts. If this is not done, the pull-up voltage will always be observed in the ADC.

```
pinMode(PIN_DIGITAL::DIO0, PIN_MODE::INPUT); 
pinMode(PIN_DIGITAL::DIO1, PIN_MODE::INPUT);
```

****

**In a normal circuit configuration, current leakage occurs during sleep.** It cannot be circumvented by software description alone.

To avoid current leakage during sleep, the GND of the analog circuit section should be disconnected with a FET switch or the like and left floating during sleep. Also, before sleep, set the pin to input and pull-up state.
{% endhint %}



## method.

### setup()

```cpp
void setup(
        bool bWaitInit = false,
        uint8_t kick_ev = E_AHI_DEVICE_TICK_TIMER,
        void (*fp_on_finish)() = nullptr) 
```

Initializes ADCs. setup() starts the regulator inside the semiconductor, specifies the timer device for periodic execution, and specifies the callback function to be called when all ADCs on the specified channel have completed.

| parameter      | explanation                                                                                                                                                                                                                                                                                                        |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `bWaitInit`    | If true is specified, waits for initialization of the regulator inside the semiconductor.                                                                                                                                                                                                                          |
| `kick_ev`      | <p>Specify the timer device to be specified for cyclic execution. The following five types of devices can be specified, and AD is started in the interrupt handler except for the first time.</p><p><code>E_AHI_DEVICE_TICK_TIMER (TickTimer)</code></p><p><code>E_AHI_DEVICE_TIMER0 .. 4 (Timer0 .. 4)</code></p> |
| `fp_on_finish` | This callback function is called from within the interrupt handler after all ADCs on the specified ports have finished, and is used when ADC measurement values are to be stored separately in a FIFO queue, etc.                                                                                                  |



### begin()

```cpp
void begin(uint8_t bmPorts, uint8_t capt_tick = 1)
```

The first parameter specifies the port where the ADC is to be made. The port specification is a bitmap with the bits set corresponding to the port numbers mentioned in the pin definition.For example, if you want to get the values of two pins `PIN_ANALOGUE::A2` and `PIN_ANALOGUE::VCC`, specify `(1 <<PIN_ANALOGUE::A1 | 1<<PIN_ANALOGUE::VCC )`. You can also use [`pack_bits`](../funcs/utility/pack\_bits.md) and write `pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::VCC)`.

After calling `begin()`, the first ADC processing starts immediately, and the processing of the next pin starts from its end interrupt. When all processing is finished (if specified), the callback function is called. It waits until the next timer interrupt occurs before starting a new ADC process.

The second parameter specifies the number of timer interrupts before AC starts. For example, `TickTimer` is called every 1 ms, but if you specify 16 for the parameter, it will be processed every 16 ms.

```cpp
void begin()
```

Starts ADC processing with default ADC pins (`PIN_ANALOGUE::A1`, `PIN_ANALOGUE::A2`). `end()` resumes the interrupted ADC processing.



### end()

```cpp
void end()
```

ADC processing is terminated and the regulator inside the semiconductor is stopped.



### available()

```cpp
inline bool available()
```

The value of the ADC is set to `true` after the acquisition. After confirmation by this function, it is `false` until the next ADC completion.



### read(), read\_raw()

```cpp
inline int16_t read(uint8_t s)
inline int16_t read_raw(uint8_t s)
```

Reads ADC values. The parameter specifies the ADC pin to be read. `read()` returns the read value converted to mV and read\_raw() returns the ADC value (0..1023).

{% hint style="info" %}
It is recommended to read Vcc with `read()`. This is because a special conversion formula must be applied to convert from `read_raw()` values to mV.
{% endhint %}

{% hint style="warning" %}
After ADC completion (available), if the value is read after a delay until near the timing when the next ADC process is executed, the next ADC value may be returned, because the ADC process is executed by an interrupt handler and the value is updated even during the `loop()` process.
{% endhint %}



## ADC interrupt handler.

The ADC interrupt handler is set to `periph_analogue::ADC_handler()` when `setup()` is called.

If a different handler is specified by the semiconductor peripheral library, it will not work properly.



## Sleep behavior

If the ADC is in cyclic execution state by `begin()`, ADC processing is resumed after returning from sleep.
