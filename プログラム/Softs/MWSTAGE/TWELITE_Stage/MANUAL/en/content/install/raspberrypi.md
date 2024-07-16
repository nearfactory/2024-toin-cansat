---
title: "RaspberryPi"
author: "Mono Wireless Inc."
description: TWELITE_Stage installation (Raspberry Pi)
---

# RaspberryPi

`RasPi`  

The TWELITE Stage app runs on a Raspberry Pi.

* Mouse and touch screen support.
* The build toolchain is included and can be used to compile the application.
* Four executable versions are available: framebuffer version, X11 version, framebuffer lightweight version, and X11 lightweight version. The lightweight version omits semi-transparency effects, etc.

{% hint style="warning" %}
Depending on the OS type, version, and installation status of your RaspberryPi, it may not work or you may need to recompile it.
{% endhint %}

## Environment

TWELITE STAGE has been developed and tested in the following environment.

* Hardware
  * Raspberry Pi 3 Model B
  * LCD Screen: Raspberry Pi Touch Display \(7"\)
* OS & distribution
  * Raspberry PI OS \(32bit\) Lite \(Version:August 2020\)


## Known Issues/Limitations.

* Operation of `/dev/serial0` may fail on the first boot.
* The operation of `/dev/serial0` has not been tested on Raspberry Pi 4B.
* Touch screen operation has not been tested on Raspberry Pi 4B.
* The input string to TWELITE STAGE is passed as is to the input string to the shell or getty running on `/dev/tty1`. It is recommended to start from `/dev/tty1`.
* This may be affected by other installations or running programs \(such as X11\).



## Extract archive

The downloaded archive file should be extracted to a folder whose path name does not contain spaces or Japanese characters.

In the following, it is extracted to the home folder of the Raspberry Pi.

```text
$ cd /home/pi
$ unzip MWSTAGE2020_XX_YYYY.zip
```

### folder structure

```text
../MWSTAGE
     TWELITE_Stage.run    TWELITE_Stage App.
     BIN/                 BIN files of App. firmware
     MWSDK/               MWSDK library, etc.
     TWELITE_Stage/       TWELITE_Stage Application related files
```



## Device Driver

In order to recognize MONOSTICK or TWELITE-R from TWELITE STAGE, you must unload the ftdi\_sio module and also have read/write permission to the USB device.

{% hint style="info" %}
The ID of the USB device will be as follows.

* Vendor ID 0x0403
* Product ID 0x6001\(MONOSTICK,TWELITE R\) or 0x6015 \(TWELITE R2\) 
{% endhint %}

A udev configuration script is provided to automate this setup. It copies the definition to `/etc/udev/rules.d` and reloads the configuration. After configuration, unplug and plug the USB device before running `TWELITE_Stage.run`. If the USB device appears on the screen right after startup, the configuration is reflected.



```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/ubuntu/
$ sudo ./set_udev_sudo.sh
```

Definition file (line breaks for easier reading)

```text
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001",
   MODE="0666",
   RUN+="/bin/sh -c 'rmmod ftdi_sio && rmmod usbserial'"
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015",
   MODE="0666",
   RUN+="/bin/sh -c 'rmmod ftdi_sio && rmmod usbserial'"
```



## UARTについて

In the environment described above, `/dev/serial0` is available by configuring the serial port from `raspi-config`.

```text
  $ sudo raspi-config

  From the Menu:
  "3 Interface Options    Configure connections to peripherals"
  →"P6 Serial Port Enable/disable shell messages on the serial connection"

  Select the following: do not use as a login shell, or enable hardware.
  "Would you like a login shell to be accessible over serial?" -> <No>
  "Would you like the serial port hardware to be enabled?" → <Yes>
```



### Wiring example

```text
 [TWELITE]               [RaspberryPi]
  GND  ------------------ Gound (#6,#9,#14,#20,#25,#30,#34,#39のいずれか)
  TXD(DIO6,DIP#10) ------ GPIO15/UART0 RXD (#10)
  PRG(SPIMISO,DIP#7) ---- GPIO23 (#16)
  RXD(DIO7,DIP#3) ------- GPIO14/UART0 TXD (#8)
  RST(RESETN,DIP#21) ---- GPIO22 (#15)
  VCC  ------------------ 3V3 (#1,#17のいずれか)
  SET(DIO12,DIP#15) ----- GPIO12 (#32)
```

* Refer to the manufacturer's manual for both TWELITE and RaspberryPi.
* DIP# is the pin number of the TWELITE DIP.
* The above wiring does not guarantee stable operation of the TWELITE wireless microcontroller module.





## Launch TWELITE Stage app.

* It will not run on the X11 desktop; close X11.
* Run `TWELITE_Stage.run`. The TWELITE Stage app will appear on the screen screen.

### Notes.

* Mouse and touch screen are supported.
* During the TWELITE Stage app, the characters you enter may also be displayed on the console screen.



## Others

### /dev/dri

You may get the following error when starting `TWELITE_Stage.run`.

```text
  "The path /dev/dri/ cannot be opened or is not available"
```

You may ignore it.



### Insufficient memory

At build time, if the number of CPUs is 4 or more, the parallel compilation is executed minus one CPU number (3 parallel compilations for 4 cores). In some cases, insufficient memory may occur. In that case, please change the number of parallels.



### RaspberryPi 4

{% hint style="warning" %}
This information has not been fully validated.
{% endhint %}

The following settings are required: OpenGL related drivers must be enabled.

* Select Advanced Settings → A2 GL Driver → G2 GL \(Fake KMS\ in `raspi-config`)
* Install the `libgles-dev` package.
* Touch screen operation has not been tested

