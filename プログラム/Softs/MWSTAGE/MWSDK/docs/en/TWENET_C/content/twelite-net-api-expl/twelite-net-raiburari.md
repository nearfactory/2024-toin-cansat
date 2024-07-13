# TWELITE NET  library structure

The user application controls the radio module via the TWELITE NET (TWENET) library and peripheral APIs. the TWELITE NET library is divided into three parts.

![fig. TWENET library stack (ペリフェラル=Peripherals, ユーザアプリケーション=user applications)](<../.gitbook/assets/image (7).png>)

### libTWENET\_{ARCH}.a

Application loop definition, event handling, wireless network handling, etc.

### libTWENETExt\_{ARCH}.a

Starting process, interface to MAC layer, etc.

### libTWENETUtils\_{ARCH}.a

General algorithms and procedures for peripherals.

(The source is provided in the SDK: `MWSDK/TWENET/???? /TWENETUtils/Source`, where"????" is the name of the folder corresponding to the library version)



{% hint style="info" %}
`{ARCH}` is the name of the TWELITE model, e.g. `BLUE`, `RED`.
{% endhint %}
