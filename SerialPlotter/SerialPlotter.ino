int adcvalue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}


void loop() {
  // put your main code here, to run repeatedly:
  adcvalue = analogRead(33);
  Serial.println(adcvalue);
  delay(200);
}
