---
title: "000desc.txt"
author: "Mono Wireless Inc."
description: "000desc.txt"
---
# 000desc.txt

If `000desc.txt` is created in the project folder, TWELITE STAGE APP will display its contents in the list of project folders.

![Example of display with 000desc.txt](../.gitbook/assets/img_fimprog_act_sel.png)

The file is written in UTF-8. There are two types of formats.



### Format 1

```text
Turn on LED when switch is pressed
act4 runs an act that turns on an LED when a switch connected to the TWELITE DIP is pressed.
https://mono-wireless.com/jp/products/act/index.html
```

* The first line is the title line.
* The second and subsequent lines are descriptions of the details.
* If the last line starts with http, it is a link to a website.



### Format 2

```text
[JAPANESE]
TITLE=Actのテンプレート
DESC=中身が何もない setup(), loop() のみのファイルです。
新しく Act を記述するのに利用してください。
URL=jp/MWX/content/act_samples/README.html
[ENGLISH]
TITLE=Act empty template
DESC=This act file only contains empty setup() and loop(),
which is intended to write a new Act. 
URL=en/MWX/content/act_samples/README.html
```

The format is like an ini file. The item name starting at the beginning of the line and up to the `=` character is the item definition, and the rest is the content of the item.

| Item Definition | Details |
| :--- | :--- |
| `[JAPANESE]`, `[ENGLISH]` | block delimiter |
| title line | description of details.
| `DESC=` | description of details. Can be multiple lines including line breaks. |
| `URL=` | Link to a website or file |

#### About URL specification

| `URL=` | Details |
| :--- | :--- |
| starting with `https:`, `http:` | open that address |
| other than that | relative folder starting with `{MWSDK_ROOT}/docs/`. If you specify `a/b/c.html`, it will be converted to `{MWSDK_ROOT}/docs/a/b/c.html`. | (default: `a/b/c.html`)
