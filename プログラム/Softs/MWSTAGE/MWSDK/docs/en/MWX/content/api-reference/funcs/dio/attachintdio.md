---
title: attachIntDio()
author: "Mono Wireless Inc."
description: to enable DIO interrupt.
---
# attachIntDio()

Enables DIO interrupts.

```cpp
void attachIntDio(uint8_t u8pin, E_PIN_INT_MODE mode)
```

For a preconfigured pin, the first parameter is the pin number for which you want to enable interrupts, the second is the interrupt direction ([rising, falling](README.md).

{% hint style="info" %}
Interrupt handlers and event handlers are written in [Application BEHAVIOR](../../behavior/).
{% endhint %}



## Example

Set up an interrupt to be generated when the DIO5 pin changes from HIGH to LOW.

```cpp
void setup() {
  the_twelite.app.use<myAppClass>();
  
  pinMode(PIN_DIGITAL::DIO5, PIN_MODE::INPUT_PULLUP);
  attachIntDio(PIN_DIGITAL::DIO5, PIN_INT_MODE::FALLING);
}

void loop() {
  ;
}
```



#### myAppClass.hpp

```cpp
class myAppClass: public mwx::BrdPal, MWX_APPDEFS_CRTP(myAppClasslMot)
{

};
```

Basic definition of the application behavior `myAppClass`. Details are omitted.

#### myAppClass.cpp

```cpp
/*****************************************************************/
// MUST DEFINE CLASS NAME HERE
#define __MWX_APP_CLASS_NAME myAppClass
#include "_mwx_cbs_cpphead.hpp"
/*****************************************************************/

MWX_DIO_INT(PIN_DIGITAL::DIO5, uint32_t arg, uint8_t& handled) {
  static uint8_t ct;
  digitalWrite(PIN_DIGITAL::DIO12, (++ct & 1) ? HIGH : LOW);
	handled = false; // if true, no further event.
}

MWX_DIO_EVENT(PIN_DIGITAL::DIO5, uint32_t arg) {
  Serial << '*';	
}

/*****************************************************************/
// common procedure (DO NOT REMOVE)
#include "_mwx_cbs_cpptail.cpp"
// MUST UNDEF CLASS NAME HERE
#undef __MWX_APP_CLASS_NAME
} // mwx
/*****************************************************************/

```

Description of the interrupt handler of the application behavior `myAppClass`, which inverts the output setting of DIO12 when an interrupt of DIO5 is generated and displays `*` on the serial port `Serial` for events occurring after the interrupt handler is finished.
