// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

#include "lets_use_display.hpp"
#include "font.hpp"

int gpsByte = -1;   //  read a byte
char gpsLine[80];  //  read line 
char test[80];
int  gpsLength;     //  length of buffer



int selecting = 0;





/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;


/*** function prototype */
void vTransmit(const char* msg,int num, uint32_t addr);
/*** application defs */
// packet message

/*** setup procedure (run once at cold boot) */
void setup() {
	displayInit(0x3C);
	displayClear(0x3C);
	displayLocate(0x3C, 0, 2);
	displayDrawChar(0x3C, 18);	// S
	displayDrawChar(0x3C, 19);	// T
	displayDrawChar(0x3C, 0);	// A
	displayDrawChar(0x3C, 17);	// R
	displayDrawChar(0x3C, 19);	// T
	displayDrawChar(0x3C, 8);	// I
	displayDrawChar(0x3C, 13);	// N
	displayDrawChar(0x3C, 6);	// G
	delay(3000);

  	Serial.begin(9600);
  	gpsLength = 0;

	
    Analogue.begin(pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::A2,PIN_ANALOGUE::VCC), 100);//center, left
	Analogue.setup();
	pinMode(0, INPUT);
	pinMode(1, INPUT);
	pinMode(17, INPUT);


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

	Wire.beginTransmission(0x3C);
	displayClear(0x3C);
}

void organizingData(){
  for(int j=0; j<gpsLength; j++){

  }
}

/*** loop procedure (called every event) */
void loop() {
	// Get 1byte through Serial
	while(Serial.available()){
		gpsByte = Serial.read();
	}

	// Cannot Get Info
	if (gpsByte == -1 || gpsByte == '\n') {
		delay(100);
	}

	if (gpsByte == '\r') {
		if (gpsLength > 0) {
			// processLine(gpsLine);
			gpsLength = 0;
			vTransmit(gpsLine,514,0x00);
		}
	} else {
		gpsLine[gpsLength++]  = gpsByte;//今の場所に書き込み、書き込み位置を一つ進める
		gpsLine[gpsLength]    = 0;//終端文字を書き込み
	}


	if(digitalRead(1) == HIGH){
		// Up Button Pushed
		while(digitalRead(1) == HIGH);

		selecting = selecting > 0 ? selecting-1 : 25;
	}

	if(digitalRead(0) == HIGH){
		// Right Button Pushed
	}

	if(digitalRead(17) == HIGH){
		// Down Button Pushed
		while(digitalRead(17) == HIGH);

		selecting = selecting < 25 ? selecting+1 : 0;
	}

	if(int(Analogue.read(PIN_ANALOGUE::A2)) > 2000){
		// Left Button Pushed
	}

	if(int(Analogue.read(PIN_ANALOGUE::A1)) > 2000){
		// Center Button Pushed
	}






	unsigned char i,p,cnt;

	displayLocate(0x3C, 4, 1);
	displayDrawChar(0x3C, 36);
	displayLocate(0x3C, 4, 4);
	displayDrawChar(0x3C, 37);
	displayLocate(0x3C, 0, 2);
	for(i=0; i<8; i++){
		p = FONT[selecting][i];// under half 4bit of font data
		cnt = 0;
		if( p & 0b00000001 ) cnt = 0b00000011;// 縦方向２倍
		if( p & 0b00000010 ) cnt |= 0b00001100;//　　〃
		if( p & 0b00000100 ) cnt |= 0b00110000;//　　〃
		if( p & 0b00001000 ) cnt |= 0b11000000;//　　〃
		Wire.beginTransmission(0x3C);
		Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
		Wire.write(cnt);// 縦２倍にするGDDRAMへの書き込み
		Wire.write(cnt);// 横２倍にするGDDRAMへの書き込み
		Wire.endTransmission();
	}

	displayLocate(0x3C, 0, 3);
	for(i=0;i<8;i++){
		p = FONT[selecting][i];// upper half 4bit of font data
		cnt = 0;
		if( p & 0b00010000) cnt = 0b00000011;// 縦方向２倍
		if( p & 0b00100000) cnt |= 0b00001100;//　　〃
		if( p & 0b01000000) cnt |= 0b00110000;//　　〃
		if( p & 0b10000000) cnt |= 0b11000000;//　　〃
		Wire.beginTransmission(0x3C);
		Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
		Wire.write(cnt);// 縦２倍にするGDDRAMへの書き込み
		Wire.write(cnt);// 横２倍にするGDDRAMへの書き込み
		Wire.endTransmission();
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
