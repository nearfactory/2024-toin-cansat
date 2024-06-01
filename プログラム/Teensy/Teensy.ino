#include <string>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "misakiUTF16.h"

using namespace std;

struct Cursor {
  int x = 0, y = 0;
};

constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;
constexpr int DISPLAY_ADDR = 0X3C;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, -1);

constexpr int CURSOR_X_MAX = 5;
constexpr int CURSOR_Y_MAX = 11;


uint8_t font[8] = {};
//string characters[28] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","backspace","return"};

string characters[11][5] = {
  { "あ", "い", "う", "え", "お" },
  { "か", "き", "く", "け", "こ" },
  { "さ", "し", "す", "せ", "そ" },
  { "た", "ち", "つ", "て", "と" },
  { "な", "に", "ぬ", "ね", "の" },
  { "は", "ひ", "ふ", "へ", "ほ" },
  { "ま", "み", "む", "め", "も" },
  { "や", "ゐ", "ゆ", "ゑ", "よ" },
  { "ら", "り", "る", "れ", "ろ" },
  { "わ", "　", "を", "　", "ん" },
  { "゛", "゜", "space", "backspace", "enter" }
};

void setup() {
  pinMode(0, INPUT_PULLDOWN);
  pinMode(1, INPUT_PULLDOWN);
  pinMode(2, INPUT_PULLDOWN);
  pinMode(3, INPUT_PULLDOWN);
  pinMode(4, INPUT_PULLDOWN);

  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
    Serial.println("failed to begin display!");
    while (true)
      ;
  }

  display.clearDisplay();
  displayString(0, 0, 8, "Cansat Test\n2024 05 32");
  display.display();

  delay(2000);
}

void loop() {
  static Cursor cursor{ 0, 0 };
  static bool button[5] = {};  // 0:left 1:up 2:right 3:down 4:center
  static bool previous_button[5] = {};
  static string input = "";
  static string output = "";

  for (int i = 0; i < 5; i++) previous_button[i] = button[i];

  button[0] = digitalRead(0);
  button[1] = digitalRead(1);
  button[2] = digitalRead(2);
  button[3] = digitalRead(3);
  button[4] = digitalRead(4);

  if(difference(previous_button, button)){
    // caliculate cursor
    if (previous_button[0] && !button[0]) cursor.y++;  // left
    if (previous_button[1] && !button[1]) cursor.x--;  // up
    if (previous_button[2] && !button[2]) cursor.y--;  // right
    if (previous_button[3] && !button[3]) cursor.x++;  // down
    cursor.x = (cursor.x % CURSOR_X_MAX + CURSOR_X_MAX) % CURSOR_X_MAX;
    cursor.y = (cursor.y % CURSOR_Y_MAX + CURSOR_Y_MAX) % CURSOR_Y_MAX;

    // make string
    if (!button[4] && previous_button[4]) {
      if (characters[cursor.y][cursor.x] == "backspace" && output.length() > 0) {
        output.pop_back();
      } else if (characters[cursor.y][cursor.x] == "enter") {
        output += "\n";
      } else if (characters[cursor.y][cursor.x] == "space") {
        //Serial.println(output.c_str());
        output += "　";
      } else {
        output += characters[cursor.y][cursor.x];
      }

      Serial.println(output.c_str());
    }

    // output data
    // cout << "x:" << cursor.x << " y:" << cursor.y << " character:" << characters[cursor.y][cursor.x] << " output:" << output << endl;
    // Serial.printf("x:%d y:%d character:%s output:%s\n", cursor.x, cursor.y, characters[cursor.y][cursor.x], output);
    // Serial.printf("%d %d %d %d %d\n", button[0], button[1], button[2], button[3], button[4]);

    display.clearDisplay();
    displayString(0, 0, 8, characters[cursor.y][cursor.x].c_str());
    string str = output + "_";
    display.setCursor(0, 10);
    displayString(0, 10, 8, str.c_str());
    display.display();
  }

  delay(50);
  return 0;
}

void displayString(int16_t x, int16_t y, int16_t size, const char *str) {
  char *ptr = str;  // 文字列参照位置

  while (*ptr) {                         // 文字列分ループ
    ptr = getFontData(font, ptr, true);  // 1文字分のフォント取得
    if (!ptr)
      break;  // エラーの場合は終

    display.drawBitmap(x, y, font, size, size, 1);  // フレームバッファにフォント書き込み

    if (DISPLAY_W - size > x) {  // 表示位置更新
      x += size;
    } else {
      x = size;
      y += size;
    }
  }
  display.display();  // 画面更新
}

template <typename T, typename U>
int difference(T& arr1, U& arr2){
    int arr1_length = sizeof(arr1)/sizeof(arr1[0]);
    int arr2_length = sizeof(arr2)/sizeof(arr2[0]);

    int min_length = arr1_length < arr2_length ? arr1_length : arr2_length;
    //cout << arr1_length << " " << arr2_length << " " << min_length;

    for(int i=0;i<min_length;i++){
        if(arr1[i]!=arr2[i]){
            return 1;
        }
    }

    return 0;
}