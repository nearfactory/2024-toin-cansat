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
void vTransmit(const char* msg, int num ,uint32_t addr);

/*** application defs */
// packet message


//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
int up = 0;
int down = 0;
int right = 0;
int left = 0;
int center = 0;
char english[27] = {'!', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char output[] = {' '};
char confirmed[] = {' '};
int count = 0;//timer
int index = 0;//0~25
int letnum = 0;
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr


/*** setup procedure (run once at cold boot) */
void setup() {
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    Analogue.begin(pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::A2,PIN_ANALOGUE::VCC), 100);//center, left
    pinMode(0, INPUT);//right
    pinMode(1, INPUT);//up
    pinMode(17, INPUT);//down
    Analogue.setup();
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
	
	
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
	Serial << "--- PingPong sample (press 't' to transmit) ---" << mwx::crlf;
}

/*** loop procedure (called every event) */
void loop() {
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    if(int(Analogue.read(PIN_ANALOGUE::A1)) > 2000){
        // Serial.println("center");
        if(english[index] == '!'){
            confirmed[letnum - 1] = ' ';
            letnum = letnum - 1;
            delay(200);
        }else{
            confirmed[letnum] = english[index];
            letnum = letnum + 1;
        }
        Serial.printfmt("Send : %s", confirmed);
        Serial.println();
        delay(200);
    }
    if(int(Analogue.read(PIN_ANALOGUE::A2)) > 2000 && index > 0){
        // Serial.println("left");
        index = index - 1;
        Serial.printfmt("%c", english[index]);
        Serial.println();
        delay(200);
    }
    if(digitalRead(0) == HIGH && index < 25){
        // Serial.println("right");
        index = index + 1;  
        Serial.printfmt("%c", english[index]);
        Serial.println();
        delay(200);
    }
    if(digitalRead(1) == HIGH){
        // Serial.println("up");
		Serial.println("trying to send message");
    	vTransmit(confirmed,0,0x00);
		Serial.println("message has been sent");
        delay(200);
    }
    if(digitalRead(17) == HIGH){
        // Serial.println("down");
    }
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
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
void vTransmit(const char* msg,int num, uint32_t addr) {
	Serial << "vTransmit()" << mwx::crlf;
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg,80) // string should be paired with length explicitly.
			, uint16_t(num)
			, uint32_t(millis()) // put timestamp here.
		);
	
		// do transmit 
		pkt.transmit();
	}
}
