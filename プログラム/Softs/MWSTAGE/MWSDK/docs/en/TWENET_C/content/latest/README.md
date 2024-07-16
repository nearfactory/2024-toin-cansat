# Get the latest version

## TWELITE STAGE

{% hint style="info" %}
TWELITE NET 1.3.0 (2020/04) and later will be distributed as part of TWELITE STAGE.

```
*** TWELITE STAGE folder Structure ***
{STAGE TO FOLDER}/
  TWELITE_Stage.exe            -- STAGE executable (Windows)
  dll/                         -- for STAGE exe (Windows)
  TWELITE_Stage/               -- STAGE related files
  BIN/                         -- stores .bin files
  
  MWSDK/                       -- TWELITE SDK(MWSDK) top dir.
```
{% endhint %}



### release version

Please download the release version from the following link. The MWSDK will be part of the TWELITE STAGE distribution.

{% embed url="https://mono-wireless.com/jp/products/stage/index.html" %}



## official release

{% hint style="info" %}
The official release will have a name. The format of the name is `MWSTAGE` followed by `{year}_{month}`, as in `MWSTAGE2020_05`. Versions of the TWELITE SDK (MWSDK) that do not include the TWELITE Stage tool are likewise named with the MWSDK followed by `{year}_{month}`, as in `MWSDK2020_05`.

If the release name is accompanied by UNOFFICIAL, it is named for "updates between releases".
{% endhint %}

It is distributed as follows.

> [https://mono-wireless.com/stage](https://mono-wireless.com/stage)



## Update between releases

Any problems found after the release, or updates to samples, etc., will be reflected in the GitHub repository (a database that stores source code and files, including their history) below.

* [MWSDK_COMMON](https://github.com/monowireless/MWSDK_COMMON) - A repository of common parts of the MWSDK. It uses submodules, so you need to be familiar with Git.
* [MWSDK_COMMON_SNAP](https://github.com/monowireless/MWSDK_COMMON_SNAP) - This is a repository of the common parts of the MWSDK that can be downloaded in zip format.

{% hint style="warning" %}
These modifications may include intermediate content, or omissions of predetermined behavior checks. There may be unconfirmed side effects or inconsistencies between configurations. Please judge whether to use them by yourself.

As a rule, we do not provide support for fixes after the release. Please ask for technical support based on the released version.

In addition, if you find any inconvenience with the fixes after the release, please contact our technical support.

Our support team cannot answer questions about how to use Git or GitHub. We recommend that you refer to general instructions. Also, we may not be able to help you with GitHub services.
{% endhint %}



### MWSDK_COMMON_SNAP repository

> [https://github.com/monowireless/MWSDK_COMMON_SNAP/releases](https://github.com/monowireless/MWSDK_COMMON_SNAP/releases)

To update the MWSDK, download the zip file from this repository, and replace the folder and files with the ones before the update.



The links include links to the complete set of files in each release (the preliminary version is UNOFFICIAL).

![GitHub画面例](<../.gitbook/assets/image (55).png>)

The repository contains the following folders:

* ChipLib
* LICENSE
* MkFiles
* TWENET

**Replace** these four folders in the TWELITE STAGE distribution folder。

```
.../MWSTAGE/              --- TWELITE STAGE distribution folder
        .../MWSDK         --- MWSDKfolder
              .../ChipLib <-- Replace the following four folders
              .../LICENSE
              .../MkFiles
              .../TWENET
```



### MWSDK_COMMON repository

> [https://github.com/monowireless/MWSDK_COMMON](https://github.com/monowireless/MWSDK_COMMON)

This repository manages the files of the common parts of MWSDK, but while the MWSDK_COMMON_SNAP described above focuses only on file differences per release, the repositories of each library, sample, etc. are sub-modules, and the update history can be referenced individually.

{% hint style="warning" %}
If you download a zip file, the files will not be stored in the folder that corresponds to the submodule. (This is a GitHub specification).
{% endhint %}



#### About Repository Use

* GIT submodule is used; if you download an archive from GitHub site in .zip format or other format, the files containing submodule will not be included. (See below for an example of git command to clone with submodule)
* The MWSDK_COMMON repository is the distributed MWSDK folder without the Tools folder and some files (the cmd file for registering environment variables in the Windows version).



#### How to clone from the repository (MWSDK 2020/04 or later)

In this section, we will describe how to clone the contents of MWSDK from GitHub using the command line. Please prepare an environment where you can use the git command beforehand, or use bash if you are using Windows.



The first step is to extract the MWSDK (TWELITE STAGE) from the distribution. It will have the following structure.

```
=== folder structure ===
.../MWSTAGE +- MWSDK +- TWENET
                      - ChipLib
                      - ...
                      - Tools               
```



Rename the MWSDK folder at the time of distribution to something else.

```
$ cd .../MWSTAGE    ...is the customer's installation folder
$ mv MWSDK MWSDK.orig

=== folder structure ===
.../MWSTAGE +- MWSDK.ORIG
                  +- TWENET
                   - ...
                   - Tools 
```



In the MWSTAGE folder, clone MWSDK_COMMON from GitHub, and add the `--recurse-submodules` option to retrieve the submodules files.

```
$ pwd
.../MWSTAGE
$ git clone --recurse-submodules https://github.com/monowireless/MWSDK_COMMON MWSDK
...(It'll take a while.)

=== folder structure ===
.../MWSTAGE +- MWSDK
                  +- TWENET
                   - ... (Tools is not included)
             - MWSDK.ORIG
                  +- TWENET
                   - ...
                   - Tools 
```



Finally, transplant the `MWSDK.ORIG/Tools` folder to MWSDK.

```
$ mv MWSDK.ORIG/Tools MWSDK/

=== フォルダ構成 ===
.../MWSTAGE +- MWSDK
                  +- TWENET
                   - ...
                   - Tools 
             - MWSDK.ORIG
                  +- TWENET
                   - ... (Tools is not included because it was mv'd.)
```



## TWELITE MWX library SDK (2019/12)

This is the first version of the SDK dedicated to the MWX library.

{% hint style="warning" %}
For development with MWX, please use the latest version of TWELITE STAGE.
{% endhint %}



## Before TWELITE NET 1.2.0 (2018/05) 

See the link below.

{% embed url="https://mono-wireless.com/jp/products/TWE-NET/index.html" %}

