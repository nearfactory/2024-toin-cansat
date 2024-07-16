# folder structure

TWELITE SDK is prepared to store source code and other project files in a predetermined location based on the installation folder.

{% hint style="danger" %}
folder names and file names, including the SDK installation folder, should be alphanumeric characters `a-z A-Z 0-9 _ -` and a period separating the extension . Do not include spaces, other special symbols, or multi-byte character strings (e.g., Japanese).
{% endhint %}

| folder                                         | content                                        |
| ------------------------------------------------- | ---------------------------------------------- |
| `.../MWSDK`                                       | The top-level folder of the TWELITE SDK.    |
| `.../MWSDK/ChipLib`                               | Microcontoroller Library                       |
| `.../MWSDK/License`                               | Mono-Wireless Software Specification Agreement |
| `.../MWSDK/MkFiles`                               | makefile library                               |
| `.../MWSDK/Tools`                                 | toolchain, programming tools, etc.             |
| `.../MWSDK/TWENET`                                | TWELITE NET library                            |
| `    .../????_libTWENET`                          | libTWENETUtils souce codes                     |
| `.../MWSDL/Wks_TWELITEE, Act_samples, Act_extras` | workspace for apps, samples.                   |

###  Workspaces

{% hint style="danger" %}
The folder where the workspace is stored must be directly under the MWSDK, and the relative folder of the Makefile (described below) must not be changed.

Note: In MWSDK2021\_09 and later, projects in other project folders can also run make, but you need to set MWSDK_ROOT.

`.../MWSDK/{workspace dir}/{project dir}/Build/Makefile `
{% endhint %}

The workspace for building programs contains multiple projects, each of which is stored in Wks_TWELITE and builds a separate application.。

```
.../MWSDK/Wks_TWELITE/ProjectName1/...
.../MWSDK/Wks_TWELITE/ProjectName2/...
...
```

The structure in the project is as follows: (Configuration varies by projec)

```
.../ProjectNameX/
       build/          : Storage location for generated binaries
           Makefile    : Generation rules
```

* Some sample projects may have a more complex structure. For example, multiple configurations, such as parent and child devices, may be maintained in a project.



