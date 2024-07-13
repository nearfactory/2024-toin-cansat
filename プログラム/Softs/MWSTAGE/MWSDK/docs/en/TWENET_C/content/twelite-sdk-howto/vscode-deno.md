# Use with VSCode

VSCode has a code interpretation ([IntelliSense](https://code.visualstudio.com/docs/editor/intellisense)) feature that makes code editing more efficient. In addition, the task feature allows you to execute [build commands](vscode-deno.md#apuriwobirudosuru) from VSCode.

## Open the project

In order to build the TWELITE app, you will need to open the project in Visual Studio Code.

The steps to open the project are as follows:

![Select "File" -> "Open Folder" from the menu bar.](<../.gitbook/assets/image (43).png>)

![Select the Preject folder you want to open.](<../.gitbook/assets/image (44).png>)

![The folder of the opened project will be added to the Explorer.](<../.gitbook/assets/image (45).png>)



## Build the app

To build the application, follow the steps below, running the make utility from VSCode. for details on make, refer to "[How to Build](komandoraindenobirudo.md)" and "[About Makefile](birudonitsuite/makefile-nitsuite.md)".

For example, if you want to build a standard TWELITE app, select the VSCode task "Build App_Twelite_Master (BLUE)".

![Select "Tasks" -> "Execute Task" from the menu bar.](<../.gitbook/assets/image (47).png>)

![Select the definition you want to build from the red frame in the figure on the right.](<../.gitbook/assets/image (48).png>)

![If the output from the terminal looks like the red frame in the figure on the right, the build is successful.](<../.gitbook/assets/image (49).png>)

{% hint style="info" %}
Since VSCode executes command line make, command line error messages are also output.
{% endhint %}

## Switch code interpretation

You can switch between code interpretation for TWELITE BLUE and code interpretation for TWELITE RED. To switch between code interpretations, click on the location shown in the figure and select the TWELITE model for your development.

{% hint style="info" %}
Switches the C definition specific to each type of TWELITE module.

It is not guaranteed that the code interpretation and compiler interpretation will match perfectly. In addition, some definitions may not be interpreted due to header file descriptions.
{% endhint %}

![Switch code interpretation.](<../.gitbook/assets/image (46).png>)

{% hint style="info" %}
Switches the C definition specific to each type of TWELITE module.
{% endhint %}

{% hint style="warning" %}
It is not guaranteed that the code interpretation and compiler interpretation will match perfectly. In addition, some definitions may not be interpreted due to header file descriptions.

The header file to be loaded does not directly refer to the header file of the compiler or library, but refers to a copy under `{SDK folder}/Tools/VSCode/include`.
{% endhint %}

## About JSON files

The above build and code interpretation are defined in the JSON file.

### .vscode/c_cpp_properties.json

Define implicit and other definitions at build time, and specify the include folder.

If additional definitions are needed, duplicate or edit the entries for `"name" : "TWELITE_BLUE"` or `"name" : "TWELITE_RED"` in the existing files.

You will need to edit `"name"` `"includePath"` and so on. If you need a specific build definition, add an entry to `"defines"`.

### .vscode/tasks.json

Defining the build task.

If you want to add a new build definition, copy and edit an already existing entry. Each build task has an entry for windows, linux, and osx.

You will need to edit `"label"`, `"options"`, `"cwd"`, and so on.

```bash
"tasks": [
{ 
  "label": "Build Samp_PingPong,PingPong (BLUE)",
  "type": "shell",
  ..
  "windows": {
     "command": "make",
     "args": [ "TWELITE=BLUE",  "clean", "all" ],
     "options": { "env": { "PATH": "${workspaceRoot}\\..\\..\\Tools\\MinGW\\msys\\1.0\\bin" } }
  },
  "problemMatcher": { "base": "$gcc", "fileLocation": [ "absolute" ] },
  "options": { "cwd": "${workspaceRoot}/PingPong/Build" }
},
..

```

{% hint style="info" %}
* The `"options": { "cwd" ... }` setting specifies the build folder.。
* If you want to add a make argument, add an entry after `TWELITE=XXX` in `"args"`.
{% endhint %}

#### Set to an existing build task

Add `group": { "kind": "build", "isDefault": true } `as shown in the following example. The task will become the default build definition and can be executed by \[Task]>\[Execute Build Task] or by using shortcut keys.

```bash
"tasks": [
{ 
  "label": "Build Samp_PingPong,PingPong (BLUE)",
  "type": "shell",
  ..
  "options": { "cwd": "${workspaceRoot}/PingPong/Build"},
  "group": { "kind": "build", "isDefault": true }
},
..
```
