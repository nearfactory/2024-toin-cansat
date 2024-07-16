---
title: "TWELITE STAGE SDK (English)"
author: "Mono Wireless Inc."
description: "TWELITE STAGE SDK (English)"
---

# TWELITE STAGE SDK

TWELITE STAGE SDK is a comprehensive package for developing and testing TWELITE wireless modules with the [TWELITE STAGE application](TWELITE_Stage/MANUAL/en/index.html) at its core.



## Installation

Extract the TWELITE STAGE SDK archive to an appropriate folder.

**The destination folder of the TWELITE STAGE SDK must not contain spaces, Japanese strings, or symbol characters other than `.` `-` `_` **




## File List

```
TWELITE_Stage.xxx : TWELITE STAGE APP Executable file (please run this file)
  TWELITE_stage.exe（Windows）
  TWELITE_stage.command（macOS）
  TWELITE_stage.run（Linux/Raspberry Pi 2+）
TWELITE_Stage.sav  : TWELITE STAGE APP Setting file
TWELITE_Stage.ini  : Additional settings such as MWSDK directory selection

TWELTIE_Stage_VSCode.xxx : TWELITE STAGE APP in VSCode use configuration
  TWELITE_stage_VSCode.exe（Windows）
  TWELITE_stage_VSCode.command（macOS）
  TWELITE_stage_VSCode.run（Linux）
TWELTIE_Stage_VSCode.sav : Pre-configured configuration file for VSCode use
TWELITE_Stage_VSCode.ini : Additional settings such as MWSDK directory selection
*Configuration files and additional configuration files should match the file name of the executable.

The following TWELITE STAGE APP is included only in the Rasbperry Pi version.
TWELITE_Stage_nox.run        : TWELITE STAGE APP Executable file without X11 (Rasbperry Pi 2+)
TWELITE_Stage_nox.sav        : TWELITE STAGE APP Setting file without X11 (Rasbperry Pi 2+)
TWELITE_Stage_nox.ini        : Additional settings such as MWSDK directory selection (Rasbperry Pi2+)
TWELITE_Stage_Zero.run       : TWELITE STAGE APP Executable file for X11 use (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_v6.sav    : TWELITE STAGE APP Setting file for X11 use (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero.ini       : Additional settings such as MWSDK directory selection (Raspberry Pi Zero/1A/1B )
TWELITE_Stage_Zero_nox.run   : TWELITE STAGE APP Executable file without X11 (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_nox.sav   : TWELITE STAGE APP Setting file without X11 (Raspberry Pi Zero/1A/1B)
TWELITE_Stage_Zero_nox.ini   : Additional settings such as MWSDK directory selection (Raspberry Pi Zero/1A/1B)

flask_wsns_db/  	 : Simple web server with Python, Flask, sqlite3
  _readme/               : Images etc
  static/                : CSS etc
  templates/             : HTML templates
  app.py                 : Main Program
  README.md/.html        : Documents

ftd2xx.dll         : Libraries required to run the above executables (Windows)

BIN/               : BIN file for TWELITE wireless microcontroller
log/               : Where TWELITE STAGE APP logging data are stored

MWSDK/             : MWSDK environment files, sample files
  000manifest      : MWSDK version information
  LICENSE          : MWSDK License Description
  README.md        : README
  ReleaseNotes.md  : Update History (Top page)
  ReleaseNotes_en.md  : Update History (English)
  ReleaseNotes_jp.md  : Update History (Japanese)

  Act_samples/     : ACT's (Act) Build Project
  Wks_TweApps/     : TWELITE APPS (ready-made software) build project
  Act_extras/      : Additional ACT build projects

  TWENET/          : TWENET C/C++ library source code
  ChipLib/         : Semiconductor Chip Library
  MkFiles/         : Makefile body processing section

MWSDK{YYYY}_{MM}/  : Older version (can be specified in TWELITE_Stage.ini)

Tools/             : Toolchain for compilation, etc.

TWELITE_Stage/     : TWELITE STAGE APP Related Files
  readme-j.txt     : TWELITE STAGE APP license description
  INSTALL/         : TWELITE STAGE APP Installation Complement
  LICENSE/         : TWELITE STAGE APP License
  MANUAL/	         : TWELITE STAGE APP Manual
```



## English support

The TWELITE STAGE App can be switched to English user interface. Please edit its configuration file as below:

1. If the app is running, close it.
2. Open the `.ini` file for the TWELITE_Stage to be used.
   e.g. `TWELITE_Stage.ini`
3. Add a line below:

```
LANG=en
```

4. Save and close the `.ini` file.
5. (Re)Launch the app.



Note: Reference - [command line arguments and ini files](TWELITE_Stage/MANUAL/en/content/misc/cmdargs_and_ini.html)



## Documents

* [TWELITE STAGE App Manual](TWELITE_Stage/MANUAL/en/index.html)
* [TWELITE STAGE SDK and MWX Library Manual](MWSDK/docs/en/MWX/index.html) - Installation, Quick develop guide and reference manual of MWX library.
* [(TWELITE NET C Library Manual)](MWSDK/docs/en/TWENET_C/index.html) - (NOTE: This is NOT mandate to refer initially, but if you need to develop with C API or see SDK structure.)
* [Simple web server with Python, Flask, sqlite3](flask_wsns_db/README.html) - Manual of a simple server in Python, Flask, and sqlite3.



