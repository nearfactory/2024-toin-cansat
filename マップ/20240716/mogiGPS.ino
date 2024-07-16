float e = 52.4;
float w = 110.4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(e);
  Serial.print(", ");
  Serial.println(w);

  e = e + 0.1;
  w = w + 0.2;
  delay(150);
}
