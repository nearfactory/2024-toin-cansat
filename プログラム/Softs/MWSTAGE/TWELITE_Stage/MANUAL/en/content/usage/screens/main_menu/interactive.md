---
title: "Interactive settings mode"
author: "Mono Wireless Inc."
description: "Interactive settings mode"
---
# Interactive settings mode

`Windows` `macOS` `Linux` `RasPi`

Transition to Interactive settings mode and operation.

![](../../../.gitbook/assets/img_interactive.png)

This screen behaves much the same as the terminal, but has additional features specific to Interactive settings mode, such as operation for Interactive mode transitions and detection of leaving.

* The TWELITE radio module to be connected must have furware written for Interactive settings mode.
* Since the system operates using input/output from the TWELITE radio microcontroller, it may not be possible to transition to or leave Interactive settings mode as expected, for example, if garbled characters or other problems occur in serial communication.
* Mouse operation is not supported. Keyboard operation (using the cursor up/down) is supported.

{% hint style="info" %}
Transition to Interactive settings mode and operation by [terminal](viewer/terminal.md) is also possible.

* The terminal does not operate the SET pin, so the SET pin must be manually set to LO.
* The operations to input `Alt(Cmd)+I` and module reset `Alt(Cmd)+R` are defined.
{% endhint %}


### Operation flow of Interactive settings mode screen

The following is a rough process flow.

```
[Screen black background].
  ↓↓
[TWELITE wireless microcontroller reset (SET=LO if controllable)]
  ↓↓
<Detect interactive mode message for intermittent operation application? > --YES--> Go to [operation screen
  ↓Time out.
['+' three times].
  ↓↓
<Detect interactive mode message for normal app? > --YES--> Go to [operation screen
  ↓Time out
Go to [operation screen] * This state is not Interactive settings mode

[operation screen]
  ↓↓
<Interactive settings mode escape message? > --> [Exit].
  ↓
<Leaving screen operation [ A ] long press etc. > --> [Exit].
  ↓
<ESC ? > -> <determination during input> --NO-> [Exit].
  ↓ ↓
[Send input string to TWELITE radio module]
  ↓↓
Return to [Operation screen

[Exit]
  ↓↓
[TWELITE radio microcontroller reset]
  ↓↓
[Exit screen] Exit Interactive settings mode screen and return to previous screen.  
```