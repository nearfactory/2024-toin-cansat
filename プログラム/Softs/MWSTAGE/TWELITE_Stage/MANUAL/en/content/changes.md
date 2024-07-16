---
title: "Revision History"
author: "Mono Wireless Inc."
description: revision history
---

# revision history

For the revision history of the source code, please refer to [https://mwm5.twelite.info/changes](https://mwm5.twelite.info/changes) and　[https://github.com/monowireless/mwm5](https://github.com/monowireless/mwm5).

The latest version of the revision history may not match the distributed version for each platform.


## 1.3 MWSTAGE2022\_07 recorded version
Major version upgrade.

* Changed internal drawing resolution from 320x240 to 640x480 pixels
* Added real-time graphs for accelerometer
* Added sensor graph to save and graph sensor data
* Added support for English display
* Changed main manual to local html file

## 1.0.8 MWSTAGE2021\_09 recorded version.

* `[ A ]` `[ B ]` `[ C ]` buttons sometimes remained out of pointer
* CRLF is now sent to TWELITE on Enter input in STAGE APP.
* updated FTDI library on Mac to work on Apple Silicon(M1) without using serial intermediary program.
* added setting the PATH of msys tools on Windows to prevent unexpected make from being invoked.
* added support for moving to the write screen even when TWELITE is not connected (by entering B,R keys and specifying the target TWELITE model).
* When VSCode is used, the screen for writing .bin files under build/ is opened instead of executing build when Act or TweApps is selected. (Build is performed from VSCode)
* Set some environment variables internally and made VSCode started from TWELITE STAGE refer to these variables so that VSCode can execute appropriate builds and refer to appropriate library sources for VSCode's code interpretation
* Sample code is stored under the MWSDK folder, but by dropping the build target folder, build and write operations can be performed in folders other than the MWSDK folder (folder names must not contain spaces, Japanese characters, etc.)
* The internal folder settings and environment variable settings are now displayed on the console screen at startup.
* STAGE APP is terminated after waiting 1 second when terminating.

## 1.0.7pre2

* Enhanced support for Raspberry Pi (1.0.7pre2)
  * Added serial0 support (TWELITE STAGE HAT)
  * Added build for Zero (disable build & draw fade function in supported libraries)
  * Added build for X11 desktop
* Added support for general FTDI devices (FT232, FT230). Firmware write mode must be done manually
* Added the function to display the COM port assigned by Windows by pressing `c` key on the serial port selection screen on Windows.
* added the ability to change the baud rate from 115200bps
* Added a command line option (`-E 0`) to disable the drawing fade function.

## 1.0.3 MWSTAGE2020\_12 recorded version

* TWELITE CUE support (parser and CUE viewer)
* Verify (compare) when writing in the rewrite menu.
* Apple Silicon tentative support (`TWELITE_Stage.command` is universal app, external command `sersrv_ftdi.command` for serial processing, Tools rebuilds intel binary that can work with Rosetta2, serial communication is slower because it is via an external command)
* Moved `MWSTAGE/MWSDK/Tools` to `MWSTAGE/Tools` in the folder structure. (To make MWSDK available as it is in MWSDK\_COMMON repository)
* TWELITE\_Stage.ini (remove extension from startup filename and add .ini) is now loaded at startup and the MWSDK folder can be selected. (It is now possible to easily switch between older versions of the library set.)
* Changed SDL2 library for screen rendering to 2.0.12 (Windows, MacOS, RaspberryPi).
* The DLL file is no longer necessary for Windows as a static build.
* The number of parallel builds by make -j is now (number of physical CPUs - 1).
* reopen serial port explicitly in some places of rewrite menu. added support for easy recovery from USB disconnection when a device is unplugged or plugged in.
* added support for opening the folder `TWENET/usever.mk` when opening mwx or twesettings by pressing Alt(Cmd)+Shift+m, t.

    　The following changes have been made.
* [Raspberry Pi] Fixed a problem that write menu transition at /dev/serial0 fails at the first boot.

### Known Issues

* Help message may not appear when Alt(Cmd) is pressed down at startup; it appears by typing Alt(Cmd)+0.
* In the rewrite menu, when the file name is too long, the line may be disrupted.
* We have not fully tested the operation on Apple Silicon.

## 0.9.11 MWSTAGE2020\_10, Raspberry Pi version (tentative)

(* This version is not comprehensively verified.)

* Operation on Raspberry Pi
* Other functional adjustments

## 0.9.9 - MWSTAGE202020_10 included version

* Added \[Web] button to the top level menu as well, so that the related links can be opened in the browser.
* Implemented the function to open in Folder, Web and VS Code of Linux version.
* It was sometimes difficult to transition to the write menu when the TWELITE radio module was frequently outputting UART.

## 1.0.3 MWSTAGE2020\_12 version

* TWELITE CUE support (parser and CUE viewer)
* Verify (compare) when writing in the rewrite menu.
* Apple Silicon tentative support (`TWELITE_Stage.command` is universal app, external command `sersrv_ftdi.command` for serial processing, Tools rebuilds intel binary that can work with Rosetta2, serial communication is slower because it is via an external command)
* Moved `MWSTAGE/MWSDK/Tools` to `MWSTAGE/Tools` in the folder structure. (To make MWSDK available as it is in MWSDK\_COMMON repository)
* TWELITE\_Stage.ini (remove extension from startup filename and add .ini) is now loaded at startup and the MWSDK folder can be selected. (It is now possible to easily switch between older versions of the library set.)
* Changed SDL2 library for screen rendering to 2.0.12 (Windows, MacOS, RaspberryPi).
* The DLL file is no longer necessary for Windows as a static build.
* The number of parallel builds by make -j is now (number of physical CPUs - 1).
* reopen serial port explicitly in some places of rewrite menu. added support for easy recovery from USB disconnection when a device is plugged or unplugged.

### Known Issues.

* Help message may not appear when Alt(Cmd) is pressed down at startup; it appears by typing Alt(Cmd)+0.
* In the rewrite menu, when the file name is too long, the line may be disrupted.
* We have not fully tested the operation on Apple Silicon.

## 0.9.11 MWSTAGE2020\_10, Raspberry Pi version (tentative)

(* This version is not comprehensively verified.)

* Operation on Raspberry Pi
* Other functional adjustments

## 0.9.9 - MWSTAGE202020_10 included version

* Added \[Web] button to the top level menu as well, so that the related links can be opened in the browser.
* Implemented the function to open in Folder, Web and VS Code of Linux version.
* It was sometimes difficult to transition to the write menu when the TWELITE radio module was frequently outputting UART.

## 0.9.8a.

> [https://github.com/monowireless/TWELITE\_STAGE\_Bin\_M5Stack/releases/tag/0.9.8a](https://github.com/monowireless/TWELITE\_STAGE\_Bin\_M5Stack/releases/tag/0.9.8a)

M5Stack version with `MW-SLA-1J,E` / `MW-OSSLA-1J,E` dual license and updated `readme-j.txt`.

## 0.9.8

Added `\[Web]' button to viewer list view, added ability to open related sites, etc.

### Revisions.

* Added Viewer > Commander.
  * Standard App 0x80 command
  * NOTICE PAL LED control (send commands to App\_Wings)
* NOTICE PAL support in Viewer > PAL Viewer.
* Add Act extras menu.
  * More advanced than Act ⌘_samples
  * External open source libraries (e.g. sensor procedures)
* Expanded mouse actions (lists, buttons, tabs)
  * Mouse move to focus, left click to confirm, right click to input \[ESC] key
* Reduced screen display load
  * Disabled screen saver when application is in background
  * Reduced CPU load by reducing the number of drawings when the application is in the background.
* Enhanced the list of build projects (Act, TWE_Apps, Act Extra)
  * Displaying a summary at the bottom when an item is selected (load 000desc.txt file, processed by TWE\_Desc class)
  * Open project folder (or open with VSCode)
  * Open related websites.
  * Open ALt+Shift+m mwx library and Alt+Shift+t twesettings library.
  * Ability to open folders and build error files currently selected in the build menu.
* Added logging (serial port input/output) function.
  * (Alt/Cmd+L) to start/end logging
  * Log files are stored in {folder with {TWELITE\_Stage executable}/log
  * File name is twestage\_{date-time}.log
  * Shift+Alt/Cmd+L to open log file folder
* Other changes/modifications, etc.
  * Changed the display method of serial (FTDI) device names and IDs
  * Fixed the problem that App\_UART did not transition to Interactive settings mode
  * Changed the behavior when dropping a folder (it used to be a binary write, but now it is a menu transition)
  * When the terminal is pressed and held down for a long time, the screen is cleared in addition to resetting by pressing [C].

### Known issues

* M5Stack may hang and initialize settings when saving settings.

## 0.8.9

2020\_05 Release Version.

* Added window icons.
* Relaxed the restriction on the maximum number of listings in the BIN file list screen (win/linux/mac)
* Added Glancer viewer
* Adjusted explanation text, etc.
* Adjusted drawing of console screen.
* The settings for the destination screen (Interactive settings mode or Terminal) after writing the firmware were not working.
* Changed the assignment of Alt(or Cmd)+W
* Fixed other bugs

## 0.8.6

First Linux release

## 0.8.5

First release
