---
title: "Environment (OS, etc.) "
author: "Mono Wireless Inc."
description: "DESC"
---

# Environment (OS, etc.)

{% hint style="warning" %}
In order to create a development environment, you need to install the software and agree to the license agreement. In addition, you may need to configure security settings on your PC or workstation.

* Although we take great care when distributing the software, we ask that you also check for viruses.
* Please check with the administrator of your environment regarding your security approach and operations (e.g., whether or not to install external applications).

In addition, the installation and operation of the development environment may require the intervention and configuration of the operating system (e.g., running an application whose developer is unknown; many of the development environments and tools introduced here do not have a built-in mechanism to prove the origin of the application) Please refer to the general information on how to configure.
{% endhint %}



To write an application using the MWX library, you will need the following:

* MWSDK(Software Development Environment)
* An editor for development (Microsoft's VisualStudio Code will be introduced)

### Windows10,11
Compiler toolchains, etc., are relatively less dependent on the environment, so they can be expected to work in many environments, but we recommend the Windows 10 version that is currently supported. If your system does not work due to differences in the operating environment, please prepare a separate environment by referring to the environment we have confirmed.

The following is the version we are using for development.

* Windows11 21H2 (Visual Studio 2019)
* FTDI driver must be running (to run MONOSTICK, TWELITE R)


### Linux
Compiler toolchains, for example, are relatively less dependent on the environment, so they can be expected to work in many environments, but we recommend distributions that are currently supported. If your system does not work due to differences in the operating environment, please prepare a separate environment by referring to the environment that we have confirmed.

The following is the version we are using for development.

* Ubuntu 18.04 LTS 64bit
* Ubuntu 20.04 LTS 64bit

32bit systems are not supported.

### macOS
Compiler toolchains, for example, are relatively less dependent on the environment, so they can be expected to work in many environments, but we recommend distributions that are currently supported. If your system does not work due to differences in the operating environment, please prepare a separate environment by referring to the environment that we have confirmed.

The following is the version we are using for development.

* macOS 10.14 (Mojave, Intel)
* macOS 12.4 (Monterey, Apple Silicon)

About Visual Studio Code and other development environments

For information on how to run and use the development environment, please refer to the information of its developer or community.

{% hint style="info" %}
The use of Visual Studio Code (VSCode) is recommended for efficient code writing.

The MWX library requires more PC resources to interpret the code on VSCode, etc., since there are more header files to read compared to C language development.
{% endhint %}



## Differences by build environment

{% hint style="warning" %}
The build results for Linux/macOS are different from the results for Windows 10. As far as we know, there is no difference in the normal operation, but the binary size tends to be a few percent larger, especially since the LTO of gcc is disabled.

If you have any doubts about the operation, be sure to run the build on Windows 10 and confirm that it reproduces before contacting us.
{% endhint %}
