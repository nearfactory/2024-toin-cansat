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

/*** function prototype */
void vTransmit(const char* msg, uint32_t addr);

/*** application defs */
// packet message
const int MSG_LEN = 80;

/*** setup procedure (run once at cold boot) */
void setup() {
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- GPS Receiver ---" << mwx::crlf;
}

/*** loop procedure (called every event) */
void loop() {
	Serial.println(".");
}

/**
 * @brief receive packet callback
 * 
 * @param rx      received data object.
 * @param handled DO NOT CHANGE, in system use.
 */
void on_rx_packet(packet_rx& rx, bool_t &handled) {
	// rx >> Serial; // debugging (display longer packet information)

	uint8_t msg[MSG_LEN];
	uint16_t num;
	uint32_t timestamp;

	// expand packet payload (shall match with sent packet data structure, see pack_bytes())
	expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
				, msg       // 4bytes of msg
							//   also can be -> std::make_pair(&msg[0], MSG_LEN)
				, num    // 2bytes, A1 value [0..1023]
				, timestamp // 4bytes of timestamp
	);

	// display the packet
	Serial << format(" %s No.%d TS=%dms>"
				, msg
				, num
				, timestamp
				)
			<< mwx::crlf
			<< mwx::flush;
	
	delay(5000);
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */