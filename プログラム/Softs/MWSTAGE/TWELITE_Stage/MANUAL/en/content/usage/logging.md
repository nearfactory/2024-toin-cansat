---
title: "Logging Function"
author: "Mono Wireless Inc."
description: "Logging Function"
---
# Logging Function

`Windows` `macOS` `Linux` `RasPi` 

TWELITE Allows logging of serial communications with a wireless microcontroller.

### Start Logging 

`Windows` `macOS` `Linux` `RasPi`

Allows logging of serial communications with the TWELITE radio microcontroller.

Press Alt(Cmd)+L key.

![](<../.gitbook/assets/img_help_ovrly.png>)



### Ending logging

Press Alt(Cmd) + L again during logging.

![](<../.gitbook/assets/img_logging_fin.png>)

Logging will finish and the log file will be opened in the OS standard way (Notepad for Windows, Console for macOS).

RaspberryPi only saves the log file and does not have the ability to open it.



### Logging

String received from the TWELITE radio microcontroller → Logged as is

TWELITE Character string sent to the wireless microcontroller → Each character is enclosed by `" " ` for Windows, `" " ` for macOS/Linux/RaspBerryPi. "`" ` to record the characters.

For example, `"t"` means you entered `t` from the keyboard.


### Logging folder and file name

`Windows` `macOS` `Linux` `RasPi`

The file will be saved in `{Folder with TWELITE STAGE APP executable}/log` with a file name based on the date and time when the logging started.

Press `Alt(Cmd)`+`Shift`+`L` to open that folder.

![](<../.gitbook/assets/img_logging_folder.png>)



