---
title: "Linux"
author: "Mono Wireless Inc."
description: TWELITE_Stage installation  (Linux)
---

# Linux

`Linux`  

{% hint style="warning" %}
USB device ID for TWELITE R2 has changed from 0403:6001 to 0403:6015. udev setting needs to be added.
{% endhint %}

{% hint style="danger" %}
32bit version is not available.
{% endhint %}

{% hint style="warning" %}
The Linux environment has different types of packages available depending on the distribution and version. You may need to install and configure packages individually.

Please refer to general information by referring to the error messages.
{% endhint %}

### Environment

TWELITE STAGE is developed and tested in the following environment.

* Ubuntu 16.04, 18.04, 20.04
* NNLinux Beta8 64bit
* Centos7


### device driver

In order to recognize MONOSTICK or TWELITE-R from TWELITE STAGE, you must unload the ftdi\_sio module and also have read/write permission to the USB device.

{% hint style="info" %}
The ID of the USB device will be as follows.

* Vendor ID 0x0403
* Product ID 0x6001\(MONOSTICK,TWELITE R\) or 0x6015 \(TWELITE R2\) 
{% endhint %}

We have a udev configuration script Ј(Ubuntu, CentOS Ј) to automate this configuration. It copies the definition to `/etc/udev/rules.d` and reloads the configuration. After configuration, unplug and plug the USB device before running `TWELITE_Stage.run`. If the USB device appears on the screen right after startup, the configuration is reflected.


#### Ubuntu 16.04, 18.04. 20.04

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



#### Centos 7

```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/centos/
$ sudo ./set_udev_sudo.sh
```

Definition file (line breaks for easier reading)

```text
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001",
   MODE="0666", 
   RUN+="/bin/sh -c '/usr/sbin/rmmod ftdi_sio'"
ACTION=="add",
   ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015",
   MODE="0666", 
   RUN+="/bin/sh -c '/usr/sbin/rmmod ftdi_sio'"
```



### Application Registration

Register your program according to your desktop environment if necessary.

{% hint style="info" %}
You may not be able to run `TWELITE_Stage.run` from the file icon on your desktop.
This is because the system may not recognize `TWELITE_Stage.run` as an executable.
{% endhint %}

#### Ubuntu 16.04, 18.04, 20.04

Definition file generation scripts for Ubuntu are available.

```text
$ cd ./MWSTAGE/TWELITE_Stage/INSTALL/ubuntu/
$ ./make_launch_icon.sh
```

This script creates a `.desktop` file (app definition) in `$HOME/.local/share/applications`.

After the script is executed, the TWELITE STAGE icon will be added to the application list.

