#include <TWELITE>
#include <NWK_SIMPLE>

#include "lets_use_display.hpp"
#include "font.hpp"

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
const int MSG_LEN = 4;
const char MSG_PING[] = "PING";
const char MSG_PONG[] = "PONG";

/*** setup procedure (run once at cold boot) */
void setup() {
	Serial.begin(115200);
	Wire.begin();

	pinMode(0, INPUT);	// RIGHT
	pinMode(1, INPUT);	// UP
	pinMode(17, INPUT);	// DOWN

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
	
	displayInit(0x3C);
	displayClear(0x3C);

	/*** INIT message */
	Serial << "--- Sample for PINGPONG on CanSat-board ---" << mwx::crlf;
}

/*** loop procedure (called every event) */

int top = 0;
int middle = 0;
int bottom = 0;

void loop() {
    // read from serial
	if(digitalRead(0) == HIGH){
		while(digitalRead(0) == HIGH);
		vTransmit(MSG_PING, 0xFF);
		displayLocate(0x3C, top*8, 1);
		displayDrawChar(0x3C, top+1);
		top += 1;
	}
	if(digitalRead(1) == HIGH){
		while(digitalRead(1) == HIGH);
		vTransmit(MSG_PING, 0xFF);
		displayLocate(0x3C, middle*8, 3);
		displayDrawChar(0x3C, middle+1);
		middle += 1;
	}
	if(digitalRead(17) == HIGH){
		while(digitalRead(17) == HIGH);
		vTransmit(MSG_PING, 0xFF);
		displayLocate(0x3C, bottom*8, 5);
		displayDrawChar(0x3C, bottom+1);
		bottom += 1;
		delay(500);
		displayClear(0x3C);
		top = 0;
		middle = 0;
		bottom = 0;
	}
}

/**
 * @brief	Transmits the given message.
 *
 *          The packet data structure is:
 *            uint8_t[4] : the message "PING" or "PONG"
 *
 * @param msg	the message with length of MSG_LEN.
 * @param addr	destination address
 *
 */
void vTransmit(const char* msg, uint32_t addr) {
	Serial << "Messege have been sent Successfully!" << mwx::crlf;

	Serial << format("Sent: TX")
			<< format(" %s\n", msg);

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
		);
	
		// do transmit 
		pkt.transmit();
	}
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
	uint16_t adcval, volt;
	uint32_t timestamp;

	// expand packet payload (shall match with sent packet data structure, see pack_bytes())
	expand_bytes(rx.get_payload().begin(), rx.get_payload().end()
				, msg       // 4bytes of msg
							//   also can be -> std::make_pair(&msg[0], MSG_LEN)
	);
	
	// if PING packet, respond pong!
	if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
		// transmit a PONG packet with specifying the address.
		vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
	}

	// display the packet
	Serial << format("Received: RX")
			<< format(" %s\n", msg)
			<< mwx::crlf
			<< mwx::flush;
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */