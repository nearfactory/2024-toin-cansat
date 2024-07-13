#include <TWELITE>
#include <NWK_SIMPLE>

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

void setup() {
    Analogue.begin(pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::A2,PIN_ANALOGUE::VCC), 100);//center, left
    pinMode(0, INPUT);//right
    pinMode(1, INPUT);//up
    pinMode(17, INPUT);//down
    Analogue.setup();
}

void loop() {
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
    }
    if(digitalRead(17) == HIGH){
        // Serial.println("down");
    }
}