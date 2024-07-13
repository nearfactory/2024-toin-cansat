# Install TWELITE SDK (MWSDK)

## TWELITE STAGE (MWSDK2020-04 or later)

{% hint style="success" %}
TWELITE STAGE (MWSDK2020-04 or later) extracts the distribution zip archive to a folder that does <mark style="color:red;">**not contain Japanese or whitespace characters**</mark>.

If you do not want to use TWELITE STAGE for the build, but want to use the make command, please refer [here](https://mwx.twelite.info/install_n_build/install_sdk) and set the MWSDK_ROOT environment variable.
{% endhint %}



## MWSDK2018-05

{% hint style="warning" %}
The TWELITE SDK can be run on Windows, Linux, and OS X. However, there may be differences in the descriptions in this document depending on the environment. It also requires knowledge of the command line.

Reference Environment:

* Windows7, Windows10
* Linux Ubuntu 16.04 TLS(32bit), 18.04 TLS(64bit)
* macOS High Sierra 10.13.4
{% endhint %}



To install the TWELITE SDK, follow the steps below.

1. Extracting a ZIP archive
2. Install VSCode

## Extracting a ZIP archive

Extract the TWELITE SDK archive.

{% hint style="danger" %}
Avoid the following folders.

* deep folder
* folders that contain Japanese names or spaces
{% endhint %}

{% hint style="info" %}
In this document, installation on Windows is described as `C:\MWSDK`.
{% endhint %}



Once the ZIP archive is extracted, the command line tools necessary for building will be available, but here we will introduce VSCode as a development environment.

## Other required installation

#### Windows
For Windows, the necessary tools are included in the SDK archiv.

#### macOS
If you get an error when executing make on the command line (bash), install XCode.

```bash
$ xcode-select --install
```

After the installation is complete, type make and if you get the following message, you are good to go.

```bash
$ make
make: *** No targets specified and no makefile found.  Stop.
```

#### Linux
For 64bit Linux, you need to install a 32bit runtime environment.

```bash
$ sudo apt-get install lib32z1
```

Installing VSCode

You can use [VSCode](https://code.visualstudio.com) to edit the VSCode source code and invoke make; the build project that comes with TWELITE SDK includes a configuration definition file for working with VSCode.

{% hint style="danger" %}
This support does not deal with inquiries on how to install or use VSCode. Please refer to the information available in the public.
{% endhint %}

VSCode allows you to do the following

* Editing the source code
* Connection to GIT (for customers who perform their own source control on GIT)
* intellisense based on source code interpretation (\*not all definitions are guaranteed to be interpreted correctly)
* Task definition for calling make to do the build

VSCode can be obtained from the link.

{% embed url="https://code.visualstudio.com" %}

### Installing plug-ins

To enable Visual Studio Code to interpret C/C++ language descriptions, install the plugin.

* C/C++ for Visual Studio Code

The installation procedure is as follows.

![Select "View" → "Extensions" from the Visual Studio Code menu bar.](<../.gitbook/assets/image (38).png>)

![Type "C/C++" in the search bar and click "C/C++".](<../.gitbook/assets/image (39).png>)

![Click the Install button shown on the right.](<../.gitbook/assets/image (40).png>)

![Click Reload to complete the installation of the plug-in.](<../.gitbook/assets/image (42).png>)

