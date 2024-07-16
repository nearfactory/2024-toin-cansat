---
title: "&lt;STG_STD&gt;"
author: "Mono Wireless Inc."
description: "BEHAVIOR for Interactve settings mode"
---
# \<STG\_STD>

\<STG\_STD> is a configuration BEHAVIOR with minimal configuration items.

```
　　　　　　　Example of setting screen
[CONFIG/MY_APP:0/SID=8102ECE3]
a: (0x1234ABCD) Application ID [HEX:32bit]
i: (         1) Device ID [1-100,etc]
c: (        13) Channel [11-26]
o: (0x00000000) Option Bits [HEX:32bit]

 [ESC]:Back [!]:Reset System [M]:Extr Menu                      
```



{% hint style="info" %}
This BEHAVIOR hooks the input/output of the Serial object and performs screen input/output in Interactive settings mode. There is no need to explicitly describe input processing in the application. Screen output from the application is suppressed during Interactive settings mode.
{% endhint %}

{% hint style="info" %}
Enter the M key to access auxiliary functions. See the Extra Menu item at the end of this section.
{% endhint %}



## Usage

### Registration

```cpp
// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <STG_STD> // interactive mode
```

Add `#include <STG_STD>` as above.



### Reading with setup()

```cpp
uint32_t APP_ID;
uint8_t CHANNEL;
uint8_t LID;

void setup() {
   ...
   auto&& set = the_twelite.settings.use<STG_STD>();

   // call reload() before reading values.   
   set.reload();
   
   // read value
   APP_ID = set.u32appid();
   CHANNEL = set.u8ch();
   LID = set.u8devid();
   
   // apply them
	 the_twelite
		<< TWENET::appid(APP_ID)
		<< TWENET::channel(CHANNEL);
		
   auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
   nwk	<< NWK_SIMPLE::logical_id(LID);
}
```

In most cases, settings are read out early in the `setup()` process.

The above example first registers the settings behavior with `the_twelite.settings.use<STG_STD>()`.

{% hint style="info" %}
Immediately after registration, you can make changes to the configuration behavior, such as changing the default Application ID, which is described later.
{% endhint %}

Next, call `set.reload()` to actually read the data from the EEPROM and interpret it. Note that it does not read the data automatically.

`set.u32appid()`, `set.u8ch()`, and `set.u8devid()` get the set values of Application ID, CHANNEL, and logical device ID, respectively. Here, each set value is stored in a variable.

{% hint style="info" %}
Since the internal data structure storing the setting values is relatively complex, it is necessary to search the data in the design array until the desired setting is obtained. If you are concerned about computational complexity, store the configuration values in a separate variable.
{% endhint %}

Then, use the setting values to reflect the values of Application ID, CHANNEL, etc.



## List of settings

The following is the list of setting IDs (`enum class E_STGSTD_SETID`) definition.

| Setting ID | Contents |
| ---------------- | --------------- |
| `APPID` | Application ID |
| `LOGICALID` | Logical device ID (8bit) |
| `CHANNEL` | CHANNEL
| `CHANNELS_3` | Channel (up to 3 channels) |
| `POWER_N_RETRY` | Outputs and retry counts |
| `OPTBITS` | Option 1 |
| `OPT_DWORD2` | Option 2 |
| `OPT_N_RETRY` | Output and Retry Count
| `OPT_DWORD3` | Option 3 | `OPT_DWORD4` | Option 4 | `ENC_MODE
| `ENC_MODE` | Encryption Mode |
| `ENC_KEY_STRING` | Encryption key (string input) |

In `<STG_STD>`, four typical settings and four freely available 32-bit values are defined. These are freely available to the user.

{% hint style="info" %}
* Settings are not reflected by simply reading them from STG_STD>.
* Unnecessary items can be hidden.
* You can change the item name and item details.
{% endhint %}



## Customizing a setting behavior

Customize all items of the configuration BEHAVIOR before `.reload()`.

### application_name

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appname("MY_APP");
	...
	set.reload();
```

The application name appears on the first line of the Interactive settings mode.

```
[CONFIG/MY_APP:0/SID=8102ECE3]
```

