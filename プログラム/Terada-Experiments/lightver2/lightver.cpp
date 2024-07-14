// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

int gpsByte = -1;   //  read a byte
char gpsLine[80];  //  read line 
char test[80];
int  gpsLength;     //  length of buffer

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

// DIO pins
const uint8_t PIN_BTN = 12;

/*** function prototype */
void vTransmit(const char* msg,int num, uint32_t addr);
/*** application defs */
// packet message

/*** setup procedure (run once at cold boot) */
void setup() {
  	Serial.begin(9600);
  	gpsLength = 0;

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x01) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	the_twelite.begin(); // start twelite!
}

void organizingData(){
  for(int j=0; j<gpsLength; j++){

  }
}

/*** loop procedure (called every event) */
void loop() {
	while(Serial.available()){
		gpsByte = Serial.read();
	}
	if (gpsByte == -1 || gpsByte == '\n') {
		delay(100);
	}else{
	}
	if (gpsByte == '\r') {
		if (gpsLength > 0) {
		// processLine(gpsLine);
		gpsLength = 0;
		vTransmit(gpsLine,514,0x00);
		}else{
		}
	}else {
		gpsLine[gpsLength++]  = gpsByte;//今の場所に書き込み、書き込み位置を一つ進める
		gpsLine[gpsLength]    = 0;//終端文字を書き込み
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
void vTransmit(const char* msg,int num , uint32_t addr) {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(addr)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(100,200,20); // send packet w/ delay (send first packet with randomized delay from 100 to 200ms, repeat every 20ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(msg, 80) // string should be paired with length explicitly.
			, uint16_t(num)
			, uint32_t(millis()) // put timestamp here.
		);
	
		// do transmit 
		pkt.transmit();
	}
}
