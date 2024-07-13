---
title: revision history
author: "Mono Wireless Inc."
---

# revision history

{% hint style="info" %}
Minor corrections will not be listed in this revision history, but only in the revision on GitHub. Please cite corrections as necessary.
{% endhint %}


## Updating

After the release of the TWELITE STAGE distribution package, fixes and additions are stored in the GitHub repository. Please replace the location of the distribution package if necessary.

Other updates to the MWSDK may be required. Please refer to the release description at the time of the update; see [here](https://sdk.twelite.info/latest) for information on updating the MWSDK.

### Updating MWX library code

The source code of the library is available on GitHub ([https://github.com/monowireless/mwx](https://github.com/monowireless/mwx)). To replace the source code of the library, please follow the steps below.

1. Click on the link for each release to clone the Git file or download the source code in zip format.
2. Replace the contents of the following folders.

```
.../MWSTAGE/              --- TWELITE STAGE distribution dir
        .../MWSDK         --- MWSDK dir
              .../TWENET/current/src/mwx <-- Replace this dir.
```

## Update before major release&#x20;

> [https://github.com/monowireless/mwx/wiki](https://github.com/monowireless/mwx/wiki)

Updated information prior to major releases may be posted on the above link.



## 0.2.0 - 2022-03-01

| Library Name | Dependent version                                              |
| ------------ | ------------------------------------------------------------ |
| mwx          | [0.2.0](https://github.com/monowireless/mwx/releases/tag/0.2.0) |
| twesettings  | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C     | 1.3.5                                                        |

* changed Wire object that reserves memory in heap area.
* changed function name from `G_OCTET()` to `G_BYTE()`](api-reference/funcs/utility/byte-array-utils.md to avoid name conflict in utils.h.
* changed an order of `vAHI_DioInterruptEnable()` in the `attachIntDio()` for code efficiency.
* added secondary network behavior the_twelite.network2 to support universal receiver (receive packets from NWK_LAYERED, NWK_SIMPLE or networkless packets in the same executable code.)
* added [NWK_LAYERED](./networks/nwk_layered.md) (At this time, only Parent Node reception is supported.)
* introduced MWX_Set_Usder_App_Ver() function to set application version during MWX intitialization, mainly for interactive mode.
* added [mwx::pnew()](./api-reference/funcs/utility/pnew.md) to describe placement new simply.
* added support of [EASTL](api-reference/external_libraries/EASTL.md)
  * added `new[]` operators for EASTL
* pre-compled  most of source codes in MWX to quicker compile.
* fixed: DIO events were being passed on to unrelated ports.
  

## 0.1.9 - 2021-12-15



| Library Name | Dependent version                                               |
| ------------ | ------------------------------------------------------------ |
| mwx          | [0.1.9](https://github.com/monowireless/mwx/releases/tag/0.1.9) |
| twesettings  | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C     | 1.3.5                                                        |



#### 主な改定内容

* Added board support for TWELITE ARIA [BRD\_ARIA](act\_samples/brd\_aria.md) and sensor definition [SHT4x](sensor\_object/sht4x.md)
* Added an internal procedure to allow output using Serial class objects in Interactive settings mode. (`Serial._force_Serial_out_during_intaractive_mode()`)



## 0.1.8 - 2021-09-09

| Library Name | Dependent version                                                       |
| ------------ | ----------------------------------------------------------------------- |
| mwx          | [0.1.8](https://github.com/monowireless/mwx/releases/tag/0.1.8)         |
| twesettings  | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C     | 1.3.5                                                                   |



Main revisions

* `Serial1`port and alternate port were not properly defined.
* Enabled to change the baud rate of (`Serial` UART0).
* Added event callbacks to notify received packets ([`on_rx_packet()`](api-reference/sys_callbacks/on_rx_packet.md)) and completed transmission ([`on_tx_comp()`](api-reference/sys_callbacks/on_tx_comp.md)).
  * If you don't define a callback function, you can use the previous procedure.
* Wrong definition ID for interactive mode setting`<STG_STD>` and some default values.
* Added support for changing the default values of channel and logical device IDs in addition to AppID in interactive mode settings`<STG_STD>`.
* added support for setting the\_twelite and `<NWK_SIMPLE>` objects in interactive mode `<STG_STD>` object for some settings.
* added support for setting the default value of the number of retransmissions in `<NWK_SIMPLE>`.
* `Serial`(UART0) input and output from the application is not performed while the interactive mode screen`<STG_STD>` is displayed.
* added `CUE::PIN_SET`, `PAL???"":PIN_SET` (Since it is unnatural to use `PIN_BTN` for CUEs without buttons)
* Move namespace of random() to mwx::.
* MONOSTICK watchdog setting is now done in 32ms units.
* When sleep was performed using `BRD_TWELITE`, the pins were not initialized correctly upon recovery.



{% hint style="warning" %}
Sorry. The following has not been translated.
{% endhint %}

## 0.1.7 - 2020-12-03

| Library Name | Dependent Version                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.7](https://github.com/monowireless/mwx/releases/tag/0.1.7)         |
| twesettings | [0.2.6](https://github.com/monowireless/twesettings/releases/tag/0.2.6) |
| TWENET C    | 1.3.4                                                                   |



#### Major Revisions.

* Added board behavior ([https://mwx.twelite.info/v/v0.1.7/boards/cue](https://mwx.twelite.info/v/v0.1.7/boards/cue)) for TWELITE CUE.
* Added method to receive other packets (without network usage) that are not in NWK\_SIMPLE format when using NWK\_SIMPLE. Add `NWK_SIMPLE::receive_nwkless_pkt()` to initialize NWK\_SIMPLE. When using this packet information, use only the TWENET C library layer structure by `.get_psRxDataApp()` and the data array obtained by `.get_payload()`. Information obtained from other methods of the incoming packet (`auto&& rx = the_twelite.receiver.read()`) is undefined.
* Refine `get_stream_helper()` code and read/write position API.
* Add EEPROM class object. ([https://mwx.twelite.info/v/v0.1.7/api-reference/predefined\_objs/eeprom](https://mwx.twelite.info/v/v0.1.7/api-reference/predefined\_objs/eeprom))
  * Samples ([https://github.com/monowireless/Act\_samples/tree/master/Unit\_EEPROM](https://github.com/monowireless/Act\_samples/tree/master/Unit\_EEPROM))
* Fixed bugs in `smplbuf::get_stream_helper()`.
* Added pktparser class ([https://mwx.twelite.info/v/v0.1.7/api-reference/classes/pktparser](https://mwx.twelite.info/v/v0.1.7/api-reference/classes/pktparser))
  * Added sample ([https://github.com/monowireless/Act\_samples/tree/master/Unit\_PktParser](https://github.com/monowireless/Act\_samples/tree/master/Unit\_PktParser))
* sample `serparser/pktparser` so that it can be built on other platforms ([https://github.com/monowireless/mwx/tree/master/stdio](https://github.com/monowireless/mwx/tree/master/stdio))



## 0.1.6 - 2020-10-09

| Library Name | Dependent Version                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.6 ](https://github.com/monowireless/mwx/releases/tag/0.1.6)        |
| twesettings | [0.2.5](https://github.com/monowireless/twesettings/releases/tag/0.2.5) |
| TWENET C    | 1.3.4                                                                   |



#### Major revisions

* Modified so that `div100()`, which calculates the quotient and remainder, can be output to Serial, etc.
* Changed implementation of `smplbuf<>` array class. The inheritance from `mwx::stream` is removed to reduce memory consumption, and a helper class and an inheritance class are defined separately.
* Added `mwx_printf()` ` `mwx_snprintf()` function.
* added `the_twelite.stop_watchdog()` and `the_twelite.restart_watchdog()` functions.
* `mwx::stream` maintenance: `operator bool()` is obsolete. Disable timeout when `.set_timeout(0xff)` is set to 0xff in the read timeout setting. Add definition of `<<` operator.
* Added NOTICE PAL / PCA9632 support (Description [https://mwx.twelite.info/v/latest/boards/pal/pal\_notice](https://mwx.twelite.info/v/latest/boards/pal/pal\_notice), sample [https://github.com/monowireless/Act\_samples/tree/master/Unit\_using\_PAL\_NOTICE](https://github.com/monowireless/Act\_samples/tree/master/Unit\_using\_PAL\_NOTICE))
* Add scale functions between 8bit and 0..1000 with no division.
* Added division by 10,100,1000 (quotient and remainder at the same time) `div10()`, `div100()`, `div1000()`. Restricted the value range and composed mainly of multiplication and bit shift.
* Added corresponding methods for encrypted packets.
  * `packet_rx::is_secure_pkt()` : determine if received packet is encrypted or not.
  * `STG_STD::u8encmode()` : Obtain encryption setting in interactive mode.
  * `STG_STD::pu8enckeystr()` : Obtain encryption key byte sequence in interactive mode.
* Serial1: Default port is DIO11(TxD), DIO9(RxD) because they are usually assigned to I2C, though DIO14,15 overlap with I2C in the semiconductor specification.
* The calculation of the baud rate is omitted for the main baud rates.
* Serial: The proxy functions for `Serial: available()` and `read()` are now held only by `void*`, and the specification memory is reduced by 8 bytes.
* Add `typedef boolean`.
* Network: added support for encryption.
  * The first parameter is the encryption key, and the second parameter is `true`, so that plaintext packets are also received. packets are also received.

      ```
      auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
      nwk << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
          << NWK_SIMPLE::secure_pkt((const uint8_t*)"0123456789ABCDEF");
          ;
      ```
* Added sensor support for SHT3x and BME280
* Sensor: added a mechanism to exchange configuration parameters and status in legacy code (C library wrapper class).
* Sensors: I2C address can be specified for SHT3x and BME280.
* Configuration: added `hide_items()`. Unnecessary items can be deleted.
* Added `H/W UTIL menu' to display DI status, I2C probe, and PAL EEPROM contents.
* Configuration: Add encryption related menus.
* I2C related fixes (to improve compatibility with code implemented using TwoWire class)
  * Processing of `requestFrom(false)` did not work correctly because there was no code to send the NO\_STOP message when processing `requestFrom(false)`.
  * Added `TwoWire` class name alias.
  * Modified not to initialize multiply in `begin()` process.
  * Added `setClock()` method (but it is a dummy function and does nothing).
  * Added `WIRE_CONF::WIRE_? KHZ` added. Added the main configuration values for the bus clock.



## 0.1.5 - 2020-08-05

| Library Name | Dependent Version                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.5](https://github.com/monowireless/mwx/releases/tag/0.1.5)         |
| twesettings | [0.2.5](https://github.com/monowireless/twesettings/releases/tag/0.2.5) |
| TWENET C    | 1.3.4                                                                   |



#### Download in bulk

> [MWSDK2020\_08\_UNOFFICIAL](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/releases/tag/MWSDK2020\_08\_UNOFFICIAL) ([README.md](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/blob/master/README.md))



#### Major revisions

* Added [settings behavior (interactive mode function)](settings/)
* Channel Manager. Implement [`chmgr`](api-reference/predefined\_objs/the\_twelite.md#twenet-chmgr-uint-8-\_t-ch1-18-uint-8-\_t-ch2-0-uint-8-\_t-ch3-0)



## 0.1.4 - 2020-07-29 (MWSDK2020\_07\_UNOFFICIAL)

| Library Name | Dependent Version                                                                 |
| ----------- | ----------------------------------------------------------------------- |
| mwx         | [0.1.4](https://github.com/monowireless/mwx/releases/tag/0.1.4)         |
| twesettings | [0.2.4](https://github.com/monowireless/twesettings/releases/tag/0.2.4) |
| TWENET C    | 1.3.3                                                                   |

####

#### Download in bulk

> [MWSDK2020\_07\_UNOFFICIAL](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/releases/tag/MWSDK2020\_07\_UNOFFICIAL) ([README.md](https://github.com/monowireless/MWSDK\_COMMON\_SNAP/blob/master/README.md))



#### Major revisions.

* Addition of `delayMilliseconds()` function
* Addition of `digitalReadBitmap()` function
* Improved accuracy of `delay()`.
* Fixed the problem that `Serial1` instance was not defined
* Fixed problem with `Analogue` interrupt handler not being called



## 0.1.3 - 2020-05-29

Support for MWSDK202020\_05

* Duplicate checker duplicate_checker was not removed as expected due to incomplete initialization, etc.
* The implementation of format() was made less machine-dependent. If 64-bit arguments are included, the number of arguments is limited.

> [https://github.com/monowireless/mwx/releases/tag/0.1.3](https://github.com/monowireless/mwx/releases/tag/0.1.3)

{% hint style="warning" %}
The fix assumes MWSDK2020\_05.

Updates are recommended for this fix.
{% endhint %}

## 0.1.2 - 2020-04-24

Support for MWSDK2020\_04

* Fixed initialization problem with Timer0..4
* Changed internal processing of mwx::format()
* Added experimental code to support interactive mode

> [https://github.com/monowireless/mwx/releases/tag/0.1.2](https://github.com/monowireless/mwx/releases/tag/0.1.2)

{% hint style="warning" %}
This fix assumes MWSDK2020\_04.

We recommend an update to this fix.
{% endhint %}

## 0.1.1 - 2020-02-28

Fixed problems with handling of relay flags in packets

> [https://github.com/monowireless/mwx/releases/tag/0.1.1](https://github.com/monowireless/mwx/releases/tag/0.1.1)

{% hint style="warning" %}
We recommend an update to this correction.
{% endhint %}

## 0.1.0 - 2019-12-23

First release (included in SDL Dec 2019)

> [https://github.com/monowireless/mwx/releases/tag/0.1.0](https://github.com/monowireless/mwx/releases/tag/0.1.0)

