---
title: "Build definition Makefile"
author: "Mono Wireless Inc."
description: "Build definition Makefile"
---
# Build definition Makefile

The Makefile is stored in build/Makefile and is pre-defined to build the act by running the make command.

{% hint style="info" %}
**MWSDK 2020-04 automatically detects the .cpp file in the project folder, so there is usually no need to modify the Makefile.**

**If the source file is to be stored in a subfolder, it will need to be edited.**

**MWSDK 2019-12 requires you to edit the Makefile if you have more than one .cpp file.**
{% endhint %}

{% hint style="warning" %}
After copying the project folder from another environment, make sure to delete the `build/objs_???` folder. If any intermediate files from the other environment remain, make will fail.

`(MWSDK 2020-04)` You can avoid errors by adding USE\_APPDEPS=0 to clean and then running make again.

`$ make USE_APPDEPS=0 TWELITE=BLUE clean`\
`...`\
`$ make TWELITE=BLUE`

``
{% endhint %}

## Parameters for make.

### TWELITE=

Specify the build target as BLUE or RED; for TWELITE BLUE, use `make TWELITE=BLUE`.

### all

Run the build. Usually, you can omit this and use `make TWELITE=BLUE`.



### clean

Remove intermediate files from the build. Do this as `make TWELITE=BLUE clean`.



### cleanall

Remove all intermediate files. Do this as `make cleanall`, the same as removing all of the `objs_???` folder in the build folder.



### USE\_APPDEPS=0 or 1

When set to 1 (the default), the build file is determined based on file dependencies. For example, if there is a change in a header file, the associated source file will be recompiled.

If set to 0, the makefile will not error if there are inconsistent intermediate files left.



## Makefile definition

Depending on the size of the act, and when defining behaviours, the source files are usually split and built separately.

One of the build files is {project folder name.cpp}.

If you want to define other files, edit the `build/Makefile` in your project folder.

```perl
##############################################################################
# Copyright (C) 2019 Mono Wireless Inc. All Rights Reserved.
# Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE
# AGREEMENT). 
##############################################################################
# USER PROJECT BUILD DEFINITION.
##############################################################################

#####################################################################
### set TWELITE model
TWELITE ?= BLUE
#TWELITE = RED

#####################################################################
### set application version (MUST SET THIS.)
VERSION_MAIN = 0
VERSION_SUB  = 1
VERSION_VAR  = 0

#####################################################################
### set an additional source file
###   the default file name is dirname.

### for C++ files compiled with g++ (must have .cpp suffix)
APPSRC_CXX += myAppBhvParent.cpp
APPSRC_CXX += myAppBhvParent-handlers.cpp
APPSRC_CXX += myAppBhvChild.cpp
APPSRC_CXX += myAppBhvChild-handlers.cpp

### for C files compiled with gcc (must have .c suffix)
#APPSRC += my_c_file.c

### Additional Src/Include Path
# if set, find source files from given dirs.
#
APP_COMMON_SRC_DIR_ADD1 = ../Parent
APP_COMMON_SRC_DIR_ADD2 = ../Child
#APP_COMMON_SRC_DIR_ADD3 = 
#APP_COMMON_SRC_DIR_ADD4 =

#####################################################################
### set misc option for compiler

### C++ flags passed to g++
# e.g. CXXFLAGS += -DMY_DEFS
#CXXFLAGS +=

### C++/C flags passed to g++/gcc
# e.g. CFLAGS += -DMY_DEFS
#CFLAGS +=

### include opts
# e.g. INCFLAGS += -I../my_common_src/
#INCFLAGS +=

### optimize flag (default is -Os, normally no need to change)
#OPTFLAG=-O2

#####################################################################
### must include mwx.mk (the makefile body part.)
MWSDK_PATH?=$(realpath $(MWSDK_ROOT))
include $(MWSDK_PATH)/MkFiles/mwx.mk
#####################################################################
```

The above is an example Makefile with sample PAL\_AMB-bhv.



### VERSION\_???

```perl
### set application version (MUST SET THIS.)
VERSION_MAIN = 0
VERSION_SUB  = 1
VERSION_VAR  = 0
```

Specify the version number. This will be reflected in the build result file name.

During compilation, it is passed as a definition like `-DVERSION_MAIN=0` `-DVERSION_SUB=1` `-DVERSION_VAR=0`.



### Adding source files

{% hint style="success" %}
`(MWSDK 2020-04)` If you do not place files in subfolders, you no longer need to specify additions. All .c .cpp files in the project file will be added.&#x20;
{% endhint %}

When you add a source file, you need `APPSRC_CXX` and `APP_COMMON_SRC_DIR_ADD?`.

{% hint style="info" %}
If you place source files in a subfolder, you must specify the folder `APP_COMMON_SRC_DIR_ADD?`.
{% endhint %}



Append the name of the source file to \`APPSRC\_CXX'. This file name must **not** contain **a folder name**. Anything in a subfolder should also be specified without a folder (i.e. if the same filename is in a subfolder, the build will fail)

```perl
APPSRC_CXX += myAppBhvParent.cpp
APPSRC_CXX += myAppBhvParent-handlers.cpp
APPSRC_CXX += myAppBhvChild.cpp
APPSRC_CXX += myAppBhvChild-handlers.cpp
```



Next, specify the search path if the source files are stored in a location other than the project folder. You can set up to **four**.

```perl
APP_COMMON_SRC_DIR_ADD1 = ../Parent
APP_COMMON_SRC_DIR_ADD2 = ../Child
```

The folder specification is relative to the Makefile.



### Compile and linker options

A number of other options can be passed to the compiler linker.

| Designation | Remarks                                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------------------- |
| `CXXFLAGS`  | Specify compilation options for C++ source files.                                                           |
| `CFLAGS`    | Specify compile options for C/C++ source files.                                                             |
| `INCFLAGS`  | Specify the include file for the header file.                                                               |
| `OPTFLAGS`  | Define this if you have a special reason for wanting to apply a compile option other than `-Os`.            |
| `LDFLAGS`   | Specify linker options. (This is not mentioned in the comments of the Makefile above, but can be specified) |

