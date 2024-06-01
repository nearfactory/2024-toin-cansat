#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "misakiUTF16.h"

Adafruit_SSD1306 OLED(128, 64, &Wire, -1); // OLEDドライバ作成
uint8_t font[8];                           // フォント格納バッファ

void setup() {
  Serial.begin(115200);
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Cannot work."));
    for(;;); 
  }
  OLED.clearDisplay();

  // "あ"の描画
  getFontData(font, "あ");              // "あ"のフォントを取得 
  OLED.drawBitmap(0, 0, font,8,8,1);    // フレームバッファにフォント書き込み 
  OLED.display();                       // 画面更新

  // 文字列の表示
  char *str="Abcあいうえお、埼玉 "      // 文字列
            "日本語表示できるかな？ " 
            "Arduinoで漢字表示"
            "出来ますよ！";
  char *ptr = str;                      // 文字列参照位置
  uint16_t x = 0, y = 8;                // 表示位置

  while(*ptr) {                         // 文字列分ループ
     ptr = getFontData(font, ptr,true); // 1文字分のフォント取得
     if (!ptr)
        break;                          // エラーの場合は終

     OLED.drawBitmap(x,y,font,8,8,1);   // フレームバッファにフォント書き込み 

     if (128-8 > x) {                   // 表示位置更新
       x+=8;
     } else {
       x=0;
       y+=8;
     }
  }
  OLED.display();                       // 画面更新
}

void loop() {
  // put your main code here, to run repeatedly:

}