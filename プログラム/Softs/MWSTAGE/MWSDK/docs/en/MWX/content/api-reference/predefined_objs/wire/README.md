---
title: TITLE
author: "Mono Wireless Inc."
description: Read/write two-wire serial (I2C) master (mwx::periph_wire)
---

# Wire

Reads and writes two-wire serial (I2C) master.



#### Alias Definition

```cpp
using TwoWire = mwx::periph_twowire<MWX_TWOWIRE_RCVBUFF>;
```

The `mwx::periph_wire<MWX_TWOWIRE_RCVBUFF>` can be referred as `TwoWire`.



#### Type Definitions

The following definition types describe the types of arguments and return values.

```cpp
typedef uint8_t size_type;
typedef uint8_t value_type;
```

## hint style="warning %}

{% hint style="warning" %}
Some APIs make calls where the STOP bit is not strictly handled.
{% endhint %}

{% hint style="info" %}
`write(), writer::operator() ()` has several arguments defined in addition to the ones described here.

* fixed array type \
  `uint8_t cmds[]={11,12};`\
  `...`\
  `Wire.write(cmds);`
* `initializer_list<>` 型\
  `Wire.write({11,12})`
{% endhint %}

## Initialization and termination

### Wire Instance Creation

Instance creation and necessary initialization is done in the library. In user code, it is made available by calling `Wire.begin()`.

When using the `requestFrom()` method, you can specify the size of the FIFO queue for temporary data storage. At compile time, compile the macro `MWX_TWOWIRE_BUFF` with the required number of bytes. The default is 32 bytes.

> Example: `-DMWX_TWOWIRE_BUFF=16`



### begin()

```cpp
void begin(
    const size_type u8mode = WIRE_100KHZ,
    bool b_portalt = false)
```

Initialize hardware.

{% hint style="danger" %}
Performing any Wire operation without initialization will cause the TWELITE radio module to hang.
{% endhint %}

{% hint style="warning" %}
When waking from sleep, if the module was operating just before sleep, it will return to the previous state.
{% endhint %}



| Parameters | Description                                                                                                                                                                                                                                                                                                              |
| ----------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `u8mode`    | Specifies the bus frequency. Default is 100Khz (`WIRE_CONF::WIRE_100KHZ`)<br>The frequency is specified by `WIRE_CONF::WIRE_? KHZ` and `? KHZ` can be `50`,`66`,`80`,`100`,`133`,`160`,`200`,`266`,`320`,`400`. |
| `b_portalt` | Change hardware pin assignments.                                                                                                                                                                                                                                                                                             |

### Example

```cpp
void setup() {
    ...
    Wire.begin();
    ...
}

void wakeup() {
    ...
    Wire.begin();
    ...
}
```





## Reading and Writing

There are two types of read/write procedures. Select and use one of them.

* [Member function (input/output using the following member functions)](wire-member.md)\
  `requestFrom(), beginTransmission(), endTransmission(), write()`
* [Helper class (stream function available)](wire-helperclass.md)\
  `reader, writer`

####

## Others

### Probe（Determine presence of device)

```cpp
bool probe(uint8_t address)
```

Checks if the device specified by `address` is responding. If the device exists, `true` is returned.



### setClock()

```cpp
void setClock(uint32_t speed)
```

The procedure is originally intended to change the bus frequency, but no action is taken.
