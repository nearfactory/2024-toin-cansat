#include <TWELITE>
#include <NWK_SIMPLE>

#include "lets_use_display.hpp"
#include "font.hpp"

//displayyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy

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
char english[27] = {'!', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const int englishLength = int(sizeof(english)/sizeof(char));
char confirmed[] = {' '};
int confirmednum[] = {};
int count = 0;//timer
int index = 0;//0~25
int letnum = 0;

//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

enum class BTN
{
    R = 24,
    U = 25,
    L = 22,
    C = 23,
    D = 19
};

//displayyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy


/*** setup procedure (run once at cold boot) */
void setup() {
    Serial.begin(115200);
    Wire.begin();
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
	Serial << "--- Send Messeges from extension unit 1 ---" << mwx::crlf;

    // // Display Init
    // byte displayInit[25] = {
    //     0x00,
    //     0x81,
    //     0xFF,
    //     0x40,
    //     0xA8,
    //     0x1F,
    //     0xA1,
    //     0xA6,
    //     0xC8,
    //     0xDA,
    //     0x00,
    //     0x2E,
    //     0x20,
    //     0x00,
    //     0x21,
    //     0x00,
    //     0xFF,
    //     0x22,
    //     0x00,
    //     0x07,
    //     0xD5,
    //     0x80,
    //     0x8D,
    //     0x14,
    //     0xAF
    // };
    
    // for(int i=0; i<int(sizeof(displayInit)/sizeof(byte)); i++){
    //     Wire.beginTransmission(0x3C);
    //     Wire.write(displayInit[i]);
    //     Wire.endTransmission();
    // }

    displayInit(0x3C);
    displayClear(0x3C);
    displayLocate(0x3C, 0, 0);

    // Display Start
    // Wire.beginTransmission(0x3C);

    Serial.println("setup() finish!!!!!!!!!!!!!!!!");
}

/*** loop procedure (called every event) */
void loop() {    
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

    static int count = 0;

    if(int(Analogue.read(PIN_ANALOGUE::A1)) > 2000){
        Serial.println("CENTER");
        if(english[index] == '!'){
            confirmed[letnum - 1] = ' ';
            confirmednum[letnum - 1] = ' ';
            letnum = letnum - 1;
            delay(200);
        }else{
            confirmed[letnum] = english[index];
            confirmednum[letnum] = int(index);
            letnum = letnum + 1;
        }
        Serial.printfmt("Send : %s", confirmed);
        Serial.println();

        
        displayLocate(0x3C, 0, 0);
        for(int moji=0; moji<letnum; moji++){
            for(int i=0;i<8;i++){
                Wire.beginTransmission(0x3C);
                Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
                Wire.write(FONT[confirmednum[moji]][i]);// GDDRAMに書き込む
                Wire.endTransmission();
            }
        }
        displayLocate(0x3C, 0, 2);
    }
    
    if(int(Analogue.read(PIN_ANALOGUE::A2)) > 2000){
        Serial.println("LEFT");
        index = index > 0 ? index - 1 : index;
        Serial.printfmt("%c", english[index]);
        Serial.println();
        
        displayLocate(0x3C, 0, 2);
        for(int i=0;i<8;i++){
            Wire.beginTransmission(0x3C);
            Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
            Wire.write(FONT[index][i]);// GDDRAMに書き込む
            Wire.endTransmission();
        }
    }

    if(digitalRead(0) == HIGH){
        Serial.println("RIGHT");
        index = index < englishLength-1 ? index + 1 : index;
        Serial.printfmt("%c", english[index]);
        Serial.println();
        
        displayLocate(0x3C, 0, 2);
        for(int i=0;i<8;i++){
            Wire.beginTransmission(0x3C);
            Wire.write(0x40);// Co=0 D/C#=1 GDDRAMにdataを送る
            Wire.write(FONT[index][i]);// GDDRAMに書き込む
            Wire.endTransmission();
        }
    }

    if(digitalRead(1) == HIGH){
        Serial.println("UP");

        
    }

    if(digitalRead(17) == HIGH){
        Serial.println("DOWN");
        displayClear(0x3C);
        displayLocate(0x3C, 0, 0);
    	Serial.println("trying to send message");
    	vTransmit(confirmed,0,0x00);
    	Serial.println("message has been sent");
    }

    Serial.println(count);

    count++;
    delay(200);
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
