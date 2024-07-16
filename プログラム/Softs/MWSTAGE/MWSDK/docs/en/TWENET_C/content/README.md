# TWELITE SDK (MWSDK) manual

The TWELITE SDK provides an environment for building firmware for the TWELITE wireless modules manufactured and sold by MonoWireless, Inc. It is sometimes referred to as MWSDK in this document.

{% hint style="info" %}
If you want to rewrite or program your application with TWELITE, please read the following first.
{% endhint %}

|               | details                                                                                                                                                                          |
| ------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| TWELITE STAGE | You can build, write, and view (terminal, simple monitor, etc.) in a single application.→<a href="https://mono-wireless.com/jp/products/stage/"> TWELITE STAGE</a> |
| MWX Library   | A C++ library for simpler programming that defines `setup()` and `loop()`.→ <a href="https://mwx.twelite.info">MWX Library</a>               |



The TWELITE SDK (MWSDK) includes the followings:

* Tools for compiling (gcc, make, etc.)
* TWELITE NET Library
  * Includes low-level libraries such as microcontrollers
  * the mwx library
  * more libraries
* TWELITE SDK manual (this document)
  * Explanation of the MWSDK folder and MWSDK in general.
  * How to rewrite the application or build it from source code without using TWELITE STAGE.
  * Details on how to program the firmware.
  * Explanation of wireless packets and relay networks.
  * Description of the API of the TWELITE NET C library
    * Explanation of internal processing such as event loops.



### About the contents of this document

The terms related to the following are not specifically explained in the text.

* About microcomputer development in C language
* Use of command lines such as bash, make, etc.
* Using the gcc toolchain



### Notation in this document

* Use `/` (slash) as the folder (path) separator; on Windows it is `\` (backslash or Yen mark), but replace it.
* In some cases, two `\` characters (a backslash and a space) are inserted in the table. This is to encourage line breaks during display. (For example, `veryVeryLong DefName` should be replaced with `veryVeryLongDefName`.

