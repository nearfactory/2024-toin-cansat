---
title: pktparser
author: "Mono Wireless Inc."
---

# pktparser

pktparser(parser\_packet) performs content interpretation on the byte sequence converted by [serparser](../ser\_parser.md) performs content interpretation on the byte sequence converted by [serparser](../ser\_parser.md).

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

The above example interprets [standard application 0x81 message](https://wings.twelite.info/how-to-use/parent-mode/receive-message/app\_twelite). parser\_ser object converts the message input from Serial into a byte string. This byte string is first identified by [`identify_packet_type()`](identify_packet_type.md) to determine the type of the message [`E_PKT`](e\_pkt.md). Once the message type is determined, the message is parsed by `.parse<TwePacketTwelite>()`. The parsed result will be of type `TwePacketTwelite`, and `.use<TwePacketTwelite>()` is the procedure to extract this object. The `TwePacketTwelite` type is a class, but it refers to its member variables directly as a structure.



### parse\<T>

```cpp
template <class T>
E_PKT parse(const uint8_t* p, const uint8_t* e)
```

Parses a sequence of bytes.

The `T` specifies the packet type to be parsed. For example, `TwePacketTwelite` is specified for 0x81 messages in standard applications.

`p` and `e` specify the next to the beginning and the end of the byte sequence.

The return value is of type `E_PKT`. In case of an error, `E_PKT::PKT_ERROR` is returned.



### user\<T>

```
template <class T> 
T& use()
```

Returns a reference to an object corresponding to the packet type of the interpreted byte sequence. It can be called if `parse<T>` was executed beforehand and there were no errors.

The `T` can be the same type as the one executed with `parse<T>`, or a `TwePacket` from which only basic information can be obtained.
