//Sensors Lab 3: Strain analysis

//Define inamp pins
const uint8_t INAMP_PIN1 = 35;
const uint8_t INAMP_PIN2 = 34;

void setup() {
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) {
    delay(10);
  }
} 

void loop() {

  //Loops to create an average of strain gauge data to reduce noise
  int n = 0;
  int totVal1 = 0;
  while (n < 5) {
    int currentVal1 = analogRead(INAMP_PIN1);
    totVal1 += currentVal1;
    n += 1; 
  }
  int av1 = totVal1/5;

  int b = 0;
  int totVal2 = 0;
  while (b < 5) {
    int currentVal2 = analogRead(INAMP_PIN2);
    totVal2 += currentVal2;
    b += 1; 
  }
  int av2 = totVal2/5;  

  int angle1, angle2, angle;

  //Find angle from first strain gauge
  if (av1 > 1210) {
    //Linear transfer function for when the first strain gauge is stretching
    angle1 = (av1 - 1213)/(-1.44);
  }
  else if (av1 <= 1210) {
    //Linear transfer function for when the first strain gauge is compressing
    angle1 = (av1 - 1200)/(-3.56);
  }
  
  //Find angle from second strain gauge
  if (av2 > 1000) {
    //Linear transfer function for when the second strain gauge is stretching
    angle2 = (av2 - 1017)/(-5.78);
  }
  else if (av2 <= 1000) {
    //Linear transfer function for when the second strain gauge is compressing
    angle2 = (av2 - 998)/(-4.67);
  }

  //Average the angle measurements for accuracy
  angle = (angle1 + angle2)/2;

  Serial.print("Angle1: ");
  Serial.print(angle1);
  Serial.print(", ");
  Serial.print("Angle2: ");
  Serial.print(angle2);
  Serial.print(", ");
  Serial.print("Angle: ");
  Serial.println(angle);



  //Prints output angle to serial monitor
  Serial.print("Variable_1:");
  Serial.println(angle);

  //Constants to set range of serial monitor
  int constant1 = 0;
  int constant2 = 50;
  int constant3 = -50;

  Serial.print("Variable_2:");
  Serial.println(constant1);

  Serial.print("Variable_3:");
  Serial.println(constant2);

  Serial.print("Variable_4:");
  Serial.println(constant3);

  //Delay so that sample rate isn't too high
  delay(10);
}