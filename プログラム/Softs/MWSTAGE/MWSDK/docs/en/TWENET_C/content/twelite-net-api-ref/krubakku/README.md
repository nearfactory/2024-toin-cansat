# The callback functions

Callback functions are the functions used to handle the various TWENET events as they occur (flow diagram), all of which need to be implemented when building an application with the TWENET library. The most important function is [`cbAppColdStart()`](cbappcoldstart.md), which is the equivalent of the general main() function.

* [cbAppColdStart](cbappcoldstart.md)() – Function called on power up
* [cbAppWarmStart](cbappwarmstart.md)() – Function called when sleep is restored
* [cbToCoNet_vMain](cbtoconet_vmain.md)() – The main loop is not an infinite loop, but is called when an interrupt or other event occurs.
* [cbToCoNet_vRxEvent](cbtoconet_vrxevent.md)() – Called when a radio packet is received
* [cbToCoNet_vTxEvent](cbtoconet_vtxevent.md)() – Called when radio packet transmission is complete
* [cbToCoNet_vNwkEvent](cbtoconet_vnwkevent.md)() – Various event notifications at MAC and network layers
* [cbToCoNet_vHwEvent](cbtoconet_vhwevent.md) () – Delayed execution section of peripheral interrupt processing. Called after interrupt processing.
* [cbToCoNet_u8HwInt](cbtoconet_u8hwint.md)() – Peripheral interrupt handlers
* User defined event handling functions – 状Describes task processing with state transitions

Both callback functions need to return control after they have been called, and writing very long processes will affect the overall behaviour. Particular care should be taken when writing interrupt handlers.

