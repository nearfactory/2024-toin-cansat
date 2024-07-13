# How to build from the command line

## How to build by command line

{% hint style="info" %}
When using VSCode, the command line is called from within VSCode, so you do not need to be aware of the following steps.
{% endhint %}

### preparation in advance

Make sure you have the command line make utility ready to run.

{% hint style="warning" %}
The following explanation assumes that the bash command line is used.
{% endhint %}

#### Windows
Use the BASH (by msys) attached to the SDK.

Run the BASH.CMD located in {MWSDK installation folder} from Explorer, and you will see the bash command line with the Wks_TWELITE workspace as the current folder. Set the PATH to the toolchain for the TWELITE SDK.

```bash
user1@MACHINE_NAME   /c/MWSDK/Wks_TWELITE
$ pwd
/c/MWSDK/Wks_TWELITE

$ ls
App_IO  App_Tag  Samp_PER  Samp_Wayback
App_Melody  
...
```

#### macOS
If an error occurs when executing make on the command line (bash), install XCode.

```c
$ xcode-select --install
```

After installation is complete, type "make" and the following message should appear.

```c
$ make
make: *** No targets specified and no makefile found.  Stop.
```

### Build

Let's build Samp_PingPong as an example.

First, go to the build folder of the build project Samp_PingPong.

```bash
$ pwd
/c/MWSDK/Wks_TWELITE
$ cd Samp_PingPong/PingPong/Build/
$ pwd
/c/MWSDK/Wks_TWELITE/Samp_PingPong/PingPong/Build/
```



The next step is to run clean. Here is an example of running clean on a TWELITE BLUE build.

```
$ make TWELITE=BLUE clean
../../../../MkFiles/chipsel.mk:36: "WARNING: TWELITE BLUE"

```

{% hint style="info" %}
When running make, you must specify either TWELITE=BLUE or TWELITE=RED, and you will get a WARNING to indicate that this is a TWELITE BLUE build, not a problem. To clean the Build folder without running make, delete the .bin file and objs_\* folder in the Build folder.
{% endhint %}



Once clean is done, run the build.

```bash
$ make TWELITE=BLUE
rm -f objs_BLUE/PingPong.o
mkdir -p objs_BLUE
Compiling /c/MWSDK/Wks_TWELITE/Samp_PingPong/Pi..
...
/c/MWSDK/ChipLib/SW4063V1416/../../Tools/ba-elf-ba2-r..
y -S -O binary objs_BLUE/Samp_PingPong_PingPong_BLUE_..
Samp_PingPong_PingPong_BLUE_L1200_V0-1-4.bin

$ ls *.bin
Samp_PingPong_PingPong_BLUE_L1200_V0-1-4.bin
```

{% hint style="info" %}
If the [`.bin`](birudonitsuite/binfuiru.md) file is created after the build is executed, it is successful. If the build does not succeed, you will get some error messages.

The intermediate files are stored in the `objs_{ARCH}` folder.
{% endhint %}

