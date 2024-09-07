// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include "display.hpp"

int gpsByte = -1;   //  read a byte
char gpsLine[100]={};  //  read line 
char send[80];
int  gpsLength;     //  length of buffer



#define CHARS_COLS_LENGTH 8
static const uint8_t FONTS[][CHARS_COLS_LENGTH]  = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20
    {0x00,0x06,0x5F,0x5F,0x06,0x00,0x00,0x00},	// 0x21
    {0x00,0x07,0x07,0x00,0x07,0x07,0x00,0x00},	// 0x22
    {0x14,0x7F,0x7F,0x14,0x7F,0x7F,0x14,0x00},	// 0x23
    {0x24,0x2E,0x6B,0x6B,0x3A,0x12,0x00,0x00},	// 0x24
    {0x46,0x66,0x30,0x18,0x0C,0x66,0x62,0x00},	// 0x25
    {0x30,0x7A,0x4F,0x5D,0x37,0x7A,0x48,0x00},	// 0x26
    {0x04,0x07,0x03,0x00,0x00,0x00,0x00,0x00},	// 0x27
    {0x00,0x1C,0x3E,0x63,0x41,0x00,0x00,0x00},	// 0x28
    {0x00,0x41,0x63,0x3E,0x1C,0x00,0x00,0x00},	// 0x29
    {0x08,0x2A,0x3E,0x1C,0x1C,0x3E,0x2A,0x08},	// 0x2A
    {0x08,0x08,0x3E,0x3E,0x08,0x08,0x00,0x00},	// 0x2B
    {0x00,0xA0,0xE0,0x60,0x00,0x00,0x00,0x00},	// 0x2C
    {0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},	// 0x2D
    {0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00},	// 0x2E
    {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00},	// 0x2F
    {0x3E,0x7F,0x59,0x4D,0x7F,0x3E,0x00,0x00},	// 0x30
    {0x42,0x42,0x7F,0x7F,0x40,0x40,0x00,0x00},	// 0x31
    {0x62,0x73,0x59,0x49,0x6F,0x66,0x00,0x00},	// 0x32
    {0x22,0x63,0x49,0x49,0x7F,0x36,0x00,0x00},	// 0x33
    {0x18,0x1C,0x16,0x13,0x7F,0x7F,0x10,0x00},	// 0x34
    {0x27,0x67,0x45,0x45,0x7D,0x39,0x00,0x00},	// 0x35
    {0x3C,0x7E,0x4B,0x49,0x79,0x30,0x00,0x00},	// 0x36
    {0x03,0x63,0x71,0x19,0x0F,0x07,0x00,0x00},	// 0x37
    {0x36,0x7F,0x49,0x49,0x7F,0x36,0x00,0x00},	// 0x38
    {0x06,0x4F,0x49,0x69,0x3F,0x1E,0x00,0x00},	// 0x39
    {0x00,0x00,0x6C,0x6C,0x00,0x00,0x00,0x00},	// 0x3A
    {0x00,0xA0,0xEC,0x6C,0x00,0x00,0x00,0x00},	// 0x3B
    {0x08,0x1C,0x36,0x63,0x41,0x00,0x00,0x00},	// 0x3C
    {0x14,0x14,0x14,0x14,0x14,0x14,0x00,0x00},	// 0x3D
    {0x00,0x41,0x63,0x36,0x1C,0x08,0x00,0x00},	// 0x3E
    {0x02,0x03,0x51,0x59,0x0F,0x06,0x00,0x00},	// 0x3F
    {0x3E,0x7F,0x41,0x5D,0x5D,0x1F,0x1E,0x00},	// 0x40
    {0x7C,0x7E,0x13,0x13,0x7E,0x7C,0x00,0x00},	// 0x41
    {0x41,0x7F,0x7F,0x49,0x49,0x7F,0x36,0x00},	// 0x42
    {0x1C,0x3E,0x63,0x41,0x41,0x63,0x22,0x00},	// 0x43
    {0x41,0x7F,0x7F,0x41,0x63,0x7F,0x1C,0x00},	// 0x44
    {0x41,0x7F,0x7F,0x49,0x5D,0x41,0x63,0x00},	// 0x45
    {0x41,0x7F,0x7F,0x49,0x1D,0x01,0x03,0x00},	// 0x46
    {0x1C,0x3E,0x63,0x41,0x51,0x73,0x72,0x00},	// 0x47
    {0x7F,0x7F,0x08,0x08,0x7F,0x7F,0x00,0x00},	// 0x48
    {0x00,0x41,0x7F,0x7F,0x41,0x00,0x00,0x00},	// 0x49
    {0x30,0x70,0x40,0x41,0x7F,0x3F,0x01,0x00},	// 0x4A
    {0x41,0x7F,0x7F,0x08,0x1C,0x77,0x63,0x00},	// 0x4B
    {0x41,0x7F,0x7F,0x41,0x40,0x60,0x70,0x00},	// 0x4C
    {0x7F,0x7F,0x06,0x0C,0x06,0x7F,0x7F,0x00},	// 0x4D
    {0x7F,0x7F,0x06,0x0C,0x18,0x7F,0x7F,0x00},	// 0x4E
    {0x1C,0x3E,0x63,0x41,0x63,0x3E,0x1C,0x00},	// 0x4F
    {0x41,0x7F,0x7F,0x49,0x09,0x0F,0x06,0x00},	// 0x50
    {0x1E,0x3F,0x21,0x71,0x7F,0x5E,0x00,0x00},	// 0x51
    {0x41,0x7F,0x7F,0x19,0x39,0x6F,0x46,0x00},	// 0x52
    {0x26,0x67,0x4D,0x59,0x7B,0x32,0x00,0x00},	// 0x53
    {0x03,0x41,0x7F,0x7F,0x41,0x03,0x00,0x00},	// 0x54
    {0x7F,0x7F,0x40,0x40,0x7F,0x7F,0x00,0x00},	// 0x55
    {0x1F,0x3F,0x60,0x60,0x3F,0x1F,0x00,0x00},	// 0x56
    {0x7F,0x7F,0x30,0x18,0x30,0x7F,0x7F,0x00},	// 0x57
    {0x63,0x77,0x1C,0x08,0x1C,0x77,0x63,0x00},	// 0x58
    {0x07,0x4F,0x78,0x78,0x4F,0x07,0x00,0x00},	// 0x59
    {0x67,0x73,0x59,0x4D,0x47,0x63,0x71,0x00},	// 0x5A
    {0x00,0x7F,0x7F,0x41,0x41,0x00,0x00,0x00},	// 0x5B
    {0x01,0x03,0x06,0x0C,0x18,0x30,0x60,0x00},	// 0x5C
    {0x00,0x41,0x41,0x7F,0x7F,0x00,0x00,0x00},	// 0x5D
    {0x08,0x0C,0x06,0x03,0x06,0x0C,0x08,0x00},	// 0x5E
    {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},	// 0x5F
    {0x00,0x00,0x03,0x07,0x04,0x00,0x00,0x00},	// 0x60
    {0x20,0x74,0x54,0x54,0x3C,0x78,0x40,0x00},	// 0x61
    {0x41,0x3F,0x7F,0x44,0x44,0x7C,0x38,0x00},	// 0x62
    {0x38,0x7C,0x44,0x44,0x6C,0x28,0x00,0x00},	// 0x63
    {0x30,0x78,0x48,0x49,0x3F,0x7F,0x40,0x00},	// 0x64
    {0x38,0x7C,0x54,0x54,0x5C,0x18,0x00,0x00},	// 0x65
    {0x48,0x7E,0x7F,0x49,0x03,0x02,0x00,0x00},	// 0x66
    {0x98,0xBC,0xA4,0xA4,0xF8,0x7C,0x04,0x00},	// 0x67
    {0x41,0x7F,0x7F,0x08,0x04,0x7C,0x78,0x00},	// 0x68
    {0x00,0x44,0x7D,0x7D,0x40,0x00,0x00,0x00},	// 0x69
    {0x40,0xC4,0x84,0xFD,0x7D,0x00,0x00,0x00},	// 0x6A
    {0x41,0x7F,0x7F,0x10,0x38,0x6C,0x44,0x00},	// 0x6B
    {0x00,0x41,0x7F,0x7F,0x40,0x00,0x00,0x00},	// 0x6C
    {0x7C,0x7C,0x0C,0x18,0x0C,0x7C,0x78,0x00},	// 0x6D
    {0x7C,0x7C,0x04,0x04,0x7C,0x78,0x00,0x00},	// 0x6E
    {0x38,0x7C,0x44,0x44,0x7C,0x38,0x00,0x00},	// 0x6F
    {0x84,0xFC,0xF8,0xA4,0x24,0x3C,0x18,0x00},	// 0x70
    {0x18,0x3C,0x24,0xA4,0xF8,0xFC,0x84,0x00},	// 0x71
    {0x44,0x7C,0x78,0x44,0x1C,0x18,0x00,0x00},	// 0x72
    {0x48,0x5C,0x54,0x54,0x74,0x24,0x00,0x00},	// 0x73
    {0x00,0x04,0x3E,0x7F,0x44,0x24,0x00,0x00},	// 0x74
    {0x3C,0x7C,0x40,0x40,0x3C,0x7C,0x40,0x00},	// 0x75
    {0x1C,0x3C,0x60,0x60,0x3C,0x1C,0x00,0x00},	// 0x76
    {0x3C,0x7C,0x60,0x30,0x60,0x7C,0x3C,0x00},	// 0x77
    {0x44,0x6C,0x38,0x10,0x38,0x6C,0x44,0x00},	// 0x78
    {0x9C,0xBC,0xA0,0xA0,0xFC,0x7C,0x00,0x00},	// 0x79
    {0x4C,0x64,0x74,0x5C,0x4C,0x64,0x00,0x00},	// 0x7A
    {0x08,0x08,0x3E,0x77,0x41,0x41,0x00,0x00},	// 0x7B
    {0x00,0x00,0x00,0x77,0x77,0x00,0x00,0x00},	// 0x7C
    {0x41,0x41,0x77,0x3E,0x08,0x08,0x00,0x00},	// 0x7D
    {0x02,0x03,0x01,0x03,0x02,0x03,0x01,0x00},	// 0x7E
    {0x78,0x7C,0x46,0x43,0x46,0x7C,0x78,0x00},	// 0x7F
};


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


