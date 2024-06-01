#include <string>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

using namespace std;

struct Cursor{
    int x=0,y=0;
};

constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;
constexpr int DISPLAY_ADDR = 0X3C;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, -1);

constexpr int CURSOR_X_MAX = 28;
constexpr int CURSOR_Y_MAX = 11;

string alphabet[28] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","backspace","return"};

/*
string characters[11][5] = {
    {"あ","い","う","え","お"},
    {"か","き","く","け","こ"},
    {"さ","し","す","せ","そ"},
    {"た","ち","つ","て","と"},
    {"な","に","ぬ","ね","の"},
    {"は","ひ","ふ","へ","ほ"},
    {"ま","み","む","め","も"},
    {"や","ゐ","ゆ","ゑ","よ"},
    {"ら","り","る","れ","ろ"},
    {"わ","　","を","　","ん"},
    {"゛", "゜", "削除", "改行", "終了"}
};
*/

void setup(){
  pinMode(0,INPUT_PULLDOWN);
  pinMode(1,INPUT_PULLDOWN);
  pinMode(2,INPUT_PULLDOWN);
  pinMode(3,INPUT_PULLDOWN);
  pinMode(4,INPUT_PULLDOWN);

  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    Serial.println("failed to begin display!");
    while(true) ;
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("Cansat 缶サット");
  display.display();

  delay(2000);
}

void loop(){
  static Cursor cursor{0,0};
  static bool button[5]={};   // 0:left 1:up 2:right 3:down 4:center
  static bool previous_button[5]={};
  static string input="";
  static string output="";

  for(int i=0;i<5;i++) previous_button[i] = button[i];

  button[0] = digitalRead(0);
  button[1] = digitalRead(1);
  button[2] = digitalRead(2);
  button[3] = digitalRead(3);
  button[4] = digitalRead(4);

  // caliculate cursor
  if(previous_button[0] && !button[0]) cursor.y++; // left
  if(previous_button[1] && !button[1]) cursor.x--; // up
  if(previous_button[2] && !button[2]) cursor.y--; // right
  if(previous_button[3] && !button[3]) cursor.x++; // down
  cursor.x = (cursor.x%CURSOR_X_MAX+CURSOR_X_MAX)%CURSOR_X_MAX;
  cursor.y = (cursor.y%CURSOR_Y_MAX+CURSOR_Y_MAX)%CURSOR_Y_MAX;

  // make string
  if(!button[4] && previous_button[4]){
      if(alphabet[cursor.x]=="backspace" && output.length()>0){
          output.pop_back();
      }else if(alphabet[cursor.x]=="return"){
          output += "\n";
      }else{
          output += alphabet[cursor.x];
      }

      Serial.println(output.c_str());
  }

  // output data
  //cout << "x:" << cursor.x << " y:" << cursor.y << " character:" << alphabet[cursor.x] << " output:" << output << endl;
  //Serial.printf("x:%d y:%d character:%s output:%s\n", cursor.x, cursor.y, alphabet[cursor.x], output);
  Serial.printf("%d %d %d %d %d\n", button[0], button[1], button[2], button[3], button[4]);
  
  display.clearDisplay();
  display.setCursor(4, 4);
  display.println(alphabet[cursor.x].c_str());
  string str = output + "_";
  display.println(str.c_str());
  display.display();

  delay(50);
  return 0;
}