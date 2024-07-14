---
title: "Glancer"
author: "Mono Wireless Inc."
description: "Glancer"
---
# Glancer


`Windows` `macOS` `Linux` `RasPi`

Glancer is an English word for a person who glances, and it is a simple display of [information in incoming messages](https://wings.twelite.info/how-to-use/parent-mode/receive-message). TWELITE Radio App\_Wings is written in the microcontroller, and the communication partner (App\_Twelite, TWELITE PAL, ...) is set to the same channel as the application ID. Application ID and CHANNEL can be mixed as long as they match).

The screen consists of two types of screens.

####

#### List view

![](<../../../../../.gitbook/assets/img_viewer_glancer_sel.png>)

Enumerates information from the communication partner. The contents displayed are (message type, logical ID, serial ID, LQI(Lq), supply voltage (if included in the information), and reception time).

| Operation        | Desc.                                  |
| ---------- | ------------------------------------ |
| `[ A ]`     | Move to the previous item on the list.                        |
| `[ A ]` long press | Exit this screen and return to the previous menu.                     |
| `[ B ]`     | Shifts to selection view.                          |
| `[ B ]` long press | Sorts items. Each time the sort is performed, the sort key changes sequentially. |
| `[ C ]`     | Move to the next item on the list.                        |
| `[ C ]` long press | Controls and resets the reset pin of the TWELITE wireless module.  |
| ESC ESC    | Exit this screen by entering the ESC key.             |



#### Selection view

![](<../../../../../.gitbook/assets/img_viewer_glancer_nodes.png>)

This screen is accessed by moving an item in the list view and making a selection when the item is highlighted. Displays information on specific communication partners in order of arrival.

The number of packets received and the average LQI value since the selection is made are displayed.

| Operation         | Desc.                                    |
| ---------- | ----------------------------------- |
| `[ A ]`     | No assignment                                |
| `[ A ]` long press | Exit this screen and return to the list view.                    |
| `[ B ]`     | No assignment                                |
| `[ B ]` long press | No assignment                                |
| `[ C ]`     | No assignment                                |
| `[ C ]` long press | Controls and resets the reset pin of the TWELITE wireless module. |
| ESC        | Exit this screen and return to the list view.                    |

