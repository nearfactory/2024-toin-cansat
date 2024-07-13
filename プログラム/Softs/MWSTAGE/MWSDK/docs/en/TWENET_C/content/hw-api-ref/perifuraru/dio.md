# DIO

The DIO (General Purpose IO) can be input or output as desired under the control of the firmware.

{% hint style="info" %}
Also commonly referred to as GPIO, but referred to as DIO throughout this manual.
{% endhint %}



The following can be done with general purpose IO:

* Digital input (H or L level readout)
  * Pull-up (50kΩ, typ) can be set.
* Digital output (H or L level output)
  * The maximum value of the supply or suction current (3 mA at 3 V typ).
* Interrupt (rising or falling)
  * The rise and fall cannot be detected simultaneously.
  * It is also available in sleep mode.

## About port numbers

There are 20 DIOs, numbered 0 to 19. These are called in order DIO0 ... DIO19. The following APIs allow you to specify the DIO number directly.

```
vPortAsInput(10); // set DIO10 as input
bool_t bPort = bPortRead(10);
   // TRUE: DIO10 is LO(GND) level
   // FALSE: DIO10 is HIGH(VCC) level
```

## About negative logic

For TWELITE wireless microcontrollers, the port state at power-on reset is at the High (Vcc) level, so in principle TWELITE NET sets the High (Vcc) level to 0 and the Lo (GND) level to 1.

## State at start-up

On power-up, all ports are initialised in the following state

* Input
* Pull-up enabled

## Input

To change a port to an input, call `vPortAsInput()`.

```
#define PORT_DI1 11 // DIO11
vPortAsInput(PORT_DI1);
```

To read the status of a port, call `bPortRead()`.

```
bool_t bPortStat;
bPortStat = bPortRead(PORT_DI1);
```

### Output

To change the port to an output, call `vPortAsOutput(c)`. Also, to set the value of a port, call `vPortSetLo()`, `vPortSetHi()`, or `vPortSet_TrueAsLo()`.

```
#define PORT_DO1 18 // DIO18
vPortSet_TrueAsLo(PORT_DO1, TRUE); // TRUE: set Lo
vPortAsOutput(PORT_DO1);
```

{% hint style="info" %}
t is recommended to set the output setpoint of the port before setting the output. This is because when observing the voltage of the port from the outside, it may behave, for example, to drop to GND for a moment and then return to the Vcc level.
{% endhint %}

### Pull-up

You can stop the pull-up by calling `vPortDisablePullup()`.

```
#define PORT_DI1 11 // DIO11

vPortDisablePullup(PORT_DI1);
vPortAsInput(PORT_DI1);
```

{% hint style="info" %}
Stopping pull-ups saves power in the following cases

* External is Lo level at input port
* Setting the Lo level on the output port
{% endhint %}

{% hint style="warning" %}
However, on ports where pull-ups have been stopped, care should be taken to use

* Nothing connected to the input port (or equivalent)
  * Extra current consumption may occur
* Stop pull-ups on ports that do not require configuration
  * Extra current consumption may occur
* When the input port is used as an interrupt wakeup pin
  * An external pull-up is mandatory when connecting buttons etc.
{% endhint %}

