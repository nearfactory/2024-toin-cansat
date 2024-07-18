
// 初期緯度と経度の値
float latitude = 33.43568634760829;
float longitude = 135.76232162319042;

// ボタンの接続ピン
const int buttonPin = 50;

// ボタンの状態
int buttonState = 0;
int lastButtonState = 0;

// デバウンス用の変数
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 100; // 50ミリ秒のデバウンス時間

void setup() {
  // シリアル通信を初期化
  Serial.begin(115200);

  // ボタンピンを入力に設定
  pinMode(buttonPin, INPUT);
}

void loop() {
  // ボタンの読み取り
  int reading = digitalRead(buttonPin);

  // デバウンス処理
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 状態が変わった場合のみ反応
    if (reading != buttonState) {
      buttonState = reading;

      // ボタンが押されたとき（LOWの場合）
      if (buttonState == LOW) {
        // 緯度と経度の値を送信
        Serial.print(latitude, 4); // 小数点以下4桁まで表示
        Serial.print(", ");
        Serial.print(longitude, 4); // 小数点以下4桁まで表示

        // メッセージを送信
        Serial.print(", ");
        Serial.write("おなかがすいています");
        Serial.println();

        // 緯度と経度の値を更新
        latitude += 0.1;
        longitude += 0.1;
      }
    }
  }

  // 状態を保存
  lastButtonState = reading;
}
