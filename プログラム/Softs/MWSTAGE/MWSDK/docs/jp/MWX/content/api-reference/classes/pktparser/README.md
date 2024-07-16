---
title: "pktparser"
author: "Mono Wireless Inc."
description: "pktparser"
---
# pktparser

pktparser(parser\_packet)は、[serparser](../ser\_parser.md)で変換したバイト列に対して、内容の解釈を行います。

```cpp
serparser_heap parser_ser;

void setup() {
    // init ser parser (heap alloc)
    parser_ser.begin(PARSER::ASCII, 256);
}

void loop() {
    int c;
    while ((c = Serial.read()) >= 0) {
        parser_ser.parse(c);
        if (parser_ser.available()) {
            // get buffer object
            auto&& payl = parser_ser.get_buf();
            // identify packet type
            auto&& typ = identify_packet_type(payl.begin(), payl.end());
            
            // if packet type is TWELITE standard 0x81 message
            if (typ == E_PKT::PKT_TWELITE) {
                pktparser pkt; // packet parser object
                // analyze packet data
                typ = pkt.parse<TwePacketTwelite>(payl.begin(), payl.end());
                
                if (typ != E_PKT::PKT_ERROR) { // success!
                    // get data object
                    auto&& atw = pkt.use<TwePacketTwelite>();
                    
                    // display packet inforamtion
                    Serial << crlf << format("TWELITE: SRC=%08X LQI=%03d "
                        , app.u32addr_src, app.u8lqi);
	                  Serial << " DI1..4="
	                      << atw.DI1 ? 'L' : 'H' << atw.DI2 ? 'L' : 'H' 
                        << atw.DI3 ? 'L' : 'H' << atw.DI4 ? 'L' : 'H';
                }
            }
        }
    }
}
```

上記の例は、[標準アプリケーションの0x81メッセージ](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_twelite)の解釈を行っています。parser\_serオブジェクトによりSerialより入力された電文をバイト列に変換します。このバイト列をまず[`identify_packet_type()`](idenify\_packet\_type.md)により電文の種別[`E_PKT`](e\_pkt.md)を特定します。電文の種別が判定できたら次に`.parse<TwePacketTwelite>()`により電文を解析します。解析結果は`TwePacketTwelite`型になりますが、このオブジェクトを取り出す手続きが`.use<TwePacketTwelite>()`です。`TwePacketTwelite`型はクラスですが構造体として直接メンバー変数を参照します。



### parse\<T>

```cpp
template <class T>
E_PKT parse(const uint8_t* p, const uint8_t* e)
```

バイト列を解析します。

`T`には解析対象のパケット型を指定します。例えば標準アプリケーションの0x81メッセージなら`TwePacketTwelite`を指定します。

`p`と`e`はバイト列の先頭と終端の次を指定します。

戻り値は`E_PKT`型です。エラーの場合は`E_PKT::PKT_ERROR`が戻ります。



### user\<T>

```
template <class T> 
T& use()
```

解釈したバイト列に対応するパケット型に対応するオブジェクトの参照を返します。事前にparse\<T>を実行しエラーがなかった場合に呼び出すせます。

`T`は`parse<T>`で実行した型と同じもの、または基本的な情報のみ取得できる`TwePacket`を指定します。
