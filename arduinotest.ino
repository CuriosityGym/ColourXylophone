int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=0; i<5; i++){
    Serial.write(0x30 + i);
    delay(1000);
  }
  delay(1000);
}