int up = 0;
int down = 0;
int right = 0;
int left = 0;
int center = 0;
char english[27] = {'!', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
int msg1_index[7] = {21, 16, -6, 19, 5, 14, 4};
int msg2_index[10] = {4, 15, 23, 14, -6, 18, 5, 19, 5, 20};
int msg3_index[13] = {19, 21, 3, 3, 5, 19, 19, 6, 21, 12, 12, 25, -31};
int msg4_index[15] = {18, 9, 7, 8, 20, -20, 12, 5, 6, 20, -6, 13, 15, 22, 5};
int msg5_index[14] = {3, 5, 14, 20, 5, 18, -6, 3, 15, 14, 6, 9, 18, 13};
char confirmed[] = {' '};
int confirmednum[] = {};
int index = 0;//0~25
int letnum = 0;

int id = 5;

uint8_t m_initCommand[33] = {
    0x00, 0xA8, 0x3F, // SetMultiplexRatio
    0x00, 0xD3, 0x00, // SetDisplayOffset
    0x80, 0x40,       // SetDisplayStartline
    0x80, 0xA1,       // SetSegmentRemap
    0x80, 0xC8,       // SetCOMOutputScanDirection
    0x00, 0xDA, 0x12, // SetCOMPingsHardwareConfiguration
    0x00, 0x81, 0x7F, // SetCntrastControl
    0x80, 0xA4,       // EntireDisplayON
    0x80, 0xA6,       // SetNormalDisplay
    0x00, 0xD5, 0x80, // SetOscFrequency
    0x00, 0x8D, 0x14, // EnableChargePumpRegulator
    0x80, 0xAF        // Display ON
};

enum class BTN
{
    R = 24,
    U = 25,
    L = 22,
    C = 23,
    D = 19
};

const uint8_t BTN_PIN[5] = {24, 25, 22, 23, 19};
Display Display;


void processLine(char *line) {
  if (strncmp(line, "$GPRMC,", 7) == 0) {
	int ct=0;
	char ido[20]={};
	int d=0;
	char kei[20]={};
	int k=0;
    char send[80]={};
	for(int i = 0; i < 100; i++){
		if(line[i] == ','){
			ct+=1;
			continue;
			}
	 	if(ct == 3 || ct == 4){
			ido[d++]=line[i];
			ido[d]=0;
			}
		if(ct == 5 || ct == 6){
			kei[k++]=line[i];
			kei[k]=0;
			}
	}
    d-=1;
    k-=1;
    ido[d]=0;
    kei[k]=0;
    sprintf(send,"%s,%s", ido , kei);
	vTransmit(send,id,0x00);
  }
}

/*** setup procedure (run once at cold boot) */
void setup() {
  	Serial.begin(9600);
    Wire.begin();
  	gpsLength = 0;

	pinMode(0, INPUT);//right
    pinMode(1, INPUT);//up
    pinMode(17, INPUT);//down
    pinMode(11, INPUT);//left
    pinMode(16, INPUT);//center
    Analogue.setup();

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
	
    for (int i = 13; i <= 16; i++)
        pinMode(i, PIN_MODE::INPUT);
    for (auto p : BTN_PIN)
        pinMode(p, PIN_MODE::INPUT);


    Wire.beginTransmission(0x3C);
    for (auto &cmd : m_initCommand)
        Wire.write(cmd);
    Wire.endTransmission();

    
    Wire.beginTransmission(0x3C);
}

        int p = 0;//文字表示位置（高さ）
        int q = 8;//文字幅
        int r = 0;//文字表示位置inCLEAN

void D_CLEAR(){
    for(int r=0; r<20; r++){
    //display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (r & 0x07));
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        for (int i = 0; i < 128; i++)
        {
            Wire.write(0x00);
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();
    }
    /*for(int j=0; j<8; j++){
    //display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (5 & 0x07));
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        for (int i = 0; i < 8; i++)
        {
            Wire.write(FONTS[msg1_index[j] + 32][i]);
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();
    }*/
//display output ending
}

void D_NOW(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (1 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        for (int i = 0; i < 112; i++)
        {
            Wire.write(0x00);
        }
        for (int i = 0; i < 8; i++)
        {
            Wire.write(FONTS[30][i]);
        }
        for (int i = 0; i < 8; i++)
        {
            if(index == 0){
                Wire.write(FONTS[index + 1][i]);
            }else{
                Wire.write(FONTS[index + 32][i]);
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_CONFIRMED(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (0 & 0x07));
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 128 - letnum*8; i++)
        {
            Wire.write(0x00);
        }*/
        for (int i = 0; i < 128 - letnum*8; i++)
        {
            Wire.write(0x00);
        }
        for(int j=0; j < letnum; j++){
            for (int i = 0; i < 8; i++)
            {
                Wire.write(FONTS[confirmednum[j] + 32][i]);
                // Wire.write(0x00);
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_MSG1(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (5 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 120; i++)
        {
            Wire.write(0x00);
        }*/
        for(int j=0; j<16; j++){
            for(int i=0; i<8; i++){
                if(j > 6){
                    Wire.write(0x00);
                }else{
                    Wire.write(FONTS[msg1_index[j] + 32][i]);
                }
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_MSG2(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (6 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 120; i++)
        {
            Wire.write(0x00);
        }*/
        for(int j=0; j<16; j++){
            for(int i=0; i<8; i++){
                if(j > 9){
                    Wire.write(0x00);
                }else{
                    Wire.write(FONTS[msg2_index[j] + 32][i]);
                }
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_MSG3(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (2 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 120; i++)
        {
            Wire.write(0x00);
        }*/
        for(int j=0; j<16; j++){
            for(int i=0; i<8; i++){
                if(j > 12){
                    Wire.write(0x00);
                }else{
                    Wire.write(FONTS[msg3_index[j] + 32][i]);
                }
            }
            delay(100);
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_MSG4(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (3 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 120; i++)
        {
            Wire.write(0x00);
        }*/
        for(int j=0; j<16; j++){
            for(int i=0; i<8; i++){
                if(j > 14){
                    Wire.write(0x00);
                }else{
                    Wire.write(FONTS[msg4_index[j] + 32][i]);
                }
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void D_MSG5(){
//display output begining
    Wire.beginTransmission(0x3C);
        Wire.beginTransmission(0x3C);
        Wire.write(0x80);
        Wire.write(0xB0 | (4 & 0x07));//&の前で文字表示位置調整
        Wire.endTransmission();
        
        Wire.beginTransmission(0x3C);
        Wire.write(0x40);
        /*for (int i = 0; i < 120; i++)
        {
            Wire.write(0x00);
        }*/
        for(int j=0; j<16; j++){
            for(int i=0; i<8; i++){
                if(j > 13){
                    Wire.write(0x00);
                }else{
                    Wire.write(FONTS[msg5_index[j] + 32][i]);
                }
            }
        }
        Wire.endTransmission();
        Wire.beginTransmission(0x3C);

    Wire.endTransmission();

    // p++;
    // q++;
//display output ending
}

void organizingData(){
  for(int j=0; j<gpsLength; j++){

  }
}

bool flag=true;
int time=0;

/*** loop procedure (called every event) */
void loop() {
    if((millis()-time)>5000){//4000ms=4s
        flag=true;
    }
    else{
        flag=false;
    }
//---------------------------------------------------------------display
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    if(digitalRead(16) == HIGH){
        //center
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
        D_CONFIRMED();
        delay(200);
        time=millis();
    }
    if(digitalRead(11) == HIGH && index > 0){//left
        index = index - 1;
        D_NOW();
        delay(200);
        time=millis();
    }
    if(digitalRead(0) == HIGH && index < 26){//right
        index = index + 1;  
        D_NOW();
        delay(200);
        time=millis();
    }
    if(digitalRead(1) == HIGH){//up
    	vTransmit(confirmed,id,0x00);
        delay(50);
        vTransmit(confirmed,id,0x00);
        delay(50);
        vTransmit(confirmed,id,0x00);
        for(int i=0; i<letnum; i++){
            confirmed[i] = {' '};
            confirmednum[i] = {' '};
        }
        index = 0;
        letnum = 0;
        D_CLEAR();
        D_MSG1();
        D_MSG2();
        D_MSG4();
        D_MSG5();
        D_MSG3();
        delay(200);
        time=millis();
    }
//-----------------------------------------------------------------------GPS
    if(flag==true){
        gpsByte = Serial.read();
        if (gpsByte == -1 || gpsByte == '\n') {
            return;
        }
        if (gpsByte == '\r') {
            if (gpsLength > 0) {
            gpsLength = 0;
            processLine(gpsLine);
            }
        }else {
            gpsLine[gpsLength++]  = gpsByte;//今の場所に書き込み、書き込み位置を一つ進める
            gpsLine[gpsLength]    = 0;//終端文字を書き込み
        }
    }
//-----------------------------------------------------------------------GPS
    if(digitalRead(17) == HIGH){//down
        for(int i=0; i<letnum; i++){
            confirmed[i] = {' '};
            confirmednum[i] = {' '};
        }
        index = 0;
        letnum = 0;
        D_CLEAR();
        D_MSG1();
        D_MSG2();
        D_MSG4();
        D_MSG5();
        time=millis();
    }
//button letterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    /*
    int32_t v = digitalReadBitmap();
    for (int i = 0; i < 1; i++)
        Serial.print(v >> i & 0b01);
    Serial.println();
    */
//---------------------------------------------------------------display
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
