# SPRINTFライブラリ

[fprintf ライブラリ](../hw-api-ref/perifuraru/uart/fprintf-raiburari/)を利用した簡易的な sprintf の実装。内部バッファを確保して、そのバッファーに対して書式に従った文字列を書き出します。続けて書き出した場合、直前の書き出しに対して追記します。

### 初期化

`SPRINTF_vInit128()` により 128バイトの領域確保と初期化を行います。初期化したのち `SPRINTF_Stream` が利用可能になります。

### 書き出し

1. `SPRINTF_vRewind()` により、バッファを先頭に巻き戻す。
2. `vfPrintf(SPRINTF_Stream, ...)` によりバッファに書き出します。
3. 必要に応じて `vfPrintf()` を呼び出します。直前の出力に追記されます。

### バッファの参照

バッファは`0x00` `'\0'` にて終端されています。先頭アドレスの取得には `SPRINTF_pu8GetBuff()` を用います。またバッファの格納文字数を得るには `SPRINTF_u16Length()` を用います。

### コード例

```c
#include <sprintf.h>

    ...
    // 初期化
    SPRINTF_vInit128(); // 128バイトの内部バッファの確保
    
    ...
    static int iCt = 0; // 何かのカウンタ
    
    // 内部バッファへの書き込み
    SPRINTF_vRewind(); // 内部ポインタを先頭に巻き戻す
    vfPrintf(SPRINTF_Stream, "HELLO %d", 1123);
    vfPrintf(SPRINTF_Stream, ",GOODBYE", 1123);
    uint16 u16len = SPRINTF_u16Length();
    
    // 出力(NUL で終端されているので、文字列として扱える)
    vfPrintf(&sSerStream, "%003d: %s"LB, 
        iCt++,
        SPRINTF_pu8GetBuff());

```
