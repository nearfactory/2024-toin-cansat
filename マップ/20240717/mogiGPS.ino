// 初期緯度と経度の値
float latitude = 33.78871818;
float longitude = 135.45651651;

// 更新周期（ミリ秒）
const unsigned long updateInterval = 1000; // 1秒ごとに更新

// 前回の更新時間を保存する変数
unsigned long previousMillis = 0;

void setup() {
  // シリアル通信を初期化
  Serial.begin(115200);
}

void loop() {
  // 現在の時間を取得
  unsigned long currentMillis = millis();

  // 更新周期が経過したかどうかをチェック
  if (currentMillis - previousMillis >= updateInterval) {
    // 前回の更新時間を現在の時間に更新
    previousMillis = currentMillis;

    // 緯度と経度の値を更新
    latitude += 0.1;
    longitude += 0.1;

    // シリアル通信で文字列を送信
    Serial.print(latitude, 4); // 小数点以下4桁まで表示
    Serial.print(", ");
    Serial.println(longitude, 4); // 小数点以下4桁まで表示
  }
}
