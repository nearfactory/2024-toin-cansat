---
title: SPI
author: "Mono Wireless Inc."
description: Reads and writes the SPI bus (as Controller).
---
# SPI

Reads and writes the SPI bus (as Controller).



## Constants

| Constant | Meaning |
| --------------------------------------------------------------------- | ---------------- |
| `const uint8_t`<br>`SPI_CONF::MSBFIRST` | MSB as the first bit |
| `const uint8_t`<br>`SPI_CONF::LSBFIRST` | make LSB the first bit |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE0` | set SPI MODE 0 |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE1` | set to SPI MODE 1 |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE2` | set to SPI MODE 2 |
| `const uint8_t`<br>`SPI_CONF::SPI_MODE3` | set to SPI MODE 3 |

## Initialization and termination

The procedure for using the SPI bus depends on the `begin()` method.

Reads and writes the SPI bus (MASTER).
### begin()

```cpp
void begin(uint8_t slave_select, SPISettings settings)
SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
```

Initialize hardware.

{% hint style="warning" %}
This process is also required after sleep recovery.
{% endhint %}

| Parameters | Description                                                                                                                                                                                                                                                                                                       |
| -------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `slave_select` | Specify the SPI slave select pin to be used. `0 : DIO19`, `1 : DIO0` (DIO 19 is reserved), `2 : DIO1` (DIO 0,19 is reserved)                                                                                                                                                 |
| `settings`     | Specifies the SPI bus setting. <br><br>The `clock`[hz] specifies the SPI bus frequency. A divisor closer to the specified frequency will be selected: 16Mhz or 16Mhz divided by an even number. <br>`bitOrder` specifies `SPI_CONF::MSBFIRST` or `SPI_CONF::LSBFIRST`. <br>`dataMode` specifies `SPI_CONF::SPIMODE0..3`. |

###

#### Example

```cpp
void setup() {
  ...
  SPI.begin(0, SPISettings(2000000, SPI_CONF::MSBFIRST, SPI_CONF::SPI_MODE3));
  ...
}

void wakeip() {
  ...
  SPI.begin(0, SPISettings(2000000, SPI_CONF::MSBFIRST, SPI_CONF::SPI_MODE3));
  ...
}
```



### end()

```cpp
void end()
```

Terminate the use of SPI hardware.



## Reading and Writing

There are two types of read/write procedures. Select and use one of them.

* [Member function version (input/output using the following member functions)](spi-member.md)
  `beginTransaction(), endTransaction(), transfer(), transfer16(), transfer32()`
* [Helper class version (stream function available)](spi-helperclass.md)
  `transceiver`

