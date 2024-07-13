# about Version.mk

{% hint style="success" %}
In the latest MWSDK (TWELITE Stage), the definition corresponding to Version.mk is in `build/makefile`.
{% endhint %}

The Version.mk file is stored in the Source folder of each target, is referenced by the Makefile, and is used to determine the version of the output file name, etc.

You can specify this as follows:

```
VERSION_MAIN = 1 	← Main version(0-255)
VERSION_SUB  = 3	← Sub version(0-255)
VERSION_VAR  = 10	← Variation(0-255)
```

The Makefile adds the following compilation options from Version.mk.

In the above example, -DVERSION_MAIN=1 -DVERSION_SUB=3 -DVERSION_VAR=10 is added at compile time.

{% hint style="warning" %}
Version.h was generated until SDK 2013/11, but is now omitted. If you have included Version.h in your source code, please make it an empty file or do not include it.
{% endhint %}

