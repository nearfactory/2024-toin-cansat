---
title: "About Environment Variables"
author: "Mono Wireless Inc."
description: "About Environment Variables"
---

# About Environment Variables

TWELITE STAGE APP internally sets environment variables for build programs such as make. It is not necessary to set environment variables in advance.



### Environment variables set internally

| Environment Variables | Explanation |
| --- | --- |
| `MWSDK_ROOT` | By default, the `MWSDK` folder in the folder where the TWELITE STAGE APP executable is stored (i.e. `. /MWSTAGE/MWSDK`) in the folder containing the TWELITE STAGE APP executable. If `MWSDK.ini` is specified, the specified folder name is adopted.                                                                                                                                                                     |
| `MWSDK_TWENET_LIBSRC` | In the source code folder of the sample code and TWELITE APPS, a definition file for Microsoft's Visual Studio Code (VS Code) is created beforehand. In this definition file, we specify references to library resource code for the purpose of code interpretation in the VS Code editor, using this environment variable. <br />If the `MWSDK_TWENET_LIBSRC` environment variable is properly specified, for better intellisense  interpretation of source codes. |
| `LANG=C` | Explicitly set to make toolchain messages in the specified language (English).    |
| `PATH` | On Windows, add a PATH to the msys utility attached to the SDK |
| `MWSDK_MAKE_JOBS` `MWSDK_MAKE_DISABLE_LTO` | Used in VS Code configuration definition. `JOBS` : pass the number of parallel builds set in STAGE APP `DISABLE_LTO` : disable LTO (`Windows`)| |


#### Reference 1

Example configuration of `.vscode/settings.json` (excerpt)

```
    "C_Cpp.default.includePath": [
        "${env:MWSDK_TWENET_LIBSRC}/include/**",
        "${env:MWSDK_TWENET_LIBSRC}/src/**"
    ],
    "C_Cpp.default.browse.path": [
        "${env:MWSDK_TWENET_LIBSRC}/include/**",
        "${env:MWSDK_TWENET_LIBSRC}/src/**"
    ],
```

The relative PATH definition like with `../..`, `/` is not necessary when opening the project from the TWELITE STAGE application. The TWELITE STAGE will automatically add `MWSDK_TWENET_LIBSRC` environment variable from your project and library location.