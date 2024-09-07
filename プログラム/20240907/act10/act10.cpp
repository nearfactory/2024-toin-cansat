// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

// DIO pins
const uint8_t PIN_BTN = 12;

/*** application defs */
// packet message
const int MSG_LEN = 80;

// const char* msg_teensy;

/*** setup procedure (run once at cold boot) */
void setup() {

	Serial.begin(115200);
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x00) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	// Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
}

/*** loop procedure (called every event) */
void loop() {
	// receive RF packet.
    while (the_twelite.receiver.available()) {
		auto&& rx = the_twelite.receiver.read();

		// rx >> Serial; // debugging (display longer packet information)

		uint8_t msg[MSG_LEN];
		uint32_t timestamp;
		uint16_t num;

		// expand packet payload (shall match with sent packet data structure, see pack_bytes())
		expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
					, msg       // 4bytes of msg
					, num			//   also can be -> std::make_pair(&msg[0], MSG_LEN)
					, timestamp // 4bytes of timestamp
        );

		// display the packet
		Serial/* << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
        //             , rx.get_psRxDataApp()->u32SrcAddr
        //             , rx.get_lqi()
        //             , rx.get_length()
		// 			, rx.get_psRxDataApp()->u8Seq
        //             )*/
				<< format("%d:%s" // note: up to 4 args!
					, num
					, msg
					)
               << mwx::crlf
			   << mwx::flush;
        /*msg_teensy = msg;
        Serial.print(msg);*/
	}
}

/**
 * @fn	void vTransmit(const char *msg, uint32_t addr)
 *
 * @brief	Transmits the given message.
 *
 *          The packet data structure is:
 *            uint8_t[4] : the message "PING" or "PONG"
 *            uint16_t   : A1=ADC0 adc value [0..1023 (max=2470mV)]
 *            uint16_t   : Vcc [mV]
 *            uint32_t   : system timer count in ms.
 *
 * @param msg	the message with length of MSG_LEN.
 * @param addr	destination address
 *
 */