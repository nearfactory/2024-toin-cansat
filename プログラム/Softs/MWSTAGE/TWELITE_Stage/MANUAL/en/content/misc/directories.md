---
title: "About Folder Structure"
author: "Mono Wireless Inc."
description: "About Folder Structure"
---

# About Folder Structure

The TWELITE STAGE APP runs as a front-end application in the TWELITE STAGE SDK package. Here is its folder structure.

```text
... /MWSTAGE/           TWELITE STAGE SDK Installation.
    /TWELITE_Stage.???? Executable (Windows .exe, macOS .command, Linux .run)
    /TWELITE_Stage.sav  Configuration file
    /TWELITE_Stage.ini  Other settings
    /TWELITE_Stage/     TWELITE STAGE APP related files

    /MWSDK/             MWSDK libraries, etc.
    /BIN/               Destination for [BIN file selection
    /log/               Logs and databases

    /Tools/             gcc compiler and other tools
````



### MWSDK folder

```text
MWSDK/
  Act_samples/ : sample code by mwx library 
  Wks_TweApps/ : source code for TWELITE APPS 
  Act_extras/  : more specialized samples by mwx library, citing other libraries
  TWENET/      : TWENET library (mwx library and others) 
  ChipLib/     : semiconductor library  
  MkFiles/     : body processing part of Makefile     
  docs/        : Library manuals, etc.   
  000manifest  : Version information of MWSDK
```

The MWSDK folder contains libraries for building software for the TWELITE wireless microcontroller, samples, and source code for the TWELITE APPS.


### TWELITE_Stage.sav

Records configuration information within the TWELITE Stage application.

The file name is the TWELITE_Stage app file name + `.sav`.

### TWELITE_Stage.ini

see [this](./cmdargs_and_ini.md) for details.

* `MWSDK=`
Edit to specify a different folder instead of the `MWSDK/` folder. This is useful if you want to mix multiple library versions. The above example uses the `MWSDK2020_10` folder.

* `LANG=`
Specify `LANG=en` to set the display language of TWELITE Stage to English.


### Run TWELITE Stage with different settings
Copy `TWELITE_Stage.exe` (on Windows) with a different file name.
For example, if you change it to `TWS1.exe`, it refers to the configuration files `TRS1.sav` and `TRS1.ini`.

### BIN folder

When you select the TWELITE STAGE APP's [BINから選択](../usage/screens/main\_menu/firm\_prog/bin.md) menu, the firmware files (.BIN) for the TWELITE radio microcontroller in this folder are enumerated.


### log folder

Log files are stored in this folder when the serial port [logging function](../usage/logging.md) is performed in the TWELITE STAGE APP.

This folder is also used to store databases when using the graphing function and to output csv files.


### Tools folder

This folder contains toolchains for gcc, g++, and other cross-compilers.

Platform specific utilities are also stored in this folder. See `Tools/reame-j.txt` for details.


## About the build project folder

{% hint style="warning" %}
This function is introduced as an extra function. It may work differently from what is described.
{% endhint %}

### Search order of folders

Search for build project folders (e.g., Act\_samples) in the following order

1. the folder where the TWELITE STAGE app was launched
2. the folder where the TWELITE STAGE app executable is located
3. `{MWSDK folder}/..`.
4. `{MWSDK folder}/..`

{% hint style="info" %}
MWSDK searches starting from the folder where the TWELITE STAGE APP executable is located.
{% endhint %}

### Wks\_Acts
If the `Wks_Acts` folder exists, this folder is referenced from the menu item `Act Build & Rewrite` instead of the `Act_samples` folder.

{% hint style="info" %}
This folder is intended to be used to store build projects that you create yourself in Wks\_Acts.
{% endhint %}

