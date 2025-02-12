#include <TWELITE>

const uint8_t PIN_DO1 = 18;
const uint8_t PIN_DO2 = 19;
int iLedCounter1 = 0;
int iLedCounter2 = 0;

/*** the setup procedure (called on boot) */
void setup() {
    pinMode(PIN_DO1, OUTPUT);
    digitalWrite(PIN_DO1, LOW); // TURN DO1 ON

    pinMode(PIN_DO2, OUTPUT);
    digitalWrite(PIN_DO2, LOW); // TURN DO2 ON

    Timer0.begin(10); // 10Hz Timer 
    Timer1.begin(5); // 5Hz Timer 

    Serial << "--- act3 (using 2 timers) ---" << crlf;
}

/*** loop procedure (called every event) */
void loop() {
    if (Timer0.available()) {
        if (iLedCounter1 == 0) {
            digitalWrite(PIN_DO1, HIGH);
            iLedCounter1 = 1;
        } else {
            digitalWrite(PIN_DO1, LOW);
            iLedCounter1 = 0;
        }
    }

    if (Timer1.available()) {
        if (iLedCounter2 == 0) {
            digitalWrite(PIN_DO2, HIGH);
            iLedCounter2 = 1;
        } else {
            digitalWrite(PIN_DO2, LOW);
            iLedCounter2 = 0;
        }
    }
}



















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
const int MSG_LEN = 4;
const char MSG_PING[] = "PING";
const char MSG_PONG[] = "PONG";

/*** setup procedure (run once at cold boot) */
void setup() {
	/*** SETUP section */
	Buttons.setup(5); // init button manager with 5 history table.
	Analogue.setup(true); // setup analogue read (check every 50ms)

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	/*** BEGIN section */
	Buttons.begin(pack_bits(PIN_BTN), 5, 10); // check every 10ms, a change is reported by 5 consequent values.
	Analogue.begin(pack_bits(PIN_ANALOGUE::A1, PIN_ANALOGUE::VCC), 50); // _start continuous adc capture.

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
}

/*** loop procedure (called every event) */
void loop() {
    // read from serial
	while(Serial.available())  {
        int c = Serial.read();

		Serial << mwx::crlf << char(c) << ':';

        switch(c) {
            case 't':
                vTransmit(MSG_PING, 0xFF);
                break;

            default:
				break;
        }
	}

	// Button press
	if (Buttons.available()) {
		uint32_t btn_state, change_mask;
		Buttons.read(btn_state, change_mask);

		// Serial << fmt("<BTN %b:%b>", btn_state, change_mask);
		if (!(change_mask & 0x80000000) && (btn_state && (1UL << PIN_BTN))) {
			// PIN_BTN pressed
			vTransmit(MSG_PING, 0xFF);
		}
	}
}

/**
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
void vTransmit(const char* msg, uint32_t addr) {
	Serial << "vTransmit()" << mwx::crlf;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg, MSG_LEN) // string should be paired with length explicitly.
			, uint16_t(analogRead(PIN_ANALOGUE::A1)) // possible numerical values types are uint8_t, uint16_t, uint32_t. (do not put other types)
			, uint16_t(analogRead_mv(PIN_ANALOGUE::VCC)) // A1 and VCC values (note: alalog read is valid after the first (Analogue.available() == true).)
			, uint32_t(millis()) // put timestamp here.
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
				, adcval    // 2bytes, A1 value [0..1023]
				, volt      // 2bytes, Module VCC[mV]
				, timestamp // 4bytes of timestamp
	);
	
	// if PING packet, respond pong!
	if (!strncmp((const char*)msg, "PING", MSG_LEN)) {
		// transmit a PONG packet with specifying the address.
		vTransmit(MSG_PONG, rx.get_psRxDataApp()->u32SrcAddr);
	}

	// display the packet
	Serial << format("<RX ad=%x/lq=%d/ln=%d/sq=%d:" // note: up to 4 args!
				, rx.get_psRxDataApp()->u32SrcAddr
				, rx.get_lqi()
				, rx.get_length()
				, rx.get_psRxDataApp()->u8Seq
				)
			<< format(" %s AD=%d V=%d TS=%dms>" // note: up to 4 args!
				, msg
				, adcval
				, volt
				, timestamp
				)
			<< mwx::crlf
			<< mwx::flush;
}

/* Copyright (C) 2019-2021 Mono Wireless Inc. All Rights Reserved. *
 * Released under MW-SLA-*J,*E (MONO WIRELESS SOFTWARE LICENSE     *
 * AGREEMENT).                                                     */