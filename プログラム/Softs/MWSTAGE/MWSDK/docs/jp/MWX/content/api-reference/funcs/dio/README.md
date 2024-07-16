---
title: "DIO 汎用ディジタルIO"
author: "Mono Wireless Inc."
description: "DIO 汎用ディジタルIO"
---

# DIO 汎用ディジタルIO

汎用ディジタルIO（DIO）の操作には以下の関数を利用します。

* `pinMode()`
* `digitalWrite()`
* `digitalRead()`
* `attachIntDio()`
* `detachIntDio()`

## 定数

### ピン名と番号

| 定義                                      | 名称        |
| --------------------------------------- | --------- |
| `const uint8_t PIN_DIGITAL::DIO0 .. 19` | DIOピン0～19 |
| `const uint8_t PIN_DIGITAL::DO0 .. 1`   | DOピン0,1   |



### ピンのモード（DIO0..19)

以下の列挙値は型名 `E_PIN_MODE`で取り扱われます。

| 定義                              | プルアップ | 名称           |
| ------------------------------- | :---: | ------------ |
| `PIN_MODE::INPUT`               |   無   | 入力           |
| `PIN_MODE::OUTPUT`              |   無   | 出力           |
| `PIN_MODE::INPUT_PULLUP`        |   有   | 入力           |
| `PIN_MODE::OUTPUT_INIT_HIGH`    |   無   | 出力(初期状態HIGH) |
| `PIN_MODE::OUTPUT_INIT_LOW`     |   無   | 出力(初期状態LOW)  |
| `PIN_MODE::WAKE_FALLING`        |   無   | 入力、起床ピン、立下り  |
| `PIN_MODE::WAKE_RISING`         |   無   | 入力、起床ピン、立上り  |
| `PIN_MODE::WAKE_FALLING_PULLUP` |   有   | 入力、起床ピン、立下り  |
| `PIN_MODE::WAKE_RISING_PULLUP`  |   有   | 入力、起床ピン、立上り  |
| `PIN_MODE::DISABLE_OUTPUT`      |   有   | 入力状態に戻す      |



### ピンのモード（DO0,1)

以下の列挙値は型名 `E_PIN_MODE`で取り扱われます。

| 定義                           | 名称           |
| ---------------------------- | ------------ |
| `PIN_MODE::OUTPUT`           | 出力           |
| `PIN_MODE::OUTPUT_INIT_HIGH` | 出力(初期状態HIGH) |
| `PIN_MODE::OUTPUT_INIT_LOW`  | 出力(初期状態LOW)  |
| `PIN_MODE::DISABLE_OUTPUT`   | 出力設定をやめる     |



### ピンの状態

以下の列挙値は型名 `E_PIN_STATE`で取り扱われます。

| 定義                |  値  | 名称               |
| ----------------- | :-: | ---------------- |
| `PIN_STATE::HIGH` |  1  | HIGHレベル(=Vccレベル) |
| `PIN_STATE::LOW`  |  0  | LOWレベル(=GNDレベル)  |



### ピンの立ち上がり、立下り

以下の列挙値は型名 `E_PIN_INT_MODE`で取り扱われます。

| 定義                      | 名称    |
| ----------------------- | ----- |
| `PIN_INT_MODE::FALLING` | 立ち下り  |
| `PIN_INT_MODE::RISING`  | 立ち上がり |

