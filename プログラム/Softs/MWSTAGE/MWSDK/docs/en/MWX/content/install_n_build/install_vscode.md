---
title: "Installing VSCode"
author: "Mono Wireless Inc."
description: "Installing VSCode"
---

# Installing VSCode

VisualStudio Code (VSCode) is recommended to make Act (source code) writing easier. The attached Act contains a file that has been configured to ensure that the code is interpreted properly in VSCode.

{% hint style="warning" %}
VSCode reads source files and header files and interprets the source code, thus providing function definition information and function and method name completion to help you write source code. The MWX library requires more header files to be loaded than the C library. In comparison to C development, the MWX library requires more header files to be loaded and the editor may be slower in some environments.
{% endhint %}


The project settings of VSCode requires some information, such as library source code location, to analyse source codes. These inforamtion are passed by TWELITE STAGE app as environmental variable when launching VSCode. Therefore, application instance of VSCode should not be present when launching from TWELITE STAGE app, otherwise VSCode will not refer to these environmental values.


## Installing VSCode

{% hint style="danger" %}
Please note that this support does not cover questions about how to install or use VSCode. Please refer to the information available in the public domain.

Depending on your environment, you may need to configure security settings or other settings in order to install the software. Please check with your system administrator whether installation is possible, and refer to the distributor or general information for instructions.
{% endhint %}

VSCode allows you to do the following:

* Editing the source code
* The IntelliSense based on source code interpretation (\*not all definitions can be guaranteed to be interpreted correctly)

VSCode can be downloaded from the link below.

{% embed url="https://code.visualstudio.com" %}



### Special note for each OS

The `code` command must be enabled to invoke VSCode from TWELITE STAGE.



The following information is from code.visualstudio.com

* [macOS](https://code.visualstudio.com/docs/setup/mac) - PATH must be set so that the `code` command can be executed.
* [Windows](https://code.visualstudio.com/docs/setup/windows)
* [Linux](https://code.visualstudio.com/docs/setup/linux)



### Installing plug-ins

To enable Visual Studio Code to interpret C/C++ language descriptions, install a plugin.

* C/C++ for Visual Studio Code


## Notes

{% hint style="warning" %}
The MWX library examples include a `.vscode` definition. This definition uses the MWSDK\_ROOT environment variable to identify the source code of the library (under `{MWSDK_ROOT}/TWENET/current`).

When launching VSCode from TWELITE STAGE, those settings such as `MWSDK_ROOT` will be setup automatically. In some cases, such as when you have already started VSCode, the settings may not be reflected.
{% endhint %}

{% hint style="warning" %}
VSCode's interpretation of source code is not always the same as the compiler's interpretation. Also, depending on how the source code is edited, the interpretation may be more incomplete.
{% endhint %}
