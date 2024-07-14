---
title: "Installation"
author: "Mono Wireless Inc."
description: Installation
---

# Installation

Depending on the environment, this application may not work or may require various settings. If you encounter any problems, please refer to the description in this document to improve your environment.

{% hint style="warning" %}
In order to build a development environment, you must install the software suite and agree to the license agreement. In addition, you may need to configure security settings on your PC or workstation.

* Although we take all due precautions when distributing the software, we ask that you take care to check for viruses, etc.
* Please check with the administrator of your environment regarding your security policy and operations (e.g., whether or not to install external applications).

In addition, please see ["Application Distribution and Execution"](https://twelite.gitbook.io/general/appinstall). Topics include topics related to the following

* Confirmation of identity with downloaded files
* Handling of code signatures on macOS/Windows
{% endhint %}


## extract TWELITE STAGE SDK

1. [Download](https://mono-wireless.com/jp/products/stage/) the TWELITE STAGE SDK for your system (Windows, macOS, Linux, etc.).
2. Extract the downloaded archive file (e.g. ZIP).
   **The folder name of each level of the destination folder should not contain any characters other than one-byte numbers `0..9`, one-byte alphabets `a..zA..Z`, and some symbols `-_. `. It must not contain spaces, Kanji characters, Hiragana characters, etc.**
   Example(Windows): × `C:\work\作業\` × `C:\Users\user1\work dir\` 〇 `C:\Work\Work1`
3. Check the folder after expansion.
   If you extract on `C:\Work`, the folder name will be something like `C:\Work\MWSTAGE`, but your extractor may create a folder with the archive file name. Rename or move the folder as needed.


In the text, the expanded folder \(e.g. `C:\WorkMWSTAGE` \)} may be written as `{MWSTAGE installation}`.

## Settings unique to each system, etc.

See below.

* [Windows](windows.md)
* [macOS](macos.md)
* [linux](linux.md)
* [RaspberryPi](raspberrypi.md)

## Folder structure of TWELITE STAGE SDK

The folder `{MWSTAGE installation}` where the TWELTIE STAGE SDK is installed contains the following

* TWELITE STAGE application executable
* MWSDK folder - libraries, source code, etc.
* Tools - toolchain for building, etc.

For details, see [Folder Structure](../misc/directories.md)" for details.