Note:  The link above is pointing to the latest sub folders in `MWSDK/docs/`, where documents are stored that match the library version.



## Use with VSCode (Visual Studio Code)

In order to improve convenience when using VSCode, the TWELITE STAGE SDK includes TWELITE STAGE application features (configuration required) and pre-configured project settings. Therefore, proper installation and configuration of VSCode is required. In particular, it is necessary to configure VSCode so that it can be started with the code command, which requires additional configuration on some systems.

See [Installing VSCode](MWSDK/docs/en/MWX/content/install_n_build/install_vscode.html) for more information.


## License
* The TWELITE STAGE APP executable files applies MW-SLA-1J,E.
* Executable files (firmwares, applications) for TWELITE wireless microcontrollers applies MW-SLA-1J,E.
* Source codes included in this package are subject to the license described in source codes.
* The deliverables of open source projects such as toolchain are subject to their respective licenses.

Unless otherwise stated, the provisions described in MW-SLA-1J and 1E apply.


## Release Notes

### MWSTAGE2022_08 (8/1/2022)

* Modified TWELITE STAGE APP manual.

#### TWELITE APPS
##### App_CUE/App_CUE_OTA v1.1.1
* Fixed unintended output of PALID in MOTION SENSE PAL mode (single shot).

##### App_ARIA/App_ARIA_OTA v1.1.1
* Fixed an anomalous value when the measured temperature reaches 42°C or higher.

##### App_PAL v1.5.2
* Fixed unintended output of PALID in MOTION SENSE PAL (single shot).
* Fixed an anomalous value when the measured temperature reaches 42°C or higher. (This is a modification for TWELITE ARIA. There are no modifications to the AMBIENT SENSE PAL program.)

##### App_PAL v1.2.1
* Fixed a problem in which the output of App_PAL data sometimes resulted in unintended values.

##### App_Uart v1.4.7
* Changed default retransmission count to 2 times.

#### Act_extras
##### ActEx_AQM0802
  * Supported MWX stream output.
  * Supported AQM1602 (16x2 row LCD).
  * Added of a contrast adjustment function.

### MWSTAGE2022_07 (7/12/2022)
* Addition of flask_wsns_db
  * Sample script to access the database created by Graph Viewer in the TWELITE STAGE APP written in Python
    * This sample converts data into tables and graphs that can be viewed in a web browser
* Modified documentation for TWELITE STAGE APP, MWX library, and TWELITE NET C library to be included in this SDK

#### TWELITE STAGE APP v1.3.8
* Added ability to draw real-time graphs for TWELITE CUE
* Added ability to save sensor data received from TWELITE ARIA, TWELITE PAL, and TWELITE CUE to SQLite before graphing
* Increased window size to 640x480
* Added ability to specify window position at startup
* Changed to display local HTML when help is selected
* Added partial support for English
* Changed menu structure

#### MWX Library v0.2.0
* Changed wire object to allocate memory to heap area
* Changed function name from G_OCTET() to G_BYTE() to avoid name conflict in utils.h
* Changed the order of vAHI_DioInterruptEnable() in attachIntDio()
* Added the_twelite.network2 to support universal receivers (NWK_LAYERED, NWK_SIMPLE or networkless packets in the same executable code)
* Added NWK_LAYERED (only Parent Node reception is supported at this time)
* Introduced MWX_Set_Usder_App_Ver() function to set application version at MWX initialization
* mwx::pnew() was added to simplify description of deployment new
* added support for EASTL
* added new[] operator for EASTL
* Precompiled most of the MWX source code to speed up compilation
* Fixed DIO events being passed to unrelated ports

#### TWELITE APPS.
* Added English translation to 000desc.txt
* Changed Samp_PingPong and Samp_I2C to newly adopted deprecated

##### CUE APPS (App_CUE/App_CUE_OTA) v1.1.0
* Added ability to send one sample of acceleration at each specified interval
* Addition of function to send average, minimum and maximum values of acceleration measured for a certain period of time

##### Aria App (App_ARIA/App_ARIA_OTA) v1.1.0
* Added the ability to multiply temperature and humidity measurements by an offset and a factor

##### Pal App (App_PAL) v1.5.1
* Added ability to send acceleration one sample at a time with motion sensor PAL
* Added ability to send average, minimum and maximum values of acceleration measured for a certain period of time in motion sensor PAL
* Added the ability to multiply the measured values of temperature and humidity by an offset and a factor in the environment sensor pal 

##### Parent Node and Repeater Application (App_PAL) v1.2.0
* Add function to output average, maximum and minimum values of acceleration

#### Act_samples
* Added English translation to 000desc.txt
* Add BRD_I2C_TEMPHUMID
  * Sample using I2C sensor device to measure and transmit from periodic wake-up

#### Act_extras
* Added English translation to 000desc.txt
* Added ActEx_AQM0802
  * Sample using LCD module AQM0802 implementing Sitronix I2C connected LCD controller ST7032

