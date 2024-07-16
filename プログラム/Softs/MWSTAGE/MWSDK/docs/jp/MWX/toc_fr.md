
### [The MWX Library](content//README.md)

 [改版履歴](content//revisions.md) <br />

### [MWXライブラリについて](content//about_mwx/README.md)

　 [設計情報](content//about_mwx/design_policy.md) <br />
　 [License](content//about_mwx/license.md) <br />
　 [用語](content//about_mwx/terms.md) <br />

### [インストール・ビルド](content//install_n_build/README.md)

　 [環境 (OSなど)](content//install_n_build/1_environment.md) <br />
　 [TWELITE STAGE SDK のインストール](content//install_n_build/2_install_sdk.md) <br />
　 [アクトのビルド](content//install_n_build/3_building-act.md) <br />
　 [新しいプロジェクトの作成](content//install_n_build/create_new_project.md) <br />
　 [VSCodeのインストール](content//install_n_build/install_vscode.md) <br />
　 [ビルド定義 Makefile](content//install_n_build/makefile.md) <br />
　 [他のプラットフォーム](content//install_n_build/nopurattofmu.md) <br />

### [サンプルアクト](content//act_samples/README.md)

　 [act0 .. 4](content//act_samples/act_opening.md) <br />
　 [BRD\_APPTWELITE](content//act_samples/brd_apptwelite.md) <br />
　 [BRD\_ARIA](content//act_samples/brd_aria.md) <br />
　 [BRD_I2C_TEMPHUMID](content//act_samples/brd_i2c_temphumid.md) <br />
　 [PAL\_AMB-bhv](content//act_samples/pal_amb-behavior.md) <br />
　 [PAL\_AMB-usenap](content//act_samples/pal_amb-usenap.md) <br />
　 [PAL\_AMB](content//act_samples/pal_amb.md) <br />
　 [PAL\_MAG](content//act_samples/pal_mag.md) <br />
　 [PAL\_MOT-single](content//act_samples/pal_mot-oneshot.md) <br />
　 [PAL\_MOT-fifo](content//act_samples/pal_mot.md) <br />
　 [Parent\_MONOSTICK](content//act_samples/parent_monostick.md) <br />
　 [PingPong](content//act_samples/pingpong.md) <br />
　 [PulseCounter](content//act_samples/pulsecounter.md) <br />
　 [Rcv_Unvsl (ユニバーサル レシーバー)](content//act_samples/rcv_univsl.md) <br />
　 [Scratch](content//act_samples/scratch.md) <br />
　 [Slp\_Wk\_and\_Tx](content//act_samples/slp_wk_and_tx.md) <br />
　 [SM\_SIMPLE ステートマシン](content//act_samples/smsimple-suttomashin.md) <br />
　 [Unit\_???](content//act_samples/unit_acts.md) <br />
　 [WirelessUART](content//act_samples/wirelessuart.md) <br />

### [API](content//api-reference/README.md)

　 [ビヘイビア](content//api-reference/behavior/README.md) <br />
　　 [PAL\_AMB-behavior](content//api-reference/behavior/pal_amb-behavior.md) <br />
　 [クラス](content//api-reference/classes/README.md) <br />
　　 [alloc](content//api-reference/classes/alloc.md) <br />
　　 [axis\_xyzt](content//api-reference/classes/axis_xyzt.md) <br />
　　 [MWX\_APIRET](content//api-reference/classes/mwx_apiret.md) <br />
　　 [packet\_rx](content//api-reference/classes/packet_rx.md) <br />
　　 [packet\_tx](content//api-reference/classes/packet_tx.md) <br />
　　 [pktparser](content//api-reference/classes/pktparser/README.md) <br />
　　　 [E\_PKT](content//api-reference/classes/pktparser/e_pkt.md) <br />
　　　 [idenify\_packet\_type()](content//api-reference/classes/pktparser/idenify_packet_type.md) <br />
　　　 [TwePacket](content//api-reference/classes/pktparser/twepacket/README.md) <br />
　　　　 [TwePacketIO](content//api-reference/classes/pktparser/twepacket/twepacketio.md) <br />
　　　　 [TwePacketPAL](content//api-reference/classes/pktparser/twepacket/twepacketpal.md) <br />
　　　　 [TwePacketTwelite](content//api-reference/classes/pktparser/twepacket/twepackettwelite.md) <br />
　　　　 [TwePacketUART](content//api-reference/classes/pktparser/twepacket/twepacketuart.md) <br />
　　 [serparser](content//api-reference/classes/ser_parser.md) <br />
　　 [smplbuf](content//api-reference/classes/smplbuf/README.md) <br />
　　　 [.get\_stream\_helper()](content//api-reference/classes/smplbuf/get_stream_helper.md) <br />
　　　 [smplbuf\_strm\_u8](content//api-reference/classes/smplbuf/smplbuf_strm_u8.md) <br />
　　 [smplque](content//api-reference/classes/smplque.md) <br />
　　 [SM\_SIMPLE ステートマシン](content//api-reference/classes/smsimple-suttomashin.md) <br />
　　 [mwx::stream](content//api-reference/classes/twe-stream/README.md) <br />
　　　 [stream\_helper](content//api-reference/classes/twe-stream/stream_helper.md) <br />
　　　 [mwx::bigendian](content//api-reference/classes/twe-stream/twe-bigendian.md) <br />
　　　 [mwx::crlf](content//api-reference/classes/twe-stream/twe-crlf.md) <br />
　　　 [mwx::flush](content//api-reference/classes/twe-stream/twe-flush.md) <br />
　　　 [format (mwx::mwx\_format)](content//api-reference/classes/twe-stream/twe-fmt.md) <br />
　 [定義](content//api-reference/defs.md) <br />
　 [外部ライブラリ](content//api-reference/external_libraries/README.md) <br />
　　 [EASTL](content//api-reference/external_libraries/EASTL.md) <br />
　 [関数](content//api-reference/funcs/README.md) <br />
　　 [DIO 汎用ディジタルIO](content//api-reference/funcs/dio/README.md) <br />
　　　 [attachIntDio()](content//api-reference/funcs/dio/attachintdio.md) <br />
　　　 [detachIntDio()](content//api-reference/funcs/dio/detachintdio.md) <br />
　　　 [digitalRead()](content//api-reference/funcs/dio/digitalread.md) <br />
　　　 [digitalReadBitmap()](content//api-reference/funcs/dio/digitalreadbitmap.md) <br />
　　　 [digitalWrite()](content//api-reference/funcs/dio/digitalwrite.md) <br />
　　　 [pinMode()](content//api-reference/funcs/dio/pinmode.md) <br />
　　 [システム関数](content//api-reference/funcs/systemfunc/README.md) <br />
　　　 [delay()](content//api-reference/funcs/systemfunc/delay.md) <br />
　　　 [delayMicroseconds()](content//api-reference/funcs/systemfunc/delaymicroseconds.md) <br />
　　　 [millis()](content//api-reference/funcs/systemfunc/millis.md) <br />
　　　 [random()](content//api-reference/funcs/systemfunc/random.md) <br />
　　 [ユーティリティ関数](content//api-reference/funcs/utility/README.md) <br />
　　　 [Byte array utils](content//api-reference/funcs/utility/byte-array-utils.md) <br />
　　　 [CRC8, XOR, LRC](content//api-reference/funcs/utility/checksum.md) <br />
　　　 [collect\_bits()](content//api-reference/funcs/utility/collect_bits.md) <br />
　　　 [div100()](content//api-reference/funcs/utility/div100.md) <br />
　　　 [expand\_bytes()](content//api-reference/funcs/utility/expand_bytes.md) <br />
　　　 [pack\_bits()](content//api-reference/funcs/utility/pack_bits.md) <br />
　　　 [pack\_bytes()](content//api-reference/funcs/utility/pack_bytes.md) <br />
　　　 [pnew()](content//api-reference/funcs/utility/pnew.md) <br />
　　　 [Printf utils](content//api-reference/funcs/utility/printf-utils.md) <br />
　　　 [Scale utils](content//api-reference/funcs/utility/scale-utils.md) <br />
　 [クラスオブジェクト](content//api-reference/predefined_objs/README.md) <br />
　　 [Analogue](content//api-reference/predefined_objs/analogue.md) <br />
　　 [Buttons](content//api-reference/predefined_objs/buttons.md) <br />
　　 [EEPROM](content//api-reference/predefined_objs/eeprom.md) <br />
　　 [PulseCounter](content//api-reference/predefined_objs/pulsecounter.md) <br />
　　 [Serial](content//api-reference/predefined_objs/serial.md) <br />
　　 [SerialParser](content//api-reference/predefined_objs/serialparser.md) <br />
　　 [SPI](content//api-reference/predefined_objs/spi/README.md) <br />
　　　 [SPI (ヘルパークラス版)](content//api-reference/predefined_objs/spi/spi-helperclass.md) <br />
　　　 [SPI (メンバ関数版)](content//api-reference/predefined_objs/spi/spi-member.md) <br />
　　 [the\_twelite](content//api-reference/predefined_objs/the_twelite.md) <br />
　　 [TickTimer](content//api-reference/predefined_objs/ticktimer.md) <br />
　　 [Timer0 .. 4](content//api-reference/predefined_objs/timers.md) <br />
　　 [Wire](content//api-reference/predefined_objs/wire/README.md) <br />
　　　 [Wire (ヘルパークラス版)](content//api-reference/predefined_objs/wire/wire-helperclass.md) <br />
　　　 [Wire (メンバ関数版)](content//api-reference/predefined_objs/wire/wire-member.md) <br />
　 [コールバック関数](content//api-reference/sys_callbacks/README.md) <br />
　　 [begin()](content//api-reference/sys_callbacks/begin.md) <br />
　　 [init\_coldboot()](content//api-reference/sys_callbacks/init_coldboot.md) <br />
　　 [init\_warmboot()](content//api-reference/sys_callbacks/init_warmboot.md) <br />
　　 [loop()](content//api-reference/sys_callbacks/loop.md) <br />
　　 [on_rx_packet()](content//api-reference/sys_callbacks/on_rx_packet.md) <br />
　　 [on_tx_comp()](content//api-reference/sys_callbacks/on_tx_comp.md) <br />
　　 [setup()](content//api-reference/sys_callbacks/setup.md) <br />
　　 [wakeup()](content//api-reference/sys_callbacks/wakeup.md) <br />

### [ボード (BRD)](content//boards/README.md)

　 [\<ARIA>](content//boards/aria.md) <br />
　 [\<BRD\_APPTWELITE>](content//boards/brd_apptwelite.md) <br />
　 [\<CUE>](content//boards/cue.md) <br />
　 [\<MONOSTICK>](content//boards/monostick.md) <br />
　 [PAL](content//boards/pal/README.md) <br />
　　 [\<PAL\_AMB>](content//boards/pal/pal_amb.md) <br />
　　 [\<PAL\_MAG>](content//boards/pal/pal_mag.md) <br />
　　 [\<PAL\_MOT>](content//boards/pal/pal_mot.md) <br />
　　 [&lt;PAL\_NOTICE&gt;](content//boards/pal/pal_notice.md) <br />

### [センサー・デバイス (SNS)](content//sensor_object/README.md)

　 [BMx280 - 環境センサー](content//sensor_object/bme280bmp280-sens.md) <br />
　 [LTR-308ALS - 照度センサー](content//sensor_object/ltr-308als.md) <br />
　 [MC3630 - 加速度センサー](content//sensor_object/mc3630.md) <br />
　 [PCA9632 - LEDドライバ](content//sensor_object/pca9632-leddoraiba.md) <br />
　 [SHT3x - 温湿度センサー](content//sensor_object/sht3x-sens.md) <br />
　 [SHT4x - 温湿度センサー](content//sensor_object/sht4x.md) <br />
　 [SHTC3 - 温湿度センサー](content//sensor_object/shtc3.md) <br />

### [ネットワーク (NWK)](content//networks/README.md)

　 [レイヤーツリーネット  \<NWK\_LAYERED>](content//networks/nwk_layered.md) <br />
　 [シンプル中継ネット \<NWK\_SIMPLE>](content//networks/nwk_simple.md) <br />

### [設定 （STG） - インタラクティブモード](content//settings/README.md)

　 [\<STG\_STD>](content//settings/stg_std.md) <br />
