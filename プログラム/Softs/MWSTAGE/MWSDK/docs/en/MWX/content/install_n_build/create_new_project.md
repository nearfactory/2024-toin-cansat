---
title: "Creating a new project"
author: "Mono Wireless Inc."
description: "Creating a new project"
---
# Creating a new project

To create a new project, copy the folder of an already existing sample act with a different name and edit the file name.

{% hint style="warning" %}
The destination folder does not have to be a folder under the MWSDK. However, the folder name **must not contain any whitespace characters or Japanese names**.
{% endhint %}



The file structure of the project is as follows (we'll use `PingPong` as an example)

```
Act_samples
  +-PingPong
    +-PingPong.cpp   : ACT file
    +-build          : build dir
    +-.vscode        : setting fils for VSCode    
```

Copy this `PingPong` folder to another location (but without Japanese characters or spaces in the folder name).

```
SomeDir
  +-AlphaBravo
    +-PingPong.cpp -> AplhaBravo.cpp (Note: Changed file name)
    +-build          : build dir
    +-.vscode        : setting files for VSCode
```

The only thing you need to edit is the file name `PingPong.cpp`. Change it to `AlphaBravo.cpp`, the same as the folder name.

{% hint style="success" %}
Run `build\build-BLUE.cmd` and if a BIN file is generated, it' is done (Windows 10).

On Linux/macOS, run `make TWELITE=BLUE` to see if the build succeeds.
{% endhint %}



## Edit Build Definition
To add files to be built, edit `build/Makefile`. The `.c` `.cpp` files directly under the project will be added automatically, but other files will need to be edited.

See [Makefile Description](makefile.md) for how to edit the file.。

## Configuration for VSCode
If you use VSCode, edit the definition under .vscode as necessary.

Most of the examples included in the TWELITE STAGE SDK are as follows
* The source code for the TWELITE STAGE SDK library cites `${env:MWSDK_TWENET_LIBSRC}/include/**` `${env:MWSDK_TWENET_LIBSRC}/src/**`. This environment variable `MWSDK_TWENET_LIBSRC` is automatically set when the project is opened in VSCode from the TWELITE STAGE app.
* For the build task, no additional options such as `-D` are set by default.
