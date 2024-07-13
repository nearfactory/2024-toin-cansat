#include <TWELITE>

/*** the setup procedure (called on boot) */
void setup() {
  Analogue.setup();
  Analogue.begin(pack_bits(PIN_ANALOGUE::A1,PIN_ANALOGUE::A2,PIN_ANALOGUE::VCC), 100);
  Serial.begin(115200);
}

/*** the loop procedure (called every event) */
void loop() {
  // digitalWrite(15, HIGH);
  // delay(2000);
  
  Serial.print(int(Analogue.read(PIN_ANALOGUE::A1)));
  Serial.print("  |  ");
  Serial.println(int(Analogue.read(PIN_ANALOGUE::A2)));
  delay(50);
}