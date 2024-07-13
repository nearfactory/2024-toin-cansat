# TWELITE SDK revision history

The history information for MWSDK2020\_05 and later is listed below.

{% embed url="https://github.com/monowireless/MWSDK_COMMON/blob/master/ReleaseNotes%2Emd" %}



## MWSDK2020\_05

{% hint style="danger" %}
Updating is recommended if you are using the MWSDK for 2019/12, 2020/04 releases。
{% endhint %}

### 更新内容

* \[TWENET] Event queue could get stuck in some cases.
* \[TWENET] There was a case that the serial port output did not work after sleep recovery.
* \[TWENET] Clearing the queue for sending and receiving wireless packets after recovering from sleep is now the default behavior.
* \[TWENET] Added support for changing the parameters of the duplicate checker DUPCHK module.
* \[MWX] Duplicate checker duplicate_checker was not initialized properly and did not remove duplicates as expected.
* \[MWX] The implementation of format() has been made less model-dependent. The number of arguments is limited to 8 when 64-bit arguments are included.
* MWSDK and below (excluding the Tools folder) are stored in the GitHub repository.

### 000manifest

```
NAME: MWSDK2020_05
DATE: 2020-05-29

COMMON FILES: https://github.com/monowireless/MWSDK_COMMON/releases/tag/MWSDK2020_05
  CHIP LIBRARY VERSION: SW4063V1416
  TWENET (TWELITE NET) C LIBRARY VERSION: L1303
  MWX C++ LIBRARY VERSION: https://github.com/monowireless/mwx/releases/tag/0.1.3
  TWESETTINGS LIBRARY: https://github.com/monowireless/twesettings/releases/tag/0.2.3
```



## MWSDK2020\_04

The environment has been changed to support TWELITE STAGE.

* Changed to Makefile, which has less description.\
  ※ You will need to rewrite the Makefile from the previous project.
* Added support for the mwx library.



### TWELITE NET 1.2.0->1.3.0 changes

* Addressed a problem where `Utils/Source/serial.[ch], uart.[ch]UART` would sometimes behave abnormally (garbled characters, etc.) when flushed.
* Implemented to suppress the disappearance of TickTimer and ADC completion events. The following variables can be disabled by setting them to 0 during cbAppColdStart(FALSE). `volatile uint32 _u32_INT_ticktimer; volatile uint32 _u32_INT_adc_comp;`
* It judges whether the queue for packet transmission requests is full or not, and returns an error if it is full.
* Fixed an issue where random number generation did not resume during WarmStart.
* Add pvExtraData to tsEvent structure (fix to make PRSEV state transition machine work with mwx library)
* `volatile uint32 u32TickCount_ms`(added volatile)
* Added `bool_t _b_cpu_doze_dirty` variable to control CPU_Doze in the main loop; if set to TRUE, it will skip the main loop DOZE.
* Modified some function header definitions to be able to be referenced externally in order to use the mwx library (user not disclosed: PRSEV: event-related, internal structures, etc.)
* Header file maintenance (extern "C") for mwx library usage
* `Utils/Source/utils.[ch]`: Fixed the PWM setting so that the DUTY tick can be set in detail. By setting a value to structure member u16duty_max, the tick value can be set to a value other than 1024. If the tick value is1024,4096,16384の場合に除算実行の替わりにビットシフトにするようにした。
* Fixed some related code to use MSB of `sToCoNet_AppContext.u32ChMask` internally.
* Fixed a code in the internal channel change function that could change to the current set channel in the source code.



## TWELITE NET 1.2.0 (2018-05-14)

* Changed toolchain from SDK toolchain for TWELITE BLUE to toolchain for TWELITE RED
  * For Windows, it is cygwin to msys.
* Changed the recommended development environment from eclipse to VSCode.
* Changed the library set to 1.2.0 (since v1.0.6)
  * Support for TWELITE RED
  * Organize the parameters of make(`make TWLIET={BLUE|RED}`)
  * Changed the method of determining the upper node in relay from active search to passive method using periodic packets (NB beacons) (implemented in App_Tag repeater)
  * Add `sToCoNet_AppContext.u8Osc32Kmode` to use 32K oscillator
  * Transmission function to the network outside the application ID (experimental implementation)
  * Other minor corrections
    * Improved timestamp update method for duplicate checker (ToCoNet_USE_MOD_DUPCHK)
    * Fixed a rare problem where the channel would not switch properly.

