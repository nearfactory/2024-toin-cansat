---
title: ".get_stream_helper()"
author: "Mono Wireless Inc."
description: ".get_stream_helper()"
---

# .get\_stream\_helper()

`uint8_t`型のsmplbuf配列を参照した[stream\_helper ](../twe-stream/stream\_helper.md)[ヘルパーオブジェクト](../twe-stream/stream\_helper.md)を経由して、[`mwx::stream`](../twe-stream/)による演算子やメソッドを用います。

```cpp
smplbuf_u8<32> b;
auto&& bs = b.get_stream_helper(); // ヘルパーオブジェクト

// データ列の生成
uint8_t FOURCHARS[]={'A', 'B', 'C', 'D'};
bs << FOURCHARS;
bs << ';';
bs << uint32_t(0x30313233); // "0123"
bs << format(";%d", 99);

Serial << b << crlf; // Serialへの出力は smplbuf_u8<32> クラス経由で

//結果: ABCD;0123;99
```

ヘルパーオブジェクトの型名は長くなるため`auto&&`により解決しています。このオブジェクトに対して`<<`演算子など`mwx::stream`で定義されたインタフェースを利用できます。

生成されたヘルパーオブジェクト`bs`は生成時に大本の配列`b`の先頭位置から読み書きを始めます。配列の末尾の場合は`append()`によりデータを追加します。読み書きを行うたびに位置は次に移動していきます

ヘルパー関数では読み出し用の`>>`演算子が利用できます。

```cpp
//..上例の続き
// ABCD;0123;99 <- bに格納されている

//読み出しデータ格納変数
uint8_t FOURCHARS_READ[4];
uint32_t u32val_read;
uint8_t c_read[2];

// >>演算子で読み出す
bs.rewind();                //ポジションを先頭に巻き戻す
bs >> FOURCHARS_READ;      //4文字
bs >> mwx::null_stream(1); //1文字スキップ
bs >> u32val_read;         //32bitデータ
bs >> mwx::null_stream(1); //1文字スキップ
bs >> c_read;              //2文字

// 結果表示
Serial << crlf << "4chars=" << FOURCHARS_READ;
Serial << crlf << format("32bit val=0x%08x", u32val_read);
Serial << crlf << "2chars=" << c_read;

// 4chars=ABCD
// 32bit val=0x30313233
// 2chars=99
```