Specify a string pointer. Local variables cannot be specified because we do not make copies internally.



### Default value.

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appid_default(0x13579be);
	set << SETTINGS::ch_default(18);
	set << SETTINGS::lid_default(7);
		...
	set.reload();
```

アプリケーションID、チャネル、論理ID(LID)については、デフォルト値を変更できます。



### Multiple CHANNEL setting menu

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::ch_multi();
	...
	set.reload();
```

When `SETTINGS::ch_multi()` is specified, multiple CHANNEL settings are specified. When multiple settings are made, `.u32chmask()` is used to read the setting values.



### Display the setup screen immediately

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::open_at_start();
	...
	set.reload();
```

Default values can be changed for Application ID, CHANNEL, and Logical ID.



### Change of item name and description of details

```cpp
const TWESTG_tsMsgReplace SET_MSGS[] = {
	{ int(E_STGSTD_SETID::OPTBITS),    "Opt1", 
			"Please set option 1" },
	{ int(E_STGSTD_SETID::OPT_DWORD2), "Opt2",
			"Please set option 1.\r\nOption 2 is ..." },
	{ 0xFF } // terminator
};

setup() {
  auto&& set = the_twelite.settings.use<STG_STD>();
	set.replace_item_name_desc(SET_MSGS);
	...
```

You can change the item name to something else. In the above example, the Japanese name is set to UTF-8, but it will not be displayed properly unless the conditions, such as terminal display, are met.

{% hint style="info" %}
In the case of TWELITE STAGE, if the number of characters in the registered font is reduced, some characters may not be displayed. Rebuild to include the necessary strings.
{% endhint %}

This array is terminated with `{ 0xFF }` at the end.

The first entry is the setting ID, the second is the item name, and the third is the description to be displayed when entering the setting. You can use `\r\n` to start a new line.



### Determine if you are currently in the settings screen

```cpp
		auto&& set = the_twelite.settings.use<STG_STD>();
		if (!set.is_screen_opened()) {
		   // Processing when the setup screen is not displayed
		}
```

Output to serial while the setup screen is being output can cause screen corruption. To confirm that it is not the setup screen, use `.is_screen_opened()`.



### Deleting Items

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
set.hide_items(E_STGSTD_SETID::OPT_DWORD3, E_STGSTD_SETID::OPT_DWORD4);

...
if(set.is_hidden(E_STGSTD_SETID::OPT_DWORD3) {
  ; // OPT_DWORD3 is hidden
}
```

Removes unwanted items. `.hide_items` hides unnecessary items with the item ID as a parameter (multiple parameters can be specified as variable arguments). You can check whether an item is hidden or not by `.is_hidden()`.

{% hint style="warning" %}
Application ID, CHANNEL, and Logical ID (LID) cannot be deleted.
{% endhint %}

{% hint style="warning" %}
The hide setting uses internal work memory. Since the minimum memory size (32 bytes) is set, if many hidden items are set, they may not be hidden due to insufficient memory. The capacity of work memory can be specified as a compile argument like `-DSIZE_SETSTD_CUST_COMMON=48`.
{% endhint %}



## Methods

### reload()

```cpp
	auto&& set = the_twelite.settings.use<STG_STD>();
	set << SETTINGS::appname(APP_NAME)
		  << SETTINGS::appid_default(DEF_APP_ID)
   		<< SETTINGS::open_at_start();
   
	set.reload();
```

設定を読み込みます。すべてのカスタマイズが終わってから実行します。



## Methods (Reading data)

The following method is called to read data.

{% hint style="warning" %}
Be sure to call `.reload()` before reading.
{% endhint %}

| method | content |
| ------------------------------- | ----------------------------------------------- |
| `uint32_t u32appid()` | Application ID |
| `uint8_t u8devid()` | logical device ID |
| `uint8_t u8ch()` | configuration channel (11..26) |
| `uint32_t u32chmask()` | CHANNEL setting mask (specified by bitmask, if 13, set bit to 1UL << 13) |
| `uint8_t u8power()` | output setting (0..3) |
| `uint8_t u8retry()` | Number of retries.
| `uint32_t u32opt1()` | Option 1 |
| `uint32_t u32opt2()` | option2 | option2 |
| Option1 | Option2 | Option3 | Option4 | Option5 | Option6 | Option7 | Option8 | Option9 | Option9
| Option3 | `uint32_t u32opt4()` | Option4 |
| `uint8_t u8encmode()` | encryption mode (0: none 1: enable 2: enable, also display plaintext packets) |
| `const uint8_t * u8enckeystr()` | get encryption key |

{% hint style="info" %}
Internally, a linear search of the structure array is performed to find the setting structure matching the setting ID, which has an overhead. Frequently referenced configuration values should be copied once to another variable for use.
{% endhint %}



### Reflecting settings

You can use this object to directly reflect settings on `the_twelite` and `<NWK_SIMPLE>` objects.

```cpp
auto&& set = the_twelite.settings.use<STG_STD>();
...
set.reload(); //This is where the actual settings are read from the EEPROM.

the_twelite << set; // Reflecting setting values (e.g. APPID)

auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();

nwk << set; // Reflection of setting values (LID, etc.)

```

The setting values to be reflected are as follows. Items hidden by `.hide_items()` are not reflected.

| Target | Item ID | Contents                                                                                                                                                  |
| -------------- | --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| `the_twelite` | `APPID` | reflected in `TWENET::appid(value)`.                                                                                                                          |
| | `CHANNEL` | <p><code>This is reflected in `TWENET::channel(value)</code>. </p><p><code>* Not reflected when SETTINGS::ch_multi()</code> is specified</p> |
| | `CHANNELS_3` | <p><code>Setting by TWENET::chmask(value)</code>. </p><p><code>* Only when SETTINGS::ch_multi()</code> is specified, it is reflected as a CHANNEL mask. </p> |
| | `POWER_N_RETRY` | <p>Setting by <code>TWENET::tx_power(value)</code> and <code>TWENET::mac_retry_count(value)</code>. </p><p>Note: The retry settings with <code>&#x3C;NWK_SIMPLE></code> also refer to the same value. </p> |
| `<NWK_SIMPLE>` | `LOGICALID` | Set by `NWK_SIMPLE::logical_id(LID)`.                                                                                                            |
| `POWER_N_RETRY` | Set by `NWK_SIMPLE::repeat_max(LID)`.                                                                                                            | | `NWK_SIMPLE::repeat_max(LID)`.



## Item ID

You may specify item IDs with `.hide_items()` and so on. This item ID is defined in `enum class E_STGSTD_SETID`.

| E_STGSTD_SETID:: | Contents |
| ------------------ | ------------ |
| `APPID` | Application ID |
| `LOGICALID` | Logical ID (0..100) |
| `CHANNEL` | CHANNEL |
| `CHANNELS_3` | CHANNEL(specify multiple)
| `POWER_N_RETRY` | output and retry settings |
| `OPTBITS` | option bits |
| `UARTBAUD` | Baud rate setting for UART |
| `OPT_DWORD2` | Option bits 2 |
| `OPT_DWORD2` | Optional bits 2 | Optional bits 3 | Optional bits 4 | Optional bits 5
| `OPT_DWORD4` | Option bit 4 |
| `ENC_MODE` | Encryption Mode |
| `ENC_KEY_STRING` | encryption key |



## Extra Menu

```
[ROOT MENU/BAT1/SID=810B645E]
0: CONFIG
1: EEPROM UTIL
2: H/W UTIL

 [ESC]:Back [!]:Reset System                                    
```

Enter the M key to access additional menus.

* CONFIG : returns to the setup screen
* EEPROM UTIL : This menu is used to perform EEPROM maintenance
* H/W UTIL: This menu allows you to check hardware status



### EEPROM UTIL

```
[EEPROM UTIL/BAT1/SID=810B645E]
r: Read sector.
R: Read ALL sectors.
e: Erase sector.
E: Erase ALL sectors.

 [ESC]:Back [!]:Reset System [M]:Extr Menu
```

Reads or deletes a sector. To read all or delete all, enter the three letters "YES" in uppercase.



### h/w util

````
[H/W UTIL/BAT1/SID=810B645E]                   
```

The function is not provided in the current version.
