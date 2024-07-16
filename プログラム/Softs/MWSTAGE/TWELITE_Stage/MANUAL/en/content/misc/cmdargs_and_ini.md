---
title: "Command line arguments and ini file."
author: "Mono Wireless Inc."
description: "Command line arguments and ini file."
---

# Command line arguments and ini file.

## command line arguments
Command line arguments provides some minor settings of TWELITE Stage.

| Arg | Desc. |
| --- | ----  |
| `-E 0`      | Disable graphical effect like fading out |
| `-R {type}`   | Set rendering type by `{type}` value: <br>`0`: default<br>`1`: OpenGL<br> `2`: DirectX(Windows) Metal(macOS)<br> `3`: Software. |
| `-J`   | Enable game controller. |
| `-x {x_pos}`, `-y {y_pos}` | Set position of TWELITE STAGE App graphical winodow at launch. The `{x_pos}` and `{y_pos}` is the screen coordinate of window top-left. |


## an ini file.

The ini file is used to set some fundamental settings of TWELITE STAGE (like reference MWSDK folder).

The ini file name is {basename of TWELITE STAGE app executable} + `.ini`, which
is normally `TWELITE_Stage.ini`.

```text
;;; change MWSDK reference.
; MWSDK=MWSDK
MWSDK=MWSDK2020_10

;;; Interface language
LANG=en

;;; Window geometry
;GEOM_X=200
;GEOM_Y=100
```

### Syntax
* The `ini` file is described as a plain text file.
* A key and value is stored in a single line separated with `=` character (e.g. `KEY=value`).
* A key value string shall begin with line head. (No spaces or other character allowed before a key string.)
* No space allowed between the `=` character.
* Comment line is started from `;` or `#` characacter.

### Settings

| Key | Value. |
| --- | ----  |
| `MWSDK` | Change MWSDK folder to another one. The default folder is `MWSDK` at the same folder where TWELITE STAGE App executable is located. If you need to use older or custom MWSDK, you can specify the name of its folder. |
| `LANG`  | `LANG=en` will change the user interface language from default(Japanese) to English. |
| `GEOM_X`, `GEOM_Y` | Change the location of TWELITE STAGE App window is shown |

### different settings of TWELITE STAGE App.
If you need to different setting type of TWELITE STAGE App, just copy
an executable at same folder of the original TWELITE_Stage executable.

e.g. if you needs English interface vesion, you just copy `TWELITE_Stage.exe`(Note: .exe is suffix of Windows executable) to `TWELITE_Stage_en.exe` and create `.ini` file with `LANG=en` setting.
```
  TWELITE_Stage.exe 
  TWELITE_Stage.ini     | no special settings

  TWELITE_Stage_en.exe  | copy of TWELITE_Stage.exe
  TWELITE_Stage_en.ini  | LANG=en is set
```