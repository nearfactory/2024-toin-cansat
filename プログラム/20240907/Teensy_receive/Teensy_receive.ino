int k=0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial1.available() != 0){
    digitalWrite(13, HIGH);
    k=Serial1.read();
    if((97<=k && k<=127)||(48<=k && k<=58)||(k==46)||(k==44)||(k==32)||(k==0)||(10<=k && k<=13)){
      Serial.write(k);
    }
  }else{
    digitalWrite(13, LOW);
  }
}
