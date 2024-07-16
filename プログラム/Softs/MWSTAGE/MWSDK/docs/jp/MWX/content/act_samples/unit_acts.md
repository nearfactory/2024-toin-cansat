---
title: Unit_???
author: "Mono Wireless Inc."
description: "Unit_???"
---

# Unit\_???

Unit\_で始まるアクト(Act)は、ごく単機能の記述や動作を確認するためのものです。



| 名前                     | 内容                                                         |
| ------------------------ | ------------------------------------------------------------ |
| Unit\_ADC                | ADCを動作させるサンプルです。100msごとにADCを連続実行しつつ約1秒おきに読み出し表示します。`[s]`キーでスリープします。 |
| Unit\_I2Cprobe           | I2Cバスをスキャンして、応答のあるデバイス番号を表示します（この手順で応答しないデバイスもあります）。 |
| Unit\_delayMicoroseconds | [`delayMicroseconds()`](../api-reference/funcs/systemfunc/delaymicroseconds.md)の動作を確認します。16MhzのTickTimerのカウントとの比較をします。 |
| Unit\_brd\_CUE           | [TWELITE CUE](https://mono-wireless.com/jp/products/twelite-cue/)の加速度センサー,磁気センサー,LEDの動作確認を行います。ターミナルから`[a]`,`[s]`,`[l]`キーを入力します。 |
| Unit\_brd\_ARIA          | TWELITE ARIAの温湿度センサー、磁気センサー、LEDの動作確認を行います。ターミナルから`[t]`,`[s]`,`[l]`キーを入力します。 |
| Unit\_brd\_PAL\_NOTICE   | <a href="../boards/pal/pal_notice.md">通知パル(NOTICE PAL)のLED点灯</a>を試します。起動時に全灯フラッシュ、その後はシリアル入力で操作します。<br>- `r`,`g`,`b`,`w` : 各色の点灯モードをトグルする<br>- `R`,`G`,`B`,`W` : 各色の明るさを変更する（消灯・全灯時は無効）<br>- `c`: 周期を変化させる（点滅時）<br>- `C`: 点滅時のデューティを変化させる（点滅時） |
| Unit\_div100             | 10,100,1000の割り算と商を求める[`div10()`,`div100()`,`div1000()`](../api-reference/funcs/utility/div100.md)の動作確認を行います。-99999～99999まで計算を行い通常の`/`,`%`による除算との経過時間の比較をします。 |
| Unit\_div\_format        | [`div10()`,`div100()`,`div1000()`](../api-reference/funcs/utility/div100.md)の結果を文字列出力します。 |
| Unit\_UART1              | UART1 ([`Serial1`](../api-reference/predefined\_objs/serial.md)) の使用サンプルです。UART0([`Serial`](../api-reference/predefined\_objs/serial.md))からの入力をUART1に出力し、UART1からの入力をUART0に出力します。 |
| Unit\_Pkt\_Parser        | パケット情報のパーサー<a href="../api-reference/classes/pktparser/">pktparser</a>の使用サンプルです。App_Wingsの出力を解釈することが出来ます。<br>※ TWELITE無線モジュール同士をシリアル接続して、一方をApp_Wingsとして他方でその出力を解釈したいような場合です。他方をTWELITE無線モジュール以外に接続したい場合は「<a href="../install_n_build/nopurattofmu.md">他のプラットフォーム</a>」を参照ください。 |
| Uint\_EEPROM             | EEPROMの読み書きテストコードです。                           |
| Unit\_Cue\_MagBuz        | TWELITE CUEの磁石センサーとSETピン（圧電ブザーを接続）を用いた、磁石を離すとブザーが鳴るプログラムです。 |
| Unit\_doint-bhv          | IO割り込みを処理するビヘイビア記述例です。                   |
| Unit_EASTL               | EASTL ライブラリを用いた断片コード集です。                   |
