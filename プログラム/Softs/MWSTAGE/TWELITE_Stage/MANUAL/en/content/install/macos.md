---
title: "macOS"
author: "Mono Wireless Inc."
description: TWELITE_Stage installation (macOS)
---

# macOS

`macOS`  

### 環境

We develop in the following environment

* macOS 10.14 (Mojave, Intel)
* macOS 12.4 (Monterey, Apple Silicon)

### Additional installation and warning dialogs.

* The download archive is not signed. At runtime, you may receive a security warning as an application downloaded from the Internet.
* You may be asked for permission to run from the path where you installed TWELITE\_Stage.
* You may get an installation dialog for the make utility when executing a build.
* The toolchain is code-signed, but if the code signature is not properly authenticated, you may be asked for permission to run each executable in the build toolchain \(ba-elf-gcc etc.}).

The above is required to give permission or installation work for the TWELITE_Stage to work.

#### Reference: Manual installation of the make utility

※ Different OS versions and other factors may require a different procedure.

If an error occurs when executing make on the command line \(bash\), install XCode.

```bash
$ xcode-select --install
```

After installation is complete, type "make" and the following message should appear.

```bash
$ make
make: *** No targets specified and no makefile found.  Stop.
```



### Handling of Serial Port

MONOSTICK and TWELITE-R use FT232 series semiconductors from FTDI \(https://www.ftdichip.com\). In order to use them, you may need to deal with device drivers.

If the serial port does not appear when you start TWELITE-R, you need to unload (disable) FTDI's driver. See "Reference" below.


#### Reference: utilities from FTDI, Inc.

{% hint style="danger" %}
This utility suppresses the loading of the OS standard device driver when our MONOSTICK or TWELITE-R is inserted, but it also suppresses it for non-MONOSTICK devices (those with the same USB ID).
{% endhint %}

Download and use D2xxHelper from https://www.ftdichip.com/Drivers/D2XX.htm. You can find the same in our archive in the TWELITE_Stage/INSTALL folder.



#### Reference: Manual unloading of FTDI device drivers

Unload FTDI-related drivers.

```bash
$ sudo kextunload -b com.apple.driver.AppleUSBFTDI
```



