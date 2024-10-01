float bias = 2.3603;
float gain = 1.2491;

double WR = 180.0;

double PR3 = 340.0;

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(12);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(34);
  //Serial.println(analogValue);

  double Vreal = (analogValue + 160)/1245.0;

  Serial.print("Vreal: ");
  Serial.println(Vreal);
  
  double removeBias = Vreal - bias; //Removes voltage bias

  double removeGain = removeBias/gain; //Removes gain adjustment

  double removeWheatstone = (WR*(removeGain+(5.0/2.0)))/((5.0/2.0)-removeGain); //Inverse of wheatstone bridge function

  double remove3PR = (removeWheatstone*PR3)/(PR3-removeWheatstone); //Finds initial resistance of thermistor

  double betaTransfer = (1084073.4)/(3636.0 + 298.15*log(0.001*remove3PR)); //Uses beta model of thermistor to find temperature based on resistance

  Serial.print("Temp in kelvin: ");
  Serial.println(betaTransfer);

  delay(1000);
}
