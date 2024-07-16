---
title: "CUE Viewer"
author: "Mono Wireless Inc."
description: "CUE Viewer"
---
# CUE Viewer

`Windows` `macOS` `Linux` `RasPi`

Interpret messages from [TWELITE CUE](https://mono-wireless.com/cue).

{% hint style="info" %}
During this commentary, the cupcake image of the previous version is shown.
{% endhint %}

## TWELITE CUE

![Screen example: Detecting upward-facing surfaces](<../../../../../.gitbook/assets/img_viewer_cue_face.png>)

The default setting (CUE mode) of TWELITE CUE wakes up due to several factors and transmits various information after waking up while driving intermittently using sleep, which can be operated even with coin cell batteries.

{% hint style="info" %}
In CUE mode, all on-board sensors and LEDs are in operation in order to test the operation of TWELITE CUE.
{% endhint %}

#### wakeup factors

* Timer wake-up (periodic wake-up by set value)
* Wake-up by acceleration detection
* Wake-up by magnetic sensor (when a magnet is detected approaching)

#### Transmission data

* Detection event (see below)
* Module power supply voltage
* Magnetic sensor detection value
* Acceleration data



### Packet

Displays basic packet information.


| Item | Explanation                                                                                                                        |
| ----- | ------------------------------------------------------------------------------------------------------------------------- |
| #???? | The number of packets received so far.                                                                                                           |
| 種別    | The value of [`E_PKT`](https://mwm5.twelite.info/references/parser/twefmt/twepacket/e\_pkt), which is the packet type; packets from CUE usually have PKT_PAL = 02. |
| ID    | Logical ID of the sender. Normally the value is 0..100.                                                                                              |
| AD    | The serial number of the sender.                                                                                                             |
| LQ    | LQI, the value corresponding to the radio wave strength.                                                                                                         |
| SQ    | Packet sequence number.                                                                                                              |



### Events

The default output of TWELITE CUE is always as an acceleration event. Regardless of the wake-up factor, a fixed number of samples of acceleration is measured after wake-up. The event corresponding to the result of this acceleration is sent.

| Detection Event | Number | Description                                                                                                                                                |
| ------ | ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| Dice    | 1(0x00) .. 6(0x06) | The judgment is made based on the regular wake-up and the wake-up detected by the magnetic sensor. If a large acceleration is detected after waking up, an event (0xFF) may be detected that is not allowed to be judged.                                                                      |
| Move    | 16(0x10)           | A move or shake event occurs when the accelerometer wakes up when it detects an acceleration above a certain level. A move event occurs when the change in measured acceleration is not very large. (When a change in acceleration is detected, but there is no continuous change in acceleration)                           |
| Shake | 8(0x08)            | A move or shake event occurs when the accelerometer wakes up when it detects an acceleration above a certain level. A shake occurs when the change in measured acceleration is relatively large. (When changes in acceleration are detected and continuous changes in acceleration can be detected. (As the name Shake implies, it occurs when the CUE is continuously shaken.) |

{% hint style="info" %}
We do not provide a quantitative definition of the detection threshold and event detection conditions for wake-up due to acceleration.
{% endhint %}



### Voltage

Supply voltage of the module\[mV].


### Magnet

Displays detected or undetected magnet poles.

{% hint style="info" %}
No indication of whether the magnetic sensor is waking up due to detection.
{% endhint %}



### Acceleration

Displays acceleration measured after waking up.

|       |                                                              |
| ----- | ------------------------------------------------------------ |
| sample  | The number of samples of acceleration is shown. 10 fixed samples.                               |
| Rate ID | The sample rate of acceleration. 04 fixed at 100 Hz.                                  |
| X,Y,Z | Acceleration of three axes, obtained as an average of 8 samples. The unit is milli-G (1000mG=1G=9.8m/s2). |


## Screen examples

![Screen example: Move (acceleration detection)](<../../../../../.gitbook/assets/img_viewer_cue_move.png>)

![Screen example: Shake (continued shaking)](<../../../../../.gitbook/assets/img_viewer_cue_shake.png>)
